#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

#include "assembler.h"
#include "main.h"



void passOne(FILE* fp)
{
	char line[MAX_INPUT]; /*This char array contains the current line we are handling*/
	int line_number = 1;/*line number starts from 1*/

	/* Initializing data and instructions counter */
	ic = 100;
	dc = 0;

	while (fgets(line, MAX_INPUT, fp) != NULL) /* Read lines until end of file */
	{
		error = EMPTY_ERROR; /* Reset the error variable at the begining of a new line*/
		if (!ignore_line(line)) /* Ignore line if it's blank*/
			line_pass_one(line);
		if (if_error()) {
			error_exist = TRUE; /* There was at least one error through all the program */
			write_error(line_number); /* Output the error */
		}
		line_number++;
	}
	/*not sure why we need the following, leaving it here ate the moment*/

   /* When the first pass ends and the symbols table is complete and IC is evaluated,
   we can calculate real final addresses */
   /*
  offset_addresses(symbols_table, MEMORY_START, FALSE);  Instruction symbols will have addresses that start from 100 (MEMORY_START)
  offset_addresses(symbols_table, ic + MEMORY_START, TRUE);  Data symbols will have addresses that start fron NENORY_START + IC */
}


void  line_pass_one(char* line)
{

	/* Initializing variables for the type of the directive/command */
	int guidance_type = UNKNOWN_TYPE;
	int command_type = UNKNOWN_COMMAND;
	boolean label = FALSE; /* This variable will hold TRUE if a label exists in this line */
	labelPtr label_node = NULL; /* This variable holds optional label in case we need to create it */
	char current_sign[LINE_LENGTH]; /* This string will hold the current sign if we analyze it */


	/*Beginning to cross a line*/
	line = skip_spaces(line);/*skip all spaces*/
	if (end_of_line(line)) return;/*starts next row in case the row is empty*/

	if ((isalpha(*line) == 0) && *line != '.') { /* first non-blank character must be a letter or a dot */
		error = INVALID_SYNTAX;
		return;
	}
	copy_sign(current_sign, line); /*taking the label and copy it*/

	if (is_label(current_sign, COLON))/*test if the first word is a label*/
	{
		label = TRUE;
		label_node = add_label(&symbol_table, current_sign, 100,/* FALSE,FALSE*/)/*Adding a new label to the Symbol table*/
			if (label_node == NULL)/*in case we didnt succeed to add the label*/
				return;
		line = next_sign(line);/*Getting the next sign*/
		if (end_of_line(line))
		{
			error = INVALID_LABEL_LINE;
			return;
		}
		copy_sign(current_sign, line);/*after we get the next the next word or symbol we continue with the proccess*/
	}

	if (if_error())/*in case the first pass for label search returns an error*/

		if ((guidance_type = if_guidance(current_sign)) != NOT_FOUND) /*in case the sign is a guidance*/
		{
			if (label != 0)
			{
				if (guidance_type == ENTRY || guidance_type == EXTERN) {
					delete_label(&symbols_table, label_node->name);
					label = FALSE;
				}
				else
					label_node->address = dc; /* Address of data label is dc */
			}
			line = next_sign(line);
			handle_guidance(guidance_type, line);

		}

		else if ((command_type = if_command(current_sign)) != NOT_FOUND) /* detecting command type (if it's a command) */
		{
			if (label != 0)
			{
				/* Setting fields accordingly in label */
				label_node->dataStorageStatment = TRUE;
				label_node->address = ic;
			}
			line = next_sign(line);
			handle_command(command_type, line);
		}
		else
			error = MISSING_SYNTAX;/*In case a line does not have a command or a guidance */
}




/* This function handles all kinds of guidance (.data, .string, .entry, .extern)
 * and sends them accordingly to the suitable function for analyzing them
 * */
int handle_guidance(int guidance_type, char* line)
{
	if (line == NULL || end_of_line(line)) /*at least one parameter must fllow a guidance*/
	{
		error = NO_PARAMETER_AVAILABLE;
		return ERROR;
	}

	switch (type)
	{
	case DATA:
		/* Handle .data  and insert values separated by comma to the memory */
		return handle_data_guidance(line);

	case STRING:
		/* Handle .string directive and insert all characters (including a '\0') to memory */
		return handle_string_guidance(line);

	case ENTRY:
		/* Only check for syntax of entry (should not contain more than one parameter) */
		if (!end_of_line(next_sign(line))) /* If there's a next token (after the first one) */
		{
			error = GUIDANCE_INVALID_NUM_PARAMS;
			return ERROR;
		}
		break;

	case EXTERN:
		/* Handle .extern directive */
		return handle_extern_guidance(line);
	}
	return NO_ERROR;
}


