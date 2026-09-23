# Java 调用 MySQL 事务完整流程（JDBC 底层 → Spring 封装）

> 核心：**Java 通过 JDBC Connection 来控制 MySQL 事务**，MySQL 才是事务真正的载体。Connection 就是 Java 和数据库之间的会话连接。

## 一、原生 JDBC 底层（无框架，最根本原理）

MySQL 默认**自动提交模式：`autoCommit=true` ** 每执行一条 DML，MySQL 自动 commit，每条 SQL 单独一个事务。 事务本质就是关闭自动提交，手动控制 commit/rollback。

### 完整示例代码

```
// 1. 获取数据库连接
Connection conn = DriverManager.getConnection(url, user, password);
try {
    // 2. 关闭自动提交【开启事务的关键】
    conn.setAutoCommit(false);

    // 3. 在同一个连接中执行多条SQL（同一个事务）
    String sql1 = "update account set balance = balance - 100 where id=1";
    PreparedStatement pstmt1 = conn.prepareStatement(sql1);
    pstmt1.executeUpdate();

    String sql2 = "update account set balance = balance + 100 where id=2";
    PreparedStatement pstmt2 = conn.prepareStatement(sql2);
    pstmt2.executeUpdate();

    // 4. 全部成功 → 提交事务，MySQL执行commit
    conn.commit();
} catch (Exception e) {
    // 5. 出现异常 → 回滚事务，MySQL执行rollback
    conn.rollback();
    e.printStackTrace();
} finally {
    // 关闭资源
    conn.close();
}
```

### 底层发给 MySQL 的指令

`setAutoCommit(false)` 等价于向 MySQL 发送 `BEGIN;`

- commit() → 发送 `COMMIT;`
- rollback() → 发送 `ROLLBACK;`

> ⚠️ **事务绑定在 Connection 上！** 同一个事务里所有 SQL**必须使用同一个 Connection**； 如果两条 SQL 拿到两个不同连接，就是两个独立事务，无法原子。

---

## 二、Spring 事务（@Transactional 底层原理）

Spring 不是自己实现事务，**只是对 JDBC Connection 做封装管理**。 `@Transactional` 是 AOP 动态代理实现。

### 执行流程

1. 调用带 `@Transactional` 的方法，AOP 拦截
2. 事务管理器 `DataSourceTransactionManager` 获取数据库连接 Connection
3. 设置 `connection.setAutoCommit(false)` → **开启事务**
4. 执行目标方法内部的 Mapper/DAO 操作（MyBatis 会复用这个 Connection）
5. 方法正常执行完毕 → AOP 执行 `connection.commit()`
6. 方法抛出指定异常 → AOP 执行 `connection.rollback()`
7. 释放连接（归还连接池，不是关闭物理连接）

> 关键点：MyBatis 在 Spring 环境下，会**从 Spring 事务管理器拿同一个 Connection**，保证所有 SQL 在同一个连接、同一个事务内。

### 伪代码模拟 Spring 事务

```
// AOP拦截伪代码
public Object invoke(MethodInvocation invocation) throws Throwable {
    Connection conn = getConnectionFromPool();
    conn.setAutoCommit(false);
    try {
        Object result = invocation.proceed(); // 执行业务方法
        conn.commit();
        return result;
    } catch (Exception e) {
        conn.rollback();
        throw e;
    }
}
```

## 三、连接池的角色（Druid/HikariCP）

连接池负责管理一批物理 Connection。 Spring 事务的核心约定：**同一个事务内，多次获取连接，拿到的是同一个 Connection**。 Spring 使用 `ThreadLocal` 把 Connection 绑定到当前线程，保证同一个线程多次取连接拿到同一个。

> ThreadLocal 存储当前线程的 Connection，这是 Spring 事务可以跨多个 Mapper 共用一个连接的核心。

### 简化理解

1. 线程进入事务方法 → 从池拿连接，放入 ThreadLocal
2. 后续所有 mapper 查询：优先拿 ThreadLocal 里绑定好的 Connection
3. 事务提交/回滚完成后，解除 ThreadLocal 绑定，连接归还池（不关闭物理连接）

## 四、几个高频面试考点

### 1. 为什么事务失效：方法内直接 this 调用

```
@Service
public class AccountService {
    public void test() {
        this.transfer(); // this是原始对象，不是代理对象！AOP拦截不到，事务不会开启
    }
    @Transactional
    public void transfer() {
        // DB操作
    }
}
```

原因：AOP 只有代理对象调用方法才会拦截；`this` 是原生对象，不走代理，不会开启事务。

### 2. 事务只对当前 Connection 有效

- 新起一个子线程：子线程拿新 Connection，**不在同一个事务**
- 新开线程执行数据库操作，不受父线程事务控制，无法一起回滚

### 3. 自动提交模式的坑

如果忘记 `setAutoCommit(false)`，每一条 DML 执行完自动 commit，多条 SQL 无法组成原子事务。

### 4. @Transactional 只能管理数据库操作

Java 内存对象、RPC 调用、文件操作，Spring 无法回滚。只有数据库层面的操作，由 MySQL 的 undo log 实现回滚。

## 五、JDBC 事务 vs Spring 事务对比

||JDBC 原生事务|Spring @Transactional|
|---|---|---|
|开启方式|手动写 `setAutoCommit(false)` |AOP 自动拦截，底层调用 setAutoCommit(false)|
|代码侵入|高，try-catch 手动 commit/rollback|低，注解声明式|
|连接管理|自己维护 Connection|Spring+ThreadLocal 绑定连接，整合连接池|
|功能|基础事务|传播机制、隔离级别、只读事务、超时|

## 六、一句话面试总结

Java 通过 JDBC 的 Connection 对象控制 MySQL 事务；核心是关闭连接的 autoCommit 自动提交，同一个连接内多条 SQL 构成一个事务；Spring @Transactional 基于 AOP+ThreadLocal，将 Connection 绑定到当前线程，自动完成开启、提交、回滚；事务的原子性最终由 MySQL InnoDB 引擎保证，事务必须复用同一个 Connection，否则事务失效。

## 拓展提问

要不要我顺带讲一下**事务隔离级别在 Java 代码里怎么设置**（JDBC / @Transactional 注解）？