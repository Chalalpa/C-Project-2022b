//
// Created by Moshe on 22/07/2022.
//
#include "preprocessing.h"


/* @ Function: int isMacroStart(char * line_data);
   @ Arguments: char * line_data
   line_data, that represents a line content (string)
   @ Description: The function checks whether the line describes a start of macro definition, or not.
   Returns 1 if it is, 0 if not. A macro start definition should be something like "macro <macro_name>"
*/
int isMacroStart(char * line_data) {
    if (isCommentLine(line_data) || isEmptyLine(line_data))
        return 0;
    char* leftTrimmedLine = removeLeadingWhiteSpaces(line_data);
    return startsWith(leftTrimmedLine, MACRO_START);
}

/* @ Function: int isMacroEnd(char * line_data);
   @ Arguments: char * line_data
   line_data, that represents a line content (string)
   @ Description: The function checks whether the line describes an end of macro definition, or not.
   Returns 1 if it is, 0 if not. A macro definition end should just be "endmacro"
*/
int isMacroEnd(char * line_data) {
    if (isCommentLine(line_data) || isEmptyLine(line_data))
        return 0;
    char* leftTrimmedLine = removeLeadingWhiteSpaces(line_data);
    return startsWith(leftTrimmedLine, MACRO_END);
}

/* @ Function: char* parseMacroName(char * line_data);
   @ Arguments: char * line_data
   line_data, that represents a line content (string)
   @ Description: The function checks whether a macro was defined properly and returns the name of the defined macro.
   If macro was not defined properly, returns 0
*/

char* parseMacroName(char * line_data) {
    char* leftTrimmedLine = removeLeadingWhiteSpaces(line_data);
    leftTrimmedLine += strlen(MACRO_START);
    if (*leftTrimmedLine != ' ') { // Should be a single space after macro def start
        printf("Error. Expected a space after macro keyword. Line data: %s\n", line_data);
        return 0;
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
            printf("Error. Found trailing chars after reading macro name. Line data: %s\n", line_data);
            return 0;
        }
    }
    *macroNamePointer = '\0';
    realloc(macroName, strlen(macroName) + 1);

    if(!isValidMacro(macroName))
        return 0; // the isValidMacro function should print the relevant error if occurs

    return macroName;
}


/* @ Function: char* isMacroCall(char * line_data, struct Macro* head);
   @ Arguments: char * line_data, struct Macro* head
   line_data, that represents a line content (string).
   head, that represents head of Macro linked list.
   @ Description: The function checks if the given line is a call of an existing macro, by comparing the called macro
   to the existing ones in the macros list.
*/
char* isMacroCall(char* line_data, struct Macro* head) {
    if (isCommentLine(line_data) || isEmptyLine(line_data))
        return 0;
    char* leftTrimmedLine = removeLeadingWhiteSpaces(line_data);
    char* macroName = (char*)calloc(strlen(leftTrimmedLine) + 1, sizeof(char));
    struct Macro *tmp;
    int macroExists = 0;
    tmp = head;
    char* macroNamePointer = macroName;

    // Reading the line until from the first non-space char until the first space char
    while (*leftTrimmedLine && !isspace(*leftTrimmedLine)) {
        *macroNamePointer = *leftTrimmedLine;
        macroNamePointer++;
        leftTrimmedLine++;
    }

    if (*removeLeadingWhiteSpaces(macroName) == NULL) {
        // Checking whether the read macroName is empty
        return 0;
    }

    while (tmp != NULL) {
        // Search in the macro list to see if we can find a macro with the found name
        if (!strcmp(tmp->name, macroName)) {
            macroExists = 1;
            break;
        }
        tmp = tmp->next;
    }

    if (!macroExists)
        return 0;

    if (isspace(*leftTrimmedLine)) {
        if (*removeLeadingWhiteSpaces(leftTrimmedLine) != 0) {
            // There shouldn't be found chars after a macro call
            printf("Error. Found trailing chars after calling a macro. Line data: %s\n", line_data);
            return 0;
        }
    }

    *macroNamePointer = '\0';
    realloc(macroName, strlen(macroName) + 1);
    return macroName;
}


