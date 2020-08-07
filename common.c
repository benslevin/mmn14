#include "main.h"
#include "struct.h"
#include <stdio.h>
#include <ctype.h>

/*********functions for main**********/

/* This function creates a file name by the name it recives from the user */
char* create_file_name(char* original, int type)
{
	char* modified = (char*)malloc(strlen(original) + MAX_EXTENSION_LENGTH);
	if (modified == NULL)
	{
		fprintf(stderr, "Couldn't allocate memory.");
		exit(ERROR);
	}

	strcpy(modified, original); /* Copying original filename to the bigger string */

	/* Concatenating the required file extension */

	switch (type)
	{
	case FILE_INPUT:
		strcat(modified, ".as");
		break;

	case FILE_OBJECT:
		strcat(modified, ".ob");
		break;

	case FILE_ENTRY:
		strcat(modified, ".ent");
		break;

	case FILE_EXTERN:
		strcat(modified, ".ext");

	}
	return modified;
}


/*********functions for passes**********/

/*checks if the line is empty or needs to be ignored (;)*/
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

/*This function check if the 'error' flag was changed meaning that an error while reading the line occured*/
int if_error()
{
	if (error != EMPTY_ERROR)
		return 1;
	else
		return 0;
}

/* This function skips spaces (blanks)*/
char* skip_spaces(char* ch)
{
	/*if (ch == NULL) return NULL;*/
	while (isspace(*ch))/*while the current char is a space we will continue to the next one*/
		ch++;
	return ch;/*return the first non space char*/
}

/* Checking for the end of line*/
int end_of_line(char* line)
{
	if (line == NULL || *line == '\0' || *line == '\n')
		return;
}

/*copies the next word from line to destination*/
void copy_sign(char* destination, char* line)
{
	int i = 0;
	if (destination == NULL || line == NULL)
		return;

	while (i < MAX_INPUT && !isspace(line[i]) && line[i] != '\0') /* Copying token until its end to *dest */
	{
		destination[i] = line[i];
		i++;
	}
	destination[i] = '\0';
}

/* This function checks whether a given token is a label or not (by syntax).
 * The parameter colon states whether the function should look for a ':' or not
 * when parsing parameter (to make it easier for both kinds of tokens passed to this function.
 */
boolean is_label(char* sign, int colon)
{

	boolean has_digits = FALSE; /* helps check if it's a command */
	int sign_len = strlen(sign);
	int i;

	/* Checking if token's length is not too short */
	if (sign == NULL || sign_len < 2)/*with a colon the min length for a label is 2*/
		return FALSE;

	if (colon && (sign[sign_len - 1] != ':'))
		return FALSE; /* if colon = TRUE, there must be a colon at the end */

	if (sign_len > MAX_LABEL) {
		if (colon) {
			err = LABEL_TOO_LONG; /*max length 32*/
		}
		return FALSE;
	}
	if (!isalpha(*sign)) { 
		if (colon) {
			err = LABEL_INVALID_FIRST_CHAR;/* First character must be a letter */
		}
		return FALSE;
	}

	if (colon) {/*remove the colon*/
		sign[sign_len - 1] = '\0'; 
		sign_len--;
	}

	/* Check if all characters are digits or letters */
	for (i = 0; i < sign_len; i++) 
	{
		if (isdigit(sign[i]))
			has_digits = TRUE;
	}

	if (!has_digits) /* It can't be a command */
	{
		if (find_command(sign) != NO_MATCH) {
			if (colon) {
				err = LABEL_CANT_BE_COMMAND; /* Label can't have the same name as a command */
			}
			return FALSE;
		}
	}

	if (is_register(sign)) /* Label can't have the same name as a register */
	{
		if (colon) {
			err = LABEL_CANT_BE_REGISTER;
		}
		return FALSE;
	}

	return TRUE;
}

char* next_sign(char* ch, char * line)
{
	char temp[MAX_INPUT];
	line = next_list_sign(ch, line);
	if (*ch != '"') {
		return line;
	}
	while (!end_of_line(line) && (ch[strlen(ch) - 1] != '"'))
	{
		line = next_list_sign(temp, line);
		if (line) strcat(ch, temp);
	}
	return line;
}



/*\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/* This function finds an index of a string in an array of strings */
int find_index(char* sign, const char* arr[], int n)
{
	int i;
	for (i = 0; i < n; i++)
		if (strcmp(sign, arr[i]) == 0) {
			return i;
		}
	return NO_MATCH;
}

/* Check if a token matches a register name */
boolean is_guidence(char* sign)
{
	/* A register must have 2 characters, the first is 'r' and the second is a number between 0-7 */
	if (strlen(sign) == REG_LEN && sign[0] == 'r' && sign[1] >= '0' && sign[1] <= '7') {
		return TRUE;
	}
	else return FALSE;
}

/* Check if a token matches a directive name */
int find_guidence(char* token)
{
	if (token == NULL || *token != '.') {
		return NO_MATCH;
	}
	return find_index(token, directives, NUM_DIRECTIVES);
}

/* Check if a token matches a command name */
int find_command(char* token)
{
	int token_len = strlen(token);
	if (token_len > MAX_COMMAND_LENGTH || token_len < MIN_COMMAND_LENGTH)
		return NO_MATCH;
	return find_index(token, commands, NUM_COMMANDS);
}
/*\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*********functions for word**********/








/***************ERRORS****************/

/*Checks what error was encoutered and prints the error and line number*/
void write_error(int line_number) {
	fprintf(stderr, "ERROR (line %d): ", line_number);

	switch (err)
	{
	case INVALID_SYNTAX:
		fprintf(stderr, "first non-blank character must be a letter or a dot.\n");

		break;

	case LABEL_ALREADY_EXISTS:
		fprintf(stderr, "label already exists.\n");

		break;
		/*
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
		*/
	case INVALID_LABEL_LINE:
		fprintf(stderr, "label must be followed by a command or guidence.\n");

		break;

	case NO_PARAMETER_AVAILABLE:
		fprintf(stderr, "guidence must have parameters.\n");

		break;
		/*
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
		*/
	case MISSING_SYNTAX:
		fprintf(stderr, "invalid command or guidence.\n");

		break;
		/*
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