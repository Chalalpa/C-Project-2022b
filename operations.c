#include "consts.h"

struct AddressingMethodsConfig
{
    int immediate;
    int direct;
    int index;
    int reg;
};

struct Operation {
    char* name;
    int opCode;
    int binaryCode;
    struct AddressingMethodsConfig* sourceAddressingMethods;
    struct AddressingMethodsConfig* targetAddressingMethods;
};

