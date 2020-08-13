#include "main.h"
#include "struct.h"



/* This function adds a node to the end of the list */
extPtr add_ext(extPtr* lptr, char* name, unsigned int reference)
{
    extPtr t = *lptr;
    extPtr temp;

    temp = (extPtr)malloc(sizeof(ext));
    if (!temp)
    {
        printf("\nerror, cannot allocate memory\n");
        exit(1);
    }

    temp->address = reference;
    strcpy(temp->name, name);

    if (!(*lptr)) /* If the list is empty */
    {
        *lptr = temp;
        temp->next = temp;
        temp->prev = temp;
        return temp;
    }


    ((*lptr)->prev)->next = temp;
    temp->next = t;
    temp->prev = t->prev;
    (*lptr)->prev = temp;

    return temp;
}

/* This function frees the allocated memory for the list */
void free_ext(extPtr* lptr)
{
    /* Frees the extern list by going over each extern node and free it*/
    extPtr temp = *lptr;
    if (temp) {
        unsigned int last_reference = ((*lptr)->prev)->address;
        unsigned int reference = 0;
        do {
            temp = *lptr;
            reference = temp->address;
            *lptr = (*lptr)->next;
            free(temp);
        } while (reference != last_reference);
    }
}

/* This function prints the ext list */
void print_ext(extPtr l)
{
    extPtr orig = l;
    do
    {
        printf("\nname: %s, reference: %d - >", l->name, l->address);
        l = l->next;
    } while (l->address != orig->address);
    printf("*\n");
}

