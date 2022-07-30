//
// Created by Moshe on 29/07/2022.
//

#include "firstrun.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>

int doesLabelExist(char* labelName, struct Symbol* head) {
    struct Symbol* headPointer = head;
    while(headPointer != NULL) {
        if (!strcmp(labelName, headPointer->name))
            return 1;
        headPointer = headPointer->next;
    }
    return 0;
}

char* getLabelFirstField(char* line_data, char* labelName) {
    char* lineDataPointer = line_data;
    lineDataPointer += strlen(labelName) + 1; // Including the ':' sign
    lineDataPointer = removeLeadingWhiteSpaces(lineDataPointer);
    char *fieldName = (char *)calloc(strlen(lineDataPointer) + 1, sizeof(char));
    char *fieldNamePointer = fieldName;
    while (*lineDataPointer && !isspace(*lineDataPointer)) {
        *fieldNamePointer = *lineDataPointer;
        fieldNamePointer++;
        lineDataPointer++;
    }
    if (!*lineDataPointer) {
        // Consider to remove
        printf("No data found after label declaration: %s\n", line_data);
        return 0;
    }
    return fieldName;
}

int isLabelDirective(char* line_data, char* labelName) {
    char* lineDataPointer = line_data;
    lineDataPointer += strlen(labelName) + 1; // Including the ':' sign
    char* lineWithoutLabelDeclaration = removeLeadingWhiteSpaces(lineDataPointer);
    char *fieldName = (char *) malloc(strlen(lineWithoutLabelDeclaration) + 1);
    char *fieldNamePointer = fieldName;
    while (*lineWithoutLabelDeclaration && !isspace(*lineWithoutLabelDeclaration)) {
        *fieldNamePointer = *lineWithoutLabelDeclaration;
        fieldNamePointer++;
        lineWithoutLabelDeclaration++;
    }
    if (!*lineWithoutLabelDeclaration) {
        printf("No data found after label declaration: %s\n", line_data);
        return 0;
    }
    return fieldNamePointer;
}

char* getLabelName(char* line_data) {
    if (isCommentLine(line_data) || isEmptyLine(line_data))
        return 0;
    char *leftTrimmedLine = removeLeadingWhiteSpaces(line_data);
    char *labelName = (char *) calloc(strlen(leftTrimmedLine) + 1, sizeof(char));
    char *labelNamePointer = labelName;
    while (*leftTrimmedLine && !isspace(*leftTrimmedLine) && *leftTrimmedLine != ':') {
        *labelNamePointer = *leftTrimmedLine;
        labelNamePointer++;
        leftTrimmedLine++;
    }
    if (!*leftTrimmedLine || isspace(*leftTrimmedLine)) {
        // No ':' found until first space
        return 0;
    }
    // We found ':'
    if(!isspace(*(leftTrimmedLine + 1))) {
        // if the next char after the ':' is not a whitespace, it's not a legal label
        printf("Found trailing chars after label's declaration, expected a whitespace: %s\n", line_data);
        return 0;
    }
    *labelNamePointer = '\0';
    realloc(labelName, strlen(labelName) + 1);
    if(isValidLabel(labelName))
        return labelName;
    // If label is not valid, the function should print the exact reason
    return 0;

}


int firstRun(char* file_name, int* IC, int* DC, struct Symbol* head) {
    char* full_file_path = (char*)calloc(strlen(file_name) + strlen(PRE_PROCESSING_FILE_EXTENSION), sizeof(char));
    strcpy(full_file_path, file_name);
    strncat(full_file_path, PRE_PROCESSING_FILE_EXTENSION, strlen(PRE_PROCESSING_FILE_EXTENSION));
    FILE *file_pointer;
    struct Symbol* tmp;
    if (access(full_file_path, F_OK) != 0) {
        printf("No such file: '%s'\n", full_file_path);
        return 0;
    }
    file_pointer = fopen(full_file_path, "r");
    if (file_pointer == NULL) {
        printf("Error reading given file '%s'\n", full_file_path);
        return 0;
    }
    char line_data[MAX_LINE_LEN + 1];
    memset(line_data, '\0', MAX_LINE_LEN + 1);
    int i = 0;
    while(fgets(line_data, MAX_LINE_LEN + 1, file_pointer)) {
        int DCCopy = *DC, ICCopy = *IC;
        if (strlen(line_data) > MAX_LINE_LEN)
            printf("Error! Line %d of file %s exceeds line length limit (%d)\n", i, full_file_path, MAX_LINE_LEN);
        char* labelName = getLabelName(line_data);
        if (labelName) {
            if (doesLabelExist(labelName, head))
                printf("Error! Label with this name was already initialized earlier in the file: %s\n", line_data);
            else {
                tmp = NULL;
                tmp = (struct Symbol*)malloc(sizeof(struct Symbol));
                tmp->next = NULL;
                tmp->name = (char*)calloc(strlen(labelName), sizeof(char));
                char* labelFirstField = getLabelFirstField(line_data, labelName);
                if (isDirective(labelFirstField)) {
                    if (isDataDirective(labelFirstField)) {
                        // Insert a data directive symbol
                        tmp->type = (char*)calloc(strlen(DATA_DIRECTIVE), sizeof(char));
                        tmp->type = DATA_DIRECTIVE;
                        tmp->value = DCCopy;
                        // DC+=...
                    } else if (isExternOrEntryDirective(labelFirstField)) {
                        if(!strcmp(labelFirstField, EXTERN)) { // Check if it's an extern directive
                            tmp->type = (char*)calloc(strlen(EXTERNAL_DIRECTIVE), sizeof(char));
                            tmp->type = EXTERNAL_DIRECTIVE;
                            // Do what's .extern directive needs...
                        }
                        else {
                            // Code for handling .entry directive...
                        }
                    }
                }
                else {
                    tmp->type = (char*)calloc(strlen(CODE), sizeof(char));
                    tmp->type = CODE;
                    tmp->value = ICCopy;
                    if (!isOperation(labelFirstField))
                        printf("Error! '%s' is not a valid operation: %s\n", labelFirstField, line_data);
                    else {
                        int operationIndex = getOperationIndex(labelFirstField);
                        struct Operation operation = OPERATIONS_TABLE[operationIndex];
                        break;
                    }
                }
            }
        }
        i++;
    }
}