// tripPlan.c

/*
The analysis considers the worst-case time complexity:
l: the number of landmarks (vertices)
w: the number of walking links (undirected edges)
f: the number of ferry schedules (directed edges/events)

1. Graph Initialization: Allocate and initialize l×l adjacency matrix -> O(l^2)
2. Loading Walking Links: Reading w links and calling insertEdge -> O(w)
3. Loading Ferry Schedules: Go through ferries per node -> O(f)
Allocating adjacency lists -> O(f); In conclusion -> O(f)
4. Searching Shortest‑Path: Initialize arrays -> O(l)
Extractions from the min‑heap -> O(logl)
All possible walking neighbors (matrix) -> O(l^2)
All outgoing ferries from that node -> O(f)
Heap insertions for relaxations -> (w+f)logl; In conclusion -> O(l^2+(w+f)logl)
DFS for pure walking -> O(l^2)
5. The worst cases per query -> O(l^2+(w+f)logl)
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "PQueue.h"
#include "Graph.h"

/* —————————— Macro Definitions —————————— */
#define MAX_LANDMARKS 999 // Maximum number of landmarks
#define MAX_NAME_LEN 32   // Maximum length for landmark names (including /0)
#define INF 0x2f3f3f3f    // Representing unreachable nodes

/* —————————— Ferry Schedule Structure —————————— */
typedef struct FerrySchedule
{
    int depIndex; // Index of departure landmark
    int arrIndex; // Index of arrival landmark
    int depTime;  // Departure time in minutes
    int arrTime;  // Arrival time in minutes
} FerryS;

/* —————————— Global State Structure —————————— */
typedef struct
{
    /* — Landmark Info — */
    int L;                                       // Total number of landmarks
    char landmarks[MAX_LANDMARKS][MAX_NAME_LEN]; // Array storing landmark names
    int currentDepTime;                          // Departure time for current query

    /* — Transportation networks — */
    Graph walkG;       // Graph representing walking connections
    int *ferryCount;   // Array counting ferries for every landmark
    FerryS **ferryAdj; // Adjacency list for ferry schedules

    /* — Dijkstra algorithm data structures — */
    HeapNode *minHeap;                  // Min Heap (Priority queue)
    int minHeapSize;                    // Current size of priority queue
    bool finalized[MAX_LANDMARKS];      // Marks finalized nodes
    int earliestArrival[MAX_LANDMARKS]; // Earliest arrival times
    int arrivalMode[MAX_LANDMARKS];     // 1=walk, 2=ferry for arrival
    int prevNode[MAX_LANDMARKS];        // Predecessor nodes in optimal path
    int walkDuration[MAX_LANDMARKS];    // Walk durations in mins
    int ferryDepartTime[MAX_LANDMARKS]; // Ferry departure times in mins
    int ferryArriveTime[MAX_LANDMARKS]; // Ferry arrival times in mins
} State;

/* —————————— Time Conversion Functions —————————— */
/**
 * @brief Converts HHMM time string to minutes
 * @param time Time string in HHMM format (e.g., "1230")
 * @return Integer representing minutes
 */
static int HHMM_to_minutes(const char *time)
{
    int hours = (time[0] - '0') * 10 + (time[1] - '0');
    int mins = (time[2] - '0') * 10 + (time[3] - '0');
    return hours * 60 + mins;
}

/**
 * @brief Converts minutes to HHMM format as integer
 * @param minutes Time in minutes
 * @return Integer representing time in HHMM format (e.g., 930 for 9:30 AM)
 */
static int minutes_to_HHMM(int minutes)
{
    int hours = minutes / 60;
    int mins = minutes % 60;
    return hours * 100 + mins;
}

/* —————————— Landmark Search Function —————————— */
/**
 * @brief Finds index of a landmark by name
 * @param st Pointer to State structure
 * @param lName Landmark name to search for
 * @return Index of landmark or -1 if not found
 */
static int getLandmark(State *st, const char *lName)
{
    for (int i = 0; i < st->L; i++)
    {
        if (strcmp(st->landmarks[i], lName) == 0)
        {
            return i; // Return index matches
        }
    }
    return -1; // Not found
}

