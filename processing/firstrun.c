#include "firstrun.h"


/* @ Function: int doesLabelExist(char* labelName, struct Symbol* head)
   @ Arguments: char* labelName, struct Symbol* head
   labelName is the name of the label wished to be searched
   head is the head of the Symbols list
   @ Description: The function iterates through the symbols list, and check if the given labelName exists there, or not.
   It will return 1 if it does exist, and 0 if it's not.
*/
int doesLabelExist(char* labelName, struct Symbol* head) {
    struct Symbol* headPointer = head;
    while(headPointer != NULL) {
        if (!strcmp(labelName, headPointer->name))
            return 1;
        headPointer = headPointer->next;
    }
    return 0;
}

/* @ Function: char** getOperands(char* trimmedLine, int operandsLimit)
   @ Arguments: char* trimmedLine, int operandsLimit
   trimmedLine is the line that we want to extract operands from (without the label/operation/directive)
   operandsLimit is amount of the maximum operands we would like to get
   @ Description: The function extracts and returns, an array containing the operands given in the sentence
   (that are separated by commas).
*/
char** getOperands(char* trimmedLine, int operandsLimit) {
    int operandsCount = 0, i;
    char* trimmedLinePointer = trimmedLine, * operand, * operandPointer, ** operands;

    operands = (char**)calloc(operandsLimit + 1, MAX_LINE_LEN * sizeof(char));

    /* Iterating through the line, to find operands separated by ',' */
    while(*trimmedLinePointer && operandsCount <= operandsLimit + 1) {
        operand = (char *)calloc(strlen(trimmedLine) + 1, sizeof(char));
        operandPointer = operand;
        while (*trimmedLinePointer && *trimmedLinePointer != ',') {
            if (*trimmedLinePointer != '\n')
                *operandPointer++ = *trimmedLinePointer;
            trimmedLinePointer++;
        }  /* Line reached its end, or a ',' was faced */
        operands[operandsCount] = operand;
        operandsCount++;
        if (*trimmedLinePointer && *trimmedLinePointer == ',')
            trimmedLinePointer++;
    }
    for (i = 0; i < operandsCount; i++) {
        /* Check if operand contains spaces, if it does, remove it and re-assign to operand object */
        char* trimmedOperand = removeLeadingWhiteSpaces(removeEndingWhiteSpaces(operands[i]));
        if (strcmp(operands[i], trimmedOperand)) {
            memmove(operands[i], trimmedOperand, strlen(trimmedOperand) + 1);
        }
    }
    return operands;
}

int getOperandsCount(char** operands) {
    int count = 0;
    char** operandsPointer = operands;
    while(*operandsPointer) {
        count++;
        operandsPointer++;
    }
    return count;
}


/* @ Function: struct DecodedLine* decodeDataDirectiveLine(char* directiveName, char* line_data)
   @ Arguments: char* directiveName, char* line_data
   directiveName is the specific directive in the line
   line_data is the data wished to be decoded
   @ Description: The function decodes the given directive sentence to binary code machine, according to the
   assembler rules. If an error was found during the decoding phase, it will return 0, and print the relevant error.
   If the decoding was successful, the function will return a new DecodedLine* object with the relevant data.
*/
struct DecodedLine* decodeDataDirectiveLine(char* directiveName, char* line_data) {
    struct Directive* directivePointer = getDirectiveByName(directiveName);
    struct DecodedLine* decoded;
    struct Directive directive;
    int operandsCount, i, status = 1, length;
    char* trimmedLine, **operands, *firstWord, *string, *binary;

    if (!directivePointer) {
        printf("Error! Wrong directive was provided: %s\n", line_data);
        return 0;
    }

    /* First initialization of the DecodedLine object */
    decoded = (struct DecodedLine*)malloc(sizeof(struct DecodedLine));
    decoded->isEmpty = 0;
    decoded->length = 0;
    decoded->next = NULL;
    decoded->binaryValue = NULL;
    decoded->srcOperandName = NULL;
    decoded->tgtOperandName = NULL;
    decoded->isData = 1;

    directive = *directivePointer;
    trimmedLine = removeEndingWhiteSpaces(removeLeadingWhiteSpaces(line_data));
    trimmedLine += strlen(directiveName);
    trimmedLine = removeEndingWhiteSpaces(removeLeadingWhiteSpaces(trimmedLine));

