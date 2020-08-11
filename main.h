#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdarg.h>
#include <math.h>

//This file includes values and enums for the entire program

/****************************General values*******************************/

#define MAX_INPUT 81 /*maximum chars per line*/
#define MAX_LABEL 32 /*maximum chars per label*/
#define MAX_CHAR 8
#define DEFAULT_IC 100
#define EMPTY_ERROR 0
#define ERROR 1
#define NO_MATCH -1 /*no match for comparison*/
#define NUM 50
#define MAX_WORDS 500

/****************************Limits*******************************/

#define REG_LEN 3
#define MIN_REGISTER_NUM 0
#define MAX_REGISTER_NUM 7

/*******************Bits aloocation in word**********************/

#define ARE_BITS 3
#define FUNCT_BITS 5
#define DEST_REG_BITS 3
#define SRC_REG_BITS 3
#define DEST_METHOD_BITS 2
#define SRC_METHOD_BITS 2
#define OPCODE_BITS 6

#define SRC_METHOD_START_POS 17
#define SRC_METHOD_END_POS 16
#define DEST_METHOD_START_POS 12
#define DEST_METHOD_END_POS 11

/*******************Enums**********************/

/* Directives types */
enum guidance { DATA, STRING, ENTRY, EXTERN, UNKNOWN_TYPE };

/* Enum of commands ordered by their opcode */
enum commands { MOV=0, CMP=1, ADD=2, SUB=2, LEA=4, CLR=5, NOT=5, INC=5, DEC=5, JMP=9, BNE=9, JSR = 9, RED=12, PRN=13, RTS=14, STOP=15, UNKNOWN_COMMAND };

/* Enum of commands funct */
enum commands_funct { ADD = 1, SUB = 2, CLR = 1, NOT = 2, INC = 3, DEC = 4, JMP = 1, BNE = 2, JSR = 3 , UNKNOWN_FUNCT=10};

/* Enum of file types to create */
enum filetype { FILE_INPUT, FILE_OBJECT, FILE_ENTRY, FILE_EXTERN };

/* A/R/E modes ordered by their numerical value */
enum ARE { EXTERNAL = 1, RELOCATABLE = 2, ABSOLUTE = 4 };

/* Addressing methods ordered by their code */
enum methods { METHOD_IMMEDIATE, METHOD_DIRECT, METHOD_RELATIVE, METHOD_REGISTER, METHOD_UNKNOWN };

/* To identify a label */
enum { NO_COLON, COLON };
 
/* Errors */
enum errors {
    INVALID_SYNTAX = 1, INVALID_LABEL_LINE, MISSING_SYNTAX, NO_PARAMETER_AVAILABLE, GUIDANCE_INVALID_NUM_PARAMS, DATA_EXPECTED_NUM,
    DATA_EXPECTED_COMMA_AFTER_NUM, DATA_COMMAS_IN_A_ROW, DATA_UNEXPECTED_COMMA, STRING_TOO_MANY_OPERANDS, STRING_OPERAND_NOT_VALID,
    EXTERN_NO_LABEL, EXTERN_INVALID_LABEL, EXTERN_TOO_MANY_OPERANDS, COMMAND_UNEXPECTED_CHAR, COMMAND_TOO_MANY_OPERANDS, COMMAND_INVALID_OPERANDS_METHODS,
    COMMAND_INVALID_NUMBER_OF_OPERANDS, COMMAND_INVALID_METHOD, CANNOT_OPEN_FILE, COMMAND_LABEL_DOES_NOT_EXIST, WRONG_SYNTAX_FOR_METHOD_RELATIVE,
    LABEL_TOO_LONG, LABEL_INVALID_FIRST_CHAR, LABEL_CANT_BE_COMMAND, LABEL_CANT_BE_REGISTER, LABEL_ALREADY_EXISTS, ENTRY_CANT_BE_EXTERN,
    ENTRY_LABEL_DOES_NOT_EXIST
};


