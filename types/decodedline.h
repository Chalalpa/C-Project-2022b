#ifndef _DECODED_LINE_
#define _DECODED_LINE_

struct DecodedLine {
    char** binaryValue;
    int value;
    int length;
    int lineNum;
    char* srcOperandName;
    char* tgtOperandName;
    int isEmpty;
    struct DecodedLine* next;
};

#endif