#include <stdio.h>
#include <stdlib.h>

#include "e_tree.h"
#include "e_list.h"
#include "e_convert.h"


int comparison_function (void * one, void * two)
{
	return *((int *) two) - *((int *) one);
}

int comparison_function_2 (void * one, void * two)
{
	return *((int *) one) - *((int *) two);
}

int main ()
{

	int number;
	int i;
	int * number_ptr;
	
	e_tree_t * tree;
	e_list_t * list;
	
	tree = e_tree_create (comparison_function);
	
	for (i = 0; i < 20; i++)
	{
		number = rand() % 50;
		e_tree_insert(tree, &number, sizeof(int));
	}
	
	list = e_convert_tree_to_list(tree);
	
	e_list_iterator_reset(list);
	while ((number_ptr = e_list_iterator_next(list)) != NULL)
		printf("%d ", *number_ptr);
	printf("\n");
	
	e_list_sort(list, comparison_function);
	
	e_list_iterator_reset(list);
	while ((number_ptr = e_list_iterator_next(list)) != NULL)
		printf("%d ", *number_ptr);
	printf("\n");
	
	number = 5;
	if (e_list_insert_front(list, &number, sizeof(int)))
		printf("error inserting item to front of list\n");
	
	e_list_iterator_reset(list);
	while ((number_ptr = e_list_iterator_next(list)) != NULL)
		printf("%d ", *number_ptr);
	printf("\n");
	
	e_list_pop_front(list);
	
	e_list_iterator_reset(list);
	while ((number_ptr = e_list_iterator_next(list)) != NULL)
		printf("%d ", *number_ptr);
	printf("\n");
	
	e_tree_destroy(tree);
	e_list_destroy(list);
	
	return 0;
	
}		
