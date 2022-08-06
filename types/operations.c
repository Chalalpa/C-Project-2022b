#include "operations.h"

int getOperationIndex(char* operationName) {
    char** operationsPointer = OPERATIONS;
    int i=0;
    while(*operationsPointer) {
        if(!strcmp(operationName, *operationsPointer))
            return i;
        i++;
        operationsPointer++;
    }
    return -1;
}