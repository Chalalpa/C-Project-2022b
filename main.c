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
    struct Macro* macroHead = NULL;
    macroHead = (struct Macro*)malloc(sizeof(struct Macro));
    macroHead->name = EMPTY_STRUCT_NAME;
    macroHead->next = NULL;

    char* fileName;
    for (i = 0; i < argc; i++) {
        fileName = argv[i];
        int readResult = readMacros(argv[i], macroHead);
        if (readResult) {
            // Only if there weren't errors reading the file
            int writeResult = writeMacros(fileName, macroHead);
            if (writeResult) {
                struct Symbol* symbolHead = NULL;
                symbolHead = (struct Symbol*)malloc(sizeof(struct Symbol));
                symbolHead->name = EMPTY_STRUCT_NAME;
                symbolHead->next = NULL;
                firstRun(fileName, &IC, &DC, symbolHead);
            }
        }
    }

    return 0;
}
