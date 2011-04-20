#ifndef register_HEADER
#define register_HEADER

#include <iostream>

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
		
        /**
        * Free this register, it is no longer being used
        */
		void free ();
		
        /**
        * Use this register. This will also set this register's symbol to NULL,
        * as we no longer know what symbol is loaded into it.
        */
		void use  ();

        /**
        * @return true if this register contains a symbol, false otherwise
        */
		bool has_symbol ();
		
        /**
         * @param symbol pointer to the Symbol that is currently loaded into
         *               this register
         */
		void s_symbol (Symbol * symbol);
        
        /**
         * @param id sets the id of this pointer (corresponds to the different
         *           registers on the CPU)
         */
		void s_id     (int id);
		
        /**
         * @return the id of this register (corresponds to the register on the
         *         CPU)
         */
		int  g_id   ();
        
        /**
         * @return true if this register is not being used, false if it is
         *         being used
         */
		bool g_free ();
		
};

#endif
