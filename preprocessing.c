//
// Created by Moshe on 22/07/2022.
//
#include "preprocessing.h"


int isMacroStart(char * line_data) {
    if (isCommentLine(line_data) || isEmptyLine(line_data))
        return 0;
    char* leftTrimmedLine = removeLeadingWhiteSpaces(line_data);
    return startsWith(leftTrimmedLine, MACRO_START);
}

int isMacroEnd(char * line_data) {
    if (isCommentLine(line_data) || isEmptyLine(line_data))
        return 0;
    char* leftTrimmedLine = removeLeadingWhiteSpaces(line_data);
    return startsWith(leftTrimmedLine, MACRO_END);
}

char* parseMacroName(char * line_data) {
    char* leftTrimmedLine = removeLeadingWhiteSpaces(line_data);
    leftTrimmedLine += strlen(MACRO_START);
    if (*leftTrimmedLine != ' ') {
        printf("Error. Expected a space after macro declaration");
        return "";
    }
    leftTrimmedLine++;
    char * macroName = (char*)malloc(strlen(leftTrimmedLine) + 1);
    char * macroNamePointer = macroName;
    while (*leftTrimmedLine && !isspace(*leftTrimmedLine)) {
        *macroNamePointer = *leftTrimmedLine;
        macroNamePointer++;
        leftTrimmedLine++;
    }
    if (isspace(*leftTrimmedLine)) {
        if (*removeLeadingWhiteSpaces(leftTrimmedLine) != 0) {
            printf("Error. Found trailing chars after reading macro name");
            return 0;
        }
    }

    *macroNamePointer = '\0';
    realloc(macroName, strlen(macroName) + 1);
    return macroName;
}

char* isMacroCall(char* line_data, struct Macro* head) {
    if (isCommentLine(line_data) || isEmptyLine(line_data))
        return 0;
    char* leftTrimmedLine = removeLeadingWhiteSpaces(line_data);
    char * macroName = (char*)malloc(strlen(leftTrimmedLine) + 1);
    struct Macro *tmp;
    int macroExists = 0;
    tmp = head;
    char * macroNamePointer = macroName;
    while (*leftTrimmedLine && !isspace(*leftTrimmedLine)) {
        *macroNamePointer = *leftTrimmedLine;
        macroNamePointer++;
        leftTrimmedLine++;
    }

    while (tmp != NULL) {
        if (strcmp(tmp->name, macroName)) {
            macroExists = 1;
            break;
        }
        tmp = tmp->next;
    }

    if (!macroExists)
        return 0;

    if (isspace(*leftTrimmedLine)) {
        if (*removeLeadingWhiteSpaces(leftTrimmedLine) != "")
            printf("Error. Found trailing chars after calling a macro");
        return 0;
    }

    *macroNamePointer = '\0';
    realloc(macroName, strlen(macroName) + 1);
    return macroName;
}

int writeMacroContent(char* macroName, FILE* file_writer_pointer, struct Macro* head) {
    struct Macro *tmp;
    tmp = head;
    while (tmp != NULL) {
        if (strcmp(tmp->name, macroName)) {
            break;
        }
        tmp = tmp->next;
    }
    if (tmp != NULL) {
        fprintf(file_writer_pointer, "%s", tmp->data);
        return 1;
    }
    return 0;
}

int readMacros(char* file_name, struct Macro* head) {
    char* full_file_path = (char*)malloc(strlen(file_name) + strlen(SOURCE_FILE_EXTENSION));
    strcpy(full_file_path, file_name);
    strncat(full_file_path, SOURCE_FILE_EXTENSION, strlen(SOURCE_FILE_EXTENSION));
    FILE *file_pointer;
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
    int foundMacro = 0;
    struct Macro *next_macro_holder;
    while(fgets(line_data, MAX_LINE_LEN + 1, file_pointer)) {
        if (strlen(line_data) > MAX_LINE_LEN)
            printf("Error! Line %d of file %s exceeds line length limit (%d)", i, full_file_path, MAX_LINE_LEN);
        else {
            if (isMacroStart(line_data)) {
                char* macroName = parseMacroName(line_data);
                if (macroName) {
                    foundMacro = 1;
                    next_macro_holder = NULL;
                    next_macro_holder = (struct Macro *)malloc(sizeof(struct Macro));
                    next_macro_holder->data = (char*)malloc(MAX_LINE_LEN * MAX_LINES);
                    next_macro_holder->name = macroName;
                    head->next = next_macro_holder;
                    head = next_macro_holder;
                }
            }
            else {
                if (foundMacro) {
                    if (isMacroEnd(line_data))
                        foundMacro = 0;
                    else {
                        strncat(next_macro_holder->data, line_data, strlen(line_data));
                    }
                }
            }
        }
        i++;
    }
    //free(full_file_path);
    fclose(file_pointer);
    return 1;
}

int writeMacros(char* file_name, struct Macro* head) {
    char* full_file_path = (char*)malloc(strlen(file_name) + strlen(SOURCE_FILE_EXTENSION));
    strcpy(full_file_path, file_name);
    strncat(full_file_path, SOURCE_FILE_EXTENSION, strlen(SOURCE_FILE_EXTENSION));
    char* new_file_path = (char*)malloc(strlen(file_name) + strlen(PRE_PROCESSING_FILE_EXTENSION));
    strcpy(new_file_path, file_name);
    strncat(new_file_path, PRE_PROCESSING_FILE_EXTENSION, strlen(PRE_PROCESSING_FILE_EXTENSION));
    int foundMacro = 0;
    FILE *file_reader_pointer;
    FILE *file_writer_pointer;
    char line_data[MAX_LINE_LEN];
    memset(line_data, '\0', MAX_LINE_LEN);
    file_reader_pointer = fopen(full_file_path,"r");
    if (file_reader_pointer == NULL) {
        printf("Error reading given file '%s'. Cannot write '.am' file\n", full_file_path);
        return -1;
    }
    file_writer_pointer = fopen(file_writer_pointer,"w");
    int i = 0;
    while(fgets(line_data, MAX_LINE_LEN + 1, file_reader_pointer)) {
        if (strlen(line_data) > MAX_LINE_LEN)
            printf("Error! Line %d of file %s exceeds line length limit (%d)", i, full_file_path, MAX_LINE_LEN);
        else if (!foundMacro) {
                if (!isMacroStart(line_data)) {
                    char* macroName = isMacroCall(line_data, head);
                    if (macroName)
                        writeMacroContent(macroName, file_writer_pointer, head);
                    else
                        fprintf(file_writer_pointer, "%s", line_data);
                }
                else
                   foundMacro = 1;
             } else
                 if (isMacroEnd(line_data))
                    foundMacro = 0;
        i++;
    }
    //free(full_file_path);
    //free(new_file_path);
    fclose(file_reader_pointer);
    fclose(file_writer_pointer);
}
