// Graph.c
#include "Graph.h"
#include <stdlib.h>

/**
 * @brief Allocate and initialize components of a graph
 * @param nV Number of vertices
 * @return Graph Returns graph pointer on success, NULL on failure
 */
Graph newGraph(int nV)
{
    // Allocate graph structure
    Graph G = malloc(sizeof *G);
    if (!G)
    {
        return NULL;
    }
    G->nV = nV;
    // Allocate and initialize visited array
    G->visited = calloc(nV, sizeof(int));
    if (!G->visited)
    {
        goto cleanup;
    }
    // Allocate and initialize predecessor array
    G->parent = malloc(nV * sizeof(int));
    if (!G->parent)
    {
        goto cleanup;
    }
    for (int i = 0; i < nV; i++)
    {
        G->parent[i] = -1; // -1 indicates no predecessor
    }
    // Allocate row pointer array for adjacency matrix
    G->edges = malloc(nV * sizeof(int *));
    if (!G->edges)
    {
        goto cleanup;
    }
    // Initialize adjacency matrix
    for (int i = 0; i < nV; i++)
    {
        G->edges[i] = malloc(nV * sizeof(int));
        if (!G->edges[i])
        {
            goto cleanup;
        }
        // Initialize all edges as disconnected
        for (int j = 0; j < nV; j++)
        {
            G->edges[i][j] = -1;
        }
    }
    return G;

cleanup:
    // Cleanup on allocation failure
    freeGraph(G);
    return NULL;
}

/**
 * @brief Update edge weight in undirected graph
 * @param G Target graph pointer
 * @param u Source vertex index (0 ≤ u < nV)
 * @param v Destination vertex index (0 ≤ v < nV)
 * @param walkTime walkTime New walking time (mins), set to -1 to remove edge
 */
void insertEdge(Graph G, int u, int v, int walkTime)
{
    // Check vertex indices
    if (u < 0 || u >= G->nV || v < 0 || v >= G->nV)
        return;
    // Set edge weight
    G->edges[u][v] = walkTime;
    G->edges[v][u] = walkTime;
}

/**
 * @brief Free all memory allocated
 * @param G Graph pointer to free
 */
void freeGraph(Graph G)
{
    if (!G)
        return;
    // Free each row of adjacency matrix
    if (G->edges)
    {
        for (int i = 0; i < G->nV; i++)
        {
            free(G->edges[i]);
        }
        free(G->edges);
    }
    // Free DFS-related arrays
    free(G->visited);
    free(G->parent);
    free(G); // Free structure
}