/* —————————— Relaxation  Edge Function —————————— */
/**
 * @brief Updates the shortest path to a node if a better path is found
 * @param st Pointer to State structure
 * @param from Index of source landmark
 * @param to Index of destination landmark
 * @param depart Departure time from source
 * @param arrive Arrival time at destination
 * @param mode Transportation mode (1=walk, 2=ferry)
 */
static void relaxEdge(State *st, int from, int to, int depart, int arrive, int mode)
{
    // Only update if node not finalized and new arrival time is better
    if (!st->finalized[to] && arrive < st->earliestArrival[to])
    {
        st->earliestArrival[to] = arrive; // Update best arrival time
        st->prevNode[to] = from;          // Update path predecessor
        st->arrivalMode[to] = mode;       // Record arrival method

        if (mode == 1)
        {
            st->walkDuration[to] = arrive - depart; // Store walk duration
        }
        else
        {
            st->ferryDepartTime[to] = depart; // Store ferry departure time
            st->ferryArriveTime[to] = arrive; // Store ferry arrival time
        }
        minHeapPush(to, arrive); // Add to PQueue for further processing
    }
}

/* —————————— Find Shortest Route Function —————————— */
/**
 * @brief Finds fastest route using both walking and ferries
 * @param st Pointer to State structure
 * @param src Index of source landmark
 * @param dst Index of destination landmark
 * @return true if route found, false otherwise
 */
static bool findRoute(State *st, int src, int dst)
{
    for (int i = 0; i < st->L; i++) // Initialize nodes
    {
        st->finalized[i] = false;     // No nodes finalized yet
        st->earliestArrival[i] = INF; // All nodes unreachable in default
        st->arrivalMode[i] = 0;       // No arrival mode set
    }

    // Start with source node at departure time
    st->earliestArrival[src] = st->currentDepTime;
    minHeapSize = 0; // Reset priority queue
    minHeapPush(src, st->currentDepTime);

    // Main algorithm loop
    while (minHeapSize > 0)
    {
        // Get node with earliest arrival time
        HeapNode entry = minHeapPop(); // Pop up earliest arriving node
        int u = entry.landmarkIndex;   // Pop up current node index
        int t = entry.arrivalTime;     // Current node arrival time

        if (st->finalized[u])
            continue;
        st->finalized[u] = true; // Marks as finalized

        if (u == dst)
            break; // Break if reached the destination

        // Relax all walking edges from current node
        for (int v = 0; v < st->L; v++)
        {
            int w = st->walkG->edges[u][v];
            if (w >= 0) // If walking connection exists
            {
                relaxEdge(st, u, v, t, t + w, 1); // Mode 1 = walking
            }
        }
        // Relax all ferry connections from current node
        for (int i = 0; i < st->ferryCount[u]; i++)
        {
            FerryS *f = &st->ferryAdj[u][i];
            if (f->depTime >= t) // Only consider ferries that depart after current time
            {
                relaxEdge(st, u, f->arrIndex, f->depTime, f->arrTime, 2); // Mode 2 = ferry
            }
        }
    }

    // Check if destination was reached
    if (st->earliestArrival[dst] == INF)
        return false;

    // Reconstruct the path from destination to source
    int routeNode[MAX_LANDMARKS]; // Nodes in the path
    int routeMode[MAX_LANDMARKS]; // Arrival modes (1=walk, 2=ferry)
    int routeWalk[MAX_LANDMARKS]; // Walk durations
    int routeFdep[MAX_LANDMARKS]; // Ferry departure times
    int routeFarr[MAX_LANDMARKS]; // Ferry arrival times
    int len = 0;                  // Path length

    // Backtrack from destination to source
    for (int v = dst; v != src;)
    {
        routeNode[len] = v;
        routeMode[len] = st->arrivalMode[v];
        if (routeMode[len] == 1)
        {
            routeWalk[len] = st->walkDuration[v];
            v = st->prevNode[v]; // Move to predecessor
        }
        else
        {
            routeFdep[len] = st->ferryDepartTime[v];
            routeFarr[len] = st->ferryArriveTime[v];
            v = st->prevNode[v]; // Move to predecessor
        }
        len++;
    }
    routeNode[len] = src; // Add source node
    routeMode[len] = 0;   // No arrival mode for source
    len++;

    // Print the path (source to destination)
    int now = st->currentDepTime;
    printf("\n");
    for (int i = len - 1; i > 0; i--)
    {
        int u = routeNode[i], v = routeNode[i - 1];
        if (routeMode[i - 1] == 1)
        {
            // Walking segment
            int w = routeWalk[i - 1];
            printf("Walk %d minute(s):\n", w);
            printf("  %04d %s\n", minutes_to_HHMM(now), st->landmarks[u]);
            now += w;
            printf("  %04d %s\n", minutes_to_HHMM(now), st->landmarks[v]);
        }
        else
        {
            // Ferry segment
            int d = routeFdep[i - 1], a = routeFarr[i - 1];
            printf("Ferry %d minute(s):\n", a - d);
            printf("  %04d %s\n", minutes_to_HHMM(d), st->landmarks[u]);
            printf("  %04d %s\n", minutes_to_HHMM(a), st->landmarks[v]);
            now = a;
        }
    }
    return true;
}

