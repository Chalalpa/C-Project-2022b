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

/* @ Function: getEntry(struct Entry* entryHead, char* entryName);
   @ Arguments: struct Entry* entryHead, char* entryName
   entryHead, represents head of Entry linked list.
   entryName is the name of the entry wished to be found
   @ Description: The function iterates through the Entries list and finds the extern requested by its name.
   It returns it, if it was found in the list, or 0, if it wasn't found.
*/
struct Entry* getEntry(struct Entry* entryHead, char* entryName) {
    struct Entry* headPointer = entryHead;
    while(headPointer != NULL) {
        if (!strcmp(entryName, headPointer->name))
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
    struct Symbol* symbol;
    int entriesCount = 0;
    char* new_file_path, *b32, *binary;
    FILE *file_writer_pointer;

    /* Conduct full file path */
    new_file_path = (char*)malloc(strlen(fileName) + strlen(ENTRIES_FILE_EXTENSION) + 1);
    strcpy(new_file_path, fileName);
    strcat(new_file_path, ENTRIES_FILE_EXTENSION);

    file_writer_pointer = fopen(new_file_path,"w");
    while (entryPointer != NULL) {
        if (strcmp(entryPointer->name, EMPTY_STRUCT_NAME)) {
            symbol = getSymbol(symbolHead, entryPointer->name);
            if (!symbol) {
                printf("Error! Entry %s declared in line %d is not initialized later in the file\n",
                       entryPointer->name, entryPointer->lineNum);
                fclose(file_writer_pointer);
                remove(new_file_path);
                free(new_file_path);
                return 0;
            }
            entriesCount++;

            if (symbol->value < MEMORY_START) {
                printf("Wrong symbol value %d for symbol %s\n", symbol->value, symbol->name);
                fclose(file_writer_pointer);
                remove(new_file_path);
                free(new_file_path);
                return 0;
            }
            binary = decToBinary(symbol->value, 8);
            b32 = binToSpecialB32(binary);
            fprintf(file_writer_pointer, "%s %s\n", entryPointer->name, b32);
            free(binary);
            free(b32);
        }
        entryPointer = entryPointer->next;
    }
    fclose(file_writer_pointer);
    free(new_file_path);
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
    struct Symbol* symbol;
    struct Extern* externPointer;
    int addressValue, externsCount = 0;
    char* new_file_path, *name, *extension, *b32, *decimal;
    FILE *file_writer_pointer;

    /* Conduct full file path */
    new_file_path = (char*)malloc(strlen(fileName) + strlen(EXTERNS_FILE_EXTENSION) + 1);
    strcpy(new_file_path, fileName);
    strcat(new_file_path, EXTERNS_FILE_EXTENSION);

    file_writer_pointer = fopen(new_file_path,"w");
    while (decodeLinePointer != NULL) {
        if (!decodeLinePointer->isEmpty) {
            if (decodeLinePointer->srcOperandName != NULL && !(isRegister(decodeLinePointer->srcOperandName))
                && !(*decodeLinePointer->srcOperandName == '#')) {
                name = (char*)calloc(strlen(decodeLinePointer->srcOperandName) + 1,
                                                 sizeof(char));
                strcpy(name, decodeLinePointer->srcOperandName);
                extension = strrchr(name, '.');
                if (extension != NULL)
                    if (!strcmp(extension, ".1") ||
                        !strcmp(extension, ".2"))
                    name[strlen(name) - 2] = '\0';
                symbol = getSymbol(symbolHead, name);
                if (symbol == NULL) {
                    externPointer = getExtern(externHead, name);
                    if (externPointer == NULL) {
                        printf("Error! Found usage of undefined symbol - %s in line %d\n",
                               name, decodeLinePointer->lineNum);
                        fclose(file_writer_pointer);
                        remove(new_file_path);
                        free(new_file_path);
                        free(name);
                        return 0;
                    }
                    decimal = decToBinary(decodeLinePointer->value + 1, 8);
                    b32 = binToSpecialB32(decimal);
                    fprintf(file_writer_pointer, "%s %s\n", externPointer->name, b32);
                    free(decimal);
                    free(b32);
                    externsCount++;
                }
                free(name);
            }
            if (decodeLinePointer->tgtOperandName != NULL && !(isRegister(decodeLinePointer->tgtOperandName))
                && !(*decodeLinePointer->tgtOperandName == '#')) {
                name = (char*)calloc(strlen(decodeLinePointer->tgtOperandName) + 1,
                                           sizeof(char));
                strcpy(name, decodeLinePointer->tgtOperandName);
                extension = strrchr(name, '.');
                if (extension != NULL)
                    if (!strcmp(extension, ".1") ||
                        !strcmp(extension, ".2"))
                    name[strlen(name) - 2] = '\0';
                symbol = getSymbol(symbolHead, name);
                if (symbol == NULL) {
                    externPointer = getExtern(externHead, name);
                    if (externPointer == NULL) {
                        printf("Error! Found usage of undefined symbol - %s in line %d\n",
                               name, decodeLinePointer->lineNum);
                        fclose(file_writer_pointer);
                        remove(new_file_path);
                        free(name);
                        free(new_file_path);
                        return 0;
                    }
                    if (decodeLinePointer->binaryValue[1] == NULL)
                        addressValue = decodeLinePointer->value + 1;
                    else
                        if (decodeLinePointer->binaryValue[2] != NULL)
                            addressValue = decodeLinePointer->value + 3;
                        else
                            addressValue = decodeLinePointer->value + 2;
                    decimal = decToBinary(addressValue, 8);
                    b32 = binToSpecialB32(decimal);
                    fprintf(file_writer_pointer, "%s %s\n", externPointer->name, b32);
                    free(decimal);
                    free(b32);
                    externsCount++;
                }
                free(name);
            }
        }
        decodeLinePointer = decodeLinePointer->next;
    }
    fclose(file_writer_pointer);
    if (externsCount <= 0)
        remove(new_file_path);
    free(new_file_path);
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
              struct Entry* entryHead, struct Extern* externHead, int IC, int DC) {
    /* Conduct full file path */
    int ICCopy = MEMORY_START;

