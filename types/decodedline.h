#ifndef DECODED_LINE
#define DECODED_LINE

struct DecodedLine {
    char** binaryValue;
    int length;
    int lineNum;
    char* srcOperandName;
    char* tgtOperandName;
    int isEmpty;
    struct DecodedLine* next;
};

#endif