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

#define REG_STACK_POINTER 7
#define REG_BASE_POINTER  6

class SymbolTable {

    private :
        std::map <std::string, Symbol> symbols;
        SymbolTable * previous;
        SymbolTable * next;
        SymbolTable * last;
        
        // this variable holds the next free position, after the base pointer of
        // this stack frame, for a variable
        int next_free_offset;

        Register registers[GENERAL_PURPOSE_REGISTERS];
        Register r0;
        Register stack_pointer;
        Register base_pointer;
        
    public :
        SymbolTable ();
        
        /**
         * Pushes a new SymbolTable onto the stack. This push/pop should mirror
         * your stack frames
         */
        void push ();
        
        /**
         * Pops a SymbolTable off the stack. This should mirror your stack
         * frames
         */
        void pop  ();

        /**
         * This will return the next available free register. Use this always.
         * Also performs some trickery to keep symbols in registers as long
         * as possible.
         * @return a pointer to a register available for use
         */
        Register * get_free_register_ptr ();
        
        /**
         * Creates a symbol, adds it to the last SymbolTable (the current
         * SymbolTable), and then reserves space for the variable on the stack
         * @param name the name of the new symbol
         */
        void add_symbol (std::string name, int stack_space = 1);
        
        /**
         * Tells us if a symbol exists in the last (current) SymbolTable
         * @return true if symbol exists in last SymbolTable, false otherwise
         */
        bool symbol_exists (std::string name);
        
        /**
         * @return the offset from BASE_POINTER of the symbol in the last
         *         SymbolTable
         */
        int        g_symbol_offset   (std::string name);
        
        /**
         * @return pointer to symbol in last SymbolTable
         */
        Symbol *   g_symbol_ptr      (std::string name);
        
        /**
         * @return pointer to a specific register
         *         (you probably don't want to use this)
         */
        Register * g_register_ptr    (int reg);
        
        Register *  g_r0_ptr            ();
        Register *  g_base_pointer_ptr  (); // only bad things can happen by
        Register *  g_stack_pointer_ptr (); // giving pointers to these two
        // registers, so use super-ninja skills to not let those bad things
        // happen
};

#endif
