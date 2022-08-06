#include "common_utils.h"


/* @ Function: char* decToBinary(int n, int len)
   @ Arguments: int n, int len
   n - the number wished to be translated to binary
   len - the length of the resulting representation
   @ Description: The function calculates and returns the given number in a binary base.
*/
char* decToBinary(int n, int len)
{
    char* binaryNumber = (char*)calloc(len, sizeof(char));
    char* binaryNumberPointer = binaryNumber;
    // Size of an integer is assumed to be <len> bits
    for (int i = len - 1; i >= 0; i--) {
        int k = n >> i; // right shift
        if (k & 1) // helps us know the state of first bit
            *binaryNumberPointer = '1';
        else
            *binaryNumberPointer = '0';
        binaryNumberPointer++;
    }
    *binaryNumberPointer = '\0';
    return binaryNumber;
}

/* @ Function: char* binToDec(char* n)
   @ Arguments: char* n
   n - the number wished to be translated to decimal
   @ Description: The function calculates and returns the given binary number in a decimal base.
*/
char* binToDec(char* n) {
    int num = 0;
    double i = 0.0;
    char* numPointer = n + strlen(n) - 1;
    while(i < strlen(n)) {
        if (*numPointer == '0' || *numPointer == '1') {
            if (*numPointer == '1') {
                num += pow(2.0, i);
            }
        }
        else {
            return 0;
        }
        numPointer--;
        i+=1.0;
    }
    return num;
}

/* @ Function: char* binToSpecialB32(char* n)
   @ Arguments: char* n
   n - the number wished to be translated to special 32 base
   @ Description: The function calculates and returns the given binary number in a 32 special base.
*/
char* binToSpecialB32(char* n)
{
    int decimal = binToDec(n);
    char* b32 = (char*)calloc(8, sizeof(char));
    char* b32Pointer = b32;
    do
    {
        int d = decimal % 32 ;
        *b32Pointer = SPECIAL_BASE32[d];
        decimal /= 32 ;
        b32Pointer++;

    } while(decimal > 0);
    while (strlen(b32) < 2)
        *b32Pointer++ = SPECIAL_BASE32[0];
    b32 = strrev(b32);
    /*
    char* specialB32 = (char*)calloc(8, sizeof(char));
    char* specialB32Pointer = specialB32;
    while (*b32Pointer) {
        int i = 0;
        *specialB32Pointer = *b32Pointer;
        while (!isdigit(*specialB32Pointer)) {
            *specialB32Pointer -= 'a';
            i++;
        }
        *specialB32Pointer = SPECIAL_BASE32[*specialB32Pointer - '0' + (i * 10)];
        b32Pointer++;
        specialB32Pointer++;
    }
    */
    return b32;
}

/* @ Function: char* removeLeadingWhiteSpaces(char* line_data)
   @ Arguments: char* line_data
   line_data - the string wished to be trimmed
   @ Description: The function returns the given string without its leading whitespace (\t or \n or just a space).
*/
char* removeLeadingWhiteSpaces(char* line_data) {
    while (*line_data) {
        if (!isspace(*line_data))
            break;
        line_data++;
    }
    return line_data;
}

/* @ Function: char* removeEndingWhiteSpaces(char* line_data)
   @ Arguments: char* line_data
   line_data - the string wished to be trimmed
   @ Description: The function returns the given string without its ending whitespace (\t or \n or just a space).
*/
char* removeEndingWhiteSpaces(char* line_data) {
    int i = strlen(line_data) - 1;
    char* lineDataPointer = line_data;
    while (i > 0) {
        if (!isspace(line_data[i]))
            break;
        i--;
    }
    lineDataPointer[i + 1] = '\0';
    return lineDataPointer;
}

/* @ Function: int isCommentLine(char* line_data)
   @ Arguments: char* line_data
   line_data - the lined requested to know if is a comment line
   @ Description: The function checks if the given line is a 'comment line'. Returns 1 if it is, and 0 if not.
*/
int isCommentLine(char* line_data) {
    char* trimmed_line = removeLeadingWhiteSpaces(line_data);
    if (*trimmed_line == ';')
        return 1;
    return 0;
}

