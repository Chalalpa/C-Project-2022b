#include "directives.h"

/* @ Function: struct Directive* getDirectiveByName(char* directiveName);
   @ Arguments: char* directiveName
   directiveName - is the requested directive name
   @ Description: The function goes through the directives array, and finds the requested directive by its name.
   It returns is if it finds it, but if not, it returns 0.
*/
struct Directive* getDirectiveByName(char* directiveName) {
    int i;
    for (i=0; i<5; i++) {
        if (!strcmp(DIRECTIVES_TABLE[i].name, directiveName))
            return (struct Directive*)&DIRECTIVES_TABLE[i];
    }
    return 0;
}
