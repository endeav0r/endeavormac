#include "e_convert.h"



void e_convert_tree_item_to_list (e_tree_item_t * tree_item, e_list_t * list)
{

	if (tree_item->left != NULL)
		e_convert_tree_item_to_list(tree_item->left, list);
	
	e_list_insert(list, tree_item->data, tree_item->size);
	
	if (tree_item->right != NULL)
		e_convert_tree_item_to_list(tree_item->right, list);

}



e_list_t * e_convert_tree_to_list (e_tree_t * tree)
{

	e_list_t * list;
	
	list = e_list_create();
	
	e_convert_tree_item_to_list (tree->tree, list);
	
	return list;

}



e_tree_t * e_convert_list_to_tree (e_list_t * list,
                                   int (* compare_function) (void *, void *))
{

	e_list_item_t * list_item;
	e_tree_t * tree;
	
	tree = e_tree_create(compare_function);
	
	list_item = list->first;
	
	while (list_item != NULL)
	{
		e_tree_insert(tree, list_item->data, list_item->size);
		list_item = list_item->next;
	}
	
	return tree;

}
