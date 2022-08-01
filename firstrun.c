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
    lineDataPointer += strlen(labelName);
    if (lineDataPointer[0] == ':') // Including the ':' sign
        lineDataPointer++;
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

char** getOperands(char* trimmedLine) {
    int operandsCount = 0;
    char** operands = (char**)calloc(3, MAX_LINE_LEN * sizeof(char));
    char* trimmedLinePointer = trimmedLine;
    while(*trimmedLinePointer && operandsCount <= 3) {
        char* operand = (char *)calloc(strlen(trimmedLine) + 1, sizeof(char));
        char* operandPointer = operand;
        while (*trimmedLinePointer && *trimmedLinePointer != ',') {
            if (*trimmedLinePointer != '\n')
                *operandPointer++ = *trimmedLinePointer;
            trimmedLinePointer++;
        }
        operands[operandsCount] = operand;
        operandsCount++;
        if (*trimmedLinePointer && *trimmedLinePointer == ',')
            trimmedLinePointer++;
    }
    int i;
    for (i = 0; i < operandsCount; i++)
        strcpy(operands[i], removeLeadingWhiteSpaces(removeEndingWhiteSpaces(operands[i])));
    return operands;
}


struct DecodedLine* decodeOperationLine(struct Operation operation, char* line_data) {
    if (isCommentLine(line_data) || isEmptyLine(line_data))
        return 0;
    char *leftTrimmedLine = removeLeadingWhiteSpaces(line_data);
    char* current = (char*)calloc(4, sizeof(char));
    while(*(leftTrimmedLine + 2)) {
        strncpy(current, leftTrimmedLine, 3);
        if (!strcmp(operation.name, current))
            break;
        leftTrimmedLine++;
    }
    if(!*(leftTrimmedLine + 2)) {
        // If we couldn't find the operation
        printf("Could not find operation %s in line %s:", operation.name, line_data);
        return 0;
    }
    leftTrimmedLine += 3;
    leftTrimmedLine = removeLeadingWhiteSpaces(leftTrimmedLine);
    char** operands = getOperands(leftTrimmedLine);
    int operandsCount = 0, i;
    for (i=0; i<3; i++) {
        if (operands[i] != 0) {
            if(containsSpace(operands[i])) {
                printf("Error! Found whitespace in an operand: %s", line_data);
                return 0;
            }
            operandsCount++;
        }
    }
    if (operandsCount != operation.operandsNum) {
        printf("Error! expected %d operands for operation %s, found more: %s", operation.operandsNum,
               operation.name, line_data);
        return 0;
    }
    for (i=0; i<operandsCount; i++)
        if (isKeyword(operands[i]) && !isRegister(operands[i]))
            printf("Error! keyword (that is not a register) cannot be used as an operand: %s", line_data);

