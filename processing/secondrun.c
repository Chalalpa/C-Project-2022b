#include "secondrun.h"

/* @ Function: struct Symbol* getSymbol(struct Symbol* symbolHead, char* symbolName);
   @ Arguments: struct Symbol* symbolHead, char* symbolName
   symbolHead, represents head of Symbol linked list.
   symbolName is the name of the symbol wished to be found
   @ Description: The function iterates through the Symbols list and finds the symbol requested by its name.
   It returns it, if it was found in the list, or 0, if it wasn't found.
*/
struct Symbol* getSymbol(struct Symbol* symbolHead, char* symbolName) {
    struct Symbol* headPointer = symbolHead;
    while(headPointer != NULL) {
        if (!strcmp(symbolName, headPointer->name))
            return headPointer;
        headPointer = headPointer->next;
    }
    return 0;
}

/* @ Function: struct Extern* getExtern(struct Extern* externHead, char* externName);
   @ Arguments: struct Extern* externHead, char* externName
   externHead, represents head of Extern linked list.
   externName is the name of the extern wished to be found
   @ Description: The function iterates through the Externs list and finds the extern requested by its name.
   It returns it, if it was found in the list, or 0, if it wasn't found.
*/
struct Extern* getExtern(struct Extern* externHead, char* externName) {
    struct Extern* headPointer = externHead;
    while(headPointer != NULL) {
        if (!strcmp(externName, headPointer->name))
            return headPointer;
        headPointer = headPointer->next;
    }
    return 0;
}

/* @ Function: int writeEntriesFile(char* fileName, struct Entry* entryHead, struct Symbol* symbolHead);
   @ Arguments: char* fileName, struct Entry* entryHead, struct Symbol* symbolHead
   fileName is the path that we should write to, without extension
   symbolHead, that represents head of Symbol linked list.
   entryHead, that represents head of Entry linked list.
   @ Description: The function writes to an .ent file, the externs usages in the read file.
   Returns 1 if it was successful, or 0 if errors were found.
*/
int writeEntriesFile(char* fileName, struct Entry* entryHead, struct Symbol* symbolHead) {
    struct Entry* entryPointer = entryHead;
    FILE *file_writer_pointer;
    char* new_file_path = (char*)malloc(strlen(fileName) + strlen(PRE_PROCESSING_FILE_EXTENSION));
    strcpy(new_file_path, fileName);
    strncat(new_file_path, ENTRIES_FILE_EXTENSION, strlen(ENTRIES_FILE_EXTENSION));
    file_writer_pointer = fopen(new_file_path,"w");
    int entriesCount = 0;
    while (entryPointer != NULL) {
        if (strcmp(entryPointer->name, EMPTY_STRUCT_NAME)) {
            struct Symbol* symbol = getSymbol(symbolHead, entryPointer->name);
            if (!symbol) {
                printf("Error! Entry %s declared in line %d is not initialized later in the file\n",
                       entryPointer->name, entryPointer->lineNum);
                return 0;
            }
            entriesCount++;
            fprintf(file_writer_pointer, "%s %s\n", entryPointer->name,
                    binToSpecialB32(decToBinary(symbol->value, 8)));
        }
        entryPointer = entryPointer->next;
    }
    fclose(file_writer_pointer);
    if (entriesCount <= 0)
        remove(new_file_path);
    return 1;
}