/* —————————— Walking Path Search（DFS） —————————— */
/**
 * @brief Depth-first search to find walking path
 * @param st Pointer to State structure
 * @param curV Current landmark index
 * @param tarV Target landmark index
 * @return true if path found, false otherwise
 */
static bool dfs(State *st, int curV, int tarV)
{
    if (curV == tarV) // Base case: found target
        return true;

    st->walkG->visited[curV] = 1; // Mark current node as visited

    // Go through all neighbors
    for (int nxt = 0; nxt < st->walkG->nV; nxt++)
    {
        if (st->walkG->edges[curV][nxt] >= 0 && !st->walkG->visited[nxt])
        {
            st->walkG->parent[nxt] = curV; // Records parent nodes
            if (dfs(st, nxt, tarV))        // Recursively search
                return true;
        }
    }
    return false; // Target not found from this path
}

/**
 * @brief Prints walking path found by DFS
 * @param st Pointer to State structure
 * @param src Source landmark index
 * @param dst Destination landmark index
 */
static void printWalkPath(State *st, int src, int dst)
{
    int path[MAX_LANDMARKS], len = 0;
    // Reconstruct path from destination to source
    for (int cur = dst; cur != -1; cur = st->walkG->parent[cur])
    {
        path[len++] = cur;
    }
    int curTime = st->currentDepTime;
    printf("\n");
    // Print every walking information
    for (int i = 0; i < len - 1; i++)
    {
        int u = path[i], v = path[i + 1];
        int walkTime = st->walkG->edges[u][v];
        int arrTime = curTime + walkTime;
        printf("Walk %d minute(s):\n", walkTime);
        printf("  %04d %s\n", minutes_to_HHMM(curTime), st->landmarks[u]);
        printf("  %04d %s\n", minutes_to_HHMM(arrTime), st->landmarks[v]);
        curTime = arrTime;
    }
}

/* —————————— Data Loading Functions —————————— */
/**
 * @brief Loads landmark names from input
 * @param st Pointer to State structure
 */
static void loadLandmarks(State *st)
{
    for (int i = 0; i < st->L; i++)
        scanf("%31s", st->landmarks[i]); // Load landmark names
}

/**
 * @brief Loads walking connections between landmarks
 * @param st Pointer to State structure
 * @param W Number of walking links to load
 */
static void loadWalkingLinks(State *st, int W)
{
    for (int i = 0; i < W; i++)
    {
        char lName_a[MAX_NAME_LEN], lName_b[MAX_NAME_LEN];
        int walkT;
        scanf("%31s %31s %d", lName_a, lName_b, &walkT);
        int idxa = getLandmark(st, lName_a), idxb = getLandmark(st, lName_b);
        insertEdge(st->walkG, idxa, idxb, walkT);
    }
}

/**
 * @brief Loads ferry schedules between landmarks
 * @param st Pointer to State structure
 * @param F Number of ferry schedules to load
 */