    struct DecodedLine* decoded = (struct DecodedLine*)malloc(sizeof(struct DecodedLine*));
    decoded->next = NULL;
    decoded->binaryValue = (char**)calloc(5, (BINARY_WORD_SIZE + 1) * sizeof(char));
    decoded->binaryValue[0] = (char*)calloc(BINARY_WORD_SIZE + 1, sizeof(char));
    strncpy(decoded->binaryValue[0], operation.binaryCode, 4);
    if (operandsCount == 0) {
        strcat(decoded->binaryValue[0], "000000");
        decoded->srcOperandName = 0;
        decoded->tgtOperandName = 0;
        // All lines except first shouldn't be filled
        for(i=1; i<5; i++)
            decoded->binaryValue[i] = NULL;
        // Finished decoding the line
        return decoded;
    }
    if (operandsCount == 1) {
        strcat(decoded->binaryValue[0], "00");
        decoded->srcOperandName = 0;
        decoded->tgtOperandName = (char*)calloc(strlen(operands[0]), sizeof(char));
        strcpy(decoded->tgtOperandName, operands[0]);
        // Lines 1 and 2 shouldn't be filled (Starting from 0)
        for(i=1; i<3; i++)
            decoded->binaryValue[i] = NULL;
    }
    else {
        decoded->srcOperandName = (char*)calloc(strlen(operands[0]), sizeof(char));
        strcpy(decoded->srcOperandName, operands[0]);
        decoded->tgtOperandName = (char*)calloc(strlen(operands[1]), sizeof(char));
        strcpy(decoded->tgtOperandName, operands[1]);
    }
    for (i=0; i<operandsCount; i++) {
        int sourceOperand = 0;
        if (operandsCount == 2 && i == 0)
            sourceOperand = 1;  // Current operand that we are looking at is the source operand
        int indexAddressing = getIndexAddressing(operands[i]);  //S3.1 for example
        if (startsWith(operands[i], "#")) {
            if (!isNumber(operands[i] + 1)) {
                printf("Error! Expected a number after '#' sign: %s", line_data);
                return 0;
            }
            else {
                if (sourceOperand) {
                    if (!operation.sourceAddressingMethods.immediate) {  // 00
                        printf("Error! operation %s does not support direct addressing from source: %s",
                               operation.name, line_data);
                        return 0;
                    }
                } else {
                    if (!operation.targetAddressingMethods.immediate) {
                        printf("Error! operation %s does not support direct addressing to target: %s", operation.name,
                               line_data);
                        return 0;
                    }

                }
                strcat(decoded->binaryValue[0], "00");
                int binaryNumberLineIndex;
                if (sourceOperand)
                    binaryNumberLineIndex = 1;
                else
                    binaryNumberLineIndex = 3;
                decoded->binaryValue[binaryNumberLineIndex] = (char*)calloc(BINARY_WORD_SIZE, sizeof(char));
                strcpy(decoded->binaryValue[binaryNumberLineIndex], decToBinary(atoi(operands[i] + 1), 8));
                strcat(decoded->binaryValue[binaryNumberLineIndex], "00");  // A,R,E
            }
        }
        else if (isRegister(operands[i])) {
            if (sourceOperand) {
                if (!operation.sourceAddressingMethods.reg) {  // 11
                    printf("Error! operation %s does not support reg addressing from source: %s",
                           operation.name, line_data);
                    return 0;
                }
            } else if (!operation.targetAddressingMethods.reg) {
                printf("Error! operation %s does not support reg addressing to target: %s", operation.name,
                       line_data);
                return 0;
            }
            int registerNumber = atoi(operands[i] + 1);
            char* binaryRegisterNumber = decToBinary(registerNumber, 4);
            if(sourceOperand) {
                decoded->binaryValue[1] = (char*)calloc(BINARY_WORD_SIZE, sizeof(char));
                strcpy(decoded->binaryValue[1], "0000");
                strcat(decoded->binaryValue[1], binaryRegisterNumber);
                strcat(decoded->binaryValue[1], "00");
            }
            else {
                decoded->binaryValue[3] = (char*)calloc(BINARY_WORD_SIZE, sizeof(char));
                strcpy(decoded->binaryValue[3], binaryRegisterNumber);
                strcat(decoded->binaryValue[3], "0000");
                strcat(decoded->binaryValue[3], "00");
                if (operandsCount > 1 && (isRegister(operands[i-1]))) {
                    decoded->binaryValue[4] = (char*)calloc(BINARY_WORD_SIZE, sizeof(char));
                    strcpy(decoded->binaryValue[4], binaryRegisterNumber);
                    char* srcRegisterCode = (char*)calloc(4, sizeof(char));
                    strncpy(srcRegisterCode, decoded->binaryValue[1] + 4, 4);
                    srcRegisterCode[4] = '\0';
                    strcat(decoded->binaryValue[4], srcRegisterCode);
                    strcat(decoded->binaryValue[4], "00");
                }
            }
            strcat(decoded->binaryValue[0], "11");
        }
        else if (indexAddressing != 0){
            char* labelName = (char*)calloc(MAX_LABEL_LEN, sizeof(char));
            strncpy(labelName, operands[i], strlen(operands[i]));
            labelName[strlen(operands[i])- strlen(".1")] = '\0';
            if (!isValidLabel(labelName)) {
                printf("Error! tried to access an invalid label's property: %s", line_data);
                return 0;
            }
            if (sourceOperand) {
                if (!operation.sourceAddressingMethods.index) {  // 10
                    printf("Error! operation %s does not support index addressing from source: %s",
                           operation.name, line_data);
                    return 0;
                }
            } else if (!operation.targetAddressingMethods.index) {
                printf("Error! operation %s does not support index addressing to target: %s", operation.name,
                       line_data);
                return 0;
            }
            int binaryNumberLineIndex;
            if (sourceOperand)
                binaryNumberLineIndex = 1;
            else
                binaryNumberLineIndex = 3;
            decoded->binaryValue[binaryNumberLineIndex] = (char*)calloc(BINARY_WORD_SIZE, sizeof(char));
            strcpy(decoded->binaryValue[binaryNumberLineIndex], TO_BE_FILLED);
            decoded->binaryValue[binaryNumberLineIndex + 1] = (char*)calloc(BINARY_WORD_SIZE, sizeof(char));
            strcpy(decoded->binaryValue[binaryNumberLineIndex + 1], decToBinary(indexAddressing, 8));
            strcat(decoded->binaryValue[binaryNumberLineIndex + 1], "00");
            strcat(decoded->binaryValue[0], "10");
        }
        else {
            if (sourceOperand) {
                if (!operation.sourceAddressingMethods.direct) {  // 01
                    printf("Error! operation %s does not support direct addressing from source: %s",
                           operation.name, line_data);
                    return 0;
                }
            } else if (!operation.targetAddressingMethods.direct) {
                printf("Error! operation %s does not support direct addressing to target: %s", operation.name,
                       line_data);
                return 0;
            }
            int binaryNumberLineIndex;
            if (sourceOperand)
                binaryNumberLineIndex = 1;
            else
                binaryNumberLineIndex = 3;
            decoded->binaryValue[binaryNumberLineIndex] = (char*)calloc(BINARY_WORD_SIZE, sizeof(char));
            strcpy(decoded->binaryValue[binaryNumberLineIndex], TO_BE_FILLED);
            strcat(decoded->binaryValue[0], "01");
        }
    }
    // A,R,E code of first line should always be 00 in such case
    decoded->binaryValue[0][BINARY_WORD_SIZE - 2] = '0';
    decoded->binaryValue[0][BINARY_WORD_SIZE - 1] = '0';
    return decoded;
}


