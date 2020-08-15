#include "main.h"
#include "external_vars.h"
#include "passFunctions.h"
#include "common.h"

/* In this file we have functions that are in use for all the program */

/*********functions for main**********/

/* This function creates a filename with the name it recives from the user */
char* create_file_name(char* original, int type) {

    char* modified = (char*)malloc(strlen(original) + 4); /* Add space for the part after the "." */
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

/* Checks if the line is empty or needs to be ignored (;) */
int ignore_line(char* line) {
    line = skip_spaces(line);
    if (*line == ';' || *line == '\0' || *line == '\n') {
        return TRUE;
    }
    return FALSE;
}

/* This function check if the 'error' flag was changed meaning that an error while reading the line occured */
int if_error() {

    if (error != EMPTY_ERROR)
        return 1;
    else
        return 0;
}

/* This function skips spaces (blanks)*/
char* skip_spaces(char* ch) {

    if (ch == NULL) 
        return NULL;
    while (isspace(*ch))/* While the current char is a space we will continue to the next one */
        ch++;
    return ch;/* Return the first non space char */
}

/* Checking if end of line reached */
int end_of_line(char* line) {

    return (line == NULL || *line == '\0' || *line == '\n');
}

/* Copies the next word from line to destination */
void copy_sign(char* destination, char* line) {

    int i = 0;
    if (destination == NULL || line == NULL) 
        return;

    while (i < MAX_INPUT && !isspace(line[i]) && line[i] != '\0') /* Copying token until its end to *dest */
    {
        destination[i] = line[i];
        i++;
    }
    destination[i] = '\0'; /* Add end of string */
}

/* This function extracts bits, given start and end positions of the bit-sequence (0 is LSB) */
unsigned int extract_bits(unsigned int word, int start, int end) {

    unsigned int result;
    int length = end - start + 1; /* Length of bit-sequence */
    unsigned int mask = (int)pow(2, length) - 1; /* Creating a '111...1' mask with above line's length */

    mask <<= start; /* Moving mask to place of extraction */
    result = word & mask; /* The bits are now in their original position, and the rest is 0's */
    result >>= start; /* Moving the sequence to LSB */
    return result;
}

/* This function checks if a sign is a label or not */
boolean is_label(char* sign, int colon) {

    boolean has_digits = FALSE; /* helps check if it's a command */
    int sign_len = strlen(sign);
    int i;

    /* Checking if token's length is not too short */
    if (colon == TRUE) {
        if (sign == NULL || sign_len < 2) /* With a colon the min length for a label is 2 */
            return FALSE;
    }

    if (colon && (sign[sign_len - 1] != ':'))
        return FALSE; /* If colon = TRUE, there must be a colon at the end */

    if (sign_len > MAX_LABEL) {
        if (colon) {
            error = LABEL_TOO_LONG; /* Max length 32 */
        }
        return FALSE;
    }
    if (!isalpha(*sign)) {
        if (colon) {
            error = LABEL_INVALID_FIRST_CHAR;/* First character must be a letter */
        }
        return FALSE;
    }

    if (colon) { /* Remove the colon */
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
                error = LABEL_CANT_BE_COMMAND; /* Label can't have the same name as a command */
            }
            return FALSE;
        }
    }

    if (is_register(sign)) /* Label can't have the same name as a register */
    {
        if (colon) {
            error = LABEL_CANT_BE_REGISTER;
        }
        return FALSE;
    }

    return TRUE; /* Its a label! */
}

/* This function gets next sign */
char* next_sign(char* seq) {

    if (seq == NULL)
        return NULL;
    while (!isspace(*seq) && !end_of_line(seq)) {/* Skip rest of characters in the current token (until a space) */
        seq++; 
    }
    seq = skip_spaces(seq); /* Skip spaces */
    if (end_of_line(seq)) {
        return NULL;
    }
    return seq;
}

/* Check if a token matches a register name */
boolean is_register(char* sign) {

    /* A register must have 2 characters, the first is 'r' and the second is a number between 0-7 */
    if (strlen(sign) == REG_LEN && sign[0] == 'r' && sign[1] >= '0' && sign[1] <= '7') {
        return TRUE;
    }
    else if (strlen(sign) == REG_LEN && sign[0] == 'r' && sign[1] > '0' && sign[1] > '7') {
         error = NOT_A_VALID_REG;
         return FALSE;
    }
    else {
        return FALSE;
    }
       
}

/* Finds registers number */
int find_reg_number(char* sign) {

    int i;
    /* A register must have 2 characters, the first is 'r' and the second is a number between 0-7 */
    if (strlen(sign) == REG_LEN && sign[0] == 'r')
    {
        int number = atoi(++sign); /* Changes the ascii value to int */
        for (i = MIN_REGISTER_NUM; i <= MAX_REGISTER_NUM; i++)
            if (number == i)
                return number;
    }
    else {
        return 0;
    }
}

/* This function finds an index of a string in an array of strings */
int find_index(char* sign, const char* arr[], int n) {

    int i;
    for (i = 0; i < n; i++)
        if (strcmp(sign, arr[i]) == 0) {
            return i;
        }
    return NO_MATCH;
}

/* Check if a token matches a directive name */
int find_guidance(char* sign) {

    if (sign == NULL || *sign != '.') {
        return NO_MATCH;
    }/* We have 4 guidance commands, check index number in enum */
    return find_index(sign, guidance, 4);
}

