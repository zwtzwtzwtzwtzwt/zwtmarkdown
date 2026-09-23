### 内连接和外连接处理 null
1. **INNER JOIN**：两边匹配上才保留，只要任意一方缺少对应数据，这条记录不显示。
2. **LEFT JOIN**：**左表所有行一定保留**，右表匹配不到，右表字段为 NULL。
3. **RIGHT JOIN**：**右表所有行一定保留**，左表匹配不到，左表字段为 NULL。
先准备两张表 **user 用户表（左表）**

|uid|username|dept_id|
|---|---|---|
|1|张三|10|
|2|李四|20|
|3|王五|30|

**dept 部门表（右表）**

|dept_id|dept_name|
|---|---|
|10|研发部|
|20|产品部|

> 部门表没有 dept_id=30 的记录

#### 1. INNER JOIN 内连接

> INNER JOIN：两边表**同时有匹配的数据，才展示；任意一边缺失，这条记录直接丢掉**

```
SELECT u.username, d.dept_name
FROM user u
INNER JOIN dept d ON u.dept_id = d.dept_id;
```

结果：

|username|dept_name|
|---|---|
|张三|研发部|
|李四|产品部|

✅ 张三、李四两边都匹配到，保留 ❌ 王五：user 里有王五，但 dept 没有 30 号部门 → 一边缺失，**直接不出现**

#### 2. LEFT JOIN 左外连接

> LEFT JOIN：**左边表全部数据都读出来；右边表找不到匹配，右边字段填 NULL**

```
SELECT u.username, d.dept_name
FROM user u
LEFT JOIN dept d ON u.dept_id = d.dept_id;
```

结果：

|username|dept_name|
|---|---|
|张三|研发部|
|李四|产品部|
|王五|NULL|

✅ user（左表）全部保留：张三、李四、王五 王五在 dept 没有对应部门，**dept_name 填 NULL，记录不会删掉**

#### 3. RIGHT JOIN 右外连接

> RIGHT JOIN：**右边表全部数据读出来；左边表找不到匹配，左边字段填 NULL**

新增一条部门记录：`dept_id=40，dept_name=测试部`，user 表没有人属于 40 部门

```
SELECT u.username, d.dept_name
FROM user u
RIGHT JOIN dept d ON u.dept_id = d.dept_id;
```

结果：

|username|dept_name|
|---|---|
|张三|研发部|
|李四|产品部|
|NULL|测试部|

✅ dept（右表）所有部门全部保留 测试部没有对应的用户，**user 字段全部填充 NULL**