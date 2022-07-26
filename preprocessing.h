//
// Created by Moshe on 22/07/2022.
//
#include "consts.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include "common_utils.h"

#define MACRO_START "macro"

/* Defining a Macro linked list structure*/
struct Macro {
    char* name;  // The macro name to be used later in the source code
    char* data;  // The macro content itself to deploy later in the code
    struct Macro* next;  // A pointer to the next Macro struct object
};

int readMacros(char* file_path, struct Macro* head);
int writeMacros(char* file_path, struct Macro* head);