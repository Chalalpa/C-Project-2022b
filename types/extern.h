#ifndef _EXTERN_
#define _EXTERN_

/* Represents an Extern, declared by a label in some external files*/
struct Extern {
    char* name;
    struct Extern* next;
};

#endif
