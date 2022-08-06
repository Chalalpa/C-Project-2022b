#ifndef _ENTRY_
#define _ENTRY_

struct Entry {
    char* name;
    struct Entry* next;
    int lineNum;
};

#endif