#ifndef end_HEADER
#define end_HEADER

#include <stdlib.h>
#include <string.h>

#include "number.h"
#include "string.h"

typedef struct end_symbol_s
{
	char * name;
	void * value;
	int type;
} end_symbol_t;



typedef struct end_symbol_node_s
{
	end_symbol_t * symbol;
	struct end_symbol_node_s * left;
	struct end_symbol_node_s * right;
	struct end_symbol_node_s * parent;
} end_symbol_node_t;



typedef struct end_symbol_table_s
{
	int size;
	struct end_symbol_node_s * top;
} end_symbol_table_t;



end_symbol_table_t * end_symbol_table_create ();
void                 end_symbol_table_destroy (end_symbol_table_t * symbol_table);

int                  end_symbol_table_assign (end_symbol_table_t * symbol_table,
                                              end_symbol_t * token);

end_symbol_t *       end_symbol_table_get (end_symbol_table_t * symbol_table,
                                          char * name);
										  
end_symbol_t * end_symbol_create  (char * name, char * value, int type);
void           end_symbol_destroy (end_symbol_t * symbol);
end_symbol_t * end_symbol_copy (end_symbol_t * symbol);
int            end_symbol_name_set (end_symbol_t * symbol, char * name);

#endif