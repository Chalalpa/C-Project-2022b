//
// Created by Moshe on 22/07/2022.
//


#include "consts.h"
#include "ctype.h"
#include <string.h>
#include <unistd.h>


int isCommentLine(char line_data[]);
int isEmptyLine(char line_data[]);
char* removeLeadingWhiteSpaces(char line_data[]);
int startsWith(char* string, char* prefix);
int isValidLabel(char* string);
int isValidMacro(char* string);
int isDataDirective(char* string);
int isExternOrEntryDirective(char* string);
int isDirective(char* string);
int isOperation(char* string);
int getOperationIndex(char* operationName);
int decimalToBinary(int decimal);
int isRegister(char* string);
int isKeyword(char* string);
int isNumber(char* string);
int getIndexAddressing(char* string);
char* decToBinary(int n, int len);
char* removeEndingWhiteSpaces(char* line_data);
int containsSpace(char* string);

