#include "SPBPriorityQueue.h"
#include "SPList.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

struct sp_bp_queue_t {
	int maxSize;
	SPList boundedPriorityQueue;
};

/*
 * creates an empty queue with a given maximum capacity
 *
 * @param maxSize is maximum capacity
 * @return new empty queue with a given maximum capacity
 */
SPBPQueue spBPQueueCreate(int maxSize) {
	if (maxSize<1){
		return NULL;
	}
	SPBPQueue queue = (SPBPQueue) malloc(sizeof(struct sp_bp_queue_t));
	if (queue == NULL){
		printf("An error occurred - allocation failure\n");
		return NULL;
	}
	queue->maxSize = maxSize;
	queue->boundedPriorityQueue = spListCreate();
	return queue;
}

/*
 * creates a copy of a given queue
 *
 * @param source is given queue
 * @return SPBPQueue which is a copy of source
 */
SPBPQueue spBPQueueCopy(SPBPQueue source) {
	if (source == NULL){
		return NULL;
	}
	SPBPQueue queue = (SPBPQueue) malloc(sizeof(struct sp_bp_queue_t));
	if (queue == NULL){
		printf("An error occurred - allocation failure\n");
		return NULL;
	}
	queue->maxSize = source->maxSize;
	queue->boundedPriorityQueue = spListCopy(source->boundedPriorityQueue);
	return queue;
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
	return spListGetSize(source->boundedPriorityQueue);
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
	SP_BPQUEUE_MSG msg;
	SP_LIST_MSG listMsg;
	if (source == NULL || element == NULL){
		msg = SP_BPQUEUE_INVALID_ARGUMENT;
		return msg;
	}
	if (spBPQueueIsEmpty(source)) {
		listMsg = spListInsertFirst(source->boundedPriorityQueue, element);
		msg=switchListMessage(listMsg);
		return msg;
	}
	if (spBPQueueIsFull(source)) { // remove the highest value in the list because it is full
		spListGetFirst(source->boundedPriorityQueue);
		for (; j < spListGetSize(source->boundedPriorityQueue); j++) {
			spListGetNext(source->boundedPriorityQueue);
		}
		listMsg = spListRemoveCurrent(source->boundedPriorityQueue);
		msg=switchListMessage(listMsg);
		if(msg != SP_BPQUEUE_SUCCESS){
			return msg;
		}
	}

	SPListElement toCompare = spListGetFirst(source->boundedPriorityQueue);
	for (; i < spBPQueueSize(source); i++) {
		if (spListElementCompare(element, toCompare) == -1) {
			listMsg = spListInsertBeforeCurrent(source->boundedPriorityQueue,
					element);
			msg=switchListMessage(listMsg);
			if(msg != SP_BPQUEUE_SUCCESS){
				return msg;
			}
		}
		toCompare = spListGetNext(source->boundedPriorityQueue);
	}
	listMsg = spListInsertLast(source->boundedPriorityQueue, element);
	msg=switchListMessage(listMsg);
	return msg;
}

/*
 * removes the element with the lowest value
 *
 * @param source - the queue we remove the element with the lowest value
 * @return msg with info on the remove proccess
 */
SP_BPQUEUE_MSG spBPQueueDequeue(SPBPQueue source) {
	SP_LIST_MSG listMsg;
	SP_BPQUEUE_MSG msg;
	if (source == NULL){
		return SP_BPQUEUE_INVALID_ARGUMENT;
	}
	spListGetFirst(source->boundedPriorityQueue);
	//remove first node which has lowest value, and return proper msg
	listMsg = spListRemoveCurrent(source->boundedPriorityQueue);
	msg=switchListMessage(listMsg);
	return msg;
}

/*
 * returns a new copy of the element with the lowest value
 *
 * @param source - queue we get element with the lowest value from
 * @return a new copy of the element with the lowest value
 */
SPListElement spBPQueuePeek(SPBPQueue source) {
	if (source == NULL){
		return NULL;
	}
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
	if (source == NULL){
		return NULL;
	}
	SPListElement toReturn;
	spListGetFirst(source->boundedPriorityQueue);
	for (int j=0 ; j < spListGetSize(source->boundedPriorityQueue); j++) {
		spListGetNext(source->boundedPriorityQueue);
	}
	toReturn = spListElementCopy(spListGetCurrent(source->boundedPriorityQueue));
	return toReturn;
}

/*
 * returns the minimum value in the queue
 *
 * @param source the queue we get value from
 * @return the min value in the queue
 */
double spBPQueueMinValue(SPBPQueue source) {
	if (source == NULL){
		return 0;
	}
	//min value in the first node in queue
	double toReturn = spListElementGetValue(spListGetFirst(source->boundedPriorityQueue));
	return toReturn;
}

/*
 * returns the maximum value in the queue
 *
 * @param source the queue we get value from
 * @return the max value in the queue
 */
double spBPQueueMaxValue(SPBPQueue source) {
	if (source == NULL){
		return 0;
	}
	//max value is in the last node of the queue
	spListGetFirst(source->boundedPriorityQueue);
	for (int j=0 ; j < spListGetSize(source->boundedPriorityQueue); j++) {
		spListGetNext(source->boundedPriorityQueue);
	}
	double res = spListElementGetValue(spListGetCurrent(source->boundedPriorityQueue));
	return res;
}

/*
 * checks if queue is empty
 *
 * @param queue to check
 * @return true if the queue is empty, false otherwise
 */
bool spBPQueueIsEmpty(SPBPQueue source) {
	if (source == NULL){
		return false;
	}
	//queue is empty <--> (source->currentSize) == 0
	return ((spBPQueueSize(source)) == 0);
}

/*
 * checks if queue is full
 *
 * @param queue to check
 * @return true if the queue is full, false otherwise
 */
bool spBPQueueIsFull(SPBPQueue source) {
	if (source == NULL){
		return false;
	}
	//queue is full <--> (source->currentSize) == (source->maxSize)
	return ((spBPQueueSize(source)) == (source->maxSize));
}

/**
 * switch between matching return messages from SPList
 */
SP_BPQUEUE_MSG switchListMessage(SP_LIST_MSG msg){
	switch (msg){
	case SP_LIST_SUCCESS:
		return SP_BPQUEUE_SUCCESS;
	case SP_LIST_NULL_ARGUMENT:
		return SP_BPQUEUE_INVALID_ARGUMENT;
	case SP_LIST_OUT_OF_MEMORY:
		return SP_BPQUEUE_OUT_OF_MEMORY;
	case SP_LIST_INVALID_CURRENT:
		return SP_BPQUEUE_INVALID_ARGUMENT;
	}
	return SP_BPQUEUE_SUCCESS;
}

/**
 * switch between matching return messages from SPListElement
 */
SP_BPQUEUE_MSG switchElementMessage(SP_ELEMENT_MSG msg){
	switch (msg){
	case SP_ELEMENT_SUCCESS:
		return SP_BPQUEUE_SUCCESS;
	case SP_ELEMENT_INVALID_ARGUMENT:
		return SP_BPQUEUE_INVALID_ARGUMENT;
	case SP_ELEMENT_OUT_OF_MEMORY:
		return SP_BPQUEUE_OUT_OF_MEMORY;
	}
	return SP_BPQUEUE_SUCCESS;
}
