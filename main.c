#include "main.h"


reset_falgs() {

}


int main(int argc, char* argv[]){

    int i;
    FILE* fp;
    if (argc < 2) {
        fprintf(stdout, "No argument found, please enter a file name to process\n");
        exit(1);
    }
    else {
        for (i = 1; i < argc; i++) {
            createFileName(argv[i]);
            if ((fp = fopen(argv[i], "r")) == NULL) {
                fprintf(stderr, "\nAn error occured while opening the file: %s\n", fileName);
            }
            else {
                if (feof(fp) == 1)/* not sure if we need to see if the file is empty here of after each char in the passOne function ep.11 1:00*/
                    fprintf(stderr, "\nThe file is empty\n");
                else {
                    fprintf(stdout, "*******Started working on file: %s*******", fileName);

                    reset_flags();
                    passOne();/*here we can add a while loop to see if teh feof flag is on*/

                    if (err == 0) {
                        rewind(fp);
                        passTwo();
                    }
                    else {
                        fprintf(stdout, "Errors found in file: %s, stoped working on file", fileName);
                    }
                    
                    fprintf(stdout, "*******Finished working on file: %s*******", fileName);
                    fclose(fp);
                }
            }
        }
    }

    return 0;
}
