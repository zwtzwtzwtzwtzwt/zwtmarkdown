# Java 枚举 enum + Record 讲解（面试版，带代码）

> 版本说明：
> 
> - `enum`：Java 5 引入
> - `record`：Java 16 正式推出（预览版 14），**数据载体类**，自动生成构造器、getter、equals、hashCode、toString

## 一、枚举类 enum

### 1. 核心定位

**枚举是一种特殊的类**，用来表示**固定有限的常量集合**（订单状态、性别、季节、业务类型），本质每个枚举项都是 `public static final` 的本类实例。

### 基础写法

```
// 简单枚举
enum Season {
    SPRING, SUMMER, AUTUMN, WINTER
}
```

等价于：4 个 `Season` 的静态常量对象。

### 带属性、构造器、方法（最常用）

> 枚举**构造器必须私有**，默认 private，不能写 public。

```
public enum OrderStatus {
    // 枚举实例写在最前面，多个逗号分隔
    WAIT_PAY(0, "待支付"),
    PAID(1, "已支付"),
    CANCEL(2, "已取消");

    // 成员变量
    private final int code;
    private final String desc;

    // 私有构造
    OrderStatus(int code, String desc) {
        this.code = code;
        this.desc = desc;
    }

    // getter，枚举变量一般final，不要setter
    public int getCode() {
        return code;
    }
    public String getDesc() {
        return desc;
    }

    // 自定义方法：根据code查找枚举
    public static OrderStatus getByCode(int code) {
        for (OrderStatus status : values()) {
            if (status.code == code) {
                return status;
            }
        }
        return null;
    }
}
```

### 枚举内置自带方法

1. `values()`：返回所有枚举实例数组（编译器自动生成，不是 Enum 父类方法）
2. `ordinal()`：返回枚举定义顺序下标，从 0 开始（**业务尽量不要依赖 ordinal，顺序改就崩**）
3. `name()`：返回枚举名字符串，如 `WAIT_PAY.name()` → `"WAIT_PAY"`
4. `valueOf("WAIT_PAY")`：通过名字获取枚举，找不到抛异常

```
// 使用
OrderStatus status = OrderStatus.PAID;
System.out.println(status.getCode());
System.out.println(OrderStatus.getByCode(1));
```

### ✅ 枚举的经典用途：**枚举实现单例（Effective Java 推荐）**

```
enum SingletonEnum{
    INSTANCE;
    // 业务方法
    public void test(){}
}
// 使用
SingletonEnum.INSTANCE.test();
```

> 优点：**天然防反射、防序列化破坏单例**，面试高频。

### enum 特点&优缺点

✅优点

1. 类型安全，只能是定义好的几个值，不能随便传别的数字字符串
2. 自带序列化、JVM 保证单例
3. 可以有属性、方法、抽象方法

❌限制

1. 构造器私有，不能 new
2. 枚举项固定，运行期不能动态新增
3. 继承限制：**enum 隐式继承 java.lang.Enum，不能再继承别的类，但可以实现接口**

```
interface ShowMsg{
    void show();
}
enum Color implements ShowMsg{
    RED{
        @Override
        public void show() {
            System.out.println("红色");
        }
    },
    BLUE{
        @Override
        public void show() {
            System.out.println("蓝色");
        }
    };
}
```

---

## 二、Record（记录类，Java 16+）

### 核心定位

**纯数据载体类**，只用来保存数据，不需要业务逻辑。

> 编译器自动帮你生成：
> 
> 1. 私有 final 成员变量
> 2. 全参构造器
> 3. 访问器方法（**不是 getXXX！直接是字段名()**）
> 4. equals()、hashCode()
> 5. toString()

语法：`record 名字(参数列表){}`

### 基础示例

```
// 定义record，括号里是组件（字段）
public record UserRecord(Long id, String name, Integer age) {}
```

> 编译器自动生成的等价代码（简化）

```
public final class UserRecord {
    private final Long id;
    private final String name;
    private final Integer age;

    // 规范构造器
    public UserRecord(Long id, String name, Integer age) {
        this.id = id;
        this.name = name;
        this.age = age;
    }
    // 访问器：名字() 不是getId()
    public Long id() { return id; }
    public String name() { return name; }
    public Integer age() { return age; }

    @Override
    public boolean equals(Object o){...}
    @Override
    public int hashCode(){...}
    @Override
    public String toString(){...}
}
```

### 使用

```
UserRecord user = new UserRecord(1L, "张三", 20);
System.out.println(user.id());   // 注意！不是getId()
System.out.println(user.name());
System.out.println(user); // 自动toString
```

### 可以写自定义代码

1. 构造器校验参数（常用）
2. 自定义方法
    
    > record 的字段默认 `final`，类默认 `final`，**不能继承**，可以实现接口
    

```
public record UserRecord(Long id, String name, Integer age) {
    // 紧凑构造器：无参数，用来做参数校验
    public UserRecord {
        if(age != null && age < 0){
            throw new IllegalArgumentException("年龄不能负数");
        }
    }

    // 自定义方法
    public String getInfo(){
        return name + ":" + age;
    }
}
```

### Record 的限制（面试重点）

1. 类隐式 `final`，**不能被继承**
2. 组件（字段）隐式 `final`，不可修改，是**不可变数据载体**
3. 不能定义额外实例字段，只能用声明的组件；静态变量可以加
4. 访问器方法是 `name()`，不是 `getName()`（很多人踩坑）
5. 构造器不能手动改字段，紧凑构造器只能校验

### 使用场景

- DTO/值对象（Value Object），只读数据载体
- 方法多返回值（以前要手写类，现在直接 record）
- 本地临时数据封装，不需要写一堆模板代码

> ⚠️ 注意：Record 是 Java 16+，低版本项目（8/11）不能用！很多公司生产还在用 JDK 8，所以项目里用得不多。

---

## 三、enum vs Record 对比（面试背诵）

||enum|record|
|---|---|---|
|用途|有限固定常量集合|不可变数据载体、值对象|
|JDK 版本|Java 5|Java 16|
|是否可实例化|实例在编译期固定，不能 new|new 创建实例|
|可变性|成员一般 final|所有组件隐式 final，不可变|
|继承|隐式继承 Enum，不能继承其他类|隐式 final，不能继承任何类|
|典型场景|状态码、类型、枚举单例|返回值、值对象、临时 DTO|

## 面试追问

1. record 的访问器为什么不是 getXXX？
    
    > 设计定位是**数据组件访问**，不是 JavaBean。JavaBean 是可变的，record 是不可变值载体，不遵循 JavaBean 规范。
    
2. 枚举可以用==比较吗？
    
    > ✅ 可以！枚举全局单例，推荐直接 `==`，比 equals 效率更高。
    
1. record 是值类型吗？
    
    > 不是，仍然是引用类型，只是语义上作为值对象。
    

---

要不要我给你写一段**枚举 + Record 联合使用**的示例代码（例如订单记录里带上枚举状态）？