/* @ Function: int writeExternsFile(char* fileName, struct Extern* externHead, struct Symbol* symbolHead,
                                struct DecodedLine* decodedLineHead);
   @ Arguments: char* fileName, struct Extern* externHead, struct Symbol* symbolHead,
                struct DecodedLine* decodedLineHead
   fileName is the path that we should write to, without extension
   symbolHead, that represents head of Symbol linked list.
   decodedLineHead, that represents head of DecodedLine linked list.
   externHead, that represents head of Extern linked list.
   @ Description: The function writes to an .ext file, the externs usages in the read file.
   Returns 1 if it was successful, or 0 if errors were found.
*/
int writeExternsFile(char* fileName, struct Extern* externHead, struct Symbol* symbolHead,
                     struct DecodedLine* decodedLineHead) {
    struct DecodedLine* decodeLinePointer = decodedLineHead;
    FILE *file_writer_pointer;
    char* new_file_path = (char*)malloc(strlen(fileName) + strlen(PRE_PROCESSING_FILE_EXTENSION));
    strcpy(new_file_path, fileName);
    strncat(new_file_path, EXTERNS_FILE_EXTENSION, strlen(EXTERNS_FILE_EXTENSION));
    file_writer_pointer = fopen(new_file_path,"w");
    int externsCount = 0;
    while (decodeLinePointer != NULL) {
        if (!decodeLinePointer->isEmpty) {
            if (decodeLinePointer->srcOperandName != NULL && !(isRegister(decodeLinePointer->srcOperandName))
                && !(*decodeLinePointer->srcOperandName == '#')) {
                char* name = (char*)calloc(strlen(decodeLinePointer->srcOperandName),
                                                 sizeof(char));
                strcpy(name, decodeLinePointer->srcOperandName);
                if (!strcmp(name + strlen(name) - 2, ".1") ||
                    !strcmp(name + strlen(name) - 2, ".2"))
                    name[strlen(name) - 2] = '\0';
                struct Symbol* symbol = getSymbol(symbolHead, name);
                if (symbol == NULL) {
                    struct Extern* externPointer = getExtern(externHead, name);
                    if (externPointer == NULL) {
                        printf("Error! Found usage of undefined symbol - %s in line %d\n",
                               name, decodeLinePointer->lineNum);
                        return 0;
                    }
                    fprintf(file_writer_pointer, "%s %s\n", externPointer->name,
                            binToSpecialB32(decToBinary(decodeLinePointer->value + 1, 8)));
                    externsCount++;
                }
            }
            if (decodeLinePointer->tgtOperandName != NULL && !(isRegister(decodeLinePointer->tgtOperandName))
                && !(*decodeLinePointer->tgtOperandName == '#')) {
                char* name = (char*)calloc(strlen(decodeLinePointer->tgtOperandName),
                                           sizeof(char));
                strcpy(name, decodeLinePointer->tgtOperandName);
                if (!strcmp(name + strlen(name) - 2, ".1") ||
                    !strcmp(name + strlen(name) - 2, ".2"))
                    name[strlen(name) - 2] = '\0';
                struct Symbol* symbol = getSymbol(symbolHead, name);
                if (symbol == NULL) {
                    struct Extern* externPointer = getExtern(externHead, name);
                    if (externPointer == NULL) {
                        printf("Error! Found usage of undefined symbol - %s in line %d\n",
                               name, decodeLinePointer->lineNum);
                        return 0;
                    }
                    int addressValue;
                    if (decodeLinePointer->binaryValue[1] == NULL)
                        addressValue = decodeLinePointer->value + 1;
                    else
                        if (decodeLinePointer->binaryValue[2] != NULL)
                            addressValue = decodeLinePointer->value + 3;
                        else
                            addressValue = decodeLinePointer->value + 2;
                    fprintf(file_writer_pointer, "%s %s\n", externPointer->name,
                            binToSpecialB32(decToBinary(addressValue, 8)));
                    externsCount++;
                }
            }
        }
        decodeLinePointer = decodeLinePointer->next;
    }
    fclose(file_writer_pointer);
    if (externsCount <= 0)
        remove(new_file_path);
    return 1;
}

