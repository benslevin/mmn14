#include "assembler.h"


#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>



void passOne(FILE* fp)
{

	char line[CURRENT_LINE]; /*This char array contains the current line we are handling*/
	int line_number = 100;/*line number starts from 100 (decimal)*/


	while (fgets(line, CURRENT_LINE, fp) != NULL) /* Read lines until end of file */
	{
		error = EMPTY_ERROR; /* Reset the error variable at the begining of a new line*/
		if (!ignore_line(line)) /* Ignore line if it's blank or ; */
			read_line(line);
		if (if_error()) {
			errorExist = TRUE; /* There was at least one error through all the program */
			write_error(line_number); /* Output the error */
		}
		line_number++;



	}
	/*not sure why we need the following, leaving it here ate the moment*/
	/* When the first pass ends and the symbols table is complete and IC is evaluated,
   we can calculate real final addresses */

   /*offset_addresses(symbols_table, MEMORY_START, FALSE); /* Instruction symbols will have addresses that start from 100 (MEMORY_START)
   offset_addresses(symbols_table, ic + MEMORY_START, TRUE);  Data symbols will have addresses that start fron NENORY_START + IC */


}








