// PQueue.c
#include "PQueue.h"
#include <stdlib.h>

// Global Heap definition
HeapNode *minHeap = NULL; // Allocated heap array, stores HeapNode elements
int minHeapSize = 0;      // Count of valid elements in current heap

/**
 * @brief Internal heap up function
 * @param i Node index that needs adjustment
 */
static void heapifyUp(int i)
{
    while (i > 0)
    {
        int parent = (i - 1) / 2;
        if (minHeap[parent].arrivalTime <= minHeap[i].arrivalTime)
            break;
        // Swap parent and child nodes
        HeapNode temp = minHeap[parent];
        minHeap[parent] = minHeap[i];
        minHeap[i] = temp;
        i = parent;
    }
}

/**
 * @brief Internal heap down function
 * @param i Node index that needs adjustment
 */
static void heapifyDown(int i)
{
    while (1)
    {
        int left = 2 * i + 1;
        int right = 2 * i + 2;
        int smallest = i;

        // Find the smallest node among the three
        if (left < minHeapSize &&
            minHeap[left].arrivalTime < minHeap[smallest].arrivalTime)
        {
            smallest = left;
        }
        if (right < minHeapSize &&
            minHeap[right].arrivalTime < minHeap[smallest].arrivalTime)
        {
            smallest = right;
        }

        // Current node meets the heap characteristic
        if (smallest == i)
            break;
        // Swap nodes
        HeapNode temp = minHeap[i];
        minHeap[i] = minHeap[smallest];
        minHeap[smallest] = temp;
        i = smallest;
    }
}

void minHeapPush(int landmarkIndex, int arrivalTime)
{
    // Add new element to the end of the heap
    int i = minHeapSize++;
    minHeap[i].landmarkIndex = landmarkIndex;
    minHeap[i].arrivalTime = arrivalTime;
    // Adjust upwards starting from new position
    heapifyUp(i);
}

HeapNode minHeapPop(void)
{
    HeapNode top = minHeap[0]; // Store the top element of heap
    // Replace top of heap with last element
    minHeap[0] = minHeap[--minHeapSize];
    heapifyDown(0); // Adjust downwards starting from heap top
    return top;
}