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
			error = INVALID_LABEL_LINE;/* only label exists in line*/
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

		}

	}

	if ((dir_type = find_directive(current_token)) != NOT_FOUND) /* detecting directive type (if it's a directive) */
	{
		if (label)
		{
			if (dir_type == EXTERN || dir_type == ENTRY) { /* we need to ignore creation of label before .entry/.extern */
				delete_label(&symbols_table, label_node->name);
				label = FALSE;
			}
			else
				label_node->address = dc; /* Address of data label is dc */
		}
		line = next_token(line);
		handle_directive(dir_type, line);
	}




}



/* This function will analyze a given line from the file and will extract the information needed by the Maman's rules */
void read_line(char* line)
{


	if ((dir_type = find_directive(current_token)) != NOT_FOUND) /* detecting directive type (if it's a directive) */
	{
		if (label)
		{
			if (dir_type == EXTERN || dir_type == ENTRY) { /* we need to ignore creation of label before .entry/.extern */
				delete_label(&symbols_table, label_node->name);
				label = FALSE;
			}
			else
				label_node->address = dc; /* Address of data label is dc */
		}
		line = next_token(line);
		handle_directive(dir_type, line);
	}

	else if ((command_type = find_command(current_token)) != NOT_FOUND) /* detecting command type (if it's a command) */
	{
		if (label)
		{
			/* Setting fields accordingly in label */
			label_node->inActionStatement = TRUE;
			label_node->address = ic;
		}
		line = next_token(line);
		handle_command(command_type, line);
	}

	else
	{
		err = COMMAND_NOT_FOUND; /* a line must have a directive/command */
	}

}









