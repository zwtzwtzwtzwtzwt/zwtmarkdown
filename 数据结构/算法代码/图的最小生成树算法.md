在考研数据结构中，最小生成树（MST）是非常经典的大题考点。

这里需要先**纠正一个常见的备考误区**：在考研的语境下，我们通常**不会**去写“基于邻接表的 Prim”或“直接在邻接表/矩阵上跑的 Kruskal”。

- **Prim 算法**：每次找离树最近的顶点。由于需要频繁读取任意两个顶点间的权值，它与**邻接矩阵**是天作之合。如果用邻接表，每次更新周边节点距离时反而麻烦。考试中 99% 都是要求基于**邻接矩阵**手写。
    
- **Kruskal 算法**：核心是“按权值对边排序”。它需要的数据结构是**边集数组**（Edge Array），无论你原本是邻接矩阵还是邻接表，第一步都是把所有的边提取出来放到边集数组里。
    

下面为你提供符合王道/严蔚敏教材规范的、最适合在卷面上拿分的标准实现。

### 1. Prim 算法 (基于邻接矩阵)

**考点提示：** 适合**稠密图**。核心思想是“加点法”，维护两个数组：`isJoin`（标记是否已加入生成树）和 `lowcost`（尚未加入的顶点到生成树的最小代价）。时间复杂度为 $O(|V|^2)$，与边数无关。

**代码实现：**

假设使用之前定义的 `MGraph`，带权图没有边的地方设为无穷大 `INF`（如 0 x 3 f 3 f 3 f 3 f）。

C

```C
#define INF 0x3f3f3f3f // 定义无穷大

void Prim(MGraph G, int v0) {
    int lowcost[MaxVertexNum]; // 记录各顶点与生成树的最小权值
    int isJoin[MaxVertexNum];  // 标记顶点是否已加入生成树 (1表示已加入)
    int i, j, k, min;

    // 1. 初始化
    for (i = 0; i < G.vexnum; i++) {
        lowcost[i] = G.Edge[v0][i]; // 初始状态下，树中只有 v0
        isJoin[i] = 0;
    }
    isJoin[v0] = 1; // 将初始顶点 v0 加入树

    // 2. 循环 n-1 次，加入剩下的 n-1 个顶点
    for (i = 1; i < G.vexnum; i++) {
        min = INF;
        k = -1;

        // (1) 找未加入树的顶点中，lowcost 最小的顶点 k
        for (j = 0; j < G.vexnum; j++) {
            if (isJoin[j] == 0 && lowcost[j] < min) {
                min = lowcost[j];
                k = j;
            }
        }

        if (k == -1) return; // 图不连通的情况

        isJoin[k] = 1; // (2) 将顶点 k 加入生成树

        // (3) 核心更新：以新加入的 k 为媒介，更新未加入顶点到树的距离
        for (j = 0; j < G.vexnum; j++) {
            // 如果 j 未加入树，且 k 到 j 的边权 < j 原本到树的距离
            if (isJoin[j] == 0 && G.Edge[k][j] < lowcost[j]) {
                lowcost[j] = G.Edge[k][j]; 
                // 注意：这里考研有时还会要求维护一个 adjvex 数组来记录边是从哪个点连过来的，
                // 以便最后输出这棵树。若只求权值和，则无需 adjvex。
            }
        }
    }
}
```

> **避坑指南：** 考研大题常考“写出 Prim 算法的执行过程表”，其实就是让你手动模拟 `lowcost` 数组每一趟更新后的状态。一定要熟练掌握手动更新过程。

### 2. Kruskal 算法 (基于边集数组与并查集)

**考点提示：** 适合**稀疏图**。核心思想是“加边法”，每次选权值最小的边，只要不构成回路就加入树。判回路的核心工具是**并查集 (Disjoint Set)**。时间复杂度为 $O(|E| \log_2 |E|)$，主要耗时在边的排序上。

**代码实现：**

考试中通常允许你假设边的排序函数 `Sort` 已经存在，核心是写出**并查集的查找**和**选边逻辑**。

C

```C
// 边集数组的结构定义
typedef struct {
    int u;      // 边的起点
    int v;      // 边的终点
    int weight; // 边的权值
} Edge;

// 并查集查找根节点函数 (考场上写这个最简版即可)
int Find(int parent[], int x) {
    while (parent[x] >= 0) {
        x = parent[x];
    }
    return x;
}

// Kruskal 算法核心
// edges: 存放所有边的数组，已经按 weight 升序排序
// vexnum: 顶点数, arcnum: 边数
void Kruskal(Edge edges[], int vexnum, int arcnum) {
    int parent[MaxVertexNum]; // 并查集数组
    int i, root1, root2;
    int edgeCount = 0;        // 记录已加入树的边数

    // 1. 初始化并查集，初始时每个顶点自成一个集合 (-1代表自己是根)
    for (i = 0; i < vexnum; i++) {
        parent[i] = -1;
    }

    // 2. 依次遍历排好序的边集数组
    for (i = 0; i < arcnum; i++) {
        // 查找这条边两端的顶点所属的集合根节点
        root1 = Find(parent, edges[i].u);
        root2 = Find(parent, edges[i].v);

        // 如果根节点不同，说明这条边的两个顶点分属不同集合，加入它不会形成环
        if (root1 != root2) {
            // 合并集合：将 root2 挂在 root1 下面
            parent[root2] = root1; 
            
            // 输出这条边，或者累加权值
            // printf("选入边: (%d, %d) 权值: %d\n", edges[i].u, edges[i].v, edges[i].weight);
            
            edgeCount++;
            // 选够 n-1 条边时，生成树构建完成，可提前退出
            if (edgeCount == vexnum - 1) {
                break;
            }
        }
    }
}
```

