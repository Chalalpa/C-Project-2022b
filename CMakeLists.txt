cmake_minimum_required(VERSION 3.21)
project(untitled4 C)

set(CMAKE_C_STANDARD 99)

add_executable(untitled4 main.c operations.c consts.h decoder.c preprocessing.c main.h preprocessing.h common_utils.c common_utils.h firstrun.c firstrun.h consts.c operations.h)
