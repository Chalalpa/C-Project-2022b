//
// Created by Moshe on 22/07/2022.
//
#include "preprocessing.h"


int containsMacroStart(char line_data[]) {
    if (isCommentLine(line_data) || isEmptyLine(line_data))
        return 0;


}

int run_preprocess(char file_path[], struct Macro * head) {
    FILE *file_pointer;
    file_pointer = fopen(file_pointer, "r");
    if (file_pointer == NULL) {
        printf("Error reading given file '%s'\n", file_path);
        return -1;
    }
    char line_data[MAX_LINE_LEN];
    memset(line_data, '\0', MAX_LINE_LEN);
    while(fgets(line_data, MAX_LINE_LEN, file_pointer)) {

    }
}