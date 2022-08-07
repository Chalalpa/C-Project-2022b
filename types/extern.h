#ifndef _EXTERN_
#define _EXTERN_

/* Represents an Extern, declared by a label in some external files*/
struct Extern {
    char* name;
    int lineNum;
    struct Extern* next;
};

#endif
