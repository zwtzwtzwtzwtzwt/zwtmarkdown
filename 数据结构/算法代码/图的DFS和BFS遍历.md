图的遍历（DFS 和 BFS）是考研数据结构大题的“地基”。无论是求连通分量、判断是否有环、还是求解最短路径问题，底层逻辑往往都是图的遍历。

在写图的遍历代码时，**最重要的一个考场原则是：永远不要忘记考虑“非连通图”的情况！** 因此，标准的遍历代码总是由两部分组成：一个是全局遍历控制函数（用于扫描所有顶点，防止漏掉非连通分量），另一个是真正的单点遍历执行函数。

这里我们沿用严蔚敏/王道教材的结构体定义，并假设已经存在一个 `visited` 全局数组和基础的队列操作。



```C
#define MaxVertexNum 100
bool visited[MaxVertexNum];  // 访问标记数组

// 假设考场上可以直接调用以下基础队列操作（BFS需要用到）
void InitQueue(Queue *Q);
bool isEmpty(Queue Q);
void EnQueue(Queue *Q, int v);
void DeQueue(Queue *Q, int *v);

// 假设存在访问函数
void visit(int v) {
    printf("%d ", v);
}
```

### 1. 深度优先搜索 (DFS)

**考点提示：** DFS 的本质就是**递归**（底层借用函数调用栈）。它的逻辑非常类似树的先根遍历。

#### 1.1 全局控制函数 (考卷必写)

C

```C
// 以邻接表为例，如果是邻接矩阵只需把参数换成 MGraph
void DFSTraverse(ALGraph G) {
    for (int i = 0; i < G.vexnum; i++) {
        visited[i] = false;       // 1. 初始化所有顶点为未访问
    }
    
    for (int i = 0; i < G.vexnum; i++) {
        if (!visited[i]) {        // 2. 如果当前顶点未访问，则调用DFS
            DFS_ALGraph(G, i);    // 这层循环的作用就是处理非连通图！
        }
    }
}
```

#### 1.2 基于邻接矩阵的 DFS

C

```
void DFS_MGraph(MGraph G, int v) {
    visit(v);                     // 1. 访问初始顶点
    visited[v] = true;            // 2. 标记已访问
    
    // 3. 遍历二维数组中 v 对应的这一行
    for (int w = 0; w < G.vexnum; w++) {
        // 如果存在边 (权值不为0或无穷) 且该邻接点未被访问过
        if (G.Edge[v][w] != 0 && !visited[w]) {
            DFS_MGraph(G, w);     // 递归深入
        }
    }
}
```

#### 1.3 基于邻接表的 DFS

C

```
void DFS_ALGraph(ALGraph G, int v) {
    visit(v);                     // 1. 访问初始顶点
    visited[v] = true;            // 2. 标记已访问
    
    // 3. 沿着顶点 v 的单链表往后走
    ArcNode *p = G.vertices[v].firstarc;
    while (p != NULL) {
        int w = p->adjvex;        // 获取邻接点下标
        if (!visited[w]) {
            DFS_ALGraph(G, w);    // 递归深入
        }
        p = p->nextarc;           // 指针后移
    }
}
```

### 2. 广度优先搜索 (BFS)

**考点提示：** BFS 的本质是**层序遍历**，必须借助**队列**来实现。它常用于求解非带权图的单源最短路径。

#### 2.1 全局控制函数

C

```
void BFSTraverse(MGraph G) {
    for (int i = 0; i < G.vexnum; i++) {
        visited[i] = false;       // 初始化访问数组
    }
    
    Queue Q;
    InitQueue(&Q);                // 初始化辅助队列
    
    for (int i = 0; i < G.vexnum; i++) {
        if (!visited[i]) {
            BFS_MGraph(G, i, &Q); // 对每个连通分量调用一次BFS
        }
    }
}
```

#### 2.2 基于邻接矩阵的 BFS

C

```
void BFS_MGraph(MGraph G, int v, Queue *Q) {
    visit(v);                     // 1. 访问初始顶点
    visited[v] = true;            // 2. 标记已访问
    EnQueue(Q, v);                // 3. 顶点入队
    
    while (!isEmpty(*Q)) {
        int u;
        DeQueue(Q, &u);           // 队头元素出队
        
        // 扫描该队头元素所在行的所有列
        for (int w = 0; w < G.vexnum; w++) {
            // 若存在边且未被访问
            if (G.Edge[u][w] != 0 && !visited[w]) {
                visit(w);         // 访问邻接点
                visited[w] = true;// 标记已访问
                EnQueue(Q, w);    // 邻接点入队
            }
        }
    }
}
```

#### 2.3 基于邻接表的 BFS

C

```
void BFS_ALGraph(ALGraph G, int v, Queue *Q) {
    visit(v);                     // 1. 访问初始顶点
    visited[v] = true;            // 2. 标记已访问
    EnQueue(Q, v);                // 3. 顶点入队
    
    while (!isEmpty(*Q)) {
        int u;
        DeQueue(Q, &u);           // 队头元素出队
        
        // 遍历该顶点依附的边链表
        ArcNode *p = G.vertices[u].firstarc;
        while (p != NULL) {
            int w = p->adjvex;
            if (!visited[w]) {
                visit(w);         // 访问
                visited[w] = true;// 标记
                EnQueue(Q, w);    // 入队
            }
            p = p->nextarc;       // 链表后移
        }
    }
}
```

### 3. 核心考点：时空复杂度对比

为了应付选择题，这四段代码的复杂度结论需要直接记住：

|**遍历方式**|**存储结构**|**时间复杂度**|**空间复杂度**|**适用场景**|
|---|---|---|---|---|
|**DFS / BFS**|**邻接矩阵**| $O(\vert V \vert^2)$ | $O(\vert V \vert)$ |稠密图|
|**DFS / BFS**|**邻接表**| $O(\vert V \vert + \vert E \vert)$ | $O(\vert V \vert)$ |稀疏图|

**考场避坑总结：**

1. **唯一性问题：** 邻接矩阵的表示是唯一的，所以其 DFS/BFS 遍历序列也是**唯一**的；邻接表的表示不唯一（取决于建立链表时是头插还是尾插），所以其 DFS/BFS 遍历序列**不唯一**。
    
2. **空间复杂度：** DFS 最坏情况下递归栈深度为 $O(\vert V \vert)$（退化成一条线）；BFS 最坏情况下队列里要装入几乎所有的顶点（比如星型图），也是 $O(\vert V \vert)$。