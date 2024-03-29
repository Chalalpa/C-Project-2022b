#include "consts.h"

/* A constants file, to store all kinds of constants that might be re-used in the project */

/* Initialization of constants that were declared in the header file */
const char* DATA_DIRECTIVES[] = {DATA, STRING, STRUCT, "\0"};
const char* EXTERN_OR_ENTRY[] = {EXTERN, ENTRY, "\0"};
const char* OPERATIONS[] = {
        MOV, /* 0 */
        CMP, /* 1 */
        ADD, /* 2 */
        SUB, /* 3 */
        NOT, /* 4 */
        CLR, /* 5 */
        LEA, /* 6 */
        INC, /* 7 */
        DEC, /* 8 */
        JMP, /* 9 */
        BNE, /* 10 */
        GET, /* 11 */
        PRN, /* 12 */
        JSR, /* 13 */
        RTS, /* 14 */
        HLT, /* 15 */
        "\0",
};
const char* KEYWORDS[] = {R0, R1, R2, R3, R4, R5, R6, R7, MOV, CMP, ADD, SUB, NOT, CLR, LEA, INC, DEC, JMP, BNE,
                          GET, PRN, JSR, RTS, HLT, MACRO_START, MACRO_END, DATA, STRING, STRUCT, ENTRY, EXTERN,
                          DATA_NO_DOT, STRING_NO_DOT, STRUCT_NO_DOT, ENTRY_NO_DOT, EXTERN_NO_DOT,"\0"};
const char* REGISTERS[] = {R0, R1, R2, R3, R4, R5, R6, R7, "\0"};

const char SPECIAL_BASE32[32] = {'!', '@', '#', '$', '%', '^', '&', '*', '<', '>', 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h',
                               'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v'};