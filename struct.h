#ifndef struct_h

#define struct_h

#include "main.h"

/* In this file there are definitions and struct used throughout the program */

/* Arrays for storing the data and words */
extern unsigned int data[];
extern unsigned int instructions[];

/* Declaring a boolian variable */
typedef enum { FALSE, TRUE } boolean;

/* Creating a linked list for a label table */
typedef struct structLabels* labelPtr;
typedef struct structLabels {
	char name[MAX_LABEL];/* The name of the label */
	unsigned int address;/* The address of the label */
	char symbol_type[MAX_LABEL];/* The symbol typr of the label */
	boolean external;/* A boolean type variable to store if the label is extern or not */
	boolean inActionStatement;/* A boolean type varialbe to store if the label is in an action statement or not */
	boolean entry;/* A boolean type varialbe to store if the label is entry or not */
	labelPtr next;/* A pointer to the next label in the list */
} Labels;

/*Creating a two-way linked list for extern labels */
typedef struct ext* extPtr;
typedef struct ext {
	char name[MAX_LABEL]; /* The name of the extern label */
	unsigned int address; /* The address in memory where the external address should be replaced */
	extPtr next; /* A pointer to the next extern in the list */
	extPtr prev; /* A pointer to the previous extern in the list */
} ext;

#endif
