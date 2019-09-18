/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -/*
Author - - - Colby Ackerman - - - - - - - - - - - - - - - - - - - - - - - - - -
Class  - - - Operating Systems (CS4760) - - - - - - - - - - - - - - - - - - - -
Date   - - - Sep. 14, 2019  - - - - - - - - - - - - - - - - - - - - - - - - - -
File   - - - "IntegerListQueue.h"  - - - - - - - - - - - - - - - - - - - - - - -
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#ifndef INTEGERLISTQUEUE_H
#define INTEGERLISTQUEUE_H

#include <stdlib.h>
#include <stdio.h>

struct IntegerListQueue {
    int numLists;
    int* sizes;
    int** lists;
};

struct IntegerListQueue* intListQueue;

void initIntegerListQueue();

void allocateSizesArray();

void appendListElement(int listSelect, int item);

#endif