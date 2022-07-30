//
// Created by Moshe on 22/07/2022.
//

#include "common_utils.h"
#include <stdio.h>
#include <string.h>
#define EMPTY_STRING = ''


int decimalToBinary(int decimal) // Maximum 15
{
    int binary = 0;
    int rem, temp = 1;
    while (decimal!=0)
    {
        rem = decimal%2;
        decimal = decimal / 2;
        binary = binary + rem*temp;
        temp = temp * 10;
    }
    return binary;
}

char* removeLeadingWhiteSpaces(char* line_data) {
    while (*line_data) {
        if (!isspace(*line_data))
            break;
        line_data++;
    }
    return line_data;
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

int isValidMacro(char* string) {
    return isValidLabelOrMacroName(string, "macro");
}

int isValidLabel(char* string) {
    return isValidLabelOrMacroName(string, "label");
}


int isLabel(char* string) {
    return 0;
}