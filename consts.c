#include "consts.h"



const char* DATA_DIRECTIVES[] = {DATA, STRING, STRUCT};
const char* EXTERN_OR_ENTRY[] = {EXTERN, ENTRY};
const char* OPERATIONS[] = {
        MOV, //0
        CMP, //1
        ADD, //2
        SUB, //3
        NOT, //4
        CLR, //5
        LEA, //6
        INC, //7
        DEC, //8
        JMP, //9
        BNE, //10
        GET, //11
        PRN, //12
        JSR, //13
        RTS, //14
        HLT  //15
};
const char* KEYWORDS[] = {R0, R1, R2, R3, R4, R5, R6, R7, MOV, CMP, ADD, SUB, NOT, CLR, LEA, INC, DEC, JMP, BNE,
                          GET, PRN, JSR, RTS, HLT, MACRO_START, MACRO_END, DATA, STRING, STRUCT, ENTRY, EXTERN};