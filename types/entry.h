#ifndef _ENTRY_
#define _ENTRY_

/* Represents an Entry, declared by a label somewhere in the files*/
struct Entry {
    char* name;
    struct Entry* next;
    int lineNum;
};

#endif