    /* Iteration helper: */
    int i, status = 1;
    char* externName, *symbolName, *new_file_path, *ICSpecialB32, *lineSpecialB32, *extension, *binary;
    struct DecodedLine* linePointer = decodedLineHead;
    struct Extern* externPointer = externHead, *externObject;
    struct Entry* entryPointer;
    struct Symbol* symbol, *symbolPointer;
    FILE *file_writer_pointer;
    /* Getting the first valuable Extern object */
    while (externPointer != NULL && (!strcmp(externPointer->name, EMPTY_STRUCT_NAME)))
        externPointer = externPointer->next;

    /* Verifying our externs are not already-defined symbols/entries, they should be defined in some other files: */
    while(externPointer != NULL) {
        externName = externPointer->name;
        entryPointer = getEntry(entryHead, externName);
        if (entryPointer != NULL) {
            printf("Error! Can't define extern and entry with the same name. Entry defined in line %d, "
                   "Extern defined in line %d\n", entryPointer->lineNum, externPointer->lineNum);
            status = 0;
        }
        symbolPointer = getSymbol(symbolHead, externName);
        if (symbolPointer != NULL) {
            printf("Error! Can't define extern and symbol with the same name. Symbol defined in line %d, "
                   "Extern defined in line %d\n", symbolPointer->lineNum, externPointer->lineNum);
            status = 0;
        }
        externPointer = externPointer->next;
    }

    /* Getting the first valuable DecodedLine object */
    while (linePointer != NULL && linePointer->isEmpty == 1)
        linePointer = linePointer->next;

