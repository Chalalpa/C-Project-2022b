#include "secondrun.h"

struct Symbol* getSymbol(struct Symbol* symbolHead, char* symbolName) {
    struct Symbol* headPointer = symbolHead;
    while(headPointer != NULL) {
        if (!strcmp(symbolName, headPointer->name))
            return headPointer;
        headPointer = headPointer->next;
    }
    return 0;
}


int secondRun(char* file_name, int* IC, int* DC, struct Symbol* symbolHead, struct DecodedLine* decodedLineHead,
              struct Entry* entryHead, struct Extern* externHead) {
    // Conduct full file path
    int ICCopy = MEMORY_START;
    int DCCopy = 0;
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

    // Iteration helper:
    struct Symbol* tmpSymbol;
    int i, isLabel;

    struct DecodedLine* linePointer = decodedLineHead;
    while (linePointer != NULL) {
        for (i=1; i < 5; i++) {
            if (linePointer->binaryValue[i] != NULL && linePointer->binaryValue[i] == TO_BE_FILLED) {
                char* symbolName = (char*)malloc(sizeof(char));
                if (0 < i < 3)
                    symbolName = strcpy(symbolName, linePointer->srcOperandName);
                else
                    symbolName = strcpy(symbolName, linePointer->tgtOperandName);
                struct Symbol* symbol = getSymbol(symbolHead, symbolName);
                if(!symbol) {
                    printf("Error! Symbol %s used in line %d is not declared anywhere\n",
                           symbolName, linePointer->lineNum);
                    return 0;
                }
                linePointer->binaryValue[i] = (char*)calloc(BINARY_WORD_SIZE, sizeof(char));
                if (symbol->type == EXTERN)
                    strcpy(linePointer->binaryValue[i], "0000000001");
                else {
                    strcpy(linePointer->binaryValue[i], decToBinary(symbol->value,
                                                                    BINARY_WORD_SIZE - 2));
                    strcat(linePointer->binaryValue[i], "10");
                }
            }
        }
        linePointer = linePointer->next;
    }

    linePointer = decodedLineHead;
    FILE *file_writer_pointer;
    char* new_file_path = (char*)malloc(strlen(file_name) + strlen(PRE_PROCESSING_FILE_EXTENSION));
    strcpy(new_file_path, file_name);
    strncat(new_file_path, OBJECT_FILE_EXTENSION, strlen(OBJECT_FILE_EXTENSION));
    file_writer_pointer = fopen(new_file_path,"w");
    while (linePointer != NULL) {
        for (i = 1; i < 5; i++) {
            if (linePointer->binaryValue[i] != NULL)
                fprintf(file_writer_pointer, "%s", linePointer->binaryValue[i]);
        }
    }
    /*
    char line_data[MAX_LINE_LEN + 1];
    memset(line_data, '\0', MAX_LINE_LEN + 1);

    while(fgets(line_data, MAX_LINE_LEN + 1, file_pointer)) {
        // Handling general edge cases:
        if (strlen(line_data) > MAX_LINE_LEN) {
            printf("Error! Line %d of file %s exceeds line length limit (%d)\n", i, full_file_path, MAX_LINE_LEN);
            return 0;
        }
        if(isCommentLine(line_data) || isEmptyLine(line_data)) {
            printf("Ignoring empty line number %d: %s\n", i, line_data);
            continue;
        }

        char* labelName = getLabelName(line_data);
        char* trimmedLine = removeLeadingWhiteSpaces(line_data) + strlen(labelName);
        char* firstField = getNextField(trimmedLine);
        if (isDataDirective(firstField) || isExternOrEntryDirective(firstField))
            if (firstField == ENTRY) {
                // Handle .entry directive..
            }
        else if (isOperation(firstField)) {

        }
        else {
            printf("Error! %s is not a valid label/directive/operation, in line %d: %s\n", line_data);
            return 0;
        }
    }
    */

    fclose(file_writer_pointer);
    return 1;
}