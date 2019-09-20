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

int* cpids = NULL; 
int ppid = -1;
int numChildren = 0;
clock_t childTimeStart;
clock_t childTimeStop;
double elapsed = 0;

void subsetSum(int* set, int* subSet, int n, int subSize, int total, int nodeCount, int sum);
void findSubset(int set[], int size, int sum);

void parentSignalListener(int sig);
void childSignalListener(int sig);
void killChildren(int* children);

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

    childTimeStart = time(NULL);
    for(i = 0; i < numLines; ++i) {
        pid = fork();

        //Fork error handler.
        if(pid < 0) {
            perror("logParse: Error: Fork");
            exit(EXIT_FAILURE);
        }

        if(pid > 0) {
            //Gather process ID info.
            ppid = getpid();
            ++numChildren;
            cpids = (int*) realloc((int*)cpids, (size_t)numChildren);
            cpids[numChildren - 1] = pid;
        }

        if(pid == 0) {
            //Set time limit for child processing life to 1 second.
            alarm(1);
            signal(SIGALRM, childSignalListener);
            printf("cpid = %d waiting for alarm.\n", getpid());
           
            //printf("pid: %d\n", getpid());
            inFile = fopen(getFlagArg(INPUT_FILE), "r");

            //Seek to start of line and get the line.
            fseek(inFile, offset + skipFirst, SEEK_SET);
            getline(&line, &len, inFile);

            //Set new offset.
            offset = ftell(inFile) - skipFirst; //-2 just because.

            fclose(inFile);
            inFile = NULL;

            //Code between these lines tokeni3zes the line returned by getline 
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
            findSubset(intArr + 1, numInts - 1, intArr[0]);

            //Cleanup.
            free(intArr);
            intArr = NULL;

            //Return current position of file pointer as offset.
            exit(offset);
        }
        
        wait(&wstatus);
        offset = WEXITSTATUS(wstatus);

        //Stop the clock after the child has been waited on.
        if(pid > 0) {
            childTimeStop = time(NULL);
            printf("childTimeStop: %ld\n", childTimeStop);
            elapsed = (double)(childTimeStop - childTimeStart);
            printf("Elapsed time: %f\n", elapsed);

            if(elapsed >= timeParam) {
                exit(0);
            }
        }

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

void displaySubset(int* subSet, int size) {
    int j;
    printf("Subset: ");
    for(j = 0; j < size; ++j)
        printf("%d ", subSet[j]);
    printf("\n");
}

void subsetSum(int* set, int* subSet, int n, int subSize, int total, int nodeCount, int sum) {
    int i;
    if(total == sum) {
        displaySubset(subSet, subSize);
        subsetSum(set, subSet, n, subSize - 1, total - set[nodeCount], nodeCount + 1, sum);
        return;
    }
    else {
        for(i = nodeCount; i < n; ++i) {
            subSet[subSize] = set[i];
            subsetSum(set, subSet, n, subSize + 1, total + set[i], i + 1, sum);
        }
    }
}

void findSubset(int* set, int size, int sum) {
    int* subSet = (int*)calloc(size, sizeof(int));
    subsetSum(set, subSet, size, 0, 0, 0, sum);
    free(subSet);
    subSet = NULL;
}

void parentSignalListener(int sig) {
    if(sig == SIGALRM) {
        //Kill children.
        printf("parent alarm\n");
        exit(0);
    }
}

void childSignalListener(int sig) {
    if(sig == SIGALRM) {
        //Kill self.
        printf("%d: No valid subset found after 1 second.\n", getpid());
        exit(0);
    }

    if(sig == SIGTERM) {
        //Die peacefully.
    }
}

void killChildren(int* children) {

}