/* Check if a sign matches a command name */
int find_command(char* sign) {

    int enum_index;
    int sign_len = strlen(sign);
    if (sign_len > 4 || sign_len < 3)/* A command is between 3 and 4 chars */
        return NO_MATCH;
    else
        enum_index = find_index(sign, commands, 16); /* We have a total of 16 commands */
    return enum_index;
}

/* Copies the next sign to the destination */
char* next_list_sign(char* dest, char* line) {

    char* temp = dest;

    if (end_of_line(line)) /* If the given line is empty, copy an empty sign */
    {
        dest[0] = '\0';
        return NULL;
    }

    if (isspace(*line)) /* If there are spaces in the beginning of the sign, skip them */
        line = skip_spaces(line);

    if (*line == ',') /* A comma deserves a separate, single-character sign */
    {
        strcpy(dest, ",");
        return ++line;
    }

    /* Manually copying sign until a ',', whitespace or end of line */
    while (!end_of_line(line) && *line != ',' && !isspace(*line))
    {
        *temp = *line;
        temp++;
        line++;
    }
    *temp = '\0'; /* Adding end of string */

    return line;
}

/* Checks if number, also with +/- signs */
boolean is_number(char* seq) {

    if (end_of_line(seq)) 
        return FALSE;
    if (*seq == '+' || *seq == '-') {/* A number can contain a plus or minus sign */
        seq++;
        if (!isdigit(*seq++)) 
            return FALSE; /* But not only a sign */
    }

    /* Check that the rest of the token is made of digits */
    while (!end_of_line(seq)) {
        if (!isdigit(*seq++)) return FALSE;
    }
    return TRUE;
}

/* Copies next string into destination */
char* next_sign_string(char* dest, char* line) {

    char temp[MAX_INPUT];
    line = next_list_sign(dest, line);/* Creat function */
    if (*dest != '"') 
        return line;
    while (!end_of_line(line) && dest[strlen(dest) - 1] != '"')
    {
        line = next_list_sign(temp, line);
        if (line) strcat(dest, temp);
    }
    return line;
}

/* This function checks if a given sequence is a valid string (wrapped with "") */
boolean is_string(char* string) {

    if (string == NULL)
        return FALSE;

    if (*string == '"') /* Starts with " */
        string++;
    else
        return FALSE;

    while (*string && *string != '"') { /* Goes until end of string */
        string++;
    }

    if (*string != '"') /* A string must end with " */
        return FALSE;

    string++;
    if (*string != '\0') /* String token must end after the ending " */
        return FALSE;

    return TRUE; /* It's a string! */
}

/* This function encodes a given string to data */
void write_string_to_data(char* str) {

    while (!end_of_line(str))
    {
        data[dc++] = (unsigned int)*str; /* Inserting a character to data array */
        str++;
    }
    data[dc++] = '\0'; /* Insert a null character to data */
}

/* This function inserts a given word to instructions memory */
void encode_to_instructions(unsigned int word) {

    instructions[ic++] = word;
}

/***************ERRORS****************/

/* Checks what error was encoutered and prints the error and line number */
void write_error(int line_number) {
  
    fprintf(stderr, "ERROR (line %d): ", line_number);

    switch (error)
    {
    case INVALID_SYNTAX:
        fprintf(stderr, "first non-blank character must be a letter or a dot.\n");

        break;

    case LABEL_ALREADY_EXISTS:
        fprintf(stderr, "label already exists.\n");

        break;

    case LABEL_TOO_LONG:
        fprintf(stderr, "label is too long (MAX_LABEL_LENGTH: %d).\n", MAX_LABEL);

        break;

    case LABEL_INVALID_FIRST_CHAR:
        fprintf(stderr, "label must start with an alphanumeric character.\n");

        break;

    case LABEL_CANT_BE_COMMAND:
        fprintf(stderr, "label can't have the same name as a command.\n");

        break;

    case LABEL_CANT_BE_REGISTER:
        fprintf(stderr, "label can't have the same name as a register.\n");

        break;

    case INVALID_LABEL_LINE:
        fprintf(stderr, "label must be followed by a command or guidance.\n");

        break;

    case NO_PARAMETER_AVAILABLE:
        fprintf(stderr, "guidance must have parameters.\n");

        break;

    case GUIDANCE_INVALID_NUM_PARAMS:
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

    case EXTERN_NO_LABEL:
        fprintf(stderr, ".extern directive must be followed by a label.\n");

        break;

    case EXTERN_INVALID_LABEL:
        fprintf(stderr, ".extern directive received an invalid label.\n");

        break;

    case EXTERN_TOO_MANY_OPERANDS:
        fprintf(stderr, ".extern must only have one operand that is a label.\n");

        break;

    case MISSING_SYNTAX:
        fprintf(stderr, "invalid command or guidance.\n");

        break;

    case COMMAND_UNEXPECTED_CHAR:
        fprintf(stderr, "invalid syntax of a command.\n");

        break;

    case COMMAND_TOO_MANY_OPERANDS:
        fprintf(stderr, "command can't have more than 2 operands.\n");

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

        break;

    case NOT_A_VALID_REG:
        fprintf(stderr, "the register number is invalid.\n");

        break;

    case WRONG_SYNTAX_FOR_METHOD_RELATIVE:
        fprintf(stderr, "wrong syntax used for relative method.\n");

        break;

    case NO_LABEL_FOUND:
        fprintf(stderr, "no such label found in file for this method.\n");

        break;

    case NOT_A_LABEL:
        fprintf(stderr, "incorrect label syntax.\n");

        break;
    }

}