/* @ Function: int secondRun(char* file_name, struct Symbol* symbolHead, struct DecodedLine* decodedLineHead,
                             struct Entry* entryHead, struct Extern* externHead);
   @ Arguments: char* file_name, struct Symbol* symbolHead, struct DecodedLine* decodedLineHead,
                struct Entry* entryHead, struct Extern* externHead
   file_name is the path that we should read from, without its extension
   symbolHead, that represents head of Symbol linked list.
   decodedLineHead, that represents head of DecodedLine linked list.
   entryHead, that represents head of Entry linked list.
   externHead, that represents head of Extern linked list.
   @ Description: The function goes through the requested file, writes the translation of it to machine code in a unique
   32' base, and writes .ext and .ent files accordingly.
*/
int secondRun(char* file_name, struct Symbol* symbolHead, struct DecodedLine* decodedLineHead,
              struct Entry* entryHead, struct Extern* externHead) {
    // Conduct full file path
    int ICCopy = MEMORY_START;

    // Iteration helper:
    int i;
    struct DecodedLine* linePointer = decodedLineHead;

    // Getting the first valuable DecodedLine object
    while (linePointer != NULL && linePointer->isEmpty == 1)
        linePointer = linePointer->next;

    // Replacing all <TO_BE_FILLED> lines with the real value, after retrieving all symbols in previous stages
    while (linePointer != NULL) {
        for (i=1; i < 5; i++) {
            if (linePointer->binaryValue[i] != NULL && !strcmp(linePointer->binaryValue[i], TO_BE_FILLED)) {
                char* symbolName = (char*)malloc(sizeof(char));
                if (0 < i && i < 3)
                    // Index for source operand
                    symbolName = strcpy(symbolName, linePointer->srcOperandName);
                else
                    // Index for target operand
                    symbolName = strcpy(symbolName, linePointer->tgtOperandName);
                // Extracting extension of struct addressing if relevant
                if (!strcmp(symbolName + strlen(symbolName) - 2, ".1") ||
                    !strcmp(symbolName + strlen(symbolName) - 2, ".2"))
                    symbolName[strlen(symbolName) - 2] = '\0';
                struct Symbol* symbol = getSymbol(symbolHead, symbolName);
                struct Extern* externObject;
                if(!symbol) {
                    // If it was not found in the Symbols list, it should be found in the Externs list
                    externObject = getExtern(externHead, symbolName);
                    if (!externObject) {
                        printf("Error! Symbol %s used in line %d is not declared anywhere\n",
                               symbolName, linePointer->lineNum);
                        return 0;
                    }
                }
                linePointer->binaryValue[i] = (char*)calloc(BINARY_WORD_SIZE, sizeof(char));
                if (!symbol && externObject != NULL)
                    // If it's an extern, fill line accordingly
                    strcpy(linePointer->binaryValue[i], "0000000001");
                else {
                    // If it's a symbol, fill the line according to the symbol value
                    strcpy(linePointer->binaryValue[i], decToBinary(symbol->value,
                                                                    BINARY_WORD_SIZE - 2));
                    strcat(linePointer->binaryValue[i], "10");
                }
            }
        }
        linePointer = linePointer->next;
    }

    // Writing entries file
    if (!writeEntriesFile(file_name, entryHead, symbolHead)) {
        printf("Error! Couldn't write entries file\n");
        return 0;
    }

    // Writing externs file
    if (!writeExternsFile(file_name, externHead, symbolHead, decodedLineHead)) {
        printf("Error! Couldn't write entries file\n");
        return 0;
    }

    linePointer = decodedLineHead;
    // Getting the first valuable DecodedLine object
    while (linePointer != NULL && linePointer->isEmpty == 1)
        linePointer = linePointer->next;

    FILE *file_writer_pointer;
    char* new_file_path = (char*)malloc(strlen(file_name) + strlen(PRE_PROCESSING_FILE_EXTENSION));
    strcpy(new_file_path, file_name);
    strncat(new_file_path, OBJECT_FILE_EXTENSION, strlen(OBJECT_FILE_EXTENSION));
    file_writer_pointer = fopen(new_file_path,"w");

    // Iterating through the decoded lines list, and printing to the objects file, the machine code according to the
    // unique 32' base
    while (linePointer != NULL) {
        int linesCount = 0, i = 0;
        while (linesCount < linePointer->length) {
            // Iterate through the decoded line binary values, and if it's not null, change it to base32 and
            // print to file
            if (linePointer->binaryValue[i] != NULL) {
                char* ICSpecialB32 = binToSpecialB32(decToBinary(ICCopy, 8));
                char* lineSpecialB32 = binToSpecialB32(linePointer->binaryValue[i]);
                fprintf(file_writer_pointer, "%s %s\n", ICSpecialB32, lineSpecialB32);
                ICCopy++;
                linesCount++;
            }
            i++;
        }
        linePointer = linePointer->next;
    }

    fclose(file_writer_pointer);
    return 1;
}