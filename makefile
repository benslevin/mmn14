myprog1: main.o passOne.o struct.o externalStructs.o passTwo.o common.o
	gcc -g -ansi -Wall -pedantic main.o passOne.o externalStructs.o passTwo.o common.o struct.o -lm -o myprog1

main.o: main.c passFunctions.h main.h external_vars.h struct.h common.h
	gcc -c -ansi -Wall -pedantic main.c -o main.o

passOne.o: passOne.c passFunctions.h main.h external_vars.h struct.h common.h
	gcc -c -ansi -Wall -pedantic passOne.c -o passOne.o

struct.o: struct.c passFunctions.h main.h external_vars.h struct.h common.h
	gcc -c -ansi -Wall -pedantic struct.c -o struct.o

common.o: common.c passFunctions.h main.h external_vars.h struct.h common.h
	gcc -c -ansi -Wall -pedantic common.c -o common.o

passTwo.o: passTwo.c passFunctions.h main.h external_vars.h struct.h
	gcc -c -ansi -Wall -pedantic passTwo.c -o passTwo.o

externalStructs.o: externalStructs.c passFunctions.h main.h external_vars.h struct.h
	gcc -c -ansi -Wall -pedantic externalStructs.c -o externalStructs.o