static void loadFerrySchedules(State *st, int F)
{
    FerryS *allferries = malloc(F * sizeof *allferries);

    // Count ferries per departure point
    for (int i = 0; i < F; i++)
    {
        char dep[MAX_NAME_LEN], dept[5], arr[MAX_NAME_LEN], arrt[5];
        scanf("%31s %4s %31s %4s", dep, dept, arr, arrt);
        allferries[i].depIndex = getLandmark(st, dep);
        allferries[i].arrIndex = getLandmark(st, arr);
        allferries[i].depTime = HHMM_to_minutes(dept);
        allferries[i].arrTime = HHMM_to_minutes(arrt);
        st->ferryCount[allferries[i].depIndex]++;
    }

    // Allocate ferry adjacency lists
    for (int u = 0; u < st->L; u++)
    {
        if (st->ferryCount[u] > 0)
            st->ferryAdj[u] = malloc(st->ferryCount[u] * sizeof(FerryS));
        st->ferryCount[u] = 0; // Reset counter
    }

    // Populate ferry adjacency lists
    for (int i = 0; i < F; i++)
    {
        int u = allferries[i].depIndex;
        st->ferryAdj[u][st->ferryCount[u]++] = allferries[i];
    }
    free(allferries); // Free temporary array
}

/* —————————— Query Handling Function —————————— */
/**
 * @brief Handles a single route query
 * @param st Pointer to State structure
 * @param from Source landmark name
 * @param to Destination landmark name
 * @param HHMM Departure time in HHMM format
 */
static void handleQuery(State *st, const char *from, const char *to, const char *HHMM)
{
    st->currentDepTime = HHMM_to_minutes(HHMM);
    int src = getLandmark(st, from), dst = getLandmark(st, to);

    // Check for direct walking connection first
    if (st->walkG->edges[src][dst] >= 0)
    {
        int w = st->walkG->edges[src][dst];
        printf("\nWalk %d minute(s):\n", w);
        printf("  %04d %s\n", minutes_to_HHMM(st->currentDepTime), from);
        printf("  %04d %s\n", minutes_to_HHMM(st->currentDepTime + w), to);
        return;
    }

    // Try to find mixed walking/ferry route
    if (findRoute(st, src, dst))
        return;

    // If no mixed route, try pure walking path using DFS
    memset(st->walkG->visited, 0, st->L * sizeof(int));
    memset(st->walkG->parent, -1, st->L * sizeof(int));
    if (dfs(st, src, dst))
    {
        printWalkPath(st, src, dst);
    }
    else
    {
        printf("\nNo route.\n");
    }
}

/* —————————— Resource Free Function —————————— */
/**
 * @brief Frees all dynamically allocated memory
 * @param st Pointer to State structure
 */
static void cleanup(State *st)
{
    // Free ferry adjacency lists memory
    for (int u = 0; u < st->L; u++)
        free(st->ferryAdj[u]);
    free(st->ferryAdj);
    free(st->ferryCount);
    free(minHeap);        // Free priority queue memory
    freeGraph(st->walkG); // Free graph memory
}

/* —————————— Main Function —————————— */
int main(void)
{
    State st;
    int W, F;

    printf("Number of landmarks: ");
    scanf("%d", &st.L);

    // Initialize landmark count and data structures
    st.walkG = newGraph(st.L);                       // Create graph for walking connections
    st.ferryCount = calloc(st.L, sizeof(int));       // Initialize ferry counts
    st.ferryAdj = calloc(st.L, sizeof(FerryS *));    // Allocate ferry adjacency lists
    minHeap = malloc((st.L + 5) * sizeof(HeapNode)); // Allocate priority queue
    minHeapSize = 0;                                 // Allocate heap size

    // Load landmark data
    loadLandmarks(&st);

    // Load walking connections
    printf("Number of walking links: ");
    scanf("%d", &W);
    loadWalkingLinks(&st, W);

    // Load ferry schedules
    printf("Number of ferry schedules: ");
    scanf("%d", &F);
    loadFerrySchedules(&st, F);

    // Process queries until user enters "done"
    while (true)
    {
        char from[MAX_NAME_LEN], to[MAX_NAME_LEN], HHMM[5];
        printf("\nFrom: ");
        if (scanf("%31s", from) != 1 || strcmp(from, "done") == 0)
        {
            printf("Happy travels!\n");
            break;
        }
        printf("To: ");
        scanf("%31s", to);
        printf("Departure time: ");
        scanf("%4s", HHMM);
        handleQuery(&st, from, to, HHMM);
    }

    // Clean up before exiting
    cleanup(&st);
    return 0;
}
