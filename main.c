#include <stdio.h>
#include "main.h"


int main(int argc, char *argv[])
{
    int IC = 0;  // Instruction counter
    int DC = 0;  // Data counter
    int i;
    /*
    we are passing the command line arguments that are the files that passed to the assembler
    to the handleSourceFiles function.
     */
    struct Macro* head = NULL;
    head = (struct Macro*)malloc(sizeof(struct Macro));
    head->name = EMPTY_MACRO_NAME;
    head->next = NULL;

    char* fileName;
    for (i = 0; i < argc; i++) {
        fileName = argv[i];
        int readResult = readMacros(argv[i], head);
        if (readResult) {
            // Only if there weren't errors reading the file
            int writeResult = writeMacros(fileName, head);
            if (writeResult) {
                firstRun(fileName);
            }
        }
    }

    return 0;
}
