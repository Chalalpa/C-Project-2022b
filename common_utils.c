//
// Created by Moshe on 22/07/2022.
//

#include "common_utils.h"
#include <stdio.h>
#include <string.h>
#define EMPTY_STRING = ''


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

int isKeyword(char* string) {
    char **keywordsPointer = KEYWORDS;
    while(*keywordsPointer) {
        if(!strcmp(string, *keywordsPointer))
            return 1;
        keywordsPointer++;
    }
    return 0;
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