    /* Reading directive sentence data */
    if (!strcmp(directiveName, DATA)) {  /* .data directive */
        decoded->binaryValue = (char**)calloc(directive.maxOperandsNum + 1, (BINARY_WORD_SIZE + 1) * sizeof(char));
        operands = getOperands(trimmedLine, directive.maxOperandsNum);
        operandsCount = 0;
        length = 0;  /* Initializing the length of the decoded line sentence */
        /* Iterate, to find data structure members */
        while(operands[operandsCount] && status) {
            if(isValidNumber(operands[operandsCount])) {
                decoded->binaryValue[operandsCount] = (char*)calloc(BINARY_WORD_SIZE + 1, sizeof(char));
                binary = decToBinary(atoi(operands[operandsCount]), 10);
                strcpy(decoded->binaryValue[operandsCount], binary);
                free(binary);
                length++;
            }
            else {
                printf("Error! Found an invalid number operand for %s data directive: %s\n", DATA, line_data);
                status = 0;
            }
            operandsCount++;
        }
        decoded->length = length;
        if (!status) {  /* An error occured while reading the Data declaration, free what needed and exit */
            freeStringsArray(operands, getOperandsCount(operands));
            freeDecodedLine(decoded);
            return 0;
        }

        if (directive.minOperandsNum > operandsCount || directive.maxOperandsNum < operandsCount) {
            printf("Error! operands count num is less than minimum %d or more than maximum %d: %s\n",
                   directive.minOperandsNum, directive.maxOperandsNum, line_data);
            freeStringsArray(operands, getOperandsCount(operands));
            freeDecodedLine(decoded);
            return 0;
        }
        freeStringsArray(operands, getOperandsCount(operands));
        return decoded;  /* Data was declared correctly, return the DecodedLine object */
    }
    if (!strcmp(directiveName, STRING)) {  /* .string directive */
        if(!isValidString(trimmedLine)) {  /* String is wrongly set */
            printf("Error! found an invalid string under %s data directive declaration: %s\n", STRING, line_data);
            freeDecodedLine(decoded);
            return 0;
        }

        /* Length equals the trimmed line length, minus 2 because of the quotes, and plus one because of the last '\0' char */
        decoded->binaryValue = (char**)calloc(strlen(trimmedLine) - 1, (BINARY_WORD_SIZE + 1) * sizeof(char));
        decoded->length = strlen(trimmedLine) - 1;
        for (i = 1; i < strlen(trimmedLine) - 1; i++) {
            /* Iterating through string's chars, and writing the binary values of them */
            decoded->binaryValue[i - 1] = (char*)calloc(BINARY_WORD_SIZE + 1, sizeof(char));
            binary = decToBinary((int)(trimmedLine[i]), 10);
            strcpy(decoded->binaryValue[i - 1], binary);
            free(binary);
        }
        decoded->binaryValue[i - 1] = (char*)calloc(BINARY_WORD_SIZE + 1, sizeof(char));
        binary = decToBinary((int)(0), 10);  /* Last word must be the closer '\0' translated to binary */
        strcpy(decoded->binaryValue[i - 1], binary);
        free(binary);
        return decoded;  /* String was declared correctly, return the DecodedLine object */
    }
    if (!strcmp(directiveName, STRUCT)) {
        operands = getOperands(trimmedLine, 1);
        if (!isValidNumber(operands[0])) {  /* Verifying the first operand is a valid number */
            printf("Error! first operand of %s data directive must be valid number: %s\n", STRUCT,
                   line_data);
            free(decoded);
            decoded = 0;
            freeStringsArray(operands, getOperandsCount(operands));
        }
        else {
            /* We have a valid number as a first operand. Translate it to machine code and write as the first word */
            firstWord = (char*)calloc(BINARY_WORD_SIZE + 1, sizeof(char));
            binary = decToBinary(atoi(operands[0]), 10);
            strcpy(firstWord, binary);
            free(binary);
            trimmedLine += strlen(operands[0]);
            freeStringsArray(operands, getOperandsCount(operands));
            trimmedLine = removeLeadingWhiteSpaces(trimmedLine);
            if (*trimmedLine != ',') {  /* Verifying we have a second operand */
                printf("Error! Expected ',' after first struct's field declaration: %s\n", line_data);
                freeDecodedLine(decoded);
                decoded = 0;
            }
            else {
                trimmedLine = removeLeadingWhiteSpaces(trimmedLine + 1);
                if (*trimmedLine != '"') {  /* Verifying second operand starts with a quote, which symbols a string */
                    printf("Error! Expected start of string declaration as struct's second field: %s\n", line_data);
                    free(decoded);
                    decoded = 0;
                }
                else {  /* We have a valid string start */
                    string = (char *) calloc(strlen(trimmedLine) + 1, sizeof(char));
                    strncpy(string, trimmedLine, strlen(trimmedLine));
                    string[strlen(trimmedLine)] = '\0';
                    if (!isValidString(string)) {  /* String is invalid */
                        free(decoded);
                        decoded = 0;
                    }
                    else {  /* String is valid */
                        trimmedLine++;
                        /* Length should equal to string length plus one because of the first number operand and plus
                         * one because of the '\0' at the end of the string, minus 2 because of the string's quotes */
                        decoded->length = strlen(string);
                        decoded->binaryValue = (char **) calloc(decoded->length + 1,
                                                                (BINARY_WORD_SIZE + 1) * sizeof(char));
                        decoded->binaryValue[0] = (char *) calloc(BINARY_WORD_SIZE + 1, sizeof(char));

                        strcpy(decoded->binaryValue[0], firstWord);
                        /* Skip the '"' */
                        for (i = 1; i < strlen(string) - 1; i++) {
                            decoded->binaryValue[i] = (char *) calloc(BINARY_WORD_SIZE + 1,
                                                                      sizeof(char));
                            binary = decToBinary((int) (string[i]), 10);
                            strcpy(decoded->binaryValue[i], binary);
                            free(binary);
                        }
                        decoded->binaryValue[i] = (char *) calloc(BINARY_WORD_SIZE + 1,
                                                                  sizeof(char));
                        binary = decToBinary((int) (0), 10);
                        strcpy(decoded->binaryValue[i], binary);
                        free(binary);
                        trimmedLine += strlen(string) - 1;  /* Skip the string and the '"' char at the end */
                        if (*trimmedLine) {
                            printf("Error! Found trailing spaces after struct declaration: %s\n", line_data);
                            freeDecodedLine(decoded);
                            decoded = 0;
                        }
                    }
                    free(string);
                }
            }
            free(firstWord);
        }
        return decoded;  /* Struct was declared correctly, return the DecodedLine object */
    }
    printf("Error! No valid directive was found under the name of %s: %s\n", directiveName, line_data);
    return 0;
}

