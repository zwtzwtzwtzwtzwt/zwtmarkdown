# Java 后端 DTO / VO / BO / PO / DO 完整讲解（SpringBoot 后端通用分层对象）

> 核心一句话：**不同对象，负责不同层级的数据传输，避免实体类到处泄露字段，解耦、安全、好维护** 很多新手坑：直接拿数据库实体 `PO` 传给前端，会把密码、删除标记、创建人这些敏感字段暴露出去；或者前端传一大堆无关字段直接绑定数据库对象。

## 一、各个对象含义 & 用途

### 1. PO / DO（Persistent Object / Data Object）数据库对象

**PO = DO，一般二者视作同一个东西**

- 对应数据库**一张表**，字段和表一一对应，放在 `entity` / `po` 包
- 只和 MyBatis / JPA 交互，**绝对不直接返回给前端，也不直接接收前端参数**
- 包含：主键 id、创建时间、更新时间、delFlag、version、password 这类数据库字段

```
// UserPO 数据库实体
@TableName("t_user")
public class UserPO {
    private Long id;
    private String username;
    private String password; // 敏感！不能返回前端
    private String phone;
    private LocalDateTime createTime;
    private Integer delFlag;
}
```

### 2. DTO（Data Transfer Object）数据传输对象

> **后端内部/前后端之间传输数据**，分两种场景：

1. **入参 DTO（接收前端请求参数）**：Controller 接收前端提交的数据（新增、修改、查询条件）
    - 放在 `dto` 包，如 `UserSaveDTO`、`UserQueryDTO`
    - 只保留前端需要传给后端的字段，不需要数据库所有字段
    - 配合校验注解 `@NotBlank @Size` 做参数校验
2. **跨服务 DTO（微服务之间调用）**：服务 A 调用服务 B，互相传递的数据对象

> ✅ 简单记：**前端传给后端 → DTO（入参）**

```
// 用户新增，前端提交过来的数据
public class UserSaveDTO {
    @NotBlank(message = "用户名不能为空")
    private String username;
    @NotBlank(message = "手机号不能为空")
    private String phone;
    // 没有 password、delFlag，前端不传这些
}
```

### 3. VO（View Object）视图对象

> **后端返回给前端页面/接口的数据**，View = 视图

- 放在 `vo` 包，如 `UserVO`、`UserPageVO`
- 只保留**前端页面需要展示**的字段，剔除敏感字段（密码、删除标记等）
- 可以组装多表数据（PO 是单表，VO 可以是关联查询拼接后的复合数据）

✅ 简单记：**后端返回给前端 → VO**

```
// 返回给前端展示
public class UserVO {
    private Long id;
    private String username;
    private String phone;
    // 没有 password、delFlag
}
```

### 4. BO（Business Object）业务对象

> **Service 层内部使用，封装复杂业务数据**

- 放在 `bo` 包，在 Service 内部流转
- 当业务复杂，一个 DTO/PO 不够承载业务信息时使用
- 把多个 PO 组装成 BO，做业务计算、组装；**一般不会传给 Controller 或者前端**
    
    > 中小型单体项目很多时候**不用 BO**，DTO 和 PO 足够；大型复杂业务、微服务才常用
    

### 5. 其他常见对象（了解即可）

- **Query**：很多项目会单独抽出来，专门放查询条件（和 DTO 很像，有些项目直接合并到 DTO）
- **Form**：部分老项目，Form=入参 DTO，接收表单提交，现在新项目基本统一叫 DTO
- **Entity**：很多项目直接用 Entity 代替 PO/DO，实体包名写 entity，含义一样

# 二、完整调用链路（最核心！必看）

```
前端请求 → Controller →【DTO（入参）】→ 校验 → 转为 BO / 直接转PO → Service → Mapper → PO（数据库）
数据库查询得到 PO → Service组装 → 转为 VO → Controller 返回给前端
```

示例流程：

1. 前端提交新增用户的 json → Controller 接收 `UserSaveDTO`
2. 校验 DTO 参数合法
3. 把 DTO 属性拷贝到 `UserPO`（工具：MapStruct / Hutool BeanUtil / Spring BeanUtils）
4. Mapper.insert(userPO)入库

查询流程：

1. 前端传查询条件 → `UserQueryDTO`
2. Mapper 查询拿到 `UserPO`
3. Service 将 PO 拷贝到 `UserVO`，剔除敏感字段
4. Controller return VO → 序列化 JSON 返回前端

> 属性拷贝工具推荐：
> 
> - 简单项目：`BeanUtil.copyProperties`（Hutool）
> - 生产规范项目：**MapStruct（编译期，性能更好，无反射坑）**

# 三、包结构标准示例（MVC 项目）

```
com.xxx.project
├── controller
├── service
├── mapper
├── entity        // PO/DO 数据库实体
├── dto           // 请求入参、跨服务传输对象
│   ├── UserSaveDTO.java
│   ├── UserQueryDTO.java
├── vo            // 返回前端视图对象
│   ├── UserVO.java
│   ├── UserPageVO.java
├── bo            // 业务对象（复杂业务才创建）
│   └── UserBO.java
```

# 四、高频区分口诀

- **PO：跟数据库绑定，表映射，绝不返回前端**
- **DTO：接收前端请求参数，前端→后端**
- **VO：返回给前端展示，后端→前端**
- **BO：Service 内部业务组装，不碰 Controller**

# 五、容易踩坑的点

1. ❌ 直接返回 PO 给前端：会泄露 password、delFlag 等敏感字段
2. ❌ 直接用 PO 接收前端参数：前端多传一个字段（比如 id），直接覆盖数据库主键，安全风险
3. ❌ DTO/VO 复用：新增 DTO 和修改 DTO 不要强行共用，新增不需要 id，修改需要 id，放一起容易引发校验混乱
4. ✅ 分页场景：统一封装 `PageVO<T>`，泛型承载 VO，不要重复写分页字段

# 六、举一个最简完整 Demo

```
// PO 数据库实体 entity/UserPO.java
@TableName("t_user")
public class UserPO {
    private Long id;
    private String username;
    private String password;
    private String phone;
}

// DTO 入参 dto/UserSaveDTO.java
public class UserSaveDTO {
    @NotBlank
    private String username;
    @NotBlank
    private String phone;
    // 前端不传密码，密码后端单独加密处理
}

// VO 返回 vo/UserVO.java
public class UserVO {
    private Long id;
    private String username;
    private String phone;
}

// Controller
@PostMapping("/user/add")
public R addUser(@Valid @RequestBody UserSaveDTO dto){
    UserPO po = BeanUtil.copyProperties(dto, UserPO.class);
    // 密码单独设置加密
    po.setPassword(encrypt("123456"));
    userService.save(po);
    return R.ok();
}

@GetMapping("/user/{id}")
public R<UserVO> getUser(@PathVariable Long id){
    UserPO po = userService.getById(id);
    UserVO vo = BeanUtil.copyProperties(po, UserVO.class);
    return R.ok(vo);
}
```

如果你想，我可以给你：

1. MapStruct 的完整示例（代替 BeanUtil，企业更推荐）
2. 分页 VO 的通用封装代码
3. 区分什么时候用 BO 的业务案例