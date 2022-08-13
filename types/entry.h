#ifndef _ENTRY_
#define _ENTRY_

/* Represents an Entry, declared by a label somewhere in the files*/
struct Entry {
    char* name;  /* The entry name (label) */
    struct Entry* next;  /* A pointer to the next Entry object in the list*/
    int lineNum;  /* The line number in which this entry is being declared (just the .entry declaration) */
};

#endif