all: main.c processing/preprocessing.c helpers/common_utils.c processing/firstrun.c helpers/consts.c types/operations.c types/directives.c processing/secondrun.c types/decodedline.c
	gcc -ansi -Wall -pedantic -g main.c processing/preprocessing.c helpers/common_utils.c processing/firstrun.c helpers/consts.c types/decodedline.c types/operations.c types/directives.c processing/secondrun.c -o compiler -lm