/* @ Function: int getDecodedLineLength(struct DecodedLine* line)
   @ Arguments: struct DecodedLine* line
   line - the DecodedLine* object we want to get its length
   @ Description: The function checks what's the number of binary machine code lines are extracted from the given line,
   and return it.
*/
int getDecodedLineLength(struct DecodedLine* line) {  /* For operative sentences */
    int i;
    int lengthCounter = 0;
    for (i=0; i < 5; i++)
        if(line->binaryValue[i] != NULL)
            lengthCounter++;
    return lengthCounter;
}

/* @ Function: struct DecodedLine* decodeOperationLine(struct Operation operation, char* line_data)
   @ Arguments: struct Operation operation, char* line_data
   operation is the Operation object of the relevant line wished to be decoded
   line_data is the data wished to be decoded
   @ Description: The function decodes the given operative sentence to binary code machine, according to the
   assembler rules. If an error was found during the decoding phase, it will return 0, and print the relevant error.
   If the decoding was successful, the function will return a new DecodedLine* object with the relevant data.
*/
struct DecodedLine* decodeOperationLine(struct Operation operation, char* line_data) {
    /* Iterations helpers */
    char *leftTrimmedLine, *current, **operands, *srcRegisterCode, *labelName, *binaryRegisterNumber, *binary;
    struct DecodedLine* decoded;
    int operandsCount = 0, i, sourceOperand, indexAddressing, binaryNumberLineIndex, registerNumber, status = 1;

    if (isCommentLine(line_data) || isEmptyLine(line_data))  /* Not an operative sentence */
        return 0;

