#ifndef _SYMBOL_
#define _SYMBOL_

/* Represents a Symbol struct, declared by a label, or externally somewhere*/
struct Symbol {
    char* name;  /* The symbol name to be used later in the source code */
    int value;  /* The symbol value in the memory */
    char* type; /* The symbol type (data/extern/..) */
    int lineNum;
    struct Symbol* next;  /* A pointer to the next Symbol struct object */
};

#endif