int firstRun(char* file_name, int* IC, int* DC, struct Symbol* symbolHead, struct DecodedLine* decodedLineHead) {
    char* full_file_path = (char*)calloc(strlen(file_name) + strlen(PRE_PROCESSING_FILE_EXTENSION)
            ,sizeof(char));
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
            if (doesLabelExist(labelName, symbolHead))
                printf("Error! Label with this name was already initialized earlier in the file: %s\n", line_data);
            else {
                tmp = NULL;
                tmp = (struct Symbol*)malloc(sizeof(struct Symbol));
                tmp->next = NULL;
                tmp->name = (char*)calloc(strlen(labelName), sizeof(char));
                strcpy(tmp->name, labelName);
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
                        struct DecodedLine* decodedLine = decodeOperationLine(operation, line_data);
                        if (decodedLine != 0) {
                            decodedLineHead->next = decodedLine;
                            decodedLineHead = decodedLine;
                        }
                    }
                }
                symbolHead->next = tmp;
                symbolHead = tmp;
            }
        } else if (!isCommentLine(line_data) && !isEmptyLine(line_data)) {
            char* labelFirstField = getLabelFirstField(line_data, "");
            if (!isOperation(labelFirstField))
                printf("Error! '%s' is not a valid operation: %s\n", labelFirstField, line_data);
            else {
                int operationIndex = getOperationIndex(labelFirstField);
                struct Operation operation = OPERATIONS_TABLE[operationIndex];
                struct DecodedLine* decodedLine = decodeOperationLine(operation, line_data);
                if (decodedLine != 0) {
                    decodedLineHead->next = decodedLine;
                    decodedLineHead = decodedLine;
                }
            }
        }
        i++;
    }
    return 1;
}