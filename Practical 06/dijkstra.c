#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "PQueue.h"

#define VERY_HIGH_VALUE 999999
#define MAX_NODES 1000

void printPath(int pred[], int v)
{
    int path[MAX_NODES];
    int len = 0;
    while (v != -1)
    {
        path[len++] = v;
        v = pred[v];
    }
    for (int i = len - 1; i >= 0; i--)
    {
        printf(i == len - 1 ? "%d" : "-%d", path[i]);
    }
}

void dijkstraSSSP(Graph g, Vertex source)
{
    int dist[MAX_NODES];
    int pred[MAX_NODES];
    bool vSet[MAX_NODES];
    int nV = numOfVertices(g);

    for (int i = 0; i < nV; i++)
    {
        dist[i] = VERY_HIGH_VALUE;
        pred[i] = -1;
        vSet[i] = true;
    }

    dist[source] = 0;
    PQueueInit();
    joinPQueue(source);

    while (!PQueueIsEmpty())
    {
        int u = leavePQueue(dist);
        if (!vSet[u])
            continue;
        vSet[u] = false;

        for (int v = 0; v < nV; v++)
        {
            int w = adjacent(g, u, v);
            if (w > 0 && vSet[v])
            {
                int alt = dist[u] + w;
                if (alt < dist[v])
                {
                    dist[v] = alt;
                    pred[v] = u;
                    joinPQueue(v);
                }
            }
        }
    }

    for (int v = 0; v < nV; v++)
    {
        if (dist[v] == VERY_HIGH_VALUE)
        {
            printf("%d: no path\n", v);
        }
        else
        {
            printf("%d: distance = %d, shortest path: ", v, dist[v]);
            printPath(pred, v);
            printf("\n");
        }
    }
}

void reverseEdge(Edge *e)
{
    Vertex temp = e->v;
    e->v = e->w;
    e->w = temp;
}

int main(void)
{
    Edge e;
    int n, source;

    printf("Enter the number of vertices: ");
    scanf("%d", &n);
    Graph g = newGraph(n);

    printf("Enter the source node: ");
    scanf("%d", &source);
    printf("Enter an edge (from): ");
    while (scanf("%d", &e.v) == 1)
    {
        printf("Enter an edge (to): ");
        scanf("%d", &e.w);
        printf("Enter the weight: ");
        scanf("%d", &e.weight);
        insertEdge(g, e);
        reverseEdge(&e); // ensure to add edge in both directions
        insertEdge(g, e);
        printf("Enter an edge (from): ");
    }
    printf("Done.\n");

    dijkstraSSSP(g, source);
    freeGraph(g);
    return 0;
}