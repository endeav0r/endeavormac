#ifndef e_tree_HEADER
#define e_tree_HEADER

#include <stdlib.h>
#include <string.h>

#define DEBUG_E_TREE 1

#if DEBUG_E_TREE == 1
	#include <stdio.h>
#endif

typedef struct e_tree_item_s
{
	int size; // size of data in bytes
	int level;
	void * data;
	struct e_tree_item_s * left;
	struct e_tree_item_s * right;
} e_tree_item_t;

typedef struct e_tree_s
{
	int (* compare_function) (void *, void *);
	int size; // number of items in tree
	e_tree_item_t * tree;
	e_tree_item_t * iterator;
} e_tree_t;



e_tree_t * e_tree_create  (int (* compare_function) (void *, void *));
void       e_tree_destroy (e_tree_t * tree);

e_tree_item_t * e_tree_skew        (e_tree_item_t * tree_item);
e_tree_item_t * e_tree_split       (e_tree_item_t * tree_item);
e_tree_item_t * e_tree_insert_item (e_tree_t * tree, e_tree_item_t * tree_item,
                                    e_tree_item_t * new_tree_item);
void            e_tree_insert      (e_tree_t * tree, void * data, int size);

e_tree_item_t * e_tree_item_search (e_tree_t * tree, e_tree_item_t * tree_item,
                                    void * data);
// Searches tree for an element that matches data based on comparison_function.
// Returns pointer to the data of the element if found, NULL otherwise.
void *          e_tree_search      (e_tree_t * tree, void * data);

#endif
