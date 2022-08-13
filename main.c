#include "main.h"

/* Freeing methods */
int freeDecodedLines(struct DecodedLine* decodedLineHead) {
    struct DecodedLine* headPointer = decodedLineHead;
    struct DecodedLine* nextPointer = decodedLineHead->next;
    do {
        freeDecodedLine(headPointer);
        headPointer = nextPointer;
        if (headPointer != NULL)
            nextPointer = headPointer->next;
    }
    while (headPointer != NULL);
    return 1;
}

int freeMacros(struct Macro* macroHead) {
    struct Macro* headPointer = macroHead;
    struct Macro* nextPointer = macroHead->next;
    do {
        if(strcmp(headPointer->name, EMPTY_STRUCT_NAME)) {
            free(headPointer->data);
            free(headPointer->name);
        }
        free(headPointer);
        headPointer = nextPointer;
        if (headPointer != NULL)
            nextPointer = headPointer->next;
    }
    while (headPointer != NULL);
    return 1;
}

int freeSymbols(struct Symbol* symbolHead) {
    struct Symbol* headPointer = symbolHead;
    struct Symbol* nextPointer = symbolHead->next;
    do {
        if(strcmp(headPointer->name, EMPTY_STRUCT_NAME)) {
            free(headPointer->name);
        }
        free(headPointer);
        headPointer = nextPointer;
        if (headPointer != NULL)
            nextPointer = headPointer->next;
    }
    while (headPointer != NULL);
    return 1;
}

int freeEntries(struct Entry* entryHead) {
    struct Entry* headPointer = entryHead;
    struct Entry* nextPointer = entryHead->next;
    do {
        if(strcmp(headPointer->name, EMPTY_STRUCT_NAME))
            free(headPointer->name);
        free(headPointer);
        headPointer = nextPointer;
        if (headPointer != NULL)
            nextPointer = headPointer->next;
    }
    while (headPointer != NULL);
    return 1;
}

int freeExterns(struct Extern* externHead) {
    struct Extern* headPointer = externHead;
    struct Extern* nextPointer = externHead->next;
    do {
        if(strcmp(headPointer->name, EMPTY_STRUCT_NAME))
            free(headPointer->name);
        free(headPointer);
        headPointer = nextPointer;
        if (headPointer != NULL)
            nextPointer = headPointer->next;
    }
    while (headPointer != NULL);
    return 1;
}


/* The compiler program */
int main(int argc, char *argv[])
{
    int IC;  /* Instruction counter */
    int DC; /* Data counter */

    /* Iterations helpers */
    int i, result;
    char* fileName;

    /* Storing the data structures: */
    struct Macro* macroHead = NULL;
    struct Symbol* symbolHead = NULL;
    struct DecodedLine* decodedLineHead = NULL;
    struct Entry* entryHead = NULL;
    struct Extern* externHead = NULL;

    if (argc <= 1) {  /* The first argument is always the executable, so we need to ignore it */
        printf("No files paths were provided");
        return 0;
    }

    /* Ignoring first command line arg, which is the executable */
    for (i = 1; i < argc; i++) {
        /* For each new file, initializing default values */
        IC = MEMORY_START;
        DC = 0;
        macroHead = (struct Macro*)malloc(sizeof(struct Macro));
        macroHead->name = EMPTY_STRUCT_NAME;
        macroHead->next = NULL;

        symbolHead = (struct Symbol*)malloc(sizeof(struct Symbol));
        symbolHead->name = EMPTY_STRUCT_NAME;
        symbolHead->next = NULL;

        decodedLineHead = (struct DecodedLine*)malloc(sizeof(struct DecodedLine));
        decodedLineHead->isEmpty = 1;
        decodedLineHead->length = 0;
        decodedLineHead->next = NULL;

        entryHead = (struct Entry*)malloc(sizeof(struct Entry));
        entryHead->name = EMPTY_STRUCT_NAME;
        entryHead->next = NULL;

        externHead = (struct Extern*)malloc(sizeof(struct Extern));
        externHead->name = EMPTY_STRUCT_NAME;
        externHead->next = NULL;
        fileName = argv[i];

        /* Start preprocessing */
        printf("==============Reading Macros===============\n");
        result = readMacros(argv[i], macroHead);
        if (result) {  /* If failed, no need to write .am file */
            printf("==============Writing Macros===============\n");
            result = writeMacros(fileName, macroHead);
            if (result) {
                printf("==============First Iteration===============\n");
                result = firstRun(fileName, &IC, &DC, symbolHead, decodedLineHead, entryHead,
                                              externHead);
                if (result) {
                    printf("==============Second Iteration===============\n");
                    result = secondRun(fileName, symbolHead, decodedLineHead, entryHead, externHead,
                                       IC, DC);
                    if (result)
                        printf("Successfully compiled file: %s\n", fileName);
                    else
                        printf("Failed in Second Iteration phase!\n");
                }
                else
                    printf("Failed in First Iteration phase!\n");
            }
            else
                printf("Failed in Preprocessor phase!\n");
        }
        else
            printf("Failed in Preprocessor phase!\n");

        /* Freeing memory allocated for this file's compiling */
        freeMacros(macroHead);
        freeSymbols(symbolHead);
        freeExterns(externHead);
        freeEntries(entryHead);
        freeDecodedLines(decodedLineHead);
    }

    return 0;
}