    /* Replacing all <TO_BE_FILLED> lines with the real value, after retrieving all symbols in previous stages */
    while (linePointer != NULL) {
        for (i=1; i < 5; i++) {
            if (status) {
                if (linePointer->binaryValue[i] != NULL && !strcmp(linePointer->binaryValue[i], TO_BE_FILLED)) {
                    if (0 < i && i < 3) {
                        /* Index for source operand */
                        symbolName = (char *) malloc((strlen(linePointer->srcOperandName) + 1) * (sizeof(char)));
                        symbolName = strcpy(symbolName, linePointer->srcOperandName);
                    } else {
                        /* Index for target operand */
                        symbolName = (char *) malloc((strlen(linePointer->tgtOperandName) + 1) * (sizeof(char)));
                        symbolName = strcpy(symbolName, linePointer->tgtOperandName);
                    }
                    /* Extracting extension of struct addressing if relevant */
                    extension = strrchr(symbolName, '.');
                    if (extension != NULL) {
                        if (!strcmp(extension, ".1") ||
                            !strcmp(extension, ".2"))
                            symbolName[strlen(symbolName) - 2] = '\0';
                    }
                    symbol = getSymbol(symbolHead, symbolName);
                    if (!symbol) {
                        /* If it was not found in the Symbols list, it should be found in the Externs list */
                        externObject = getExtern(externHead, symbolName);
                        if (!externObject) {
                            printf("Error! Symbol %s used in line %d is not declared anywhere\n",
                                   symbolName, linePointer->lineNum);
                            status = 0;
                        }
                        else {
                            /* If it's an extern, fill line accordingly */
                            strcpy(linePointer->binaryValue[i], "0000000001");
                        }
                    }
                    else {
                        /* If it's a symbol, fill the line according to the symbol value */
                        binary = decToBinary(symbol->value, BINARY_WORD_SIZE - 2);
                        strcpy(linePointer->binaryValue[i], binary);
                        free(binary);
                        strcat(linePointer->binaryValue[i], "10");
                    }
                    free(symbolName);
                }
            }
        }
        linePointer = linePointer->next;
    }

    /* Writing entries file */
    if (!writeEntriesFile(file_name, entryHead, symbolHead)) {
        printf("Error! Couldn't write entries file\n");
        status = 0;
    }

    /* Writing externs file */
    if (!writeExternsFile(file_name, externHead, symbolHead, decodedLineHead)) {
        printf("Error! Couldn't write entries file\n");
        status = 0;
    }

    if (status) {
        linePointer = decodedLineHead;
        /* Getting the first valuable DecodedLine object */
        while (linePointer != NULL && linePointer->isEmpty == 1)
            linePointer = linePointer->next;

        new_file_path = (char *) malloc(strlen(file_name) + strlen(OBJECT_FILE_EXTENSION) + 1);
        strcpy(new_file_path, file_name);
        strcat(new_file_path, OBJECT_FILE_EXTENSION);
        file_writer_pointer = fopen(new_file_path, "w");

        /* Writing IC and DC */
        binary = decToBinary(IC - MEMORY_START - DC, 8);
        lineSpecialB32 = binToSpecialB32(binary);
        fprintf(file_writer_pointer, "%s ", lineSpecialB32);
        free(binary);
        free(lineSpecialB32);
        binary = decToBinary(DC, 8);
        lineSpecialB32 = binToSpecialB32(binary);
        fprintf(file_writer_pointer, "%s\n", lineSpecialB32);
        free(binary);
        free(lineSpecialB32);

        /* Iterating through the decoded lines list, and printing to the objects file, the machine code according to the */
        /* unique 32' base */
        while (linePointer != NULL) {
            int linesCount = 0, i = 0;
            while (linesCount < linePointer->length) {
                /* Iterate through the decoded line binary values, and if it's not null, change it to base32 and */
                /* print to file */
                if (linePointer->binaryValue[i] != NULL) {
                    binary = decToBinary(ICCopy, 8);
                    ICSpecialB32 = binToSpecialB32(binary);
                    lineSpecialB32 = binToSpecialB32(linePointer->binaryValue[i]);
                    fprintf(file_writer_pointer, "%s %s\n", ICSpecialB32, lineSpecialB32);
                    free(binary);
                    free(lineSpecialB32);
                    free(ICSpecialB32);
                    ICCopy++;
                    linesCount++;
                }
                i++;
            }
            linePointer = linePointer->next;
        }

        fclose(file_writer_pointer);
        free(new_file_path);
    }
    return status;
}