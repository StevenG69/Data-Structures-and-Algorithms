#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "Graph.h"

// DFS 輔助函數：返回 true 表示發現循環
bool DFS(Graph g, int v, int parent, bool *visited, int n) {
    visited[v] = true;  // 標記當前節點為已訪問
    for (int w = 0; w < n; w++) {
        if (adjacent(g, v, w)) {  // 檢查 v 和 w 是否相鄰
            if (!visited[w]) {  // 如果 w 未被訪問
                if (DFS(g, w, v, visited, n))  // 遞歸訪問 w
                    return true;  // 發現循環
            } else if (w != parent) {  // 如果 w 已訪問且不是父節點
                return true;  // 發現循環
            }
        }
    }
    return false;  // 未發現循環
}

// 檢查圖是否有循環
bool hasCycle(Graph g, int n) {
    bool *visited = calloc(n, sizeof(bool));  // 初始化 visited 陣列
    if (visited == NULL) {
        return false;  // 內存分配失敗
    }

    for (int v = 0; v < n; v++) {
        if (!visited[v]) {  // 如果 v 未被訪問
            if (DFS(g, v, -1, visited, n)) {  // 從 v 開始 DFS
                free(visited);
                return true;  // 發現循環
            }
        }
    }
    free(visited);
    return false;  // 未發現循環
}

int main(void) {
    int n;
    printf("Enter the number of vertices: ");
    if (scanf("%d", &n) != 1 || n < 1) {
        return EXIT_FAILURE;  // 無效的節點數量
    }

    Graph g = newGraph(n);  // 創建圖
    if (g == NULL) {
        return EXIT_FAILURE;  // 創建圖失敗
    }

    // 讀取邊，直到遇到非數字輸入
    int from, to;
    Edge e;
    while (1) {
        printf("Enter an edge (from): ");
        if (scanf("%d", &from) != 1)
            break;
        printf("Enter an edge (to): ");
        if (scanf("%d", &to) != 1)
            break;
        e.v = from;
        e.w = to;
        insertEdge(g, e);  // 插入邊
    }
    printf("Done.\n");

    // 檢查圖是否有循環
    if (hasCycle(g, n))
        printf("The graph has a cycle.\n");
    else
        printf("The graph is acyclic.\n");

    freeGraph(g);  // 釋放圖的內存
    return EXIT_SUCCESS;
}