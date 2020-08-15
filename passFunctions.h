#ifndef passFunctions_h

#define passFunctions_h

#include "main.h"
#include "struct.h"

/*This file includes all functions from the  file passes*/

/***********Pass one fuctions***********/
void passOne(FILE* fp);
void line_pass_one(char* line);
int handle_guidance(int guidance_type, char* line);
int handle_data_guidance(char* line);
int handle_string_guidance(char* line);
int handle_extern_guidance(char* line);
int handle_command(int type, char* line);
void write_num_to_data(int num);
int detect_method(char* operand);
boolean command_accept_num_operands(int type, boolean first, boolean second);
boolean command_accept_methods(int type, int first_method, int second_method);
unsigned int build_first_word(int type, int is_first, int is_second, int first_method, int second_method, int first_register, int second_register);
int calculate_command_num_additional_words(int is_first, int is_second, int first_method, int second_method);
int num_words(int method);
int command_funct(int commands);
int find_new_type(int type);

/***********Pass two fuctions***********/
void passTwo(FILE* fp, char* filename);
void line_pass_two(char* line);
int creat_output_files(char* name);
FILE* open_file(char* filename, int type);
void creat_object_file(FILE* fp);
void creat_entry_file(FILE* fp);
void creat_external_file(FILE* fp);
void does_operand_exists(int ope, boolean* is_source, boolean* is_destination);
int handle_command_pass_two(int type, char* line);
int encode_additional_words(char* src, char* dest, boolean is_src, boolean is_dest, int src_method, int dest_method);
void encode_label(char* label);
void encode_label_relative(char* label);
void encode_num_immediate(char* operand);
void encode_additional_word(boolean is_dest, int method, char* operand);

#endif
