#ifndef symboltable_HEADER
#define symboltable_HEADER

#include <map>
#include <string>

#include "exception.hpp"

class Symbol {
    
    private :
        std::string name;
        bool absolute; // is this an absolute address, or relative to BP
                       // IE is it static/global or in the scope of the
                       // stack frame
        int  address;  // address or offset based on value of absolute
        int  offset;   // what's 4 bytes anyway, two names for clarity
    
    public :
        Symbol (std::string name);
        Symbol ();
        
        std::string g_name     ();
        bool        g_absolute ();
        int         g_offset   ();
        int         g_address  ();
        
        void s_name     (std::string name);
        void s_absolute (bool absolute);
        void s_offset   (int offset);
        void s_address  (int address);
        
        Symbol& operator= (Symbol symbol);
        
};
        


class SymbolTable {

    private :
        std::map <std::string, Symbol> symbols;
        SymbolTable * previous;
        SymbolTable * next;
        SymbolTable * last;
        int next_free_offset;
        
    public :
        SymbolTable ();
        
        void push ();
        void pop  ();
        
        // add_symbol returns the number of bytes this symbol is taking
        // up, IE if this is a stack variable than SP + offset
        int add_symbol (std::string name, bool absolute);
        bool symbol_exists (std::string name);
        
        int  g_symbol_offset   (std::string name);
        bool g_symbol_absolute (std::string name);
        int  g_symbol_address  (std::string name);
        
        
};

#endif