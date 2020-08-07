#include "struct.h"


labelPtr add_label(labelPtr* lptr, char* name, unsigned int address, boolean external, ...) {

	va_list p;

	labelPtr t = *lptr;
	labelPtr temp; /* Auxiliary variable to store the info of the label and add to the list */

	if (is_existing_label(*lptr, name))
	{
		err = LABEL_ALREADY_EXISTS;
		return NULL;
	}
	temp = (labelPtr)malloc(sizeof(Labels));
	if (!temp) /*if we couldn't allocate memory to temp then print an error massage and exit the program*/
	{
		printf("\nerror, cannot allocate memory\n");
		exit(ERROR);
	}

	/* Storing the info of the label in temp */
	strcpy(temp->name, name);
	temp->entry = FALSE;
	temp->address = address;
	temp->external = external;

	if (!external) /* An external label can't be in an action statement */
	{
		va_start(p, external);
		temp->inActionStatement = va_arg(p, boolean);
	}
	else
	{
		extern_exists = TRUE;
	}

	/* If the list is empty then we set the head of the list to be temp */
	if (!(*lptr))
	{
		*lptr = temp;
		temp->next = NULL;
		return temp;
	}

	/* Setting a pointer to go over the list until he points on the last label and then stting temp to be the new last label */
	while (t->next != NULL)
		t = t->next;
	temp->next = NULL;
	t->next = temp;

	va_end(p);
	return temp;
}


void offset_address(labelPtr l, char* name, boolean is_data) {

}

int make_entry(labelPtr l, char* name) {
	labelPtr label = get_label(l, name);
	if (label != NULL)
	{
		if (label->external)
		{
			err = ENTRY_CANT_BE_EXTERN; /*add to error list*/
			return FALSE;
		}
		label->entry = TRUE;
		entry_exists = TRUE; /* Global variable that holds that there was at least one entry in the program */
		return TRUE;
	}
	else
		err = ENTRY_LABEL_DOES_NOT_EXIST;/*add to error list*/
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

boolean is_existing_label(labelPtr l, char* name) {
	
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


/* Free the label list by going over each label and free it */
void free_label_table(labelPtr *lptr) {

	labelPtr temp;
	while (*lptr)
	{
		temp = *lptr;
		*hptr = (*lptr) -> next;
		free(temp);
	}
}

int delete_label(labelPtr* lptr, char* name)
{
	/* Goes over the label list and checking if a label by a given name is in the list if it is then deletes it by
	free its space and change the previous label's pointer to point to the next label */
	labelPtr temp = *lptr;
	labelPtr prevtemp;
	while (temp) {
		if (strcmp(temp->name, name) == 0) {
			if (strcmp(temp->name, (*lptr)->name) == 0) {
				*hptr = (*lptr)->next;
				free(temp);
			}
			else {
				prevtemp->next = temp->next;
				free(temp);
			}
			return 1;
		}
		prevtemp = temp;
		temp = temp->next;
	}
	return 0;

}


void print_labels(labelPtr l) {
	while (l) {
		printf("\nname: %s, address: %d, external: %d", l->name, l->address, l->external);
		if (l->external == 0)
			printf(", is in action statement: %d -> ", l->inActionStatement);
		else
			printf(" -> ");
		l = l->next;
	}
	printf("*");
}

