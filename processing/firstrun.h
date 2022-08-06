#ifndef _FIRST_RUN_
#define _FIRST_RUN_
#include <ctype.h>
#include <unistd.h>
#include "../helpers/consts.h"
#include "../helpers/common_utils.h"
#include "../types/operations.h"
#include "../types/decodedline.h"
#include "../types/directives.h"
#include "../types/entry.h"
#include "../types/extern.h"
#include "../types/symbol.h"
#include "../types/entry.h"


/* @ Function: firstRun(char* file_name, int* IC, int* DC, struct Symbol* symbolHead, struct DecodedLine* decodedLineHead,
        struct Entry* entryHead, struct Extern* externHead);
   @ Arguments: char* file_name, int* IC, int* DC, struct Symbol* symbolHead, struct DecodedLine* decodedLineHead,
        struct Entry* entryHead, struct Extern* externHead
   file_name is the path that we should read from, without its extension
   IC, is a pointer to current Instruction Counter
   DC, is a pointer to current Data Counter
   symbolHead, that represents head of Symbol linked list.
   decodedLineHead, that represents head of DecodedLine linked list.
   entryHead, that represents head of Entry linked list.
   externHead, that represents head of Extern linked list.
   @ Description: The function goes through the .am file that should've been created earlier, verifies and decodes the
   file into binary machine code. In addition, it assigns values to the relevant lists, through the heads provided.
   The function will return 1 if the first run was successful, and 0 if not.
*/
int firstRun(char* file_name, int* IC, int* DC, struct Symbol* symbolHead, struct DecodedLine* decodedLineHead,
             struct Entry* entryHead, struct Extern* externHead);

#endif
