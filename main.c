#include "assembler.h"

int main(int argc, char* argv[])
{
    if (argc < 2) {
        printf("No argument found, please enter a file name to process\n");
        exit(1);
    }
    else {
        for (int i = 1; i < argc; i++) {
            strcpy(filename, argv[i]);
            strcat(filename, ".as");
            if ((fd = fopen(argv[i], "r")) == NULL)
                fprintf(stderr, "\nAn error occured while opening the file\n");
            exit(0)
            else {
                if (feof(fd) == 1)/* not sure if we need to see if the file is empty here of after each char in the passOne function ep.11 1:00*/
                    fprintf(stderr, "\nThe file is empty\n");
                else {
                    passOne();/*here we can add a while loop to see if teh feof flag is on*/
                    passTwo();
                    fclose(fd);
                }
            }
        }
    }
    exit(0);



}