#include "main.h"
#include "external_vars.h"
#include "passFunctions.h"
#include "common.h"

/* This is the main file of the program */

/* List of avaiable commands */
const char* commands[] = { "mov", "cmp", "add", "sub", "lea", "clr", "not", "inc", "dec", "jmp", "bne","jsr", "red", "prn", "rts", "stop" };

/* List of avaiable guidance */
const char* guidance[] = { ".data", ".string", ".entry", ".extern" };

/* Declaring global vars (extern vars) */
unsigned int data[MAX_WORDS];
unsigned int instructions[MAX_WORDS];
int ic;
int dc;
int error;
labelPtr symbols_table;
extPtr ext_list;
boolean entry_exists, extern_exists, error_exist;

/* The fucnction that resets all flags for next file */
void reset_flags() {
    symbols_table = NULL;
    ext_list = NULL;

    entry_exists = FALSE;
    extern_exists = FALSE;
    error_exist = FALSE;
}

/* The main function of the program */
int main(int argc, char* argv[]) {

    int i;
    char* fileName = NULL;
    FILE* fp;
    if (argc < 2) {
        fprintf(stdout, "No argument found, please enter a file name to process\n");
        exit(1);
    }
    else {
        for (i = 1; i < argc; i++) {
            fileName = create_file_name(argv[i], FILE_INPUT); /* Uses the file name given in the command line */
            if ((fp = fopen(fileName, "r")) == NULL) { /* Open the file with reading permision */
                fprintf(stderr, "\nAn error occured while opening the file: %s\n", fileName);
            }
            else {
                if (feof(fp) == 1)/* Checks if EOF reached */
                    fprintf(stderr, "\nThe file is empty\n");
                else {
                    fprintf(stdout, "*******Started working on file: %s*******\n", fileName);

                    reset_flags();/* Resets all flags for the next file */
                    passOne(fp);

                    if (error == 0) {/* If there are no errors, continue with second pass */
                        rewind(fp);/*Starts the second pass from the start of the file*/
                        passTwo(fp, argv[i]);
                    }
                    else {/* First pass contains errors, stop processing the file */
                        fprintf(stdout, "Errors found in file: %s, stoped working on file\n", fileName);
                    }

                    fprintf(stdout, "*******Finished working on file: %s*******\n", fileName);
                    fclose(fp); /* Close file */
                }
            }
        }
    }
    return 0;
}

