#include "decodedline.h"
int freeDecodedLine(struct DecodedLine* headPointer) {
    int count, i;
    if(!headPointer->isEmpty) {
        if (headPointer->srcOperandName != NULL)
            free(headPointer->srcOperandName);
        if (headPointer->tgtOperandName != NULL)
            free(headPointer->tgtOperandName);
        if (headPointer->binaryValue != NULL) {
            count = 0, i = 0;
            while(count < headPointer->length) {
                if(headPointer->binaryValue[i] != NULL) {
                    free(headPointer->binaryValue[i]);
                    count++;
                }
                i++;
            }
            free(headPointer->binaryValue);
        }
    }
    free(headPointer);
    return 1;
}

