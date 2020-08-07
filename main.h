#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdarg.h>

#define MAX_INPUT 81 /*maximum chars per line*/
#define MAX_LABEL 32 /*maximum chars per label*/
#define MAX_CHAR 8
#define DEFAULT_IC 100
#define EMPTY_ERROR 0
#define ERROR 1
#define NO_MATCH -1 /*no match for comparison*/
#define NUM 50

/****************************Limits*******************************/
#define REG_LEN 2
#define MIN_REGISTER_NUM 0
#define MAX_REGISTER_NUM 7



char filename[MAX_INPUT];
char line[MAX_INPUT];


/* Directives types */
enum guidance { DATA, STRING, ENTRY, EXTERN, UNKNOWN_TYPE };

/* Enum of commands ordered by their opcode */
enum commands { MOV=0, CMP=1, ADD=2, SUB=2, LEA=4, CLR=5, NOT=5, INC=5, DEC=5, JMP=9, BNE=9, JSR = 9, RED=12, PRN=13, RTS=14, STOP=15, UNKNOWN_COMMAND };

/*Enum of commands funct*/
enum commands_funct { ADD = 1, SUB = 2, CLR = 1, NOT = 2, INC = 3, DEC = 4, JMP = 1, BNE = 2, JSR = 3 };


/**/
enum filetype { FILE_INPUT, FILE_OBJECT, FILE_ENTRY, FILE_EXTERN };

/* A/R/E modes ordered by their numerical value */
enum ARE { ABSOLUTE, EXTERNAL, RELOCATABLE };

/* Addressing methods ordered by their code */
enum methods { METHOD_IMMEDIATE, METHOD_DIRECT, METHOD_RELATIVE, METHOD_REGISTER, METHOD_UNKNOWN };

enum errors {
    SYNTAX_ERR = 1,
};

/* To identify a label */
enum { NO_COLON, COLON };





/*-----------------------------------------------------------------------------------------------*/
//for word construction
unsigned char* word, temp;
unsigned char* temp = (char*) malloc(sizeof(char) * 3);/* The size of 24 bit array, a char is 8, multiply by 3, and convert it to a char pointer */
if (temp == NULL) {
    fprintf(stderr, "Could not allocate memory for word");
    exit(ERROR);
}

*word = temp;

/*bits aloocation in word*/
#define ARE_BITS 3
#define FUNCT_BITS 5
#define DEST_REG_BITS 3
#define SRC_REG_BITS 3
#define DEST_METHOD_BITS 2
#define SRC_METHOD_BITS 2
#define OPCODE_BITS 6

fprintf(stdout, "%hh6X", (unsigned char)* word)//chose one of the examples, or ff or convertion to unsigned char
