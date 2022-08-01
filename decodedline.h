#include "consts.h"


struct DecodedLine {
    char** binaryValue;  // There is a maximum of 5 lines per command
    char* srcOperandName;
    char* tgtOperandName;
    int isEmpty;
    struct DecodedLine* next;
};
