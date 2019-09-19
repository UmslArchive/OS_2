/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -/*
Author - - - Colby Ackerman - - - - - - - - - - - - - - - - - - - - - - - - - -
Class  - - - Operating Systems (CS4760) - - - - - - - - - - - - - - - - - - - -
Date   - - - Sep. 14, 2019  - - - - - - - - - - - - - - - - - - - - - - - - - -
File   - - - "Files.c"  - - - - - - - - - - - - - - - - - - - - - - - - - - - 
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#include "Files.h"

//Infile data.
static int numLines = -1;
static int* currentLineArray = NULL;

//File handlers.
static FILE* inFile = NULL;
static FILE* outFile = NULL;

void subsetSum(const int* set, int setLen);

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

    fclose(inFile);
    inFile = NULL;

    //Fork loop.
    int i;
    pid_t pid;
    int wstatus;
    int offset = 0;
    char* line = NULL;
    size_t len = 0;
    int* intArr = NULL;
    int size;
    int* cpids = NULL;
    int ppid = -1;
    int numChildren = 0;

    for(i = 0; i < numLines; ++i) {
        pid = fork();

        //Fork error handler.
        if(pid < 0) {
            perror("logParse: Error: Fork");
            exit(EXIT_FAILURE);
        }

        if(pid != 0) {
            ppid = getpid();
            //printf("pid: %d\n", pid);
            ++numChildren;
            //printf("children: %d\n", numChildren);
            cpids = (int*) realloc((int*)cpids, (size_t)numChildren);
            cpids[numChildren - 1] = pid;
        }

        if(pid == 0) {
            //printf("pid: %d\n", getpid());
            inFile = fopen(getFlagArg(INPUT_FILE), "r");

            //Seek to start of line and get the line.
            fseek(inFile, offset + skipFirst, SEEK_SET);
            getline(&line, &len, inFile);

            //Set new offset.
            offset = ftell(inFile) - 2; //-2 just because.

            fclose(inFile);
            inFile = NULL;

            //Code between these lines tokenizes the line returned by getline 
            //and sticks the integers into an array.
            //---------------------------

            char* lineCopy = (char*) malloc(len * sizeof(char));

            //copy line into lineCopy.
            int i;
            for(i = 0; i < len; ++i) {
                lineCopy[i] = line[i];
            }

            /* Get number of tokens in lineCopy... */
            int numInts = 0;
            int scannerState = 0; //Out-of-a-word(0) vs in-a-word(1)
            
            //for every character in the line.
            for(i = 0; i < len; ++i) {

                //If non-digit on ascii...
                if((int)lineCopy[i] > 57 || (int)lineCopy[i] < 48) {
                    scannerState = 0;
                }

                //Otherwise in a word.
                else if(scannerState == 0) {
                    scannerState = 1;
                    ++numInts;
                }
            }

            //Allocate the array.
            intArr = (int*)calloc(numInts, sizeof(int));

            i = 0;
            char* token;
            token = strtok(lineCopy, " \n");
            while(token != NULL) {
                intArr[i] = atoi(token);
                token = strtok(NULL, " \n");
                ++i;
            }

            //DEBUG
            printf("ArrayInts: ");
            for(i = 0; i < numInts; ++i) {
                printf("%d ", intArr[i]);
            }
            printf("\n");

            //Cleanup.
            free(lineCopy);
            lineCopy = NULL;

            //----------------------------------

            //Run the alg....
            subsetSum(intArr, numInts);

            //Cleanup.
            free(intArr);
            intArr = NULL;

            //Return current position of file pointer as offset.
            exit(offset);
        }
        
        wait(&wstatus);
        offset = WEXITSTATUS(wstatus);

    }

    //DEBUG
    for(i = 0; i < numChildren; ++i) {
        printf("Child #%d PID = %d\n", i, cpids[i]);
    }
    printf("ppid: %d\n", ppid);

    //Cleanup.
    free((int*)cpids);
    cpids = NULL;

    return 0;
}

void subsetSum(const int* set, int setLen) {
    int i;
    printf("set: ");
    for(i = 0; i < setLen; ++i) {
        printf("%d ", set[i]);
    }
    printf("\n");
}