#include "main.h"

/*********Pass one functions**********/
void reset_flags() {

}


/*********Pass one functions**********/

void error() {

}

void read_line() {

}

void ignore_line(char line) {
	int i = 0;
	while (line[i] == '\t' || (isspace(line[i]))) {
		i++;
	}
	if (line[i] == '\n') {
		return
	}
}

void error_exist() {

}

void write_error() {

}