/* @ Function: int isEmptyLine(char* line_data)
   @ Arguments: char* line_data
   line_data - the lined requested to know if is an empty line
   @ Description: The function checks if the given line is empty (contains just whitespaces, or nothing at all).
   Returns 1 if it is, and 0 if not.
*/
int isEmptyLine(char* line_data) {
    char* trimmed_line = removeLeadingWhiteSpaces(line_data);
    if (*trimmed_line == '\0')
        return 1;
    return 0;
}

/* @ Function: int startsWith(char* string, char* prefix)
   @ Arguments: char* string, char* prefix
   string - the tested string
   prefix - the string we wish to check if exists in the start of the 'string' param
   @ Description: The function checks if the given 'string' contains the 'prefix' at the start of it, and returns 1 if
   it does, and 0 if not.
*/
int startsWith(char* string, char* prefix)
{
    while(*prefix)
        if(*prefix++ != *string++)
            return 0;

    return 1;
}

/* @ Function: int containsSpace(char* string)
   @ Arguments: char* string
   string - the tested string
   @ Description: The function checks if the given 'string' contains a whiteSpace and returns 1 if it does,
   and 0 if not.
*/
int containsSpace(char* string) {
    char* stringPointer = string;
    while(*stringPointer) {
        if (isspace(*stringPointer))
            return 1;
        stringPointer++;
    }
    return 0;
}

/* @ Function: int isStringInArray(char* string, char** arr)
   @ Arguments: char* string, char** arr
   string - the tested string
   arr - the array of strings that we want to check if string exists in
   @ Description: The function checks if the given 'string' parameter exists in the given array of string, returns
   1 if it does, and 0 if not.
*/
int isStringInArray(char* string, char** arr) {
    char** arrPointer = arr;
    while(*arrPointer && **arrPointer) {
        if(!strcmp(string, *arrPointer))
            return 1;
        arrPointer++;
    }
    return 0;
}

/* @ Function: int isKeyword(char* string)
   @ Arguments: char* string
   string - the tested string
   @ Description: The function checks if the given 'string' is an assembly keyword (as defined in the project reqs).
   It returns 1 if it is, and 0 if it's not.
*/
int isKeyword(char* string) {
    return isStringInArray(string, KEYWORDS);
}

/* @ Function: int isDataDirective(char* string)
   @ Arguments: char* string
   string - the tested string
   @ Description: The function checks if the given 'string' is a data directive keyword (as defined in the project reqs).
   It returns 1 if it is, and 0 if it's not.
*/
int isDataDirective(char* string) {
    return isStringInArray(string, DATA_DIRECTIVES);
}

/* @ Function: int isExternOrEntryDirective(char* string)
   @ Arguments: char* string
   string - the tested string
   @ Description: The function checks if the given 'string' is an extern or entry directive keyword
   (as defined in the project reqs). It returns 1 if it is, and 0 if it's not.
*/
int isExternOrEntryDirective(char* string) {
    return isStringInArray(string, EXTERN_OR_ENTRY);
}

/* @ Function: int isDirective(char* string)
   @ Arguments: char* string
   string - the tested string
   @ Description: The function checks if the given 'string' is a directive keyword (as defined in the project reqs).
   It returns 1 if it is, and 0 if it's not.
*/
int isDirective(char* string) {
    return isDataDirective(string) || isExternOrEntryDirective(string);
}

/* @ Function: int isOperation(char* string)
   @ Arguments: char* string
   string - the tested string
   @ Description: The function checks if the given 'string' is an operation keyword (as defined in the project reqs).
   It returns 1 if it is, and 0 if it's not.
*/
int isOperation(char* string) {
    return isStringInArray(string, OPERATIONS);
}

