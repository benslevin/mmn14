#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdarg.h>

#define MAX_INPUT 81
#define MAX_LABLE 32
#define MAX_CHAR 8
#define DEFAULT_IC 100

char filename[MAX_INPUT];
char line[MAX_INPUT];
FILE* fd;