    /* Verifying the operation name is part of the line data */
    leftTrimmedLine = removeLeadingWhiteSpaces(line_data);
    current = (char*)calloc(4, sizeof(char));
    while(*(leftTrimmedLine + 2)) {
        strncpy(current, leftTrimmedLine, 3);
        if (!strcmp(operation.name, current))
            break;
        leftTrimmedLine++;
    }
    free(current);

    if(!*(leftTrimmedLine + 2)) {
        /* If we couldn't find the operation */
        printf("Could not find operation %s in line %s:\n", operation.name, line_data);
        return 0;
    }

    leftTrimmedLine += 3;  /* Skipping the operation name */
    leftTrimmedLine = removeLeadingWhiteSpaces(leftTrimmedLine);
    operands = getOperands(leftTrimmedLine, 2);
    for (i=0; i<3; i++) {
        if (operands[i] != 0) {
            if(containsSpace(operands[i])) {
                printf("Error! Found whitespace in an operand: %s\n", line_data);
                status = 0;
            }
            operandsCount++;
        }
    }
    if (!status) {
        freeStringsArray(operands, getOperandsCount(operands));
        return 0;
    }

    /* Checking for appropriate operands number */
    if (operandsCount > operation.operandsNum) {
        printf("Error! expected %d operands for operation %s, found more: %s\n", operation.operandsNum,
               operation.name, line_data);
        freeStringsArray(operands, getOperandsCount(operands));
        return 0;
    }
    if (operandsCount < operation.operandsNum) {
        printf("Error! expected %d operands for operation %s, found less: %s\n", operation.operandsNum,
               operation.name, line_data);
        freeStringsArray(operands, getOperandsCount(operands));
        return 0;
    }
    for (i=0; i<operandsCount; i++)  /* Validating operands */
        if (isKeyword(operands[i]) && !isRegister(operands[i]))
            printf("Error! keyword (that is not a register) cannot be used as an operand: %s\n", line_data);

    /* Start building the decodedLine object */
    decoded = (struct DecodedLine*)malloc(sizeof(struct DecodedLine));
    decoded->isEmpty = 0;
    decoded->isData = 0;
    decoded->binaryValue = (char**)calloc(5, (BINARY_WORD_SIZE + 1) * sizeof(char));
    decoded->next = NULL;
    decoded->binaryValue[0] = (char*)calloc(BINARY_WORD_SIZE + 1, sizeof(char));
    strncpy(decoded->binaryValue[0], operation.binaryCode, 4);

    if (operandsCount == 0) {  /* No operands */
        strcat(decoded->binaryValue[0], "000000");
        decoded->srcOperandName = 0;
        decoded->tgtOperandName = 0;
        /* All lines except first shouldn't be filled */
        for(i=1; i<5; i++)
            decoded->binaryValue[i] = NULL;
        /* Finished decoding the line */
        decoded->length = 1;
        freeStringsArray(operands, getOperandsCount(operands));
        return decoded;
    }

    if (operandsCount == 1) {  /* 1 operand, we need to fill what we already know about it */
        strcat(decoded->binaryValue[0], "00");
        decoded->srcOperandName = 0;
        decoded->tgtOperandName = (char*)calloc(strlen(operands[0]) + 1, sizeof(char));
        strcpy(decoded->tgtOperandName, operands[0]);
        decoded->tgtOperandName = realloc(decoded->tgtOperandName, strlen(decoded->tgtOperandName) + 1);
        decoded->tgtOperandName[strlen(decoded->tgtOperandName)] = '\0';
        /* Lines 1 and 2 shouldn't be filled (Starting from 0) */
        for(i=1; i<3; i++)
            decoded->binaryValue[i] = NULL;
    }
    else {  /* 2 operands */
        decoded->srcOperandName = (char*)calloc(strlen(operands[0]) + 1, sizeof(char));
        strcpy(decoded->srcOperandName, operands[0]);
        decoded->srcOperandName = realloc(decoded->srcOperandName, strlen(decoded->srcOperandName) + 1);
        decoded->srcOperandName[strlen(decoded->srcOperandName)] = '\0';
        decoded->tgtOperandName = (char*)calloc(strlen(operands[1]) + 1, sizeof(char));
        strcpy(decoded->tgtOperandName, operands[1]);
        decoded->tgtOperandName = realloc(decoded->tgtOperandName, strlen(decoded->tgtOperandName) + 1);
        decoded->tgtOperandName[strlen(decoded->tgtOperandName)] = '\0';
    }

