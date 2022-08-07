#ifndef _PRE_PROCESSING_
#define _PRE_PROCESSING_
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include "../helpers/consts.h"
#include "../helpers/common_utils.h"


/* Defining a Macro linked list structure*/
struct Macro {
    char* name;  /* The macro name to be used later in the source code */
    char* data;  /* The macro content itself to deploy later in the code */
    struct Macro* next;  /* A pointer to the next Macro struct object */
};

/* @ Function: int readMacros(char* file_name, struct Macro* head);
   @ Arguments: char* file_name, struct Macro* head
   file_name is the path that we should read macros from, without its extension
   head, that represents head of Macro linked list.
   @ Description: The function reads the macros from the file, and puts them into the macros list.
   It returns 1 if there were no errors reading the file, and 0 if there were errors reading it.
*/
int readMacros(char* file_path, struct Macro* head);

/* @ Function: int writeMacros(char* file_name, struct Macro* head);
   @ Arguments: char* file_name, struct Macro* head
   file_name is the path that we should read macros from, without its extension
   head, that represents head of Macro linked list.
   @ Description: The function writes a new .am file, while replacing the macros calls with the data that was defined
   In addition, macros definition shouldn't be included in the newly written file.
   It returns 1 if there were no errors writing the file, and 0 if there were errors writing it.
*/
int writeMacros(char* file_path, struct Macro* head);

#endif