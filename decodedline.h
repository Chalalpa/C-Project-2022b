#include "consts.h"


struct DecodedLine {
    char** binaryValue;
    int length;
    char* srcOperandName;
    char* tgtOperandName;
    int isEmpty;
    struct DecodedLine* next;
};
