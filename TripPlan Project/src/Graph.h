// Graph.h
#ifndef GRAPH_H
#define GRAPH_H

/**
 * @struct GraphRep
 * @brief Adjacency matrix representation of a graph
 */
typedef struct GraphRep
{
    int nV;       // Number of vertices (landmarks) in graph
    int *visited; // Visited marker array (0=unvisited, 1=visited)
    int *parent;  // Predecessor array (-1 indicates no predecessor)
    int **edges;  // adjacency matrix storing walking times between vertices
} *Graph;

/**
 * @brief Create and initialize adjacency matrix graph
 * @param nV Number of vertices in graph
 * @return Graph Initialized graph pointer
 */
Graph newGraph(int nV);

/**
 * @brief Insert/update edge in undirected graph
 * @param G Target graph pointer
 * @param u Source vertex index
 * @param v Destination vertex index
 * @param walkTime Walking time in mins
 * @note Sets both edges[u][v] and edges[v][u]
 */
void insertEdge(Graph G, int u, int v, int walkTime);

/**
 * @brief Free memory used by graph
 * @param G Graph pointer to free
 */
void freeGraph(Graph G);

#endif // GRAPH_H