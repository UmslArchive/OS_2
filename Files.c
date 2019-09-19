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

//Helper functions.
int* createIntArrayFromLine(const char* line, const int len, int* arr);

int* test() {
    printf("test Function ran\n");
}

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

    int i;
    pid_t pid;
    int wstatus;
    int offset = 0;

    char* line = NULL;
    size_t len = 0;

    int* intArr = NULL;

    for(i = 0; i < numLines; ++i) {
        pid = fork();

        //Fork error handler.
        if(pid < 0) {
            perror("logParse: Error: Fork");
            exit(EXIT_FAILURE);
        }

        //Child process.
        if(pid == 0) {
            printf("pid: %d\n", getpid());
            inFile = fopen(getFlagArg(INPUT_FILE), "r");

            fseek(inFile, offset + skipFirst, SEEK_SET);
            getline(&line, &len, inFile);

            printf("outer: %s", line);

            createIntArrayFromLine(line, len, intArr);

            //Run the alg....

            //Cleanup.
            free(intArr);
            intArr = NULL;

            //Return current position of file pointer as offset.
            offset = ftell(inFile) - 2; //-2 because reasons.
            fclose(inFile);
            inFile = NULL;
            exit(offset);
        }
        
        wait(&wstatus);
        offset = WEXITSTATUS(wstatus);

    }
    printf("\n");

    return 0;
}

int* createIntArrayFromLine(const char* line, const int len, int * arr) {
    char* lineCopy = (char*) malloc(len * sizeof(char));

    //copy line into lineCopy.
    int i;
    for(i = 0; i < len; ++i) {
        lineCopy[i] = line[i];
    }

    printf("Inner: ");
    for(i = 0; i < len; ++i) {
        printf("%c", lineCopy[i]);
    }

    //Get number of tokens in lineCopy.
    int numInts = 0;
    int scannerState = 0; //Out of a word.
    
    //for every character in the line.
    for(i = 0; i < len; ++i) {

        //If non digit on ascii...
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
    arr = (int*)calloc(numInts, sizeof(int));

    //Tokenize lineCopy.
    /* scannerState = 0;
    char token[256];
    int tokenLen = 0;
    for(i = 0; i < len; ++i) {
        if((int)lineCopy[i] > 57 || (int)lineCopy[i] < 48) {
            scannerState = 0;
        }
        else if(scannerState == 0) {
            scannerState = 1;
        }

        if(scannerState == 1) {

        }
    } */

    i = 0;
    char* token;
    token = strtok(lineCopy, " \n");
    while(token != NULL) {
        arr[i] = atoi(token);
        token = strtok(NULL, " \n");
        ++i;
    }

    printf("arrayInts: ");
    for(i = 0; i < numInts; ++i) {
        printf("%d ", arr[i]);
    }
    printf("\n");

    //Cleanup.
    free(lineCopy);
    lineCopy = NULL;

    return NULL;
}