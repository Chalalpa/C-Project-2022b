#ifndef _EXTERN_
#define _EXTERN_

/* Represents an Extern, declared by a label in some external files*/
struct Extern {
    char* name;  /* The extern name (label) */
    int lineNum;  /* The line number in which this extern is being declared (just the .extern declaration) */
    struct Extern* next;  /* A pointer to the next Extern object in the list*/
};

#endif
