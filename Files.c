/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -/*
Author - - - Colby Ackerman - - - - - - - - - - - - - - - - - - - - - - - - - -
Class  - - - Operating Systems (CS4760) - - - - - - - - - - - - - - - - - - - -
Date   - - - Sep. 14, 2019  - - - - - - - - - - - - - - - - - - - - - - - - - -
File   - - - "Files.h"  - - - - - - - - - - - - - - - - - - - - - - - - - - - 
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#include "Files.h"

//Infile data.
static int numLines = -1;
static int* currentLineArray = NULL;

//File handlers.
static FILE* inFile = NULL;
static FILE* outFile = NULL;

//Helper functions.
static int* createIntArrayFromLine(char* line);

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
        numLines = currentInt;
    }
    else {
        printf("logParse: Error: Could not read first value from infile\n");
    }

    //Save current position of file pointer after reading first line for offset
    //during forking.
    int skipFirst = ftell(inFile);

    int i;
    pid_t pid, cpid, w;
    int wstatus = 0;
    int offset = 0;

    for(i = 0; i < numLines; ++i) {
        cpid = fork();

        //Fork error handler.
        if(cpid < 0) {
            perror("logParse: Error: Fork");
            exit(EXIT_FAILURE);
        }

        //Child process.
        if(cpid == 0) {
            char* line = NULL;
            int numInts = 0;
            size_t len = 0;

            fseek(inFile, offset + skipFirst, SEEK_SET);
            getline(&line, &len, inFile);

            int* intArr = createIntArrayFromLine(line);


            printf("%s", line);

            //Return current position of file pointer as offset.
            offset = ftell(inFile);
            exit(offset);
        }
        
        wait(&wstatus);
        offset = WEXITSTATUS(wstatus);

    }

    printf("\n");

    return 0;
}

static int* createIntArrayFromLine(char* line) {

}