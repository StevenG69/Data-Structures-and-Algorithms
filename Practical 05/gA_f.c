#include <stdio.h>
#include <stdlib.h>
#include "Graph.h"

int *calculateDegrees(Graph g, int n)
{
    int *degree = calloc(n, sizeof(int));
    if (degree == NULL)
    {
        fprintf(stderr, "Memory allocation error.\n");
        return NULL;
    }

    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            if (adjacent(g, i, j))
            {
                degree[i]++;
            }
        }
    }
    return degree;
}

void findAndPrint3Cliques(Graph g, int n)
{
    printf("3-cliques:\n");

    for (int i = 0; i < n - 2; i++)
    {
        for (int j = i + 1; j < n - 1; j++)
        {
            if (!adjacent(g, i, j))
                continue;
            for (int k = j + 1; k < n; k++)
            {
                if (adjacent(g, i, k) && adjacent(g, j, k))
                {
                    printf("%d-%d-%d\n", i, j, k);
                }
            }
        }
    }
}

double calculateDensity(int *degree, int n)
{
    int totalDegree = 0;
    for (int i = 0; i < n; i++)
    {
        totalDegree += degree[i];
    }
    int numEdges = totalDegree / 2;
    return (2.0 * numEdges) / (n * n);
}

int main(void)
{
    int n;
    printf("Enter the number of vertices: ");
    if (scanf("%d", &n) != 1 || n < 1)
    {
        fprintf(stderr, "Invalid number of vertices.\n");
        return EXIT_FAILURE;
    }

    Graph g = newGraph(n);
    if (g == NULL)
    {
        fprintf(stderr, "Failed to create graph.\n");
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

    int *degree = calculateDegrees(g, n);
    if (degree == NULL)
    {
        freeGraph(g);
        return EXIT_FAILURE;
    }

    for (int i = 0; i < n; i++)
    {
        printf("Degree of node %d: %d\n", i, degree[i]);
    }

    findAndPrint3Cliques(g, n);

    double density = calculateDensity(degree, n);
    printf("Density: %.3f\n", density);

    free(degree);
    freeGraph(g);
    return EXIT_SUCCESS;
}