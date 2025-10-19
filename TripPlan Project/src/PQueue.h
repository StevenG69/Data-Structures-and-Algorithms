// PQueue.h
#ifndef PQUEUE_H
#define PQUEUE_H

/**
 * @struct HeapNode
 * @brief Priority queue node structure, used for min-heap storage
 */
typedef struct
{
    int landmarkIndex; // Index position of the landmark in the array
    int arrivalTime;   // Arrival time at the landmark (in minutes)
} HeapNode;

/*  Global heap array pointer (defined in PQueue.c) */
extern HeapNode *minHeap; // Dynamically allocated min-heap array
extern int minHeapSize;   // Current number of valid elements in heap

/**
 * @brief Insert a new node into the min-heap
 * @param landmarkIndex Index of the landmark to insert
 * @param arrivalTime Arrival time at the landmark
 * @note Automatically heapify-up after insertion to maintain heap properties
 */
void minHeapPush(int landmarkIndex, int arrivalTime);

/**
 * @brief Pop out smallest element from the heap top
 * @return HeapNode Heap node containing the minimum arrival time
 * @note Automatically heapify-down after popping to maintain heap properties
 */
HeapNode minHeapPop(void);

#endif // PQUEUE_H