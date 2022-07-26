#include <stdio.h>
#include "main.h"


int main(int argc, char *argv[])
{
    int IC, DC, i;
    /*
    we are passing the command line arguments that are the files that passed to the assembler
    to the handleSourceFiles function.
     */
    struct Macro* tail = NULL;
    struct Macro* head = NULL;
    head = (struct Macro*)malloc(sizeof(struct Macro*));
    char* fileName;
    for (i = 0; i < argc; i++) {
        fileName = argv[i];
        int result = readMacros(argv[i], head);
        if (result) {
            writeMacros(fileName, head);
        }
    }

    return 0;
}