int handle_data_guidance(char* line)
{
	char sign[NUM]; /* Holds tokens */

	/* These booleans mark if there was a number or a comma before current token,
	 * so that if there wasn't a number, then a number will be required and
	 * if there was a number but not a comma, a comma will be required */
	boolean number = FALSE;
	boolean comma = FALSE;

	while (!end_of_line(line))
	{
		line = next_list_sign(sign, line); /* Getting current sign */

		if (strlen(sign) > 0) /* Not an empty sign */
		{
			if (number == FALSE) { /* if there wasn't a number before */
				if (is_number(sign) == FALSE) { /* then the sign must be a number */
					error = DATA_EXPECTED_NUM;
					return ERROR;
				}

				else {
					number = TRUE; /* A valid number was inputted */
					comma = FALSE; /* Resetting comma (now it is needed) */
					write_num_to_data(atoi(sign)); /* encoding number to data */
				}
			}

			else if (*sign != ',') /* If there was a number, now a comma is needed */
			{
				error = DATA_EXPECTED_COMMA_AFTER_NUM;
				return ERROR;
			}

			else /* If there was a comma, it should be only once (comma should be false) */
			{
				if (comma == TRUE) {
					error = DATA_COMMAS_IN_A_ROW;
					return ERROR;
				}
				else {
					comma = TRUE;
					number = FALSE;
				}
			}

		}
	}
	if (comma == TRUE)
	{
		error = DATA_UNEXPECTED_COMMA;
		return ERROR;
	}
	return NO_ERROR;
}


/* This function handles a .string directive by analyzing it and encoding it to data */
int handle_string_guidance(char* line)
{
	char sign[MAX_INPUT];

	line = next_sign_string(sign, line);
	if (!end_of_line(sign) && is_string(sign)) { /* If token exists and it's a valid string */
		line = skip_spaces(line);
		if (end_of_line(line)) /* If there's no additional sign */
		{
			/* "Cutting" quotation marks and encoding it to data */
			token[strlen(sign) - 1] = '\0';
			write_string_to_data(sign + 1);
		}

		else /* There's another sign */
		{
			error = STRING_TOO_MANY_OPERANDS;
			return ERROR;
		}

	}

	else /* Invalid string */
	{
		error = STRING_OPERAND_NOT_VALID;
		return ERROR;
	}

	return NO_ERROR;
}


//----------------------------------------------
//
//handle_extern_guidance
//-----------------------------------------------------------------------------------

int handle_command(int type, char* line)
{
	boolean is_first = FALSE;
	boolean is_second = FALSE; /* These booleans will tell which of the operands were
													 received (not by source/dest, but by order) */
	int first_method, second_method; /* These will hold the addressing methods of the operands */
	char first_op[20], second_op[20]; /* These strings will hold the operands */

	/* Trying to parse 2 operands */
	line = next_list_token(first_op, line);
	if (!end_of_line(first_op)) /* If first operand is not empty */
	{
		is_first = TRUE; /* First operand exists! */
		line = next_list_token(second_op, line);
		if (!end_of_line(second_op)) /* If second operand (should hold temporarily a comma) is not empty */
		{
			if (second_op[0] != ',') /* A comma must separate two operands of a command */
			{
				err = COMMAND_UNEXPECTED_CHAR;
				return ERROR;
			}

			else
			{
				line = next_list_token(second_op, line);
				if (end_of_line(second_op)) /* If second operand is not empty */
				{
					err = COMMAND_UNEXPECTED_CHAR;
					return ERROR;
				}
				is_second = TRUE; /* Second operand exists! */
			}
		}
	}
	line = skip_spaces(line);
	if (!end_of_line(line)) /* If the line continues after two operands */
	{
		err = COMMAND_TOO_MANY_OPERANDS;
		return ERROR;
	}

	if (is_first)
		first_method = detect_method(first_op); /* Detect addressing method of first operand */
	if (is_second)
		second_method = detect_method(second_op); /* Detect addressing method of second operand */

	if (!is_error()) /* If there was no error while trying to parse addressing methods */
	{
		if (command_accept_num_operands(type, is_first, is_second)) /* If number of operands is valid for this specific command */
		{
			if (command_accept_methods(type, first_method, second_method)) /* If addressing methods are valid for this specific command */
			{
				/* encode first word of the command to memory and increase ic by the number of additional words */
				encode_to_instructions(build_first_word(type, is_first, is_second, first_method, second_method));
				ic += calculate_command_num_additional_words(is_first, is_second, first_method, second_method);
			}

			else
			{
				err = COMMAND_INVALID_OPERANDS_METHODS;
				return ERROR;
			}
		}
		else
		{
			err = COMMAND_INVALID_NUMBER_OF_OPERANDS;
			return ERROR;
		}
	}

	return NO_ERROR;
}

void write_num_to_data(int num)
{
	data[dc++] = (unsigned int)num;
}



