#ifndef _OPERATIONS_
#define _OPERATIONS_
#include <string.h>
#include "../helpers/consts.h"

/* A struct to represent the possible addressing methods and the possible configs per operation*/
struct AddressingMethodsConfig {
    int immediate; /* 0 -> 00 */
    int direct;  /* 1 -> 01 */
    int index;  /* 2 -> 10 */
    int reg;  /* 3 -> 11 */
};

/* A struct to represent each of the supported operations and their properties*/
struct Operation {
    char* name;  /* The operation name as it's shown in the assembly file */
    int opCode;  /* 0 to 15, decimal */
    char binaryCode[4]; /* 0 to 15, binary */
    int operandsNum;  /* The operands number required for this operation */
    struct AddressingMethodsConfig sourceAddressingMethods;  /* The addressing methods config for src operand possible for this operation */
    struct AddressingMethodsConfig targetAddressingMethods;  /* The addressing methods config for tgt operand possible for this operation */
};


/* A static const list to represent the possible operations that are supported and each with its configuration */
static const struct Operation OPERATIONS_TABLE[OP_SIZE] = {
        {MOV, 0, "0000", 2, {1, 1, 1, 1}, {0, 1, 1, 1}},
        {CMP, 1, "0001", 2, {1, 1, 1, 1}, {1, 1, 1, 1}},
        {ADD, 2, "0010", 2, {1, 1, 1, 1}, {0, 1, 1, 1}},
        {SUB, 3, "0011", 2, {1, 1, 1, 1}, {0, 1, 1, 1}},
        {NOT, 4, "0100", 1, {0, 0, 0, 0}, {0, 1, 1, 1}},
        {CLR, 5, "0101", 1,{0, 0, 0, 0}, {0, 1, 1, 1}},
        {LEA, 6, "0110", 2, {0, 1, 1, 0}, {0, 1, 1, 1}},
        {INC, 7, "0111", 1, {0, 0, 0, 0}, {0, 1, 1, 1}},
        {DEC, 8, "1000", 1, {0, 0, 0, 0}, {0, 1, 1, 1}},
        {JMP, 9, "1001", 1, {0, 0, 0, 0}, {0, 1, 1, 1}},
        {BNE, 10, "1010", 1, {0, 0, 0, 0}, {0, 1, 1, 1}},
        {GET, 11, "1011", 1, {0, 0, 0, 0}, {0, 1, 1, 1}},
        {PRN, 12, "1100", 1, {0, 0, 0, 0}, {1, 1, 1, 1}},
        {JSR, 13, "1101", 1, {0, 0, 0, 0}, {0, 1, 1, 1}},
        {RTS, 14, "1110", 0, {0, 0, 0, 0}, {0, 0, 0, 0}},
        {HLT, 15, "1111", 0, {0, 0, 0, 0}, {0, 0, 0, 0}}
};

/* @ Function: int getOperationIndex(char* operationName);
   @ Arguments: char* operationName
   operationName - is the requested operation name
   @ Description: The function goes through the operations array, and finds the requested operation index by its name.
   It returns is if it finds it, but if not, it returns -1.
*/
int getOperationIndex(char* operationName);

#endif
