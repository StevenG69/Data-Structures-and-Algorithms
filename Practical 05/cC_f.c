#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "Graph.h"

bool DFS(Graph g, int v, int parent, bool *visited, int n)
{
    visited[v] = true; 
    for (int w = 0; w < n; w++)
    {
        if (adjacent(g, v, w))
        {
            if (!visited[w])
            {                                 
                if (DFS(g, w, v, visited, n)) 
                    return true;              
            }
            else if (w != parent)
            {                
                return true; 
            }
        }
    }
    return false; 
}

bool hasCycle(Graph g, int n)
{
    bool *visited = calloc(n, sizeof(bool)); 
    if (visited == NULL)
    {
        return false; 
    }

    for (int v = 0; v < n; v++)
    {
        if (!visited[v])
        { 
            if (DFS(g, v, -1, visited, n))
            { 
                free(visited);
                return true; 
            }
        }
    }
    free(visited);
    return false; 
}

int main(void)
{
    int n;
    printf("Enter the number of vertices: ");
    if (scanf("%d", &n) != 1 || n < 1)
    {
        return EXIT_FAILURE; 
    }

    Graph g = newGraph(n); 
    if (g == NULL)
    {
        return EXIT_FAILURE; 
    }

    int from, to;
    Edge e;
    while (1)
    {
        printf("Enter an edge (from): ");
        if (scanf("%d", &from) != 1)
            break;
        printf("Enter an edge (to): ");
        if (scanf("%d", &to) != 1)
            break;
        e.v = from;
        e.w = to;
        insertEdge(g, e);
    }
    printf("Done.\n");

    if (hasCycle(g, n))
        printf("The graph has a cycle.\n");
    else
        printf("The graph is acyclic.\n");

    freeGraph(g); 
    return EXIT_SUCCESS;
}