/* @ Function: int writeMacroContent(char* macroName, FILE* file_writer_pointer, struct Macro* head);
   @ Arguments: char* macroName, FILE* file_writer_pointer, struct Macro* head
   macroName is the macro name that its content is requested to be written to the given file
   file_writer_pointer is the FILE handler to write the data into
   head, that represents head of Macro linked list.
   @ Description: The function writes a given macro's data to the given file, return 1 if it found the macro content
   in the macros list, 0 if not.
*/
int writeMacroContent(char* macroName, FILE* file_writer_pointer, struct Macro* head) {
    struct Macro *tmp;
    tmp = head;
    while (tmp != NULL) {
        if (!strcmp(tmp->name, macroName)) {
            // Found requested macro
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


/* @ Function: int readMacros(char* file_name, struct Macro* head);
   @ Arguments: char* file_name, struct Macro* head
   file_name is the path that we should read macros from, without its extension
   head, that represents head of Macro linked list.
   @ Description: The function reads the macros from the file, and puts them into the macros list.
   It returns 1 if there were no errors reading the file, and 0 if there were errors reading it.
*/
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
    struct Macro *tmp;
    while(fgets(line_data, MAX_LINE_LEN + 1, file_pointer)) {
        if (strlen(line_data) > MAX_LINE_LEN)
            printf("Error! Line %d of file %s exceeds line length limit (%d)\n", i, full_file_path, MAX_LINE_LEN);
        else {
            if (isMacroStart(line_data)) {
                char* macroName = parseMacroName(line_data);
                if (macroName) {
                    foundMacro = 1;
                    tmp = NULL;
                    tmp = (struct Macro*)malloc(sizeof(struct Macro));
                    tmp->name = (char*)calloc(MAX_LINE_LEN, sizeof(char));
                    tmp->data = (char*)calloc(MAX_LINE_LEN * MAX_LINES, sizeof(char));
                    tmp->next = NULL;
                    strcpy(tmp->name, macroName);
                    head->next = tmp;
                    head = tmp;
                }
            }
            else {
                if (foundMacro) {
                    if (isMacroEnd(line_data))
                        foundMacro = 0;
                    else {
                        strncat(tmp->data, line_data, strlen(line_data));
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


/* @ Function: int writeMacros(char* file_name, struct Macro* head);
   @ Arguments: char* file_name, struct Macro* head
   file_name is the path that we should read macros from, without its extension
   head, that represents head of Macro linked list.
   @ Description: The function writes a new .am file, while replacing the macros calls with the data that was defined
   In addition, macros definition shouldn't be included in the newly written file.
   It returns 1 if there were no errors writing the file, and 0 if there were errors writing it.
*/
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
        return 0;
    }
    file_writer_pointer = fopen(new_file_path,"w");
    int i = 0;
    while(fgets(line_data, MAX_LINE_LEN + 1, file_reader_pointer)) {
        if (strlen(line_data) > MAX_LINE_LEN)
            printf("Error! Line %d of file %s exceeds line length limit (%d)\n", i, full_file_path, MAX_LINE_LEN);
        else if (!foundMacro) {
                if (!isMacroStart(line_data)) {
                    char* macroName = isMacroCall(line_data, head);
                    if (macroName)
                        writeMacroContent(macroName, file_writer_pointer, head);
                    else
                        fprintf(file_writer_pointer, "%s", line_data);
                }
                else
                    if(parseMacroName(line_data))
                        foundMacro = 1;
                    else // If the line is not a valid macro start, copy it to the .am file
                        fprintf(file_writer_pointer, "%s", line_data);
             } else
                 if (isMacroEnd(line_data))
                    foundMacro = 0;
        i++;
    }
    //free(full_file_path);
    //free(new_file_path);
    fclose(file_reader_pointer);
    fclose(file_writer_pointer);
    return 1;
}
