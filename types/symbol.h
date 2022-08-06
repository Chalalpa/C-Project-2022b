#ifndef _SYMBOL_
#define _SYMBOL_

struct Symbol {
    char* name;  // The symbol name to be used later in the source code
    int value;  // The symbol value
    char* type; // The symbol type
    struct Symbol* next;  // A pointer to the next Symbol struct object
};

#endif