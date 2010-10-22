#include "e_list.h"

e_list_t * e_list_create ()
{
	
	e_list_t * list;
	
	list = (e_list_t *) malloc (sizeof(e_list_t));
	list->first = NULL;
	list->last = NULL;
	list->iterator = NULL;

	return list;

}



void e_list_destroy (e_list_t * list)
{

	e_list_item_t * next;
	e_list_item_t * this;
	
	next = list->first;
	while (next != NULL)
	{
		this = next;
		next = next->next;
		free(this);
	}
	
	free(list);

}



int e_list_insert (e_list_t * list, void * data, int size)
{

	e_list_item_t * next;
	
	next = (e_list_item_t *) malloc(sizeof(e_list_item_t));
	if (next == NULL)
		return -1;
		
	next->data = (void *) malloc(size);
	if (next->data == NULL)
	{
		free(next);
		return -1;
	}
	
	memcpy(next->data, data, size);
	next->size = size;
	next->next = NULL;

	if (list->first == NULL)
	{
		list->first = next;
		list->last = next;
	}
	else
	{
		list->last->next = next;
		list->last = next;
	}
	
	list->size++;
	
	return 0;

}



int e_list_insert_front (e_list_t * list, void * data, int size)
{

	e_list_item_t * item;
	
	item = (e_list_item_t *) malloc(sizeof(e_list_item_t));
	if (item == NULL)
		return -1;
		
	item->data = (void *) malloc(size);
	if (item->data == NULL)
	{
		free(item);
		return -1;
	}
	
	memcpy(item->data, data, size);
	item->next = list->first;
	list->first = item;
	if (list->last == NULL)
		list->last = item;
	
	list->size++;
	
	return 0;
	
}
	
	
	

int e_list_pop_front (e_list_t * list)
{

	e_list_item_t * item;
	
	if (list->first == NULL)
		return 0;
	
	item = list->first;
	
	if (item->next != NULL)
		list->first = item->next;
	else
	{
		list->first = NULL;
		list->last = NULL;
	}
	
	free(item->data);
	free(item);
	
	list->size--;
	
	return 0;
	
}



int e_list_save (e_list_t * list, char * filename)
{

	e_list_item_t * next;
	FILE * fh;
	
	fh = fopen(filename, "w");
	if (fh == NULL)
		return -1;
	
	next = list->first;
	
	while (next != NULL)
	{
		fwrite(&(next->size), sizeof(int), 1, fh);
		fwrite(next->data, next->size, 1, fh);
		next = next->next;
	}
	
	fclose(fh);
	
	return 0;

}



e_list_t * e_list_open (char * filename)
{

	int size;
	void * data;
	e_list_t * list;
	FILE * fh;
	
	fh = fopen(filename, "r");
	if (fh == NULL)
		return NULL;
		
	list = e_list_create();

	while (feof(fh) == 0)
	{
		if (fread(&size, sizeof(int), 1, fh) == 0)
			break;
		data = (void *) malloc(size);
		if (fread(data, size, 1, fh) == 0)
		{
			fclose(fh);
			free(data);
			e_list_destroy(list);
			return NULL;
		}
		e_list_insert(list, data, size);
		free(data);
	}
	
	fclose(fh);
	
	return list;

}



e_list_item_t * e_list_sorter (e_list_item_t * items,
                               int (* comparison_function) (void *, void *))
{

	int compare;
	e_list_item_t * a, * b;
	e_list_item_t * result, * next;
	
	// check real quick to make sure we have at least 2 elements
	if (items == NULL)
		return NULL;
	else if (items->next == NULL)
		return items;

	// find midpoint
	a = items;
	b = items->next;
	while (b != NULL)
	{
		b = b->next;
		if (b == NULL)
			break;
		a = a->next;
		b = b->next;
	}
	
	// set b to second half
	b = a->next;
	// terminate the first half
	a->next = NULL;
	// set a to first half
	a = items;

	a = e_list_sorter(a, comparison_function);
	b = e_list_sorter(b, comparison_function);
	
	if (comparison_function(a->data, b->data) < 0)
	{
		result = a;
		a = a->next;
	}
	else
	{
		result = b;
		b = b->next;
	}
	
	next = result;
	
	while (1)
	{
		if (a == NULL)
		{
			if (b == NULL)
				break;
			next->next = b;
			next = next->next;
			b = b->next;
		}
		else if (b == NULL)
		{
			if (a == NULL)
				break;
			next->next = a;
			next = next->next;
			a = a->next;
		}
		else if (comparison_function(a->data, b->data) < 0)
		{
			next->next = a;
			next = next->next;
			a = a->next;
		}
		else
		{
			next->next = b;
			next = next->next;
			b = b->next;
		}
	}
	
	next->next = NULL;
	
	return result;
	
}

		

int e_list_sort (e_list_t * list, int (* comparison_function) (void *, void *))
{

	e_list_item_t * next;

	list->first = e_list_sorter(list->first, comparison_function);
	
	if (list->size <= 1)
		list->last = list->first;
	else
	{
		// find last
		next = list->first;
		while (next->next != NULL)
			next = next->next;
		list->last = next;
	}
	
	return 0;

}
		


void * e_list_search (e_list_t * list, int (* comparison_function)
                     (void *, void *), void * data)
{
	
	e_list_item_t * next;
	
	next = list->first;
	
	while (next != NULL)
	{
		if (comparison_function(next->data, data) == 0)
			return next->data;
		next = next->next;
	}
	
	return NULL;

}



int e_list_iterator_reset (e_list_t * list)
{

	list->iterator = list->first;

}



void * e_list_iterator_next (e_list_t * list)
{

	void * data;
	
	if (list->iterator != NULL)
	{
		data = list->iterator->data;
		list->iterator = list->iterator->next;
	}
	else
		data = NULL;
	
	return data;

}