/* @ Function: int isValidLabelOrMacroName(char* string, char* objectName)
   @ Arguments: char* string, char* objectName
   string - the tested string
   objectName - label/macro
   @ Description: The function checks if the given macro/label name is valid, according to the project reqs, or not.
   It returns 1 if it is valid, and 0 if it's not.
*/
int isValidLabelOrMacroName(char* string, char* objectName) {
    if (strlen(string) > MAX_LABEL_LEN) {
        printf("Error. %s name exceeds limit (%d): %s\n", objectName, MAX_LABEL_LEN, string);
        return 0;
    }

    if (!isalpha(*string)) {
        printf("Error. %s's first char should be a letter (lowercase/uppercase): %s\n", objectName, string);
        return 0;
    }

    char* string_copy = string;
    while(*string_copy) {
        if (!(isalnum(*string_copy))) {
            printf("Error. Found non alpha-numeric char in %s's name declaration: %s\n", objectName, string);
            return 0;
        }
        string_copy++;
    }

    if (isKeyword(string)) {
        printf("Error. %s's name is actually a keyword: %s\n", objectName, string);
        return 0;
    }
    return 1;
}

/* @ Function: int isRegister(char* string)
   @ Arguments: char* string
   string - the tested string
   @ Description: The function checks if the given string is a register keyword (according to project's reqs) or not.
   It returns 1 if it is, and 0 if it's not.
*/
int isRegister(char* string) {
    return isStringInArray(string, REGISTERS);
}

/* @ Function: int isValidMacro(char* string)
   @ Arguments: char* string
   string - the tested string
   @ Description: The function checks if the given macro name valid, or not. Returns 1 if it is, and 0 if it's not.
*/
int isValidMacro(char* string) {
    return isValidLabelOrMacroName(string, "macro");
}

/* @ Function: int isValidLabel(char* string)
   @ Arguments: char* string
   string - the tested string
   @ Description: The function checks if the given label name valid, or not. Returns 1 if it is, and 0 if it's not.
*/
int isValidLabel(char* string) {
    return isValidLabelOrMacroName(string, "label");
}

/* @ Function: int isValidNumber(char* string)
   @ Arguments: char* string
   string - the tested string
   @ Description: The function checks if string is a valid number (according to the project's reqs), or not.
   Returns 1 if it is, and 0 if it's not.
*/
int isValidNumber(char* string) {
    char* strPointer = string;
    if (*strPointer == '-' || *strPointer == '+')
        strPointer++;
    while(*strPointer)
        if(!isdigit(*strPointer++))
            return 0;
    return 1;
}

/* @ Function: int isValidString(char* string)
   @ Arguments: char* string
   string - the tested string
   @ Description: The function checks if string is a valid string (according to the project's reqs), or not.
   Returns 1 if it is, and 0 if it's not.
*/
int isValidString(char* string) {
    if (*string != '"')
        return 0;
    if (*(string + strlen(string) - 1) != '"')
        return 0;
    int i;
    for (i=1; i < strlen(string) - 1; i++) {
        if(string[i] == '"')
            return 0;
    }
    return 1;
}

/* @ Function: int getAddressingIndex(char* string)
   @ Arguments: char* string
   string - the tested string.
   @ Description: The function checks if the string is an addressing of some index of a variable (struct), or not.
   Returns the index number if it is, and 0 if it's not (0 is not a valid addressing index).
*/
int getAddressingIndex(char* string) {
    char* strPointer = removeEndingWhiteSpaces(string);
    strPointer += strlen(string) - 2;
    if (!strcmp(strPointer, ".1"))
        return 1;
    if (!(strcmp(strPointer, ".2")))
        return 2;
    return 0;
}

/* @ Function: getLabelName(char* line_data)
   @ Arguments: char* line_data
   line_data is the line that we want to extract the label from
   @ Description: The function extracts and returns, the label declared in the given line.
   It returns 0 if the label is invalid or no label was found.
*/
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

/* @ Function: char* getNextField(char* line_data)
   @ Arguments: char* line_data
   line_data is the line that we want to extract the next field from
   @ Description: The function extracts and returns, the next field found in the given line.
   It returns 0 if no field was found.
*/
char* getNextField(char* line_data) {
    char* lineDataPointer = removeLeadingWhiteSpaces(removeEndingWhiteSpaces(line_data));
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
    if (isEmptyLine(fieldName))
        return 0;
    return fieldName;
}