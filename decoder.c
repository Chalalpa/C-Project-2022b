#include <string.h>

int opcode(char op[]) /*Returns the opcode type in decimal base*/
{
    switch (!strcmp(op, "mov"))
        return 0;
    if (!strcmp(op, "cmp"))
        return 1;
    if (!strcmp(op, "add"))
        return 2;
    if (!strcmp(op, "sub"))
        return 3;
    if (!strcmp(op, "not"))
        return 4;
    if (!strcmp(op, "clr"))
        return 5;
    if (!strcmp(op, "lea"))
        return 6;
    if (!strcmp(op, "inc"))
        return 7;
    if (!strcmp(op, "dec"))
        return 8;
    if (!strcmp(op, "jmp"))
        return 9;
    if (!strcmp(op, "bne"))
        return 10;
    if (!strcmp(op, "get"))
        return 11;
    if (!strcmp(op, "prn"))
        return 12;
    if (!strcmp(op, "jsr"))
        return 13;
    if (!strcmp(op, "rts"))
        return 14;
    if (!strcmp(op, "hlt"))
        return 15;


    return -1;
}