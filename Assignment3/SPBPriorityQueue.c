#include "SPBPriorityQueue.h"
#include "SPList.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

struct sp_bp_queue_t{
	int maxSize;
	int currentSize;
	SPList boundedPriorityQueue;
};

SPBPQueue spBPQueueCreate(int maxSize){
	struct sp_bp_queue_t *queue = malloc(sizeof(struct sp_bp_queue_t));
	queue.currentSize=0;
	queue.maxSize=maxSize;
	queue.boundedPriorityQueue=spListCreate();
	return queue;
	//TODO deal with errors
}

SPBPQueue spBPQueueCopy(SPBPQueue source){
	struct sp_bp_queue_t *queue = malloc(sizeof(struct sp_bp_queue_t));
	queue.currentSize=source->currentSize;
	queue.maxSize=source->maxSize;
	queue.boundedPriorityQueue=spListCopy(source->boundedPriorityQueue);
	return queue;
	//TODO deal with errors
}

void spBPQueueDestroy(SPBPQueue source){
	spListDestroy(source->boundedPriorityQueue);
	free(source);
}

void spBPQueueClear(SPBPQueue source){
	enum SP_LIST_MSG MSG;
	MSG = spListClear(source->boundedPriorityQueue);
	source->currentSize=0;
	//TODO deal with errors
}

int spBPQueueSize(SPBPQueue source){
	return source->currentSize;
	//TODO deal with errors
}

int spBPQueueGetMaxSize(SPBPQueue source){
	return source->maxSize;
	//TODO deal with errors
}

SP_BPQUEUE_MSG spBPQueueEnqueue(SPBPQueue source, SPListElement element){
	if (spBPQueueIsEmpty(source)){
		spListInsertFirst(source->boundedPriorityQueue);
		//TODO return proper message enum
	}
	if (spBPQueueIsFull(source)){// remove the highest value in the list because it is full
		spListGetFirst(source->boundedPriorityQueue);
		for (int j=0; j<spListGetSize(source->boundedPriorityQueue); j++){
			spListGetNext(source->boundedPriorityQueue);
		}
		spListRemoveCurrent(source->boundedPriorityQueue);
	}

	SPListElement toCompare = spListGetFirst(source->boundedPriorityQueue);
	enum SP_LIST_MSG MSG;
	for (int i=0; i<source->currentSize; i++){
		if (spListElementCompare(element, toCompare) == -1){
			MSG = spListInsertBeforeCurrent(source->boundedPriorityQueue, element);
			//TODO return proper message enum
			source->currentSize = spListGetSize(source);
		}
		toCompare = spListGetNext(source->boundedPriorityQueue);
	}
	MSG = spListInsertLast(source->boundedPriorityQueue, element);
	//TODO return proper message enum
	source->currentSize = spListGetSize(source);
}

SP_BPQUEUE_MSG spBPQueueDequeue(SPBPQueue source){
	enum SP_LIST_MSG MSG;
	spListGetFirst(source->boundedPriorityQueue);
	MSG = spListRemoveCurrent(source->boundedPriorityQueue);
	//TODO return proper message enum
}

SPListElement spBPQueuePeek(SPBPQueue source){
	return spListElementCopy(spListGetFirst(source));
	//TODO deal with errors
}

SPListElement spBPQueuePeekLast(SPBPQueue source){
	return spListElementCopy(source->boundedPriorityQueue->tail->data);
	//TODO deal with errors
}

double spBPQueueMinValue(SPBPQueue source){
	return spListGetFirst(source)->value;
	//TODO deal with errors
}

double spBPQueueMaxValue(SPBPQueue source){
	return source->boundedPriorityQueue->tail->data->value;
	//TODO deal with errors
}

bool spBPQueueIsEmpty(SPBPQueue source){
	return (source->currentSize) == 0;
	//TODO deal with errors
}

bool spBPQueueIsFull(SPBPQueue source){
	return (source->currentSize) == (source->maxSize);
	//TODO deal with errors
}
