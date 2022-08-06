#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include "../helpers/consts.h"
#include "../helpers/common_utils.h"
#include "../types/operations.h"
#include "../types/decodedline.h"
#include "../types/directives.h"
#include "../types/entry.h"
#include "../types/extern.h"
#include "../types/symbol.h"


int secondRun(char* file_name, int* IC, int* DC, struct Symbol* symbolHead, struct DecodedLine* decodedLineHead,
              struct Entry* entryHead, struct Extern* externHead);
