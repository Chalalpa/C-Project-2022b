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
    int operandsCount = 0;
    char** operands = (char**)calloc(operandsLimit + 1, MAX_LINE_LEN * sizeof(char));
    char* trimmedLinePointer = trimmedLine;
    while(*trimmedLinePointer && operandsCount <= operandsLimit + 1) {
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
    if (!directivePointer) {
        printf("Error! Wrong directive was provided: %s\n", line_data);
        return 0;
    }
    struct DecodedLine* decoded = (struct DecodedLine*)malloc(sizeof(struct DecodedLine));
    decoded->next = NULL;
    decoded->srcOperandName = NULL;
    decoded->tgtOperandName = NULL;
    struct Directive directive = *directivePointer;
    char* trimmedLine = removeEndingWhiteSpaces(removeLeadingWhiteSpaces(line_data));
    trimmedLine += strlen(directiveName);
    trimmedLine = removeEndingWhiteSpaces(removeLeadingWhiteSpaces(trimmedLine));
    if (!strcmp(directiveName, DATA)) {  // .data directive
        decoded->binaryValue = (char**)calloc(directive.maxOperandsNum + 1, (BINARY_WORD_SIZE + 1) * sizeof(char));
        char** operands = getOperands(trimmedLine, directive.maxOperandsNum);
        int operandsCount = 0;
        while(operands[operandsCount]) {
            if(isValidNumber(operands[operandsCount])) {
                decoded->binaryValue[operandsCount] = (char*)calloc(BINARY_WORD_SIZE + 1, sizeof(char));
                strcpy(decoded->binaryValue[operandsCount], decToBinary(atoi(operands[operandsCount]), 10));
            }
            else {
                printf("Error! Found an invalid number operand for %s data directive: %s\n", DATA, line_data);
                free(operands);
                return 0;
            }
            operandsCount++;
        }
        if (directive.minOperandsNum > operandsCount || directive.maxOperandsNum < operandsCount) {
            printf("Error! operands count num is less than minimum %d or more than maximum %d: %s\n",
                   directive.minOperandsNum, directive.maxOperandsNum, line_data);
            free(operands);
            return 0;
        }
        decoded->length = operandsCount;
        free(operands);
        return decoded;
    }
    if (!strcmp(directiveName, STRING)) {
        decoded->binaryValue = (char**)calloc(strlen(trimmedLine) - 1, (BINARY_WORD_SIZE + 1) * sizeof(char));
        if(!isValidString(trimmedLine)) {
            printf("Error! found an invalid string under %s data directive declaration: %s\n", STRING, line_data);
            return 0;
        }
        int i;
        for (i = 1; i < strlen(trimmedLine) - 1; i++) {
            decoded->binaryValue[i - 1] = (char*)calloc(BINARY_WORD_SIZE + 1, sizeof(char));
            strcpy(decoded->binaryValue[i - 1], decToBinary((int)(trimmedLine[i]), 10));
        }
        decoded->binaryValue[i - 1] = (char*)calloc(BINARY_WORD_SIZE + 1, sizeof(char));
        strcpy(decoded->binaryValue[i - 1], decToBinary((int)(0), 10));
        decoded->length = strlen(trimmedLine) - 1;
        return decoded;
    }
    if (!strcmp(directiveName, STRUCT)) {
        char **operands = getOperands(trimmedLine, 1);
        if (!isValidNumber(operands[0])) {
            printf("Error! first operand of %s data directive must be valid number: %s\n", STRUCT,
                   line_data);
            free(operands);
            return 0;
        }
        char* firstWord = (char *) calloc(BINARY_WORD_SIZE + 1, sizeof(char));
        strcpy(firstWord, decToBinary(atoi(operands[0]), 10));
        trimmedLine += strlen(operands[0]);
        trimmedLine = removeLeadingWhiteSpaces(trimmedLine);
        if (*trimmedLine != ',') {
            printf("Error! Expected ',' after first struct's field declaration: %s\n", line_data);
            free(operands);
            return 0;
        }
        trimmedLine = removeLeadingWhiteSpaces(trimmedLine + 1);
        if (*trimmedLine != '"') {
            printf("Error! Expected start of string declaration as struct's second field: %s\n", line_data);
            free(operands);
            return 0;
        }
        char *string = (char *)calloc(strlen(trimmedLine), sizeof(char));
        strncpy(string, trimmedLine, strlen(trimmedLine));
        string[strlen(trimmedLine)] = '\0';
        if (!isValidString(string)) {
            printf("Error! Struct's second field must be a valid string: %s\n", line_data);
            free(operands);
            return 0;
        }
        trimmedLine++;
        decoded->length = strlen(string);
        decoded->binaryValue = (char**)calloc(decoded->length + 1, (BINARY_WORD_SIZE + 1) * sizeof(char));
        decoded->binaryValue[0] = (char *) calloc(BINARY_WORD_SIZE + 1, sizeof(char));
        strcpy(decoded->binaryValue[0], firstWord);
        // Skip the '"'
        int i;
        for (i = 1; i < strlen(string) - 1; i++) {
            decoded->binaryValue[i] = (char *) calloc(BINARY_WORD_SIZE + 1,
                                                          sizeof(char));
            strcpy(decoded->binaryValue[i], decToBinary((int) (string[i]), 10));
        }
        decoded->binaryValue[i] = (char *) calloc(BINARY_WORD_SIZE + 1,
                                                      sizeof(char));
        strcpy(decoded->binaryValue[i], decToBinary((int)(0), 10));
        trimmedLine += strlen(string) - 1;  // Skip the string and the '"' char at the end
        if(*trimmedLine) {
            printf("Error! Found trailing spaces after struct declaration: %s\n", line_data);
            free(operands);
            return 0;
        }
        free(operands);
        return decoded;
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
int getDecodedLineLength(struct DecodedLine* line) {
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
        printf("Could not find operation %s in line %s:\n", operation.name, line_data);
        return 0;
    }
    leftTrimmedLine += 3;
    leftTrimmedLine = removeLeadingWhiteSpaces(leftTrimmedLine);
    char** operands = getOperands(leftTrimmedLine, 2);
    int operandsCount = 0, i;
    for (i=0; i<3; i++) {
        if (operands[i] != 0) {
            if(containsSpace(operands[i])) {
                printf("Error! Found whitespace in an operand: %s\n", line_data);
                free(operands);
                return 0;
            }
            operandsCount++;
        }
    }
    if (operandsCount > operation.operandsNum) {
        printf("Error! expected %d operands for operation %s, found more: %s\n", operation.operandsNum,
               operation.name, line_data);
        free(operands);
        return 0;
    }
    if (operandsCount < operation.operandsNum) {
        printf("Error! expected %d operands for operation %s, found less: %s\n", operation.operandsNum,
               operation.name, line_data);
        free(operands);
        return 0;
    }
    for (i=0; i<operandsCount; i++)
        if (isKeyword(operands[i]) && !isRegister(operands[i]))
            printf("Error! keyword (that is not a register) cannot be used as an operand: %s\n", line_data);

    struct DecodedLine* decoded = (struct DecodedLine*)malloc(sizeof(struct DecodedLine*));
    decoded->next = NULL;
    decoded->binaryValue = (char**)calloc(6, (BINARY_WORD_SIZE + 1) * sizeof(char));
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
        decoded->length = 1;
        free(operands);
        return decoded;
    }
    if (operandsCount == 1) {
        strcat(decoded->binaryValue[0], "00");
        decoded->srcOperandName = 0;
        decoded->tgtOperandName = (char*)calloc(strlen(operands[0]) + 1, sizeof(char));
        strcpy(decoded->tgtOperandName, operands[0]);
        // Lines 1 and 2 shouldn't be filled (Starting from 0)
        for(i=1; i<3; i++)
            decoded->binaryValue[i] = NULL;
    }
    else {
        decoded->srcOperandName = (char*)calloc(strlen(operands[0]) + 1, sizeof(char));
        strcpy(decoded->srcOperandName, operands[0]);
        decoded->tgtOperandName = (char*)calloc(strlen(operands[1]) + 1, sizeof(char));
        strcpy(decoded->tgtOperandName, operands[1]);
    }
    for (i=0; i<operandsCount; i++) {
        int sourceOperand = 0;
        if (operandsCount == 2 && i == 0)
            sourceOperand = 1;  // Current operand that we are looking at is the source operand
        int indexAddressing = getAddressingIndex(operands[i]);  //S3.1 for example
        if (startsWith(operands[i], "#")) {
            if (!isValidNumber(operands[i] + 1)) {
                printf("Error! Expected a number after '#' sign: %s\n", line_data);
                free(operands);
                return 0;
            }
            else {
                if (sourceOperand) {
                    if (!operation.sourceAddressingMethods.immediate) {  // 00
                        printf("Error! operation %s does not support direct addressing from source: %s\n",
                               operation.name, line_data);
                        free(operands);
                        return 0;
                    }
                } else {
                    if (!operation.targetAddressingMethods.immediate) {
                        printf("Error! operation %s does not support direct addressing to target: %s\n", operation.name,
                               line_data);
                        free(operands);
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
                    printf("Error! operation %s does not support reg addressing from source: %s\n",
                           operation.name, line_data);
                    free(operands);
                    return 0;
                }
            } else if (!operation.targetAddressingMethods.reg) {
                printf("Error! operation %s does not support reg addressing to target: %s\n", operation.name,
                       line_data);
                free(operands);
                return 0;
            }
            int registerNumber = atoi(operands[i] + 1);
            char* binaryRegisterNumber = decToBinary(registerNumber, 4);
            if(sourceOperand) {
                decoded->binaryValue[1] = (char*)calloc(BINARY_WORD_SIZE, sizeof(char));
                strcpy(decoded->binaryValue[1], binaryRegisterNumber);
                strcat(decoded->binaryValue[1], "0000");
                strcat(decoded->binaryValue[1], "00");
            }
            else {
                if (operandsCount > 1 && (isRegister(operands[i-1]))) {
                    char* srcRegisterCode = (char*)calloc(4, sizeof(char));
                    strncpy(srcRegisterCode, decoded->binaryValue[1], 4);
                    srcRegisterCode[4] = '\0';
                    decoded->binaryValue[1] = (char*)calloc(BINARY_WORD_SIZE, sizeof(char));
                    strcpy(decoded->binaryValue[1], srcRegisterCode);
                    strcat(decoded->binaryValue[1], binaryRegisterNumber);
                    strcat(decoded->binaryValue[1], "00");
                }
                else {
                    decoded->binaryValue[3] = (char*)calloc(BINARY_WORD_SIZE, sizeof(char));
                    strcpy(decoded->binaryValue[3], "0000");
                    strcat(decoded->binaryValue[3], binaryRegisterNumber);
                    strcat(decoded->binaryValue[3], "00");
                }
            }
            strcat(decoded->binaryValue[0], "11");
        }
        else if (indexAddressing != 0){
            char* labelName = (char*)calloc(MAX_LABEL_LEN, sizeof(char));
            strncpy(labelName, operands[i], strlen(operands[i]));
            labelName[strlen(operands[i])- strlen(".1")] = '\0';
            if (!isValidLabel(labelName)) {
                printf("Error! tried to access an invalid label's property: %s\n", line_data);
                free(operands);
                return 0;
            }
            if (sourceOperand) {
                if (!operation.sourceAddressingMethods.index) {  // 10
                    printf("Error! operation %s does not support index addressing from source: %s\n",
                           operation.name, line_data);
                    free(operands);
                    return 0;
                }
            } else if (!operation.targetAddressingMethods.index) {
                printf("Error! operation %s does not support index addressing to target: %s\n", operation.name,
                       line_data);
                free(operands);
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
                    printf("Error! operation %s does not support direct addressing from source: %s\n",
                           operation.name, line_data);
                    free(operands);
                    return 0;
                }
            } else if (!operation.targetAddressingMethods.direct) {
                printf("Error! operation %s does not support direct addressing to target: %s\n", operation.name,
                       line_data);
                free(operands);
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
    decoded->length = getDecodedLineLength(decoded);
    free(operands);
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
    // Conduct full file path
    char* full_file_path = (char*)calloc(strlen(file_name) + strlen(PRE_PROCESSING_FILE_EXTENSION)
            ,sizeof(char));
    strcpy(full_file_path, file_name);
    strncat(full_file_path, PRE_PROCESSING_FILE_EXTENSION, strlen(PRE_PROCESSING_FILE_EXTENSION));
    FILE *file_pointer;

    // Verifying file exists
    if (access(full_file_path, F_OK) != 0) {
        printf("No such file: '%s'\n", full_file_path);
        return 0;
    }

    // Verifying file is reachable
    file_pointer = fopen(full_file_path, "r");
    if (file_pointer == NULL) {
        printf("Error reading given file '%s'\n", full_file_path);
        return 0;
    }

    // We initialize it with + 3, to ensure we have space for \n, for the '\0' char,
    // and for another potential char (to verify we don't exceed from max line length limit)
    char line_data[MAX_LINE_LEN + 3];
    memset(line_data, '\0', MAX_LINE_LEN + 3);

    // Iteration helper:
    struct Symbol* symbolHeadPointer = symbolHead;
    struct Symbol* tmpSymbol;
    int i = 0, isLabel;

    while(fgets(line_data, MAX_LINE_LEN + 3, file_pointer)) {
        // Handling general edge cases:
        if (strlen(line_data) > MAX_LINE_LEN + 1) {  // Including the \n char
            printf("Error! Line %d of file %s exceeds line length limit (%d)\n",
                   i + 1, full_file_path, MAX_LINE_LEN);
            return 0;
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

        isLabel = 0;
        char* labelName = getLabelName(line_data);
        if (labelName) {
            // If a valid label was found, turn on proper flag
            isLabel = 1;
            if (doesLabelExist(labelName, symbolHeadPointer)) {
                printf("Error! Label with this name was already initialized earlier in the file, err line %d: %s\n",
                       i, line_data);
                return 0;
            }
        }
        else
            labelName = "";
        // Initialize Symbol struct
        tmpSymbol = NULL;
        tmpSymbol = (struct Symbol*)malloc(sizeof(struct Symbol));
        tmpSymbol->next = NULL;
        tmpSymbol->name = (char*)calloc(strlen(labelName), sizeof(char));
        tmpSymbol->lineNum = i + 1;
        strcpy(tmpSymbol->name, labelName);


        char* labelFirstField = getNextField(removeLeadingWhiteSpaces(line_data) + strlen(labelName));
        if (!labelFirstField) {
            printf("Error! No content after label declaration in line %d: %s", i + 1, line_data);
            return 0;
        }
        if (isDirective(labelFirstField)) {
            // Handling directives
            if (isDataDirective(labelFirstField)) {
                // Handling data directives
                tmpSymbol->type = (char*)calloc(strlen(DATA_DIRECTIVE), sizeof(char));
                tmpSymbol->type = DATA_DIRECTIVE;
                tmpSymbol->value = *IC;
                struct DecodedLine* decodedLine = decodeDataDirectiveLine(labelFirstField,
                        line_data + strlen(labelName) + 1);
                if (decodedLine != 0) {
                    // If line was encoded correctly, add it to decoded objects list
                    decodedLine->value = *IC;
                    *DC += decodedLine->length;
                    decodedLineHead->next = decodedLine;
                    decodedLineHead = decodedLine;
                    *IC += decodedLine->length;
                }
                else {
                    printf("Error! Couldn't decode line number %d: %s\n", i + 1, line_data);
                    return 0;
                }

            } else if (isExternOrEntryDirective(labelFirstField)) {
                // Ignore label, it's meaningless in this case
                tmpSymbol = NULL;
                char* name = getNextField(
                        line_data + strlen(labelName) + strlen(labelFirstField));
                if (name == 0) {
                    printf("Error! There must be a name attached to %s directives in line %d: %s\n",
                           labelFirstField, i + 1, line_data);
                    return 0;
                }
                if(!isValidLabel(name)) {
                    printf("Error! Found %s declaration for an invalid label in line %d: %s\n",
                           labelFirstField, i + 1, line_data);
                    return 0;
                }
                char* trailingChars = removeLeadingWhiteSpaces(line_data);
                trailingChars += strlen(labelFirstField);
                trailingChars = removeLeadingWhiteSpaces(trailingChars);
                trailingChars += strlen(name);
                trailingChars = getNextField(trailingChars);
                if (trailingChars != 0) {
                    printf("Error! Found trailing chars after entry declaration in line %d: %s\n", i + 1,
                           line_data);
                    return 0;
                }
                // Entry/Extern is valid. Adding it to Entries/Externs and Symbols lists
                tmpSymbol = (struct Symbol*)malloc(sizeof(struct Symbol));
                tmpSymbol->next = NULL;
                tmpSymbol->name = (char*)calloc(strlen(labelName), sizeof(char));
                strcpy(tmpSymbol->name, name);
                strcpy(tmpSymbol->name, name);
                tmpSymbol->type = labelFirstField;
                if(!strcmp(labelFirstField, EXTERN)) { // Check if it's an extern directive
                    struct Extern* externTmp = (struct Extern*)malloc(sizeof(struct Extern));
                    externTmp->lineNum = i + 1;
                    externTmp->next = NULL;
                    externTmp->name = (char*)calloc(strlen(name), sizeof(char));
                    strcpy(externTmp->name, name);
                    externHead->next = externTmp;
                    externHead = externTmp;
                }
                else {
                    struct Entry* entryTmp = (struct Entry*)malloc(sizeof(struct Entry));
                    entryTmp->next = NULL;
                    entryTmp->name = (char*)calloc(strlen(name), sizeof(char));
                    entryTmp->lineNum = i + 1;
                    strcpy(entryTmp->name, name);
                    entryHead->next = entryTmp;
                    entryHead = entryTmp;
                }
            }
        }
        else {
            // Handling operative sentence
            tmpSymbol->type = (char*)calloc(strlen(CODE), sizeof(char));
            tmpSymbol->type = CODE;
            tmpSymbol->value = *IC;
            if (!isOperation(labelFirstField)) {
                printf("Error! '%s' is not a valid operation in line %d: %s\n",
                       labelFirstField, i + 1, line_data);
                return 0;
            }
            else {
                int operationIndex = getOperationIndex(labelFirstField);
                struct Operation operation = OPERATIONS_TABLE[operationIndex];
                struct DecodedLine* decodedLine = decodeOperationLine(operation, line_data);
                if (decodedLine != 0) {
                    decodedLine->value = *IC;
                    decodedLine->lineNum = i + 1;
                    decodedLineHead->next = decodedLine;
                    decodedLineHead = decodedLine;
                    *IC += decodedLine->length;
                    *DC += decodedLine->length;
                }
                else {
                    printf("Error! Couldn't decode line number %d: %s\n", i + 1, line_data);
                    return 0;
                }
            }
        }
        if (isLabel && tmpSymbol != 0) {
            symbolHead->next = tmpSymbol;
            symbolHead = tmpSymbol;
        }
        i++;
    }

    fclose(file_pointer);
    return 1;
}