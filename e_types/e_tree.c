#include "e_tree.h"

e_tree_t * e_tree_create (int (* compare_function) (void *, void *))
{

	e_tree_t * tree;
	
	tree = (e_tree_t *) malloc(sizeof(e_tree_t));
	
	tree->compare_function = compare_function;
	tree->size = 0;
	tree->tree = NULL;
	
	return tree;

}



void e_tree_destroy_item (e_tree_item_t * tree_item)
{

	if (tree_item->left != NULL)
		e_tree_destroy_item(tree_item->left);
	if (tree_item->right != NULL)
		e_tree_destroy_item(tree_item->right);
	free(tree_item->data);
	free(tree_item);

}



void e_tree_destroy (e_tree_t * tree)
{

	e_tree_destroy_item(tree->tree);
	free(tree);

}



e_tree_item_t * e_tree_skew (e_tree_item_t * tree_item)
{

	e_tree_item_t * l;

	if (tree_item == NULL)
		return NULL;
	
	if (tree_item->left == NULL)
		return tree_item;
	
	if (tree_item->left->level == tree_item->level)
	{
		l = tree_item->left;
		tree_item->left = l->right;
		l->right = tree_item;
		return l;
	}
	
	return tree_item;
	
}



e_tree_item_t * e_tree_split (e_tree_item_t * tree_item)
{

	e_tree_item_t * r;

	if (tree_item == NULL)
		return NULL;
	
	if (tree_item->right == NULL)
		return tree_item;
	
	if (tree_item->right->right == NULL)
		return tree_item;
	
	if (tree_item->level == tree_item->right->right->level)
	{
		r = tree_item->right;
		tree_item->right = r->left;
		r->left = tree_item;
		r->level++;
		return r;
	}
	
	return tree_item;
	
}



e_tree_item_t * e_tree_insert_item (e_tree_t * tree, e_tree_item_t * tree_item,
                                    e_tree_item_t * new_tree_item)
{

	if (tree_item == NULL)
		return new_tree_item;
	if (tree->compare_function(tree_item->data, new_tree_item->data) < 0)
	{
	    tree_item->left = e_tree_insert_item(tree, tree_item->left,
	                                         new_tree_item);
	}
	else if (tree->compare_function(tree_item->data, new_tree_item->data) > 0)
	{
		tree_item->right = e_tree_insert_item(tree, tree_item->right,
		                                      new_tree_item);
	}

	tree_item = e_tree_skew(tree_item);
	tree_item = e_tree_split(tree_item);
	
	return tree_item;

}



void e_tree_insert (e_tree_t * tree, void * data, int size)
{

	e_tree_item_t * tree_item;
	
	tree_item = (e_tree_item_t *) malloc(sizeof(e_tree_item_t));
	tree_item->size = size;
	tree_item->level = 0;
	tree_item->data = (void *) malloc(size);
	memcpy(tree_item->data, data, size);
	tree_item->left = NULL;
	tree_item->right = NULL;
	
	tree->tree = e_tree_insert_item(tree, tree->tree, tree_item);
	
}



e_tree_item_t * e_tree_item_search (e_tree_t * tree, e_tree_item_t * tree_item,
                                    void * data)
{

	if (tree_item == NULL)
		return NULL;

	if (tree->compare_function(tree_item->data, data) < 0)
		return e_tree_item_search(tree, tree_item->left, data);
	if (tree->compare_function(tree_item->data, data) > 0)
		return e_tree_item_search(tree, tree_item->right, data);
	else
		return tree_item;

}



void * e_tree_search (e_tree_t * tree, void * data)
{

	e_tree_item_t * tree_item;
	
	tree_item = e_tree_item_search(tree, tree->tree, data);
	
	if (tree_item == NULL)
		return NULL;
	else
		return tree_item->data;

}


		