    /* Loop through the operands (maximum 2), and fill data accordingly */
    for (i=0; i<operandsCount; i++) {
        sourceOperand = 0;
        if (operandsCount == 2 && i == 0)
            sourceOperand = 1;  /* Current operand that we are looking at is the source operand */
        indexAddressing = getAddressingIndex(operands[i]);  /* S3.1 for example */
        if (startsWith(operands[i], "#")) {
            if (!isValidNumber(operands[i] + 1)) {
                printf("Error! Expected a number after '#' sign: %s\n", line_data);
                freeStringsArray(operands, getOperandsCount(operands));
                decoded->length = getDecodedLineLength(decoded);
                freeDecodedLine(decoded);
                return 0;
            }
            else {
                if (sourceOperand) {
                    if (!operation.sourceAddressingMethods.immediate) {  /* 00 */
                        printf("Error! operation %s does not support direct addressing from source: %s\n",
                               operation.name, line_data);
                        freeStringsArray(operands, getOperandsCount(operands));
                        decoded->length = getDecodedLineLength(decoded);
                        freeDecodedLine(decoded);
                        return 0;
                    }
                } else {
                    if (!operation.targetAddressingMethods.immediate) {
                        printf("Error! operation %s does not support direct addressing to target: %s\n", operation.name,
                               line_data);
                        freeStringsArray(operands, getOperandsCount(operands));
                        decoded->length = getDecodedLineLength(decoded);
                        freeDecodedLine(decoded);
                        return 0;
                    }

                }
                strcat(decoded->binaryValue[0], "00");
                if (sourceOperand)
                    binaryNumberLineIndex = 1;
                else
                    binaryNumberLineIndex = 3;
                decoded->binaryValue[binaryNumberLineIndex] = (char*)calloc(BINARY_WORD_SIZE + 1,
                                                                            sizeof(char));
                binary = decToBinary(atoi(operands[i] + 1), 8);
                strcpy(decoded->binaryValue[binaryNumberLineIndex], binary);
                strcat(decoded->binaryValue[binaryNumberLineIndex], "00");  /* A,R,E */
                free(binary);
            }
        }
        else if (isRegister(operands[i])) {
            if (sourceOperand) {
                if (!operation.sourceAddressingMethods.reg) {  /* 11 */
                    printf("Error! operation %s does not support reg addressing from source: %s\n",
                           operation.name, line_data);
                    freeStringsArray(operands, getOperandsCount(operands));
                    decoded->length = getDecodedLineLength(decoded);
                    freeDecodedLine(decoded);
                    return 0;
                }
            } else if (!operation.targetAddressingMethods.reg) {
                printf("Error! operation %s does not support reg addressing to target: %s\n", operation.name,
                       line_data);
                freeStringsArray(operands, getOperandsCount(operands));
                decoded->length = getDecodedLineLength(decoded);
                freeDecodedLine(decoded);
                return 0;
            }
            registerNumber = atoi(operands[i] + 1);
            binaryRegisterNumber = decToBinary(registerNumber, 4);;
            if(sourceOperand) {
                decoded->binaryValue[1] = (char*)calloc(BINARY_WORD_SIZE + 1, sizeof(char));
                strcpy(decoded->binaryValue[1], binaryRegisterNumber);
                strcat(decoded->binaryValue[1], "0000");
                strcat(decoded->binaryValue[1], "00");
            }
            else {
                if (operandsCount > 1 && (isRegister(operands[i-1]))) {
                    srcRegisterCode = (char*)calloc(4 + 1, sizeof(char));
                    strncpy(srcRegisterCode, decoded->binaryValue[1], 4);
                    srcRegisterCode[4] = '\0';
                    if(decoded->binaryValue[1] != NULL)
                        free(decoded->binaryValue[1]);
                    decoded->binaryValue[1] = (char*)calloc(BINARY_WORD_SIZE + 1, sizeof(char));
                    strcpy(decoded->binaryValue[1], srcRegisterCode);
                    free(srcRegisterCode);
                    strcat(decoded->binaryValue[1], binaryRegisterNumber);
                    strcat(decoded->binaryValue[1], "00");
                }
                else {
                    decoded->binaryValue[3] = (char*)calloc(BINARY_WORD_SIZE + 1, sizeof(char));
                    strcpy(decoded->binaryValue[3], "0000");
                    strcat(decoded->binaryValue[3], binaryRegisterNumber);
                    strcat(decoded->binaryValue[3], "00");
                }
            }
            free(binaryRegisterNumber);
            strcat(decoded->binaryValue[0], "11");
        }
        else if (indexAddressing != 0){
            labelName = (char*)calloc(MAX_LABEL_LEN + 1, sizeof(char));
            strncpy(labelName, operands[i], strlen(operands[i]));
            labelName[strlen(operands[i])- strlen(".1")] = '\0';
            if (!isValidLabel(labelName)) {
                printf("Error! tried to access an invalid label's property: %s\n", line_data);
                freeStringsArray(operands, getOperandsCount(operands));
                free(labelName);
                decoded->length = getDecodedLineLength(decoded);
                freeDecodedLine(decoded);
                return 0;
            }
            free(labelName);
            if (sourceOperand) {
                if (!operation.sourceAddressingMethods.index) {  /* 10 */
                    printf("Error! operation %s does not support index addressing from source: %s\n",
                           operation.name, line_data);
                    freeStringsArray(operands, getOperandsCount(operands));
                    decoded->length = getDecodedLineLength(decoded);
                    freeDecodedLine(decoded);
                    return 0;
                }
            } else if (!operation.targetAddressingMethods.index) {
                printf("Error! operation %s does not support index addressing to target: %s\n", operation.name,
                       line_data);
                freeStringsArray(operands, getOperandsCount(operands));
                decoded->length = getDecodedLineLength(decoded);
                freeDecodedLine(decoded);
                return 0;
            }
            if (sourceOperand)
                binaryNumberLineIndex = 1;
            else
                binaryNumberLineIndex = 3;
            decoded->binaryValue[binaryNumberLineIndex] = (char*)calloc(BINARY_WORD_SIZE + 1,
                                                                        sizeof(char));
            strcpy(decoded->binaryValue[binaryNumberLineIndex], TO_BE_FILLED);
            decoded->binaryValue[binaryNumberLineIndex + 1] = (char*)calloc(BINARY_WORD_SIZE + 1,
                                                                            sizeof(char));
            binary = decToBinary(indexAddressing, 8);
            strcpy(decoded->binaryValue[binaryNumberLineIndex + 1], binary);
            free(binary);
            strcat(decoded->binaryValue[binaryNumberLineIndex + 1], "00");
            strcat(decoded->binaryValue[0], "10");
        }
        else {
            if (sourceOperand) {
                if (!operation.sourceAddressingMethods.direct) {  /* 01 */
                    printf("Error! operation %s does not support direct addressing from source: %s\n",
                           operation.name, line_data);
                    freeStringsArray(operands, getOperandsCount(operands));
                    decoded->length = getDecodedLineLength(decoded);
                    freeDecodedLine(decoded);
                    return 0;
                }
            } else if (!operation.targetAddressingMethods.direct) {
                printf("Error! operation %s does not support direct addressing to target: %s\n", operation.name,
                       line_data);
                freeStringsArray(operands, getOperandsCount(operands));
                decoded->length = getDecodedLineLength(decoded);
                freeDecodedLine(decoded);
                return 0;
            }
            if (sourceOperand)
                binaryNumberLineIndex = 1;
            else
                binaryNumberLineIndex = 3;
            decoded->binaryValue[binaryNumberLineIndex] = (char*)calloc(BINARY_WORD_SIZE + 1, sizeof(char));
            strcpy(decoded->binaryValue[binaryNumberLineIndex], TO_BE_FILLED);
            strcat(decoded->binaryValue[0], "01");
        }
    }
    /* A,R,E code of first line should always be 00 in such case */
    decoded->binaryValue[0][BINARY_WORD_SIZE - 2] = '0';
    decoded->binaryValue[0][BINARY_WORD_SIZE - 1] = '0';
    decoded->length = getDecodedLineLength(decoded);
    freeStringsArray(operands, getOperandsCount(operands));
    return decoded;
}

