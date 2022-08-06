#ifndef _SECOND_RUN_
#define _SECOND_RUN_
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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


/* @ Function: int secondRun(char* file_name, struct Symbol* symbolHead, struct DecodedLine* decodedLineHead,
                             struct Entry* entryHead, struct Extern* externHead);
   @ Arguments: char* file_name, struct Symbol* symbolHead, struct DecodedLine* decodedLineHead,
                struct Entry* entryHead, struct Extern* externHead
   file_name is the path that we should read from, without its extension
   symbolHead, that represents head of Symbol linked list.
   decodedLineHead, that represents head of DecodedLine linked list.
   entryHead, that represents head of Entry linked list.
   externHead, that represents head of Extern linked list.
   @ Description: The function goes through the requested file, writes the translation of it to machine code in a unique
   32' base, and writes .ext and .ent files accordingly.
*/
int secondRun(char* file_name, struct Symbol* symbolHead, struct DecodedLine* decodedLineHead,
              struct Entry* entryHead, struct Extern* externHead);

#endif