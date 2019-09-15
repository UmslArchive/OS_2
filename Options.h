/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -/*
Author - - - Colby Ackerman - - - - - - - - - - - - - - - - - - - - - - - - - -
Class  - - - Operating Systems (CS4760) - - - - - - - - - - - - - - - - - - - -
Date   - - - Sep. 14, 2019  - - - - - - - - - - - - - - - - - - - - - - - - - -
File   - - - "Options.h"  - - - - - - - - - - - - - - - - - - - - - - - - - - - 
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#ifndef OPTIONS_H
#define OPTIONS_H

#define NUM_FLAGS 4

//Enum to be used to index into the flagStates array.
enum FLAG_LABELS {
    HELP,
    INPUT_FILE,
    OUTPUT_FILE,
    TIME
};

extern enum FLAG_LABELS flagLabel;

//Array which contains the states flags determined by user-passed args.
extern int flagStates[NUM_FLAGS];

//Option parameters variables.
extern char* inputFileParam;
extern char* outputFileParam;
extern char* timeParam;

void initFlags();

void setFlags(int argc, char** argv);

int getFlagState(int flag);

int timeParamToInt();

#endif