/* @ Function: firstRun(char* file_name, int* IC, int* DC, struct Symbol* symbolHead, struct DecodedLine* decodedLineHead,
        struct Entry* entryHead, struct Extern* externHead);
   @ Arguments: char* file_name, int* IC, int* DC, struct Symbol* symbolHead, struct DecodedLine* decodedLineHead,
        struct Entry* entryHead, struct Extern* externHead
   file_name is the path that we should read from, without its extension
   IC, is a pointer to current Instruction Counter
   DC, is a pointer to current Data Counter
   symbolHead, that represents head of Symbol linked list.
   decodedLineHead, that represents head of DecodedLine linked list.
   entryHead, that represents head of Entry linked list.
   externHead, that represents head of Extern linked list.
   @ Description: The function goes through the .am file that should've been created earlier, verifies and decodes the
   file into binary machine code. In addition, it assigns values to the relevant lists, through the heads provided.
   The function will return 1 if the first run was successful, and 0 if not.
*/
int firstRun(char* file_name, int* IC, int* DC, struct Symbol* symbolHead, struct DecodedLine* decodedLineHead,
             struct Entry* entryHead, struct Extern* externHead) {
    char* full_file_path, line_data[MAX_LINE_LEN + 3], *labelFirstField, *name, *trailingChars, * labelName;
    struct Symbol* symbolHeadPointer = symbolHead;
    struct Symbol* tmpSymbol;
    struct DecodedLine* decodedLine;
    struct Operation operation;
    struct Extern* externTmp;
    struct Entry* entryTmp;
    int i = 0, operationIndex, status = 1;
    int freeLabelName;
    FILE *file_pointer;

    /* Conduct full file path */
    full_file_path = (char*)calloc(strlen(file_name) + strlen(PRE_PROCESSING_FILE_EXTENSION) + 1
            ,sizeof(char));
    strcpy(full_file_path, file_name);
    strcat(full_file_path, PRE_PROCESSING_FILE_EXTENSION);

    /* Verifying file exists */
    if (access(full_file_path, F_OK) != 0) {
        printf("No such file: '%s'\n", full_file_path);
        return 0;
    }

    /* Verifying file is reachable */
    file_pointer = fopen(full_file_path, "r");
    if (file_pointer == NULL) {
        printf("Error reading given file '%s'\n", full_file_path);
        return 0;
    }

    /* We initialize it with + 3, to ensure we have space for \n, for the '\0' char,
       and for another potential char (to verify we don't exceed from max line length limit) */
    memset(line_data, '\0', MAX_LINE_LEN + 3);

    /* Iterate through source file */
    while(fgets(line_data, MAX_LINE_LEN + 3, file_pointer)) {
        /* Handling general edge cases: */
        if (strlen(line_data) > MAX_LINE_LEN + 1) {  /* Including the \n char */
            printf("Error! Line %d of file %s exceeds line length limit (%d)\n",
                   i + 1, full_file_path, MAX_LINE_LEN);
            status = 0;
        }
        if(isEmptyLine(line_data)) {
            printf("Ignoring empty line number %d: %s\n", i + 1, line_data);
            i++;
            continue;
        }

        if(isCommentLine(line_data)) {
            printf("Ignoring comment line number %d: %s\n", i + 1, line_data);
            i++;
            continue;
        }

        labelName = getLabelName(line_data);
        if (labelName) {
            /* If a valid label was found, turn on proper flag */
            if (doesLabelExist(labelName, symbolHeadPointer)) {
                printf("Error! Label with this name was already initialized earlier in the file, err line %d: %s\n",
                       i, line_data);
                status = 0;
            }
            freeLabelName = 1;
        }
        else {
            free(labelName);
            freeLabelName = 0;
            labelName = "";
        }
        /* Initialize Symbol struct */
        tmpSymbol = NULL;
        labelFirstField = getNextField(removeLeadingWhiteSpaces(line_data) + strlen(labelName));
        if (!labelFirstField) {
            printf("Error! No content after label declaration in line %d: %s", i + 1, line_data);
            status = 0;
        }
        else if (isDirective(labelFirstField)) {
            /* Handling directives */
            if (isDataDirective(labelFirstField)) {
                /* Handling data directives */
                tmpSymbol = (struct Symbol*)malloc(sizeof(struct Symbol));
                tmpSymbol->next = NULL;
                tmpSymbol->name = (char*)calloc(strlen(labelName) + 1, sizeof(char));
                tmpSymbol->lineNum = i + 1;
                strcpy(tmpSymbol->name, labelName);
                tmpSymbol->type = DATA_DIRECTIVE;
                tmpSymbol->value = *IC;
                decodedLine = decodeDataDirectiveLine(labelFirstField,
                        line_data + strlen(labelName) + 1);
                if (decodedLine != 0) {
                    /* If line was encoded correctly, add it to decoded objects list */
                    decodedLine->value = *IC;
                    *DC += decodedLine->length;
                    decodedLineHead->next = decodedLine;
                    decodedLineHead = decodedLine;
                    *IC += decodedLine->length;
                    decodedLine->isEmpty = 0;
                }
                else {
                    printf("Error! Couldn't decode line number %d: %s\n", i + 1, line_data);
                    status = 0;
                }

            } else if (isExternOrEntryDirective(labelFirstField)) {
                /* Ignore label, it's meaningless in this case */
                name = getNextField(
                        line_data + strlen(labelName) + strlen(labelFirstField));
                if (name == 0) {
                    printf("Error! There must be a name attached to %s directives in line %d: %s\n",
                           labelFirstField, i + 1, line_data);
                    status = 0;
                }
                else if(!isValidLabel(name)) {
                    printf("Error! Found %s declaration for an invalid label in line %d: %s\n",
                           labelFirstField, i + 1, line_data);
                    status = 0;
                }
                else {
                    trailingChars = removeLeadingWhiteSpaces(line_data);
                    trailingChars += strlen(labelFirstField);
                    trailingChars = removeLeadingWhiteSpaces(trailingChars);
                    trailingChars += strlen(name);
                    trailingChars = getNextField(trailingChars);
                    if (trailingChars != 0) {
                        printf("Error! Found trailing chars after entry declaration in line %d: %s\n", i + 1,
                               line_data);
                        status = 0;
                        free(trailingChars);
                    }
                    else {
                        /* Entry/Extern is valid. Adding it to Entries/Externs and Symbols lists */
                        tmpSymbol = NULL;
                        if (!strcmp(labelFirstField, EXTERN)) { /* Check if it's an extern directive */
                            externTmp = (struct Extern *) malloc(sizeof(struct Extern));
                            externTmp->lineNum = i + 1;
                            externTmp->next = NULL;
                            externTmp->name = (char *)calloc(strlen(name) + 1, sizeof(char));
                            strcpy(externTmp->name, name);
                            externHead->next = externTmp;
                            externHead = externTmp;
                        } else {
                            entryTmp = (struct Entry *) malloc(sizeof(struct Entry));
                            entryTmp->next = NULL;
                            entryTmp->name = (char *) calloc(strlen(name) + 1, sizeof(char));
                            entryTmp->lineNum = i + 1;
                            strcpy(entryTmp->name, name);
                            entryHead->next = entryTmp;
                            entryHead = entryTmp;
                        }
                    }
                }
                free(name);
            }
        }
        else {
            /* Handling operative sentence */
            tmpSymbol = (struct Symbol *)malloc(sizeof(struct Symbol));
            tmpSymbol->next = NULL;
            tmpSymbol->name = (char*)calloc(strlen(labelName) + 1, sizeof(char));
            tmpSymbol->lineNum = i + 1;
            strcpy(tmpSymbol->name, labelName);
            tmpSymbol->type = CODE;
            tmpSymbol->value = *IC;
            if (!isOperation(labelFirstField)) {
                printf("Error! '%s' is not a valid operation in line %d: %s\n",
                       labelFirstField, i + 1, line_data);
                status = 0;
            }
            else {
                operationIndex = getOperationIndex(labelFirstField);
                operation = OPERATIONS_TABLE[operationIndex];
                decodedLine = decodeOperationLine(operation, line_data);
                if (decodedLine != 0) {
                    decodedLine->value = *IC;
                    decodedLine->lineNum = i + 1;
                    decodedLineHead->next = decodedLine;
                    decodedLineHead = decodedLine;
                    *IC += decodedLine->length;
                    decodedLine->isEmpty = 0;
                }
                else {
                    printf("Error! Couldn't decode line number %d: %s\n", i + 1, line_data);
                    status = 0;
                }
            }
        }
        if (tmpSymbol != 0) {
            symbolHead->next = tmpSymbol;
            symbolHead = tmpSymbol;
        }
        i++;
        free(labelFirstField);
        if (freeLabelName)
            free(labelName);
    }

    fclose(file_pointer);
    free(full_file_path);
    return status;
}