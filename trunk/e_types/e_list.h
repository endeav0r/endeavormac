#ifndef E_LIST_HEADER
#define E_LIST_HEADER

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct e_list_item_s
{
	int size; // in bytes of data
	void * data;
	struct e_list_item_s * next;
} e_list_item_t;

typedef struct e_list_s
{
	int size; // number of items in list
	e_list_item_t * first;
	e_list_item_t * last;
	e_list_item_t * iterator;
} e_list_t;



e_list_t * e_list_create  ();
void       e_list_destroy (e_list_t * list);

// returns 0 on success, -1 on error (well doesn't return anything yet)
int        e_list_insert  (e_list_t * list, void * data, int size);

// saves the list to filename
// returns 0 on success, -1 on failure
int        e_list_save (e_list_t * list, char * filename);

// opens a saved list from filename. returns a pointer to the list on success,
// or NULL on failure
e_list_t * e_list_open (char * filename);

// Sorts list with an in-place merge sort. Should not require any additional
// memory to sort.
// returns 0 on success, non-zero otherwise
int e_list_sort (e_list_t * list, int (* comparison_function) (void *, void *));

// used internally to sort lists
e_list_item_t * e_list_sorter (e_list_item_t * items,
                               int (* comparison_function) (void *, void *));

/*
* iterates over the list (not reseting the iterator) checking each item with
* comparison_function against data.
* comparison function must return 0 when the two items match, and non-zero
* otherwise
* returns a pointer to the data when found, NULL when not found
*/
void * e_list_search (e_list_t * list, int (* comparison_function)
                     (void *, void *), void * data);

int    e_list_iterator_reset (e_list_t * list);
void * e_list_iterator_next  (e_list_t * list);

#endif
