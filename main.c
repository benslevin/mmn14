#include "main.h"





/* List of avaiable commands */
const char* commands[] = { "mov", "cmp", "add", "sub", "lea", "clr", "not", "inc", "dec", "jmp", "bne","jsr", "red", "prn", "rts", "stop"};

/* List of avaiable guidance */
const char* guidence[] = {".data", ".string", ".entry", ".extern"};

/* The fucnction that resets all flags for next file */
void reset_falgs() {
    symbols_table = NULL;
    ext_list = NULL;

    entry_exists = FALSE;
    extern_exists = FALSE;
    error_exist = FALSE;
}

/* The main function of the program */
int main(int argc, char* argv[]){

    int i;
    FILE* fp;
    if (argc < 2) {
        fprintf(stdout, "No argument found, please enter a file name to process\n");
        exit(1);
    }
    else {
        for (i = 1; i < argc; i++) {
            create_file_name(argv[i]); /* Uses the file name given in the command line */
            if ((fp = fopen(argv[i], "r")) == NULL) { /* Open the file with reading permision */
                fprintf(stderr, "\nAn error occured while opening the file: %s\n", fileName);
            }
            else {
                if (feof(fp) == 1)/* Checks if EOF reached */
                    fprintf(stderr, "\nThe file is empty\n");
                else {
                    fprintf(stdout, "*******Started working on file: %s*******", fileName);

                    reset_flags();/* Resets all flags for the next file */
                    passOne(fp);

                    if (err == 0) {/* If there are no errors, continue with second pass */
                        rewind(fp);/*Starts the second pass from the start of the file*/
                        passTwo(fp, argv[i]);
                    }
                    else {/* First pass contains errors, stop processing the file */
                        fprintf(stdout, "Errors found in file: %s, stoped working on file", fileName);
                    }
                    
                    fprintf(stdout, "*******Finished working on file: %s*******", fileName);
                    fclose(fp); /* Close file */
                }
            }
        }
    }
    return 0;
}
