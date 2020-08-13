#ifndef common_H

#define common_H

#include "main.h"
#include "struct.h"

/* Here we declare all the fuctions in common.c, struct.c and externalStruct.c */

/* Functions from common.c */
char* create_file_name(char* original, int type);
int ignore_line(char* line);
int if_error();
char* skip_spaces(char* ch);
int end_of_line(char* line);
void copy_sign(char* destination, char* line);
unsigned int extract_bits(unsigned int word, int start, int end);
boolean is_label(char* sign, int colon);
char* next_sign(char* seq);
boolean is_register(char* sign);
int find_reg_number(char* sign);
int find_index(char* sign, const char* arr[], int n);
int find_guidence(char* sign);
int find_command(char* sign);
int find_command_funct(char* sign);
char* next_list_sign(char* dest, char* line);
boolean is_number(char* seq);
char* next_sign_string(char* dest, char* line);
boolean is_string(char* string);
void write_string_to_data(char* str);
void encode_to_instructions(unsigned char word[3]);
void write_error(int line_number);

/* Functions from struct.c */
labelPtr add_label(labelPtr* lptr, char* name, unsigned int address, boolean external, ...);
void offset_address(labelPtr l, int num, boolean is_data);
int make_entry(labelPtr l, char* name);
unsigned int get_label_address(labelPtr l, char* name);
boolean is_external_label(labelPtr l, char* name);
boolean is_existing_label(labelPtr l, char* name);
labelPtr get_label(labelPtr l, char* name);
void free_label_table(labelPtr* lptr);
int delete_label(labelPtr* lptr, char* name);
void print_labels(labelPtr l);

/* Functions from externalStruct.c */
extPtr add_ext(extPtr* lptr, char* name, unsigned int reference);
void free_ext(extPtr* lptr);
void print_ext(extPtr l);

#endif
