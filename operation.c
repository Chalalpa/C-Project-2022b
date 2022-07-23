/*AddrMethodsOptions is a struct for addressing mehod, each member represent operand addressing method
and the value can be 0 or 1 and the meaning is to  tell us if the addressing method is legal/optional
For each operation's operand that we have in our assembly language.
*/
typedef struct
{
    unsigned int immediate : 1;
    unsigned int direct : 1;
    unsigned int index : 1;
    unsigned int reg : 1;
} AddrMethodsOptions;


/*Operation is a struct for holding all the operation table data*/
typedef struct
{
    unsigned int op;        /*op- is the binary value of the first sixteen bits in the first word for each operation*/
    unsigned int funct : 4; /*the function code of the operation*/
    char keyword[4];        /*the keyword for the operation name*/
    AddrMethodsOptions src; /*legal addressing methods for the source operand*/
    AddrMethodsOptions des; /*legal addressing methods for the destination operand*/
} Operation;