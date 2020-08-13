#ifndef external_vars_h

#define external_vars_h


#include "main.h"
#include "struct.h"

extern int ic, dc;
extern int error;
extern boolean error_exist;
extern labelPtr symbols_table;
extern extPtr ext_list;
extern const char* commands[];
extern const char* guidance[];
extern boolean entry_exists, extern_exists;

#endif
