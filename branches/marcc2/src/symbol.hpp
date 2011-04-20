#ifndef symbol_HEADER
#define symbol_HEADER

#include <string>

#include "exception.hpp"
#include "register.hpp"

class Register;

class Symbol {
    
    private :
        std::string name;
        int offset;   // offset from base pointer
        Register * reg;

    public :
        Symbol (std::string name);
        Symbol ();

        /**
         * @return true if the register pointer for this symbol is set, meaning
         *         this symbol is currently loaded in that register. false
         *         otherwise
         */
		bool register_isset ();
        
        /**
         * Free register (this symbol no longer held in register)
         */
		void register_free  ();

        std::string g_name              ();
        int         g_offset            ();
        Register *  g_register_ptr      ();
        
        void s_name     (std::string name);
        void s_offset   (int offset);
        
        /**
         * Sets this symbol's register to register, and sets this register's
         * symbol to this symbol.
         * @param reg The register this symbol is currently loaded into
         */
        void s_register_ptr (Register * reg);
        
        Symbol& operator= (Symbol symbol);
        
};

#endif
