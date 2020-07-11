#include "assembler.h"

void passOne() {
	int *DC = 0;
	int *IC = 100;
	int errCount = 0;
	char word[MAX_INPUT];
	char nextWord[MAX_INPUT];
	while (fgets(line, MAX_INPUT, fd) != NULL) {
		errCount += 1;
		if (line[0] == ";") {
			break;
		}
		else {
			strtok_s(word, " ", nextWord);
			if (strlen(word) == 0) {
				break;
			}
			else if (word[0] == ".") {
				if (strcmp(word, ".data")) {
					isData();
				}
				else if ((strcmp(word, ".string"))) {
					isString();
				}
				else if ((strcmp(word, ".entry"))) {
					isEntry();
				}
				else if ((strcmp(word, ".extern"))) {
					isExtern();
				}
				else {
					fprintf(stderr, "\nNo such guide found\n");

				}
			}
		}

	}

}
