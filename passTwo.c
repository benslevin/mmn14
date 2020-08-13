#include "main.h"
#include "external_vars.h"
#include "passFunctions.h"
#include "common.h"



/* The function for the second pass of the file */
void passTwo(FILE* fp, char* filename) {
	char line[MAX_INPUT];/* Read each line */
	int line_number = 1;/* Line numbering for error messages */
	ic = 0;

	while (fgets(line, MAX_INPUT, fp) != NULL) {
		error = EMPTY_ERROR;
		if (!ignore(line)) {/* Checks if needs to ignore line */
			line_pass_two(line);
		}
		if (if_error()) {
			error_exist = TRUE; /* There was at least one error through all the program in the second pass */
			write_error(line_number); /* Print the error */
		}
		line_number++;

		if (!error_exist) { /* No errors found int the file */
			creat_output_files(filename); /* creating files */
		}

		/*Free memory of linked list/tabels*/
		free_labels(&symbols_table);
		free_ext(&ext_list);
	}
}

/* The function that reads each line and checks it */
void line_pass_two(char* line) {
	int guidance_type, command_type;
	char current_sign[MAX_INPUT];

	line = skip_spaces(line);
	if (end_of_line(line))
		return;

	copy_sign(current_sign, line);
	if (is_label(current_sign, COLON)) {/* Check how to creat the seperation with the ':' */
		line = next_sign(line);
			copy_sign(current_sign, line);
	}

	if ((guidance_type = find_guidance(current_sign)) != NO_MATCH) /* We need to handle only .entry directive */
	{
		line = next_sign(line);
			if (guidance_type == ENTRY)
			{
				copy_sign(current_sign, line);
				make_entry(symbols_table, current_sign);
			}
	}

	else if ((command_type = find_command(current_sign)) != NO_MATCH) /* Encoding command's additional words if necessary */
	{
		line = next_sign(line);
		handle_command_pass_two(command_type, line);
	}
	
}

/* The fuction that creats the output files */
void creat_output_files(char* name) {
	FILE* file;

	file = open_file(name, FILE_OBJECT);
	creat_object_file(file); /* Creating object file */

	if (entry_exists) {
		file = open_file(name, FILE_ENTRY);
		creat_entry_file(file);/* Creating entry file if entry exists */
	}

	if (extern_exists) {
		file = open_file(name, FILE_EXTERN);
		creat_extern_file(file);/* Creating external file if extern exists */
	}

	return EMPTY_ERROR; /* No errors found during the passes */
}

FILE* open_file(char* filename, int type) {

	FILE* file;
	filename = create_file_name(filename, type); /* Creating filename with extension */

	file = fopen(filename, "w"); /* Opening file with permissions */
	free(filename); /* Allocated modified filename is no longer needed */

	if (file == NULL)
	{
		error = CANNOT_OPEN_FILE;
		return NULL;
	}
	return file;
}

void creat_object_file(FILE* fp) {

	unsigned int address = DEFAULT_IC;/*start of memory*/
	int i;
	char* param1 = ic, * param2 = dc;

	fprintf(fp, "%s \t %s\n\n", param1, param2); /* First line */
	free(param1);
	free(param2);

	for (i = 0; i < ic; address++, i++) /* Instructions memory */
	{
		param1 = address;
		param2 = instructions[i];

		fprintf(fp, "%07d \t %hh6X\n", param1, param2);

		free(param1);
		free(param2);
	}

	for (i = 0; i < dc; address++, i++) /* Data memory */
	{
		param1 = address;
		param2 = data[i];

		fprintf(fp, "%07d \t %hh6X\n", param1, param2);

		free(param1);
		free(param2);
	}

	fclose(fp);
}

void creat_entry_file(FILE* fp) {

	char* param1, * param2;
	labelPtr label = symbols_table;

	/* Go through symbols table and print only symbols that have an entry */
	while (label)
	{
		if (label->entry)
		{
			param1 = label->name;
			param2 = label->address;
			fprintf(fp, "%s \t %hh6X\n", param1, param2);
			free(param1);
			free(param2);
		}
		label = label->next;
	}
	fclose(fp);
}

void creat_external_file(FILE* fp) {

	char* param1, param2;
	extPtr node = ext_list;

	/* Going through external circular linked list and pulling out values */
	do
	{
		param1 = node->name;
		param2 = node->address;
		fprintf(fp, "%s \t %hh6X\n", param1, param2); /* Printing to file */
		free(param1);
		free(param2);
		node = node->next;
	} while (node != ext_list);
	fclose(fp);
}

