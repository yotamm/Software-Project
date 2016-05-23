#ifndef SPBPRIORITYQUEUE_H_
#define SPBPRIORITYQUEUE_H_
#include "SPListElement.h"
#include <stdbool.h>
/**
 * SP Bounded Priority Queue summary
 *
 * TODO Complete documentation
 */


/** type used to define Bounded priority queue **/
typedef struct sp_bp_queue_t* SPBPQueue;

/** type for error reporting **/
typedef enum sp_bp_queue_msg_t {
	SP_BPQUEUE_OUT_OF_MEMORY,
	SP_BPQUEUE_FULL,
	SP_BPQUEUE_EMPTY,
	SP_BPQUEUE_INVALID_ARGUMENT,
	SP_BPQUEUE_SUCCESS
} SP_BPQUEUE_MSG;

/*
 * creates an empty queue with a given maximum capacity
 *
 * @param maxSize is maximum capacity
 * @return new empty queue with a given maximum capacity
 */
SPBPQueue spBPQueueCreate(int maxSize);

/*
 * creates a copy of a given queue
 *
 * @param source is given queue
 * @return SPBPQueue which is a copy of source
 */
SPBPQueue spBPQueueCopy(SPBPQueue source);

/*
 * frees all memory allocation associated with the queue
 *
 * @param source is the queue to free memmory allocs from
 * @return
 */
void spBPQueueDestroy(SPBPQueue source);

/*
 * removes all the elements in the queue
 *
 * @param soure is the queue to remove all elems from
 * @return
 */
void spBPQueueClear(SPBPQueue source);

/*
 * returns the number of elements in the queue
 *
 * @param queue whom we count elems from
 * @return number of elements in the queue
 */
int spBPQueueSize(SPBPQueue source);

/*
 * returns the maximum capacity of the queue
 *
 * @param source whom we want his max cap'
 * @return source's max cap'
 */
int spBPQueueGetMaxSize(SPBPQueue source);

/*
 * Inserts a new copy (which is allocated) element to the queue
 *
 * @param source - the queue we enter the new elemnt into
 * @param element - the element we enter to source
 * @return msg containing info on the insert
 */
SP_BPQUEUE_MSG spBPQueueEnqueue(SPBPQueue source, SPListElement element);

/*
 * removes the element with the lowest value
 *
 * @param source - the queue we remove the element with the lowest value
 * @return msg with info on
 */
SP_BPQUEUE_MSG spBPQueueDequeue(SPBPQueue source);

/*
 * returns a new copy of the element with the lowest value
 *
 * @param source - queue we get element with the lowest value from
 * @return a new copy of the element with the lowest value
 */
SPListElement spBPQueuePeek(SPBPQueue source);

/*
 * returns a new copy of the element with the highest value
 *
 * @param source - queue we get element with the highest value from
 * @return a new copy of the element with the highest value
 */
SPListElement spBPQueuePeekLast(SPBPQueue source);

/*
 * returns the minimum value in the queue
 *
 * @param source the queue we get value from
 * @return the min value in the queue
 */
double spBPQueueMinValue(SPBPQueue source);

/*
 * returns the maximum value in the queue
 *
 * @param source the queue we get value from
 * @return the max value in the queue
 */
double spBPQueueMaxValue(SPBPQueue source);

/*
 * checks if queue is empty
 *
 * @param queue to check
 * @return true if the queue is empty, false otherwise
 */
bool spBPQueueIsEmpty(SPBPQueue source);

/*
 * checks if queue is full
 *
 * @param queue to check
 * @return true if the queue is full, false otherwise
 */
bool spBPQueueIsFull(SPBPQueue source);

#endif
