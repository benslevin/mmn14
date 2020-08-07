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

void write_error() {


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


