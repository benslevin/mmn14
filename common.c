#include "main.h"
#include <stdio.h>
#include <ctype.h>



/*********Pass one functions**********/
void reset_flags() {

}


/*********Pass one functions**********/

void ignore_line(char* line) {
	int i = 0;
	line = skip_spaces(line);
	while (*line == ';' || *line == '\0') {
		i++;
	}
	if (*line == '\n') {
		return;
	}
}


/*This function check is the 'error' flag was changed meaning that an error while reading the line accured*/
int if_error()
{
	if (error != EMPTY_ERROR)
		return 1;
	else
		return 0;
}

void write_error(int line_number) {
    fprintf(stderr, "ERROR (line %d): ", line_number);

    switch (err)
    {
    case SYNTAX_ERR:
        fprintf(stderr, "first non-blank character must be a letter or a dot.\n");

        break;
/*
    case LABEL_ALREADY_EXISTS:
        fprintf(stderr, "label already exists.\n");

        break;

    case LABEL_TOO_LONG:
        fprintf(stderr, "label is too long (LABEL_MAX_LENGTH: %d).\n", LABEL_LENGTH);

        break;

    case LABEL_INVALID_FIRST_CHAR:
        fprintf(stderr, "label must start with an alphanumeric character.\n");

        break;

    case LABEL_ONLY_ALPHANUMERIC:
        fprintf(stderr, "label must only contain alphanumeric characters.\n");

        break;

    case LABEL_CANT_BE_COMMAND:
        fprintf(stderr, "label can't have the same name as a command.\n");

        break;

    case LABEL_CANT_BE_REGISTER:
        fprintf(stderr, "label can't have the same name as a register.\n");

        break;

    case LABEL_ONLY:
        fprintf(stderr, "label must be followed by a command or a directive.\n");

        break;

    case DIRECTIVE_NO_PARAMS:
        fprintf(stderr, "directive must have parameters.\n");

        break;

    case DIRECTIVE_INVALID_NUM_PARAMS:
        fprintf(stderr, "illegal number of parameters for a directive.\n");

        break;

    case DATA_COMMAS_IN_A_ROW:
        fprintf(stderr, "incorrect usage of commas in a .data directive.\n");

        break;

    case DATA_EXPECTED_NUM:
        fprintf(stderr, ".data expected a numeric parameter.\n");

        break;

    case DATA_EXPECTED_COMMA_AFTER_NUM:
        fprintf(stderr, ".data expected a comma after a numeric parameter.\n");

        break;

    case DATA_UNEXPECTED_COMMA:
        fprintf(stderr, ".data got an unexpected comma after the last number.\n");

        break;

    case STRING_TOO_MANY_OPERANDS:
        fprintf(stderr, ".string must contain exactly one parameter.\n");

        break;

    case STRING_OPERAND_NOT_VALID:
        fprintf(stderr, ".string operand is invalid.\n");

        break;

    case STRUCT_INVALID_NUM:
        fprintf(stderr, ".struct first parameter must be a number.\n");

        break;

    case STRUCT_EXPECTED_STRING:
        fprintf(stderr, ".struct must have 2 parameters.\n");

        break;

    case STRUCT_INVALID_STRING:
        fprintf(stderr, ".struct second parameter is not a string.\n");

        break;

    case STRUCT_TOO_MANY_OPERANDS:
        fprintf(stderr, ".struct must not have more than 2 operands.\n");

        break;

    case EXPECTED_COMMA_BETWEEN_OPERANDS:
        fprintf(stderr, ".struct must have 2 operands with a comma between them.\n");

        break;

    case EXTERN_NO_LABEL:
        fprintf(stderr, ".extern directive must be followed by a label.\n");

        break;

    case EXTERN_INVALID_LABEL:
        fprintf(stderr, ".extern directive received an invalid label.\n");

        break;

    case EXTERN_TOO_MANY_OPERANDS:
        fprintf(stderr, ".extern must only have one operand that is a label.\n");

        break;

    case COMMAND_NOT_FOUND:
        fprintf(stderr, "invalid command or directive.\n");

        break;

    case COMMAND_UNEXPECTED_CHAR:
        fprintf(stderr, "invalid syntax of a command.\n");

        break;

    case COMMAND_TOO_MANY_OPERANDS:
        fprintf(stderr, "command can't have more than 2 operands.\n");

        break;

    case COMMAND_INVALID_METHOD:
        fprintf(stderr, "operand has invalid addressing method.\n");

        break;

    case COMMAND_INVALID_NUMBER_OF_OPERANDS:
        fprintf(stderr, "number of operands does not match command requirements.\n");

        break;

    case COMMAND_INVALID_OPERANDS_METHODS:
        fprintf(stderr, "operands' addressing methods do not match command requirements.\n");

        break;

    case ENTRY_LABEL_DOES_NOT_EXIST:
        fprintf(stderr, ".entry directive must be followed by an existing label.\n");

        break;

    case ENTRY_CANT_BE_EXTERN:
        fprintf(stderr, ".entry can't apply to a label that was defined as external.\n");

        break;

    case COMMAND_LABEL_DOES_NOT_EXIST:
        fprintf(stderr, "label does not exist.\n");

        break;

    case CANNOT_OPEN_FILE:
        fprintf(stderr, "there was an error while trying to open the requested file.\n");
    }
    */

	/*INVALID_SYNTAX*/ /*in case the line does not starts with alpha or .*/
	/*INVALID_LABEL_LINE*/ /*in case there is only a label in a line*/
}

/* This function skips spaces of a string*/
char* skip_spaces(char* ch)
{
	/*if (ch == NULL) return NULL;*/
	while (isspace(*ch))/*while the current char is a space we will continue to the next one*/
		ch++;
	return ch;/*return the first position with non space*/
}

/* Checking for the end of line*/
int end_of_line(char* line)
{
	if (line == NULL || *line == '\0' || *line == '\n')
		return;
}


int convert_to_HEX() {

}



void copy_sign(char* dest, char* line)
{

}



/* This function checks whether a given token is a label or not (by syntax).
 * The parameter colon states whether the function should look for a ':' or not
 * when parsing parameter (to make it easier for both kinds of tokens passed to this function.
 */
boolean is_label(char* token, int colon)
{
   
}


char* next_sign(char* ch)
{

}



int if_guidance(char* line)
{
	/*if (token == NULL || *token != '.') return NOT_FOUND;
	return find_index(token, directives, NUM_DIRECTIVES);*/
}


int if_command(char* line)
{
	/*if (token == NULL || *token != '.') return NOT_FOUND;
	return find_index(token, directives, NUM_DIRECTIVES);*/
}



char* next_list_sign(char* dest, char* line)
{

}


boolean is_number(char* seq)
{

}


/* This function copies supposedly next string into dest array and returning a pointer to the
 * first character after it
 */
char* next_sign_string(char* dest, char* line)
{
	char temp[LINE_LENGTH];
	line = next_list_token(dest, line);
	if (*dest != '"') return line;
	while (!end_of_line(line) && dest[strlen(dest) - 1] != '"')
	{
		line = next_list_token(temp, line);
		if (line) strcat(dest, temp);
	}
	return line;
}


boolean is_string(char* string)
{

}




/* This function encodes a given string to data */
void write_string_to_data(char* str)
{


