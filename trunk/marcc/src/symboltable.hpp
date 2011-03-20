#ifndef symboltable_HEADER
#define symboltable_HEADER

#include <map>
#include <string>

#include "register.hpp"
#include "symbol.hpp"
#include "exception.hpp"

// the number of general purpose registers to use
// 0 = always 0x0000
// 6 = base pointer
// 7 = stack pointer
// 1-5 available
#define GENERAL_PURPOSE_REGISTERS 5


class SymbolTable {

    private :
        std::map <std::string, Symbol> symbols;
        SymbolTable * previous;
        SymbolTable * next;
        SymbolTable * last;
        int next_free_offset;

        Register registers[GENERAL_PURPOSE_REGISTERS];
        
    public :
        SymbolTable ();
        
        void push ();
        void pop  ();

        int  get_free_register ();
        void use_register      (int reg);
        void free_register     (int reg);
        
        // add_symbol returns the number of bytes this symbol is taking
        // up, IE if this is a stack variable than SP + offset
        int add_symbol (std::string name, bool absolute);
        bool symbol_exists (std::string name);
        
        int        g_symbol_offset   (std::string name);
        bool       g_symbol_absolute (std::string name);
        int        g_symbol_address  (std::string name);
        Symbol &   g_symbol          (std::string name);
        Register * g_register_ptr    (int reg);
};

#endif
