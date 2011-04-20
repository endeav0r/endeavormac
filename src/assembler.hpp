#ifndef assembler_HEADER
#define assembler_HEADER

#include <iostream>
#include <list>
#include <string>

#include "astree.hpp"
#include "instruction.hpp"
#include "symboltable.hpp"

class Assembler {

    private :
    
        std::list <Instruction *> instructions;
        
        ASTree * tree;
        SymbolTable symbolTable;
        
        /**
         * loads the value immediate into Register * pointed to by reg
         * @param reg A pointer to a register WHICH HAS ALREADY been marked as used
         * @param immediate Value to load into register (these bits: 0x0000FFFF)
         */
        void load_immediate (Register * reg, int immediate);
        
        /**
         * Gets the address of a variable from the symbolTable, places the
         * address in the register and returns that register to the user
         * @param var Variable to look up in the symbol table
         * @return a pointer to a register which contains the address of the
         *         variables
         */
        Register * variable_address (ASTreeExprVar * var);
        
        void declare (ASTreeExprVar * var);
        
        /**
         * There are numerous cases for dealing with expressions. This case, the default
         * case, is for evaluating expressions and loading them into variables. This is
         * not for declaring variables, etc.
         * @return a pointer to a register which has been marked used and will contain
         *         the value of the evaluated expression during runtime
         */
        Register * expression (ASTreeExpr * expression);
        
        void assign (ASTreeAssign * assign);
        
        void       statement  (ASTreeStatement * statement);
        
    public :
        Assembler (ASTree * tree);
        void assemble();
        std::list <Instruction *> g_instructions ();
        
        /**
         * Because assembler will sometimes call assembler on sub-parts of
         * itself and then use the instructions from those instances, we need
         * fine-grained control over when we will delete/free the instructions
         * of an assembler.
         * This should only be called when the instructions are no longer needed
         */
        void free_instructions();

};

#endif
