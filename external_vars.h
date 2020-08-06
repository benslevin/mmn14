
#include "structs.h"
#include "main.h"

/*#define MACHINE_RAM 2000*/

extern int ic, dc;
extern int error;
extern boolean error_exist;
extern labelPtr symbols_table;
extern extPtr ext_list;
extern const char base24[24];
extern const char* commands[];
extern const char* directives[];
extern boolean entry_exists, extern_exists;