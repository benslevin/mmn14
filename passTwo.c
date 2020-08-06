#include "external_vars.h"
#include "common.h"


void passTwo(FILE *fp, char *filename) {
	char line[MAX_INPUT];/*read each line*/
	int line_number = 1;
	ic = 0;

	while (fgets(line, MAX_INPUT, fp) != NULL) {
		error = EMPTY_ERROR;
		if (!ignore(line)) {/* checks if needs to ignore line */
			line_pass_two(line);
		}
		if (if_error()) {
			error_exist = TRUE; /* There was at least one error through all the program */
			write_error(line_number); /* Output the error */
		}
		line_number++;
		
		if (!error_exist) {
			creat_output_files(filename);
		}

		/*Free memory of linked list/tabels*/
		/*need to add functions*/
		/******/free_labels(&symbol_table);
		/******/free_ext(&ext_list);
	}
}

void line_pass_two() {
	int guidence_type, command_type;
	char current_sign[MAX_INPUT];

	line = skip_spaces(line);
	if (end_of_line(line)) return;

	copy_sign(current_sign, line);
	if (is_label(current_sign, COLON) {/*check how to creat the seperation with the :*/
		line = next_sign(line);
		copy_sign(current_sign, line);
	}

	if ((guidence_type = find_guidence(current_sign)) != NOT_FOUND) /* We need to handle only .entry directive */
	{
		line = next_sign(line);
			if (guidence_type == ENTRY)
			{
				copy_sign(current_sign, line);
				make_entry(symbols_table, current_sign); /* write the command*/
			}
	}

	else if ((command_type = find_command(current_sign)) != NOT_FOUND) /* Encoding command's additional words */
	{
		line = next_sign(line);
		handle_command_second_pass(command_type, line);/*write the command*/
	}
}

void creat_output_files(char* name) {
	FILE* file;

	file = open_file(name, FILE_OBJECT);
	creat_object_file(file);

	if (entry_exist) {
		file = open_file(name, FILE_ENTRY);
		creat_entry_file(file);
	}

	if (extern_exist) {
		file = open_file(name, FILE_EXTERN);
		creat_extern_file(file);
	}

	return NO_ERRORS;/*check if its declared*/
}

FILE* open_file(char* filename, int type) {

}

void creat_object_file() {

}

void creat_entry_file() {

}

void creat_external_file() {

}

