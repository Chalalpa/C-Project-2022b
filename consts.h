
#define BINARY_WORD_SIZE 10
#define MEMORY_START 100
#define OP_SIZE 16
#define RAM_MEMORY_SIZE 256
#define MAX_LABEL_LEN 30
#define MAX_LINE_LEN 80
#define MAX_LINES 100
#define SOURCE_FILE_EXTENSION ".as"
#define PRE_PROCESSING_FILE_EXTENSION ".am"

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

extern const char* DATA_DIRECTIVES[];
extern const char* EXTERN_OR_ENTRY[];
extern const char* OPERATIONS[];
extern const char* KEYWORDS[];


/* Hash Table Hashsize (Result is currently 64)  */
#define HASHSIZE ((RAM_MEMORY_SIZE / (MAX_LABEL_LEN + 1)) / 4)

/* Masks for creation of to binary word memory image */
#define A 0x0004
#define R 0x0002
#define E 0x0001
#define IMMEDIATE_ADDR 0x00
#define DIRECT_ADDR 0x01
#define INDEX_ADDR 0xA
#define REGISTER_DIRECT_ADDR 0xB


/* Given just random Numbers that will be different then 0*/
#define _TYPE_DATA 11
#define _TYPE_STRING 22
#define _TYPE_ENTRY 33
#define _TYPE_EXTERNAL 44
#define _TYPE_CODE 55

/* Sentences */
#define NEWLINE_DELIMITER '\n'
#define COMMENT_PREFIX ';'