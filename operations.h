#include "consts.h"

struct AddressingMethodsConfig
{
    int immediate; // 0 -> 00
    int direct;  // 1 -> 01
    int index;  // 2 -> 10
    int reg;  // 3 -> 11
};

struct Operation {
    char* name;
    int opCode;  // 0 to 15, decimal
    char binaryCode[4]; // 0 to 15, binary
    int operandsNum;
    struct AddressingMethodsConfig sourceAddressingMethods;
    struct AddressingMethodsConfig targetAddressingMethods;
};


static const struct Operation OPERATIONS_TABLE[OP_SIZE] = {
        {MOV, 0, "0000\0", 2, {1, 1, 1, 1}, {0, 1, 1, 1}},
        {CMP, 1, "0001\0", 2, {1, 1, 1, 1}, {1, 1, 1, 1}},
        {ADD, 2, "0010\0", 2, {1, 1, 1, 1}, {0, 1, 1, 1}},
        {SUB, 3, "0011\0", 2, {1, 1, 1, 1}, {0, 1, 1, 1}},
        {NOT, 4, "0100\0", 1, {0, 0, 0, 0}, {0, 1, 1, 1}},
        {CLR, 5, "0101\0", 1,{0, 0, 0, 0}, {0, 1, 1, 1}},
        {LEA, 6, "0110\0", 2, {0, 1, 1, 0}, {0, 1, 1, 1}},
        {INC, 7, "0111\0", 1, {0, 0, 0, 0}, {0, 1, 1, 1}},
        {DEC, 8, "1000\0", 1, {0, 0, 0, 0}, {0, 1, 1, 1}},
        {JMP, 9, "1001\0", 1, {0, 0, 0, 0}, {0, 1, 1, 1}},
        {BNE, 10, "1010\0", 1, {0, 0, 0, 0}, {0, 1, 1, 1}},
        {GET, 11, "1011\0", 1, {0, 0, 0, 0}, {0, 1, 1, 1}},
        {PRN, 12, "1100\0", 1, {0, 0, 0, 0}, {1, 1, 1, 1}},
        {JSR, 13, "1101\0", 1, {0, 0, 0, 0}, {0, 1, 1, 1}},
        {RTS, 14, "1110\0", 0, {0, 0, 0, 0}, {0, 0, 0, 0}},
        {HLT, 15, "1111\0", 0, {0, 0, 0, 0}, {0, 0, 0, 0}}
};