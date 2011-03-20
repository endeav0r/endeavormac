#ifndef register_HEADER
#define register_HEADER

#include "symbol.hpp"
#include "exception.hpp"

class Symbol;

class Register {

	private :
		int id;
		bool bool_free;
		Symbol * symbol;
	
	public :
		Register (int id);
		Register ();
		
		void free ();
		// also unsets symbol->reg if symbol is set
		void use  ();
		bool has_symbol ();
		
		void s_symbol (Symbol * symbol);
		void s_id     (int id);
		
		int  g_id   ();
		bool g_free ();
		
};

#endif
