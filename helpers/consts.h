#ifndef CONSTS
#define CONSTS

#define BINARY_WORD_SIZE 10
#define MEMORY_START 100
#define OP_SIZE 16
#define RAM_MEMORY_SIZE 256
#define MAX_LABEL_LEN 30
#define MAX_LINE_LEN 80
#define MAX_LINES 100
#define SOURCE_FILE_EXTENSION ".as"
#define PRE_PROCESSING_FILE_EXTENSION ".am"
#define OBJECT_FILE_EXTENSION ".ob"

/* Directives */
#define DATA ".data"
#define STRING ".string"
#define STRUCT ".struct"
#define ENTRY ".entry"
#define EXTERN ".extern"

/* Symbols */
#define CODE "code"
#define DATA_DIRECTIVE "data"
#define EXTERNAL_DIRECTIVE "external"

/* Macro */
#define MACRO_START "macro"
#define MACRO_END "endmacro"

#define EMPTY_STRUCT_NAME "!empty_struct!"

/* Registers*/
#define R0 "r0"
#define R1 "r1"
#define R2 "r2"
#define R3 "r3"
#define R4 "r4"
#define R5 "r5"
#define R6 "r6"
#define R7 "r7"
#define REGS_SIZE 8


/* Operations */
#define MOV "mov"
#define CMP "cmp"
#define ADD "add"
#define SUB "sub"
#define NOT "not"
#define CLR "clr"
#define LEA "lea"
#define INC "inc"
#define DEC "dec"
#define JMP "jmp"
#define BNE "bne"
#define GET "get"
#define PRN "prn"
#define JSR "jsr"
#define RTS "rts"
#define HLT "hlt"


/* Decoding */
#define TO_BE_FILLED "?"

extern const char* DATA_DIRECTIVES[];
extern const char* EXTERN_OR_ENTRY[];
extern const char* OPERATIONS[];
extern const char* KEYWORDS[];
extern const char* REGISTERS[];

#endif
