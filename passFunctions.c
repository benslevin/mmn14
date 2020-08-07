
/*This file includes all the functions from the passes*/




/*Pass one fuctions*/

void passOne(FILE* fp);
void  line_pass_one(char* line);
int handle_guidance(int guidance_type, char* line);
int handle_data_guidance(char* line);
int handle_string_guidance(char* line);
int handle_extern_guidance(char* line);
int handle_command(int type, char* line);
void write_num_to_data(int num);
int detect_method(char* operand);

/*Pass two fuctions*/

void passTwo(FILE* fp, char* filename);
void line_pass_two(char* line);
void creat_output_files(char* name);
FILE* open_file(char* filename, int type);
void creat_object_file();
void creat_entry_file();
void creat_external_file();
void does_operand_exists(int ope, boolean* is_source, boolean* is_destination);
int handle_command_second_pass(int type, char* line);
int encode_additional_words(char* src, char* dest, boolean is_src, boolean is_dest, int src_method,	int dest_method);
int encode_additional_words(char* src, char* dest, boolean is_src, boolean is_dest, int src_method, int dest_method);
unsigned int build_register_word(boolean is_dest, char* reg);
void encode_label(char* label);
void encode_additional_word(boolean is_dest, int method, char* operand);