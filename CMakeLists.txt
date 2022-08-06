cmake_minimum_required(VERSION 3.21)
project(untitled4 C)

set(CMAKE_C_STANDARD 99)

add_executable(untitled4 main.c helpers/consts.h processing/preprocessing.c main.h processing/preprocessing.h helpers/common_utils.c helpers/common_utils.h processing/firstrun.c processing/firstrun.h helpers/consts.c types/operations.h types/decodedline.h types/directives.c types/directives.h processing/secondrun.c processing/secondrun.h types/entry.h types/extern.h types/symbol.h)
