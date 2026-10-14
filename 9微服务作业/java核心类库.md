# Java 核心类库（JDK 内置核心包）

Java 核心类库就是 JDK 自带的基础 API，都在 `java.*`、`javax.*` 包下，开发 Java 程序默认依赖。下面按模块划分，重点标出面试高频包。

## 一、基础语言包（最核心，默认导入）

### `java.lang`

> 自动导入，不用 `import`，Java 语言根基

- `Object`：所有类的父类
- 基础包装类：`Integer`、`Long`、`Boolean`、`Double` 等
- 字符串：`String`、`StringBuilder`、`StringBuffer`
- 异常：`Exception`、`RuntimeException`、`Error`
- 线程：`Thread`、`Runnable`
- 系统：`System`、`Runtime`
- 类反射基础：`Class`

## 二、集合框架（面试重中之重）

### `java.util`

集合 + 工具类

1. 集合接口与实现
    - `Collection`：`List`（ArrayList、LinkedList、Vector）、`Set`（HashSet、TreeSet）
    - `Map`：`HashMap`、`TreeMap`、`LinkedHashMap`、`Hashtable`
2. 工具类
    - `Collections`：集合工具
    - `Arrays`：数组工具
    - 日期旧 API：`Date`、`Calendar`（**不推荐，已过时**）
    - 随机：`Random`
    - 枚举：`Enum`
    - 队列：`Queue`、`Deque`

## 三、IO / NIO 包

### `shturl.`：传统阻塞 IO（BIO）

- 字节流：`InputStream` / `OutputStream`（FileInputStream）
- 字符流：`Reader` / `Writer`（FileReader）
- 文件：`File`
- 序列化：`Serializable`

### `java.nio`：新 IO（NIO，JDK 1.4+）

- Buffer、Channel、Selector（多路复用）

### `java.nio.file`：NIO.2 文件操作（JDK 1.7+，推荐替代 shturl..File）

`Paths`、`Files`

## 四、日期时间（JDK 8 新时间 API，推荐）

### `java.time`（JDK 8 新增）

- `LocalDate`、`LocalTime`、`LocalDateTime`：本地日期时间，**线程安全**
- `Instant`：时间戳
- `Duration`、`Period`：时间间隔
- `DateTimeFormatter`：格式化

## 五、并发编程包（JUC，面试高频）

### `java.util.concurrent`

俗称 JUC（java.util.concurrent）

- 线程池：`ExecutorService`、`ThreadPoolExecutor`
- 锁：`Lock`、`ReentrantLock`、`ReadWriteLock`
- 并发集合：`ConcurrentHashMap`、`CopyOnWriteArrayList`
- 工具类：`CountDownLatch`、`CyclicBarrier`、`Semaphore`
- 原子类：`java.util.concurrent.atomic`（AtomicInteger 等 CAS）

## 六、反射

### `java.lang.reflect`

- `Method`、`Field`、`Constructor`
    
    > 配合 `java.lang.Class` 做反射，Spring、MyBatis 底层大量使用
    

## 七、网络编程

### `java.net`

- `Socket`、`ServerSocket`（TCP）
- `DatagramSocket`（UDP）
- `URL`、`URI`、`InetAddress`

## 八、数学工具

### `java.math`

高精度计算

- `BigInteger`：大整数
- `BigDecimal`：高精度小数（金融计算）

## 九、其他常用包

1. `java.util.regex`：正则表达式（Pattern、Matcher）
2. `java.sql`：JDBC 数据库操作接口（Connection、Statement、ResultSet）
3. `java.beans`：JavaBean 内省（Spring 用到）

## 补充：javax.* 扩展包（很多已经逐步移出 JDK）

`javax.servlet`、`javax.xml`、`javax.annotation` 等，**不属于 Java 基础核心语言库**，属于扩展 API。

---

# 快速记忆清单（面试简答版）

1. `java.lang`：基础类型、String、Thread、Object（自动导入）
2. `java.util`：集合框架、Arrays、Collections、旧日期
3. `java.util.concurrent`：JUC 并发包
4. `shturl. / java.nio`：IO 流、文件
5. `java.time`：JDK 8 新时间 API
6. `java.math`：BigDecimal、BigInteger
7. `java.net`：网络 Socket
8. `java.lang.reflect`：反射
9. `java.sql`：JDBC

如果你需要，我可以帮你整理**面试问答题版本**或者画一张核心类库思维导图。