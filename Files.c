/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -/*
Author - - - Colby Ackerman - - - - - - - - - - - - - - - - - - - - - - - - - -
Class  - - - Operating Systems (CS4760) - - - - - - - - - - - - - - - - - - - -
Date   - - - Sep. 14, 2019  - - - - - - - - - - - - - - - - - - - - - - - - - -
File   - - - "Files.h"  - - - - - - - - - - - - - - - - - - - - - - - - - - - 
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#include "Files.h"
#include "Options.h"

 //Data to be read from infile.
 int numLists = 0;

static FILE* inFile = NULL;
static FILE* outFile = NULL;

static char* readNextWord();

int readInFile() {
    //Open the infile as read-only.
    inFile = fopen(getFlagArg(INPUT_FILE), "r");

    //Check if file exists.
    if(inFile == NULL) {
        perror("logParse: Error: Invalid infile specified");
        return -1;
    }
    else {
        //Check for empty file.
        fseek(inFile, 0, SEEK_END);
        int size = ftell(inFile);
        if(size == 0) {
            printf("logParse: Error: Infile is an empty file\n");
            return -1;
        }
            
        rewind(inFile);
    }

    int currentInt = 0;

    //Read the first line and set numLists.
    if(fscanf(inFile, "%d", &currentInt) > 0) {
        numLists = currentInt;
        printf("numLists = %d\n", numLists);
    }
    else {
        printf("logParse: Error: Could not read first value from infile\n");
    }
        

    //Read each list of ints

    return 0;
}