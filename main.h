#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdarg.h>

#define MAX_INPUT 81
#define MAX_LABEL 32 /*maximum chars per label*/
#define MAX_CHAR 8
#define DEFAULT_IC 100
#define EMPTY_ERROR 0

/* Directives types */
enum guidance { DATA, STRING, STRUCT, ENTRY, EXTERN, UNKNOWN_TYPE };

/* Enum of commands ordered by their opcode */
enum commands { MOV, CMP, ADD, SUB, LEA, CLR, NOT, INC, DEC, JMP, JSR, BNE, RED, PRN, RTS, STOP, UNKNOWN_COMMAND };

char filename[MAX_INPUT];
char line[MAX_INPUT];



enum errors {
    SYNTAX_ERR = 1,
};



/* To identify a label */
enum { NO_COLON, COLON };