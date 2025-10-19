#include <stdio.h>
#include <stdlib.h>
#include "WGraph.h"

int main(void)
{
    int V;
    printf("Enter the number of vertices: ");
    if (scanf("%d", &V) != 1 || V <= 0)
    {
        return EXIT_FAILURE;
    }

    Graph g = newGraph(V);
    if (!g)
    {
        return EXIT_FAILURE;
    }

    int *indegree = NULL, *outdegree = NULL, *vertices = NULL;
    double *popularity = NULL;

    /* 一次性分配所有需要的内存 */
    if (!(indegree = calloc(V, sizeof(int))))
        goto cleanup;
    if (!(outdegree = calloc(V, sizeof(int))))
        goto cleanup;
    if (!(popularity = malloc(V * sizeof(double))))
        goto cleanup;
    if (!(vertices = malloc(V * sizeof(int))))
        goto cleanup;

    /* 读取边并更新度数 */
    int from, to;
    printf("Enter an edge (from): ");
    while (scanf("%d", &from) == 1)
    {
        printf("Enter an edge (to): ");
        if (scanf("%d", &to) != 1)
        {
            break;
        }
        if (from >= 0 && from < V && to >= 0 && to < V)
        {
            Edge e = {from, to, 1};
            insertEdge(g, e);
            outdegree[from]++;
            indegree[to]++;
        }
        printf("Enter an edge (from): ");
    }
    printf("Done.\n");

    /* 计算流行度并初始化顶点数组 */
    for (int v = 0; v < V; v++)
    {
        if (outdegree[v] == 0)
        {
            popularity[v] = indegree[v] / 0.5;
        }
        else
        {
            popularity[v] = (double)indegree[v] / outdegree[v];
        }
        vertices[v] = v;
    }

    /* 插入排序：按流行度降序，流行度相同时按顶点号升序 */
    for (int i = 1; i < V; i++)
    {
        int key = vertices[i];
        double key_pop = popularity[key];
        int j = i - 1;
        while (j >= 0 &&
               (popularity[vertices[j]] < key_pop ||
                (popularity[vertices[j]] == key_pop && vertices[j] > key)))
        {
            vertices[j + 1] = vertices[j];
            j--;
        }
        vertices[j + 1] = key;
    }

    /* 输出结果 */
    printf("\nPopularity ranking:\n");
    for (int i = 0; i < V; i++)
    {
        printf("%d %.1f\n", vertices[i], popularity[vertices[i]]);
    }

cleanup:
    /* 统一释放资源 */
    free(indegree);
    free(outdegree);
    free(popularity);
    free(vertices);
    if (g)
        freeGraph(g);

    return EXIT_SUCCESS;
}