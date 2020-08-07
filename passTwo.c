#include "external_vars.h"
#include "common.h"
#include "struct.h"

void passTwo(FILE* fp, char* filename) {
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

void line_pass_two(char* line) {
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

	return EMPTY_ERROR; /*check if its declared*/
}

FILE* open_file(char* filename, int type) {

}

void creat_object_file() {

}

void creat_entry_file() {

}

void creat_external_file() {

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
	case JSR:
		is_source = FALSE;
		is_destination = TRUE;
		break;

	case BNE:
	case RED:
	case PRN:
	case RTS:
	case STOP:
		is_source = FALSE;
		is_destination = FALSE;
	}

}

/*------------------------------------------------------------------------------------------------------*/


/* This function handles commands for the second pass - encoding additional words */
int handle_command_second_pass(int type, char* line)
{
	char first_op[MAX_INPUT], second_op[MAX_INPUT]; /* will hold first and second operands */
	char* src = first_op, * dest = second_op; /* after the check below, src will point to source and
 *                                              dest to destination operands */
	boolean is_src = FALSE, is_dest = FALSE; /* Source/destination operands existence */
	int src_method = METHOD_UNKNOWN, dest_method = METHOD_UNKNOWN; /* Their addressing methods */

	does_operand_exists(type, &is_src, &is_dest);

	/* Extracting source and destination addressing methods */
	if (is_src)
		src_method = extract_bits(instructions[ic], SRC_METHOD_START_POS, SRC_METHOD_END_POS);//check how to place bits
	if (is_dest)
		dest_method = extract_bits(instructions[ic], DEST_METHOD_START_POS, DEST_METHOD_END_POS);//check how to place bits

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
	/* There's a special case where 2 register operands share the same additional word */
	if (is_src && is_dest && src_method == METHOD_REGISTER && dest_method == METHOD_REGISTER)
	{
		encode_to_instructions(build_register_word(FALSE, src) | build_register_word(TRUE, dest));
	}
	else /* It's not the special case */
	{
		if (is_src) encode_additional_word(FALSE, src_method, src);
		if (is_dest) encode_additional_word(TRUE, dest_method, dest);
	}
	return is_error();
}


/* This function encodes the additional words of the operands to instructions memory */
int encode_additional_words(char* src, char* dest, boolean is_src, boolean is_dest, int src_method,
	int dest_method) {
	/* There's a special case where 2 register operands share the same additional word */
	if (is_src && is_dest && src_method == METHOD_REGISTER && dest_method == METHOD_REGISTER)
	{
		encode_to_instructions(build_register_word(FALSE, src) | build_register_word(TRUE, dest));
	}
	else /* It's not the special case */
	{
		if (is_src) encode_additional_word(FALSE, src_method, src);
		if (is_dest) encode_additional_word(TRUE, dest_method, dest);
	}
	return is_error();
}

/* This function builds the additional word for a register operand */
unsigned int build_register_word(boolean is_dest, char* reg)
{
	unsigned int word = (unsigned int)atoi(reg + 1); /* Getting the register's number */
	/* Inserting it to the required bits (by source or destination operand) */
	if (!is_dest)
		word <<= BITS_IN_REGISTER;
	word = insert_are(word, ABSOLUTE);
	return word;
}

/* This function encodes a given label (by name) to memory */
void encode_label(char* label)
{
	unsigned int word; /* The word to be encoded */

	if (is_existing_label(symbols_table, label)) { /* If label exists */
		word = get_label_address(symbols_table, label); /* Getting label's address */

		if (is_external_label(symbols_table, label)) { /* If the label is an external one */
			/* Adding external label to external list (value should be replaced in this address) */
			add_ext(&ext_list, label, ic + MEMORY_START);
			word = insert_are(word, EXTERNAL);
		}
		else
			word = insert_are(word, RELOCATABLE); /* If it's not an external label, then it's relocatable */

		encode_to_instructions(word); /* Encode word to memory */
	}
	else /* It's an error */
	{
		ic++;
		err = COMMAND_LABEL_DOES_NOT_EXIST;
	}
}

/* This function encodes an additional word to instructions memory, given the addressing method */
void encode_additional_word(boolean is_dest, int method, char* operand)
{
	unsigned int word = EMPTY_WORD; /* An empty word */
	char* temp;

	switch (method)
	{
	case METHOD_IMMEDIATE: /* Extracting immediate number */
		word = (unsigned int)atoi(operand + 1);
		word = insert_are(word, ABSOLUTE);
		encode_to_instructions(word);
		break;

	case METHOD_DIRECT:
		encode_label(operand);
		break;

	case METHOD_STRUCT: /* Before the dot there should be a label, and after it a number */
		temp = strchr(operand, '.');
		*temp = '\0';
		encode_label(operand); /* Label before dot is the first additional word */
		*temp++ = '.';
		word = (unsigned int)atoi(temp);
		word = insert_are(word, ABSOLUTE);
		encode_to_instructions(word); /* The number after the dot is the second */
		break;

	case METHOD_REGISTER:
		word = build_register_word(is_dest, operand);
		encode_to_instructions(word);
	}
}

