//
// Created by Moshe on 22/07/2022.
//

#include "common_utils.h"


char* removeLeadingWhiteSpaces(char line_data[]) {
    int i;
    for (i=0; i < strlen(line_data); i++) {
        if (!isspace(line_data[i]))
            break;
    }
    if (i >= strlen(line_data))
        return "";
    char new_string[strlen(line_data) - i];
    strncpy(new_string, &line_data[i], strlen(line_data) - i);
    new_string[strlen(line_data) - i] = '\0';
    char* new_string_p = new_string;
    return new_string_p;
}

int isCommentLine(char line_data[]) {
    char* trimmed_line = removeLeadingWhiteSpaces(line_data);
    if (*trimmed_line == ';')
        return 1;
    return 0;
}


int isEmptyLine(char line_data[]) {
    char* trimmed_line = removeLeadingWhiteSpaces(line_data);
    if (*trimmed_line == '\0')
        return 1;
    return 0;
}