//
// Created by Moshe on 29/07/2022.
//

#include "firstrun.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>



int firstRun(char* file_name) {
    char* full_file_path = (char*)malloc(strlen(file_name) + strlen(PRE_PROCESSING_FILE_EXTENSION));
    strcpy(full_file_path, file_name);
    strncat(full_file_path, PRE_PROCESSING_FILE_EXTENSION, strlen(PRE_PROCESSING_FILE_EXTENSION));
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
    int isLabel = 0;
    while(fgets(line_data, MAX_LINE_LEN + 1, file_pointer)) {
        if (strlen(line_data) > MAX_LINE_LEN)
            printf("Error! Line %d of file %s exceeds line length limit (%d)", i, full_file_path, MAX_LINE_LEN);
    }
}