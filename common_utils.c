//
// Created by Moshe on 22/07/2022.
//

#include "common_utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define EMPTY_STRING = ''


char* decToBinary(int n, int len)
{
    char* binaryNumber = (char*)calloc(8, sizeof(char));
    char* binaryNumberPointer = binaryNumber;
    // Size of an integer is assumed to be 8 bits
    for (int i = len - 1; i >= 0; i--) {
        int k = n >> i; // right shift
        if (k & 1) // helps us know the state of first bit
            *binaryNumberPointer = '1';
        else
            *binaryNumberPointer = '0';
        binaryNumberPointer++;
    }
    *binaryNumberPointer = '\0';
    return binaryNumber;
}

char* removeLeadingWhiteSpaces(char* line_data) {
    while (*line_data) {
        if (!isspace(*line_data))
            break;
        line_data++;
    }
    return line_data;
}

char* removeEndingWhiteSpaces(char* line_data) {
    int i = strlen(line_data) - 1;
    char* lineDataPointer = line_data;
    while (i > 0) {
        if (!isspace(line_data[i]))
            break;
        i--;
    }
    lineDataPointer[i + 1] = '\0';
    return lineDataPointer;
}

int isCommentLine(char* line_data) {
    char* trimmed_line = removeLeadingWhiteSpaces(line_data);
    if (*trimmed_line == ';')
        return 1;
    return 0;
}


int isEmptyLine(char* line_data) {
    char* trimmed_line = removeLeadingWhiteSpaces(line_data);
    if (*trimmed_line == '\0')
        return 1;
    return 0;
}

int startsWith(char* string, char* prefix)
{
    while(*prefix)
        if(*prefix++ != *string++)
            return 0;

    return 1;
}

int containsSpace(char* string) {
    char* stringPointer = string;
    while(*stringPointer) {
        if (isspace(*stringPointer))
            return 1;
        stringPointer++;
    }
    return 0;
}

int isStringInArray(char* string, char** arr) {
    char** arrPointer = arr;
    while(*arrPointer) {
        if(!strcmp(string, *arrPointer))
            return 1;
        arrPointer++;
    }
    return 0;
}

int isKeyword(char* string) {
    return isStringInArray(string, KEYWORDS);
}

int isDataDirective(char* string) {
    return isStringInArray(string, DATA_DIRECTIVES);
}

int isExternOrEntryDirective(char* string) {
    return isStringInArray(string, EXTERN_OR_ENTRY);
}

int isDirective(char* string) {
    return isDataDirective(string) || isExternOrEntryDirective(string);
}

int isOperation(char* string) {
    return isStringInArray(string, OPERATIONS);
}

int getOperationIndex(char* operationName) {
    char** operationsPointer = OPERATIONS;
    int i=0;
    while(*operationsPointer) {
        if(!strcmp(operationName, *operationsPointer))
            return i;
        i++;
        operationsPointer++;
    }
    return -1;
}

int isValidLabelOrMacroName(char* string, char* objectName) {
    if (strlen(string) > MAX_LABEL_LEN) {
        printf("Error. %s name exceeds limit (%d): %s\n", objectName, MAX_LABEL_LEN, string);
        return 0;
    }

    if (!isalpha(*string)) {
        printf("Error. %s's first char should be a letter (lowercase/uppercase): %s\n", objectName, string);
        return 0;
    }

    char* string_copy = string;
    while(*string_copy) {
        if (!(isalnum(*string_copy))) {
            printf("Error. Found non alpha-numeric char in %s's name declaration: %s\n", objectName, string);
            return 0;
        }
        string_copy++;
    }

    if (isKeyword(string)) {
        printf("Error. %s's name is actually a keyword: %s\n", objectName, string);
        return 0;
    }
    return 1;
}

int isRegister(char* string) {
    return isStringInArray(string, REGISTERS);
}

int isValidMacro(char* string) {
    return isValidLabelOrMacroName(string, "macro");
}

int isValidLabel(char* string) {
    return isValidLabelOrMacroName(string, "label");
}


int isLabel(char* string) {
    return 0;
}

int isNumber(char* string) {
    char* strPointer = string;
    if (*strPointer == '-' || *strPointer == '+')
        strPointer++;
    while(*strPointer)
        if(!isdigit(*strPointer++))
            return 0;
    return 1;
}

int isValidString(char* string) {
    if (*string != '"')
        return 0;
    if (*(string + strlen(string) - 1) != '"')
        return 0;
    int i;
    for (i=1; i < strlen(string) - 1; i++) {
        if(string[i] == '"')
            return 0;
    }
    return 1;
}

int getIndexAddressing(char* string) {
    char* strPointer = string;
    strPointer += strlen(string) - 2;
    if (!strcmp(strPointer, ".1"))
        return 1;
    if (!(strcmp(strPointer, ".2")))
        return 2;
    return 0;
}