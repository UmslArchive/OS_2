/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -/*
Author - - - Colby Ackerman - - - - - - - - - - - - - - - - - - - - - - - - - -
Class  - - - Operating Systems (CS4760) - - - - - - - - - - - - - - - - - - - -
Date   - - - Sep. 14, 2019  - - - - - - - - - - - - - - - - - - - - - - - - - -
File   - - - "Files.h"  - - - - - - - - - - - - - - - - - - - - - - - - - - - 
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#include "Files.h"

static FILE* inFile = NULL;
static FILE* outFile = NULL;

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

    static int currentInt = 0;

    //Read the first line and set numLists.
    if(fscanf(inFile, "%d\n", &currentInt) > 0) {
        intListQueue->numLists = currentInt;
        allocateSizesArray();
    }
    else {
        printf("logParse: Error: Could not read first value from infile\n");
    }
        
    //Read each line of the file into a string, then parse for ints.
    char* line = NULL;
    int numInts = 0;
    size_t len = 0;
    ssize_t read;

    while((read = getline(&line, &len, inFile)) != EOF) {
        //Tokenize the line.
        char* token = strtok(line, " ");
        while(token != NULL) {
            printf("%s\n", token);
            token = strtok(NULL, " ");
        }
    }

    return 0;
}