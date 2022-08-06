#ifndef _DECODED_LINE_
#define _DECODED_LINE_

/* This struct represents a line that we decode into binary words. We store a few properties of it to be able
 * later, easily, to parse it into base32 machine code as to the imaginary computer */
struct DecodedLine {
    char** binaryValue;  // A strings array that holds the binary machine code of each of the translated words
    int value;  // The initial value of the line
    int length;  // The number of how many words this assembly line is translated into
    int lineNum;  // Original line number, in the assembly file
    char* srcOperandName;  // The name of the source operand
    char* tgtOperandName;  // The name of the target operand
    int isEmpty;  // Symbols whether the line object is empty (no content) or not
    struct DecodedLine* next;  // A pointer to the next-in-list DecodedLine object
};

#endif