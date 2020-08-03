#include "struct.h"


labelPtr add_label() {

}


void offset_address(labelPtr l, char* name, boolean is_data) {

}

int make_entry(labelPtr l, char* name) {
	labelPtr label = get_label(l, name);
	if (label != NULL)
	{
		if (label->external)
		{
			err = ENTRY_CANT_BE_EXTERN;
			return FALSE;
		}
		label->entry = TRUE;
		entry_exists = TRUE; /* Global variable that holds that there was at least one entry in the program */
		return TRUE;
	}
	else
		err = ENTRY_LABEL_DOES_NOT_EXIST;
	return FALSE;
}

unsigned int get_label_address(labelPtr l, char* name) {
	labelPtr label = get_label(l, name);
	if (get_label != NULL) {
		return (l -> address);
	}
	else return FALSE;
}

boolean is_external_label(labelPtr l, char* name) {
	labelPtr label = get_label(l, name);
	if (get_label != NULL) {
		return (l -> external);
	}
	else return FALSE;
}

boolean is_existing_label() {
	
	return (get_label(l, name) != NULL);

}

labelPtr get_label(labelPtr l, char* name) {
	while (l) {
		if (strcmp(l->name, name) == 0) {
			return l;
		}
		else l = l->next;
	}
	return NULL;
}

int delet_label() {

}

void free_label_table() {

}

/*
void print_labels(labelPtr l) {
	while (l) {
		printf("\nname: %s, address: %d, external: %d", l->name, l->address, l->external);
		if (h->external == 0)
			printf(", is in action statement: %d -> ", l->inActionStatement);
		else
			printf(" -> ");
		l = l->next;
	}
	printf("*");
}
*/
