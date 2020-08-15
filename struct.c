#include "main.h"
#include "common.h"
#include "external_vars.h"

/* In this file we have all functions regarding the label table */

/*This function adds a label to the label table*/
labelPtr add_label(labelPtr* lptr, char* name, unsigned int address, boolean external, ...) {

	va_list p;

	labelPtr t = *lptr;
	labelPtr temp; /* Auxiliary variable to store the info of the label and add to the list */

	if (is_existing_label(*lptr, name))
	{
		error = LABEL_ALREADY_EXISTS;
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

/* This functions sets the correct address for the label (memory starts at 100) */
void offset_address(labelPtr l, int num, boolean is_data) {
	while (l)
	{
		if (!(l->external) && (is_data ^ (l->inActionStatement)))
		{
			l->address += num;
		}
		l = l->next;
	}
}

/* This function sets the bool var of an existing entry to true */
int make_entry(labelPtr l, char* name) {
	labelPtr label = get_label(l, name);
	if (label != NULL)
	{
		if (label->external)
		{
			error = ENTRY_CANT_BE_EXTERN; /*add to error list*/
			return FALSE;
		}
		label->entry = TRUE;
		entry_exists = TRUE; /* Global variable that holds that there was at least one entry in the program */
		return TRUE;
	}
	else
		error = ENTRY_LABEL_DOES_NOT_EXIST;/*add to error list*/
	return FALSE;
}

/* This function gets the label address */
unsigned int get_label_address(labelPtr l, char* name) {
	labelPtr label = get_label(l, name);
	if (label != NULL) {
		return (label->address);
	}
	else return FALSE;
}

/* This function checks if the label is external */
boolean is_external_label(labelPtr l, char* name) {
	labelPtr label = get_label(l, name);
	if (label != NULL) {
		return (label->external);
	}
	else return FALSE;
}

/* This function checks if the label already exists */
boolean is_existing_label(labelPtr l, char* name) {
	
	return (get_label(l, name) != NULL);
}

/* This function gets the label */
labelPtr get_label(labelPtr l, char* name) {
	
	while (l) {
		if (strcmp(l->name, name) == 0)
			return l;
		l = l->next;
	}
	return NULL;
}

/* This function frees the label list */
void free_label_table(labelPtr * lptr) {
		
	labelPtr temp;
		while (*lptr)
		{
			temp = *lptr;
			*lptr = (*lptr)->next;
			free(temp);
		}
}

/* This function delets label in the label list in case we entered a wrong label */
int delete_label(labelPtr * lptr, char* name) {
	
	labelPtr temp = *lptr;
		labelPtr prevtemp;
		while (temp) {
			if (strcmp(temp->name, name) == 0) {
				if (strcmp(temp->name, (*lptr)->name) == 0) {
					*lptr = (*lptr)->next;
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

