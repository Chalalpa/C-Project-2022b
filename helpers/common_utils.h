#ifndef _COMMON_UTILS_
#define _COMMON_UTILS_
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ctype.h"
#include "consts.h"
#include <math.h>


/* @ Function: int isCommentLine(char* line_data)
   @ Arguments: char* line_data
   line_data - the lined requested to know if is a comment line
   @ Description: The function checks if the given line is a 'comment line'. Returns 1 if it is, and 0 if not.
*/
int isCommentLine(char line_data[]);

/* @ Function: int isEmptyLine(char* line_data)
   @ Arguments: char* line_data
   line_data - the lined requested to know if is an empty line
   @ Description: The function checks if the given line is empty (contains just whitespaces, or nothing at all).
   Returns 1 if it is, and 0 if not.
*/
int isEmptyLine(char line_data[]);

/* @ Function: char* removeLeadingWhiteSpaces(char* line_data)
   @ Arguments: char* line_data
   line_data - the string wished to be trimmed
   @ Description: The function returns the given string without its leading whitespace (\t or \n or just a space).
*/
char* removeLeadingWhiteSpaces(char line_data[]);

/* @ Function: int startsWith(char* string, char* prefix)
   @ Arguments: char* string, char* prefix
   string - the tested string
   prefix - the string we wish to check if exists in the start of the 'string' param
   @ Description: The function checks if the given 'string' contains the 'prefix' at the start of it, and returns 1 if
   it does, and 0 if not.
*/
int startsWith(char* string, char* prefix);

/* @ Function: int isValidLabel(char* string)
   @ Arguments: char* string
   string - the tested string
   @ Description: The function checks if the given label name valid, or not. Returns 1 if it is, and 0 if it's not.
*/
int isValidLabel(char* string);

/* @ Function: int isValidMacro(char* string)
   @ Arguments: char* string
   string - the tested string
   @ Description: The function checks if the given macro name valid, or not. Returns 1 if it is, and 0 if it's not.
*/
int isValidMacro(char* string);

/* @ Function: int isDataDirective(char* string)
   @ Arguments: char* string
   string - the tested string
   @ Description: The function checks if the given 'string' is a data directive keyword (as defined in the project reqs).
   It returns 1 if it is, and 0 if it's not.
*/
int isDataDirective(char* string);

/* @ Function: int isExternOrEntryDirective(char* string)
   @ Arguments: char* string
   string - the tested string
   @ Description: The function checks if the given 'string' is an extern or entry directive keyword
   (as defined in the project reqs). It returns 1 if it is, and 0 if it's not.
*/
int isExternOrEntryDirective(char* string);


/* @ Function: int isDirective(char* string)
   @ Arguments: char* string
   string - the tested string
   @ Description: The function checks if the given 'string' is a directive keyword (as defined in the project reqs).
   It returns 1 if it is, and 0 if it's not.
*/
int isDirective(char* string);

/* @ Function: int isOperation(char* string)
   @ Arguments: char* string
   string - the tested string
   @ Description: The function checks if the given 'string' is an operation keyword (as defined in the project reqs).
   It returns 1 if it is, and 0 if it's not.
*/
int isOperation(char* string);

/* @ Function: int isRegister(char* string)
   @ Arguments: char* string
   string - the tested string
   @ Description: The function checks if the given string is a register keyword (according to project's reqs) or not.
   It returns 1 if it is, and 0 if it's not.
*/
int isRegister(char* string);

/* @ Function: int isKeyword(char* string)
   @ Arguments: char* string
   string - the tested string
   @ Description: The function checks if the given 'string' is an assembly keyword (as defined in the project reqs).
   It returns 1 if it is, and 0 if it's not.
*/
int isKeyword(char* string);

/* @ Function: int isValidNumber(char* string)
   @ Arguments: char* string
   string - the tested string
   @ Description: The function checks if string is a valid number (according to the project's reqs), or not.
   Returns 1 if it is, and 0 if it's not.
*/
int isValidNumber(char* string);

/* @ Function: int getAddressingIndex(char* string)
   @ Arguments: char* string
   string - the tested string.
   @ Description: The function checks if the string is an addressing of some index of a variable (struct), or not.
   Returns the index number if it is, and 0 if it's not (0 is not a valid addressing index).
*/
int getAddressingIndex(char* string);

/* @ Function: char* decToBinary(int n, int len)
   @ Arguments: int n, int len
   n - the number wished to be translated to binary
   len - the length of the resulting representation
   @ Description: The function calculates and returns the given number in a binary base.
*/
char* decToBinary(int n, int len);

/* @ Function: char* removeEndingWhiteSpaces(char* line_data)
   @ Arguments: char* line_data
   line_data - the string wished to be trimmed
   @ Description: The function returns the given string without its ending whitespace (\t or \n or just a space).
*/
char* removeEndingWhiteSpaces(char* line_data);

/* @ Function: int containsSpace(char* string)
   @ Arguments: char* string
   string - the tested string
   @ Description: The function checks if the given 'string' contains a whiteSpace and returns 1 if it does,
   and 0 if not.
*/
int containsSpace(char* string);

/* @ Function: int isValidString(char* string)
   @ Arguments: char* string
   string - the tested string
   @ Description: The function checks if string is a valid string (according to the project's reqs), or not.
   Returns 1 if it is, and 0 if it's not.
*/
int isValidString(char* string);

/* @ Function: getLabelName(char* line_data)
   @ Arguments: char* line_data
   line_data is the line that we want to extract the label from
   @ Description: The function extracts and returns, the label declared in the given line.
   It returns 0 if the label is invalid or no label was found.
*/
char* getLabelName(char* line_data);

/* @ Function: char* getNextField(char* line_data)
   @ Arguments: char* line_data
   line_data is the line that we want to extract the next field from
   @ Description: The function extracts and returns, the next field found in the given line.
   It returns 0 if no field was found.
*/
char* getNextField(char* line_data);

/* @ Function: char* binToSpecialB32(char* n)
   @ Arguments: char* n
   n - the number wished to be translated to special 32 base
   @ Description: The function calculates and returns the given binary number in a 32 special base.
*/
char* binToSpecialB32(char* n);

#endif