void does_operand_exists(int ope, boolean* is_source, boolean* is_destination) {

	switch (ope) {
	case MOV:
	case CMP:
	case ADD:
	case SUB:
	case LEA:
		is_source = TRUE;
		is_destination = TRUE;
		break;

	case CLR:
	case NOT:
	case INC:
	case DEC:
	case JMP:
	case BNE:
	case JSR:
	case RED:
	case PRN:
		is_source = FALSE;
		is_destination = TRUE;
		break;

	case RTS:
	case STOP:
		is_source = FALSE;
		is_destination = FALSE;
	}

}

/* This function handles commands for the second pass - encoding additional words */
int handle_command_pass_two(int type, char* line)
{

	char first_op[MAX_INPUT], second_op[MAX_INPUT]; /* will hold first and second operands */
	char* src = first_op, * dest = second_op; /* after the check below, src will point to source and
 *                                              dest to destination operands */
	boolean is_src = FALSE, is_dest = FALSE; /* Source/destination operands existence */
	int src_method = METHOD_UNKNOWN, dest_method = METHOD_UNKNOWN; /* Their addressing methods */

	does_operand_exists(type, &is_src, &is_dest);

	/* Extracting source and destination addressing methods */
	if (is_src)
		src_method = extract_bits(instructions[ic], SRC_METHOD_START_POS, SRC_METHOD_END_POS);/*check how to place bits*/
	if (is_dest)
		dest_method = extract_bits(instructions[ic], DEST_METHOD_START_POS, DEST_METHOD_END_POS);/*check how to place bits*/

	/* Matching src and dest pointers to the correct operands (first or second or both) */
	if (is_src || is_dest)
	{
		line = next_list_sign(first_op, line);
		if (is_src && is_dest) /* There are 2 operands */
		{
			line = next_list_sign(second_op, line);
			next_list_sign(second_op, line);
		}
		else
		{
			dest = first_op; /* If there's only one operand, it's a destination operand */
			src = NULL;
		}
	}

	ic++; /* The first word of the command was already encoded in this IC in the first pass */
	return encode_additional_words(src, dest, is_src, is_dest, src_method, dest_method);
}

/* This function encodes the additional words of the operands to instructions memory */
int encode_additional_words(char* src, char* dest, boolean is_src, boolean is_dest, int src_method,
	int dest_method) {
	if (is_src)
		encode_additional_word(FALSE, src_method, src);
	if (is_dest)
		encode_additional_word(TRUE, dest_method, dest);
	return if_error();
}


/* This function encodes a given label (by name) to memory */
void encode_label(char* label)
{
	unsigned int word = 0; /* The word to be encoded */

	if (is_existing_label(symbols_table, label)) { /* If label exists */
		word = get_label_address(symbols_table, label); /* Getting label's address */

		if (is_external_label(symbols_table, label)) { /* If the label is an external one */
			/* Adding external label to external list (value should be replaced in this address) */
			add_ext(&ext_list, label, ic + DEFAULT_IC);
			word = 0;/*making sure that the word is zero, and sets the External bit*/
			word = EXTERNAL;
		}
		else
			word <<= ARE_BITS;
		
		word |= RELOCATABLE; /* If it's not an external label, then it's relocatable */

		encode_to_instructions(word); /* Encode word to memory */
	}
	else /* It's an error */
	{
		ic++;
		error = COMMAND_LABEL_DOES_NOT_EXIST;
	}
}

void encode_label_relative(char* label) {

	unsigned int word = 0; /* The word to be encoded */

	if (label[0] == '&') {
		label++;
		if (is_existing_label(symbols_table, label) && !is_external_label(symbols_table, label)) {/* If label exists, and not external */
			word = (ic - get_label_address(symbols_table, label)); /* Getting label's address, and calculating the distance to label */
		}
		encode_to_instructions(word); /* Encode word to memory */
	}
	else /* It's an error */
	{
		ic++;
		error = WRONG_SYNTAX_FOR_METHOD_RELATIVE;
	}

}

/* This function encodes an additional word to instructions memory, given the addressing method */
void encode_additional_word(boolean is_dest, int method, char* operand)
{
	unsigned int word = 0; /* An empty word */
	char* temp;

	switch (method)
	{
	case METHOD_DIRECT:
		encode_label(operand);
		break;

	case METHOD_RELATIVE:
		encode_label_relative(operand);
		break;

	case METHOD_REGISTER:
		word = build_register_word(is_dest, operand);
		encode_to_instructions(word);
	}
}
