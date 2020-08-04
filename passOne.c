#include "assembler.h"


#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>



void passOne(FILE* fp)
{

	char line[CURRENT_LINE]; /*This char array contains the current line we are handling*/
	int line_number = 1;/*line number starts from 1*/

	/* Initializing data and instructions counter */
	ic = 100;
	dc = 0;

	while (fgets(line, CURRENT_LINE, fp) != NULL) /* Read lines until end of file */
	{
		error = EMPTY_ERROR; /* Reset the error variable at the begining of a new line*/
		if (!ignore_line(line)) /* Ignore line if it's blank*/
			line_pass(line);
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


void  line_pass(char* line)
{

    /* Initializing variables for the type of the directive/command */
    int guidance_type = UNKNOWN_TYPE;
    int command_type = UNKNOWN_COMMAND;
    boolean label = FALSE; /* This variable will hold TRUE if a label exists in this line */
    labelPtr label_node = NULL; /* This variable holds optional label in case we create it */
    char current_sign[LINE_LENGTH]; /* This string will hold the current sign if we analyze it */


    /*Beginning to cross a line*/
    line = skip_spaces(line);
    if (end_of_line(line)) return;

}



/* This function will analyze a given line from the file and will extract the information needed by the Maman's rules */
void read_line(char* line)
{
    /* Initializing variables for the type of the directive/command */
    int dir_type = UNKNOWN_TYPE;
    int command_type = UNKNOWN_COMMAND;

    boolean label = FALSE; /* This variable will hold TRUE if a label exists in this line */
    labelPtr label_node = NULL; /* This variable holds optional label in case we create it */
    char current_token[LINE_LENGTH]; /* This string will hold the current token if we analyze it */

    line = skip_spaces(line); /* skips to the next non-blank/whitepsace character */
    if (end_of_line(line)) return; /* a blank line is not an error */
    if (!isalpha(*line) && *line != '.') { /* first non-blank character must be a letter or a dot */
        err = SYNTAX_ERR;
        return;
    }

    copy_token(current_token, line); /* Assuming that label is separated from other tokens by a whitespace */
    if (is_label(current_token, COLON)) { /* We check if the first token is a label (and it should contain a colon) */
        label = TRUE;
        label_node = add_label(&symbols_table, current_token, 0, FALSE, FALSE); /* adding label to the global symbols table */
        if (label_node == NULL) /* There was an error creating label */
            return;
        line = next_token(line); /* Skipping to beginning of next token */
        if (end_of_line(line))
        {
            err = LABEL_ONLY; /* A line can't be label-only */
            return;
        }
        copy_token(current_token, line); /* Proceed to next token */
    } /* If there's a label error then exit this function */

    if (is_error()) /* is_label might return an error */
        return;

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









