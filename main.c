#include <stdio.h>
#include "main.h"

int main(int argc, char *argv[])
{
    int IC, DC, i;
    /*
    we are passing the command line arguments that are the files that passed to the assembler
    to the handleSourceFiles function.
     */
    for (i = 1; i < argc; i++) {
        int result = run_preprocess();
    }

    return 0;
}
