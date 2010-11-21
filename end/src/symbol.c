#include "symbol.h"
#include "end.h"



end_symbol_table_t * end_symbol_table_create ()
{
	end_symbol_table_t * symbol_table;
	symbol_table = (end_symbol_table_t *) malloc(sizeof(end_symbol_table_t));
	symbol_table->top = NULL;
	symbol_table->size = 0;
	return symbol_table;
}



void end_symbol_table_node_destroy (end_symbol_node_t * node)
{
	if (node->left != NULL)
		end_symbol_table_node_destroy(node->left);
	if (node->right != NULL)
		end_symbol_table_node_destroy(node->right);
	end_symbol_destroy(node->symbol);
}



void end_symbol_table_destroy (end_symbol_table_t * symbol_table)
{
	if (symbol_table->top != NULL)
		end_symbol_table_node_destroy(symbol_table->top);
	free(symbol_table);
}



void end_symbol_node_delete (end_symbol_table_t * symbol_table,
                             end_symbol_node_t * node)
{

	// free up memory
	if (node->symbol != NULL)
		end_symbol_destroy(node->symbol);

	// if we have at least one child missing
	if ((node->left == NULL) || (node->right == NULL))
	{
        #ifdef SYMBOL_DEBUG
            printf("at least one child missing - ");
        #endif
		// fix pointers
		if (node->left != NULL)
		{
            #ifdef SYMBOL_DEBUG
                printf("left not null - ");
            #endif
			if (node->parent != NULL)
			{
                #ifdef SYMBOL_DEBUG
                    printf("parent not null - ");
                #endif
				if (node->parent->left == node)
					node->parent->left = node->left;
				else
					node->parent->right = node->left;
                node->left->parent = node->parent;
			}
			else
            {
				symbol_table->top = node->left;
                node->left->parent = NULL;
            }
		}
		else if (node->right != NULL)
		{
            #ifdef SYMBOL_DEBUG
                printf("right not null - ");
            #endif
			if (node->parent != NULL)
			{
                #ifdef SYMBOL_DEBUG
                    printf("parent not null - ");
                #endif
				if (node->parent->right == node)
					node->parent->right = node->right;
				else
					node->parent->left = node->right;
                node->right->parent = node->parent;
			}
			else
            {
				symbol_table->top = node->right;
                node->right->parent = NULL;
            }
		}
		else
        {
            if (node->parent == NULL)
                symbol_table->top = NULL;
            else if (node->parent->left == node)
                node->parent->left = NULL;
            else
                node->parent->right = NULL;
        }
		
		// free node
		free(node);
	}
	// we have two children
	else
	{
        #ifdef SYMBOL_DEBUG
            printf("both children present\n");
        #endif
		// set this node's symbol data to in order successor's data
		node->symbol = node->right->symbol;
		// set in order successor's data to NULL
		node->right->symbol = NULL;
		// delete inorder sucessor
		end_symbol_node_delete(symbol_table, node->right);
	}
	
}



// does not copy memory, so don't free it
int end_symbol_table_assign (end_symbol_table_t * symbol_table, end_symbol_t * symbol)
{

	end_symbol_node_t * node;
	
	if (symbol_table->top == NULL)
	{
		symbol_table->top = (end_symbol_node_t *) malloc(sizeof(end_symbol_node_t));
		node = symbol_table->top;
		node->parent = NULL;
	}
	else
	{
		node = symbol_table->top;
		while (node != NULL)
		{
			if (strcmp(node->symbol->name, symbol->name) < 0)
			{
                #ifdef SYMBOL_DEBUG
                    printf("left ");
                #endif
				if (node->left == NULL)
				{
					node->left = (end_symbol_node_t *) malloc(sizeof(end_symbol_node_t));
					node->left->parent = node;
					node = node->left;
					break;
				}
				else
					node = node->left;
			}
			else if (strcmp(node->symbol->name, symbol->name) > 0 )
			{
                #ifdef SYMBOL_DEBUG
                    printf("right ");
                #endif
				if (node->right == NULL)
				{
					node->right = (end_symbol_node_t *) malloc(sizeof(end_symbol_node_t));
					node->right->parent = node;
					node = node->right;
					break;
				}
				else
					node = node->right;
			}
			// symbol already exists
			else
			{
				// delete it
				#ifdef SYMBOL_DEBUG
					printf("delete symbol to reapply\n");
				#endif
				end_symbol_node_delete(symbol_table, node);
				// and now add again
				end_symbol_table_assign(symbol_table, symbol);
				return 0;
			}
		}
	}
	
	node->left = NULL;
	node->right = NULL;
	node->symbol = symbol;
	
    #ifdef SYMBOL_DEBUG
        printf("\n");
    #endif
	return 0;

}



end_symbol_t * end_symbol_table_get (end_symbol_table_t * symbol_table, char * name)
{

	end_symbol_node_t * node;
	
	node = symbol_table->top;
	while (node != NULL)
	{
		if (strcmp(node->symbol->name, name) < 0)
			node = node->left;
		else if (strcmp(node->symbol->name, name) > 0)
			node = node->right;
		else
			break;
	}
	if (node == NULL)
		return NULL;
	return node->symbol;

}



end_symbol_t * end_symbol_create (char * name, char * text_value, int type)
{
	end_symbol_t * symbol;
	
	symbol = (end_symbol_t *) malloc(sizeof(end_symbol_t));
	if (name == NULL)
		symbol->name = NULL;
	else
	{
		symbol->name = (char *) malloc(strlen(name) + 1);
		strcpy(symbol->name, name);
	}
	
	symbol->type = type;

	switch (type)
	{
		case TYPE_NUMBER :
			symbol->value = end_number_create(text_value);
			break;
        case TYPE_STRING :
            symbol->value = end_string_create(text_value);
            break;
		case TYPE_NONE :
			symbol->value = NULL;
			break;
	}
	
	return symbol;
	
}



end_symbol_t * end_symbol_copy (end_symbol_t * symbol)
{
	end_symbol_t * copy;
	
	copy = (end_symbol_t *) malloc(sizeof(end_symbol_t));
	
	if (symbol->name == NULL)
		copy->name = NULL;
	else
	{
		copy->name = (char *) malloc(strlen(symbol->name) + 1);
		strcpy(copy->name, symbol->name);
	}
	
	switch (symbol->type)
	{
		case TYPE_NUMBER :
			copy->value = end_number_create(end_number_get(symbol->value));
			break;
        case TYPE_STRING :
            copy->value = end_string_create(end_string_get(symbol->value));
            break;
		case TYPE_NONE :
			copy->value = NULL;
			break;
	}
	
	copy->type = symbol->type;
	
	return copy;
}



void end_symbol_destroy (end_symbol_t * symbol)
{

	free(symbol->name);
	
	switch (symbol->type)
	{
		case TYPE_NUMBER :
			end_number_destroy((end_number_t *) symbol->value);
			break;
        case TYPE_STRING :
            end_string_destroy((end_string_t *) symbol->value);
            break;
	}
	
	free(symbol);
	
}



int end_symbol_name_set (end_symbol_t * symbol, char * name)
{
	if (symbol->name != NULL)
		free(symbol->name);
	symbol->name = (char *) malloc(strlen(name) + 1);
	strcpy(symbol->name, name);
	return 0;
}