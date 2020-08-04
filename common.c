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
