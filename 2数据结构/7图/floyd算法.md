![](./assets/floyd算法/image.webp)![](./assets/floyd算法/image-1.webp)
动态规划思想  


# 算法执行过程  
 ![](./assets/floyd算法/image-2.webp)

  ![](./assets/floyd算法/image-4.webp)

![](./assets/floyd算法/image-5.webp)  

![](./assets/floyd算法/image-6.webp)  
![](./assets/floyd算法/image-8.webp)
<font color="#ff0000">如果没有节点通过一个点，那么这个点就不可以设置为中转点 可以直接跳过 </font>
<font color="#ff0000">每次找中转点  是根据前一步状态来进行的  </font>
![](./assets/floyd算法/image-3.webp)
![](./assets/floyd算法/image-9.webp)

4 没有改变 最后结果  
![](./assets/floyd算法/image-10.webp)





# 核心代码实现  
![](./assets/floyd算法/image-7.webp)  
 
# 算法总结  

![](./assets/floyd算法/image-12.webp)
![](./assets/floyd算法/image-13.webp)bfs 有邻接矩阵和邻接表两种方法  
在原对比表格中插入“有回路的图（非负权）”这一行后的完整对比表如下：

### 最短路径算法对比表

|**特性 / 图的类型**|**BFS 算法**|**Dijkstra 算法**|**Floyd 算法**|
|---|---|---|---|
|**无权图**| $\checkmark$ | $\checkmark$ | $\checkmark$ |
|**带权图**| $\times$ | $\checkmark$ | $\checkmark$ |
|**带负权值的图**| $\times$ | $\times$ | $\checkmark$ |
|**有回路的图（非负权）** _(新增)_|** $\checkmark$ **|** $\checkmark$ **|** $\checkmark$ **|
|**带负权回路的图**| $\times$ | $\times$ | $\times$ |
|**时间复杂度**| $O(\vert{}V\vert{}^2)$ 或 $O(\vert{}V\vert{}+\vert{}E\vert{})$ | $O(\vert{}V\vert{}^2)$ | $O(\vert{}V\vert{}^3)$ |
|**通常用于**|求无权图的单源最短路径|求带权图的单源最短路径|求带权图中各顶点间的最短路径|

### 原理解析

为什么这三种算法都可以正常处理**非负权回路**：

1. **BFS 算法**：
    
    - 靠 `visited` 标记数组（或 `d[]` 距离数组）避免顶点被重复入队，天然杜绝了在回路中死循环的问题。
        
2. **Dijkstra 算法**：
    
    - 只要回路的权值和 $\ge 0$，沿着回路绕圈只会让路径长度变长（或不变），不会产生更短的路径。结合贪心策略和标记已确定最短路径的 `final` 数组，算法能够正常收敛。
        
3. **Floyd 算法**：
    
    - 三重循环迭代更新 $d[i][j] = \min(d[i][j], d[i][k] + d[k][j])$。只要没有负权回路（导致绕圈能无限缩小距离），有回路完全不影响正确的松弛操作。