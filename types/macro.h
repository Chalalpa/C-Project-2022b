/* Defining a Macro linked list structure*/
struct Macro {
    char* name;  /* The macro name to be used later in the source code */
    char* data;  /* The macro content itself to deploy later in the code */
    struct Macro* next;  /* A pointer to the next Macro struct object */
};