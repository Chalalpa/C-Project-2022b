#include "operations.h"

/* @ Function: int getOperationIndex(char* operationName);
   @ Arguments: char* operationName
   operationName - is the requested operation name
   @ Description: The function goes through the operations array, and finds the requested operation index by its name.
   It returns is if it finds it, but if not, it returns -1.
*/
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