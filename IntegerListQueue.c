/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -/*
Author - - - Colby Ackerman - - - - - - - - - - - - - - - - - - - - - - - - - -
Class  - - - Operating Systems (CS4760) - - - - - - - - - - - - - - - - - - - -
Date   - - - Sep. 14, 2019  - - - - - - - - - - - - - - - - - - - - - - - - - -
File   - - - "IntegerListQueue.h"  - - - - - - - - - - - - - - - - - - - - - -  
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#include "IntegerListQueue.h"

void initIntegerListQueue() {
    intListQueue = (struct IntegerListQueue*)malloc(sizeof(struct IntegerListQueue));
    intListQueue->numLists = 0;
    intListQueue->sizes = NULL;
    intListQueue->lists = NULL;
}

void allocateSizesArray() {
    intListQueue->sizes = (int*)malloc(intListQueue->numLists * sizeof(int));
}

void appendListElement(int listSelect, int item) {
    
}
