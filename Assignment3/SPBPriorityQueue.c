#include "SPBPriorityQueue.h"
#include "SPList.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

struct sp_bp_queue_t {
	int maxSize;
	int currentSize;
	SPList boundedPriorityQueue;
};

/*
 * creates an empty queue with a given maximum capacity
 *
 * @param maxSize is maximum capacity
 * @return new empty queue with a given maximum capacity
 */
SPBPQueue spBPQueueCreate(int maxSize) {
	SPBPQueue queue = (SPBPQueue) malloc(sizeof(struct sp_bp_queue_t));
	queue->currentSize = 0;
	queue->maxSize = maxSize;
	queue->boundedPriorityQueue = spListCreate();
	return queue;
	//TODO deal with errors
}

/*
 * creates a copy of a given queue
 *
 * @param source is given queue
 * @return SPBPQueue which is a copy of source
 */
SPBPQueue spBPQueueCopy(SPBPQueue source) {
	SPBPQueue queue = (SPBPQueue) malloc(sizeof(struct sp_bp_queue_t));
	queue->currentSize = source->currentSize;
	queue->maxSize = source->maxSize;
	queue->boundedPriorityQueue = spListCopy(source->boundedPriorityQueue);
	return queue;
	//TODO deal with errors
}

/*
 * frees all memory allocation associated with the queue
 *
 * @param source is the queue to free memmory allocs from
 * @return
 */
void spBPQueueDestroy(SPBPQueue source) {
	spListDestroy(source->boundedPriorityQueue);
	free(source);
}

/*
 * removes all the elements in the queue
 *
 * @param soure is the queue to remove all elems from
 * @return
 */
void spBPQueueClear(SPBPQueue source) {
	if(source == NULL)
		return;
	source->currentSize = 0; //set size to be 0 since no elems in queue
	if(!(source->boundedPriorityQueue == NULL))
		//clear queue if it's not null
		spListClear(source->boundedPriorityQueue);
}

/*
 * returns the number of elements in the queue
 *
 * @param queue whom we count elems from
 * @return number of elements in the queue
 */
int spBPQueueSize(SPBPQueue source) {
	return source->currentSize;
}

/*
 * returns the maximum capacity of the queue
 *
 * @param source whom we want his max cap'
 * @return source's max cap'
 */
int spBPQueueGetMaxSize(SPBPQueue source) {
	return source->maxSize;
}

/*
 * Inserts a new copy (which is allocated) element to the queue
 *
 * @param source - the queue we enter the new elemnt into
 * @param element - the element we enter to source
 * @return msg containing info on the insert
 */
SP_BPQUEUE_MSG spBPQueueEnqueue(SPBPQueue source, SPListElement element) {
	int j = 0, i = 0;
	if (spBPQueueIsEmpty(source)) {
		spListInsertFirst(source->boundedPriorityQueue, element);
		//TODO return proper message enum
	}
	if (spBPQueueIsFull(source)) { // remove the highest value in the list because it is full
		//TODO isn't the last node got highest value? we have spListGetLast or so?
		spListGetFirst(source->boundedPriorityQueue);
		for (; j < spListGetSize(source->boundedPriorityQueue); j++) {
			spListGetNext(source->boundedPriorityQueue);
		}
		spListRemoveCurrent(source->boundedPriorityQueue);
	}

	SPListElement toCompare = spListGetFirst(source->boundedPriorityQueue);
	SP_LIST_MSG MSG;
	for (; i < source->currentSize; i++) {
		if (spListElementCompare(element, toCompare) == -1) {
			MSG = spListInsertBeforeCurrent(source->boundedPriorityQueue,
					element);
			//TODO return proper message enum
			source->currentSize = spListGetSize(source);
		}
		toCompare = spListGetNext(source->boundedPriorityQueue);
	}
	MSG = spListInsertLast(source->boundedPriorityQueue, element);
	//TODO return proper message enum
	source->currentSize = spListGetSize(source);
}

/*
 * removes the element with the lowest value
 *
 * @param source - the queue we remove the element with the lowest value
 * @return msg with info on the remove proccess
 */
SP_BPQUEUE_MSG spBPQueueDequeue(SPBPQueue source) {
	spListGetFirst(source->boundedPriorityQueue);
	//remove first node which has lowest value, and return proper msg
	return (spListRemoveCurrent(source->boundedPriorityQueue));
}

/*
 * returns a new copy of the element with the lowest value
 *
 * @param source - queue we get element with the lowest value from
 * @return a new copy of the element with the lowest value
 */
SPListElement spBPQueuePeek(SPBPQueue source) {
	SPListElement temp_first = spListGetFirst(source->boundedPriorityQueue);
	return spListElementCopy(temp_first);
}

/*
 * returns a new copy of the element with the highest value
 *
 * @param source - queue we get element with the highest value from
 * @return a new copy of the element with the highest value
 */
SPListElement spBPQueuePeekLast(SPBPQueue source) {
	return spListElementCopy(source->boundedPriorityQueue->tail->data);
}

/*
 * returns the minimum value in the queue
 *
 * @param source the queue we get value from
 * @return the min value in the queue
 */
double spBPQueueMinValue(SPBPQueue source) {
	//min value in the first node in queue
	return (spListElementGetValue(spListGetFirst(source->boundedPriorityQueue)));
}

/*
 * returns the maximum value in the queue
 *
 * @param source the queue we get value from
 * @return the max value in the queue
 */
double spBPQueueMaxValue(SPBPQueue source) {
	//max value in the last node in queue
	return (source->boundedPriorityQueue->tail->data->value);
}

/*
 * checks if queue is empty
 *
 * @param queue to check
 * @return true if the queue is empty, false otherwise
 */
bool spBPQueueIsEmpty(SPBPQueue source) {
	//queue is empty <--> (source->currentSize) == 0
	return ((source->currentSize) == 0);
}

/*
 * checks if queue is full
 *
 * @param queue to check
 * @return true if the queue is full, false otherwise
 */
bool spBPQueueIsFull(SPBPQueue source) {
	//queue is full <--> (source->currentSize) == (source->maxSize)
	return ((source->currentSize) == (source->maxSize));
}
