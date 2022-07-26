//
// Created by Moshe on 22/07/2022.
//

#include "common_utils.h"

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