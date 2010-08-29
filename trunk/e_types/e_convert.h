#ifndef e_convert_HEADER
#define e_convert_HEADER

#include "e_list.h"
#include "e_tree.h"

void       e_convert_tree_item_to_list (e_tree_item_t * tree_item,
                                        e_list_t * list);
e_list_t * e_convert_tree_to_list      (e_tree_t * tree);

e_tree_t * e_convert_list_to_tree (e_list_t * list,
                                   int (* compare_function) (void *, void *));

#endif
