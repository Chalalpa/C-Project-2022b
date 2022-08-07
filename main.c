#include "main.h"

int freeMacros(struct Macro* macroHead) {
    struct Macro* headPointer = macroHead;
    struct Macro* nextPointer = macroHead->next;
    do {
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
        free(headPointer);
        headPointer = nextPointer;
        if (headPointer != NULL)
            nextPointer = headPointer->next;
    }
    while (headPointer != NULL);
    return 1;
}


int main(int argc, char *argv[])
{
    int IC;  /* Instruction counter */
    int DC; /* Data counter */
    int i, result;
    char* fileName;
    struct Macro* macroHead = NULL;
    struct Symbol* symbolHead = NULL;
    struct DecodedLine* decodedLineHead = NULL;
    struct Entry* entryHead = NULL;
    struct Extern* externHead = NULL;

    if (argc == 0) {
        printf("No files paths were provided");
        return 0;
    }

    for (i = 0; i < argc; i++) {
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
        printf("==============Reading Macros===============\n");
        result = readMacros(argv[i], macroHead);
        if (result) {
            printf("==============Writing Macros===============\n");
            result = writeMacros(fileName, macroHead);
            if (result) {
                printf("==============First Iteration===============\n");
                firstRun(fileName, &IC, &DC, symbolHead, decodedLineHead, entryHead,
                                              externHead);
                if (result == 1) {
                    printf("==============Second Iteration===============\n");
                    result = secondRun(fileName, symbolHead, decodedLineHead, entryHead,
                                                    externHead);
                    if (result)
                        printf("Successfully compiled file: %s\n", fileName);
                }
            }
        }
        freeMacros(macroHead);
        freeSymbols(symbolHead);
        freeExterns(externHead);
        freeEntries(entryHead);
    }

    return 0;
}
