//
// Created by Moshe on 29/07/2022.
//

#include "consts.h"
#include <ctype.h>
#include <unistd.h>
#include "common_utils.h"
#include "operations.h"
#include "decodedline.h"


struct Symbol {
    char* name;  // The symbol name to be used later in the source code
    char* value;  // The symbol value
    char* type; // The symbol type
    struct Symbol* next;  // A pointer to the next Symbol struct object
};


int firstRun(char* file_name, int* IC, int* DC, struct Symbol* symbolHead, struct DecodedLine* decodedLineHead);

