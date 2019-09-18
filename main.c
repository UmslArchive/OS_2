/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -/*
Author - - - Colby Ackerman - - - - - - - - - - - - - - - - - - - - - - - - - -
Class  - - - Operating Systems (CS4760) - - - - - - - - - - - - - - - - - - - -
Date   - - - Sep. 14, 2019  - - - - - - - - - - - - - - - - - - - - - - - - - -
File   - - - "main.c" - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#include "Options.h"
#include "Files.h"

int main(int argc, char** argv) {

    //Option parsing.
    setFlags(argc, argv);

    if(getFlagState(HELP) == 1) {
        printf("HELP\n");
        return 0;
    }

    readInFile();


    return 0;
}
