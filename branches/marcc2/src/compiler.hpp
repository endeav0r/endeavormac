#ifndef compiler_HEADER
#define compiler_HEADER

#include <iostream>
#include <list>
#include <string>

#include "astree.hpp"
#include "instruction.hpp"
#include "labelmaker.hpp"
#include "memorydefinition.hpp"
#include "symboltable.hpp"

class Compiler {

    private :
    
        std::list <Instruction *> instructions;
        std::list <MemoryDefinition> definitions;
        
        LabelMaker labelMaker;
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
         * @param force_load For the loading of variables from memory, don't use
         *                   values already in registers
         * @return a pointer to a register which contains the address of the
         *         variables
         */
        Register * variable_address (ASTreeExprVar * var, bool force_load = false);
        
        void declare (ASTreeExprVar * var);
        
        /**
         * There are numerous cases for dealing with expressions. This case, the default
         * case, is for evaluating expressions and loading them into variables. This is
         * not for declaring variables, etc.
         * @param expression the expression to evaluate
         * @param force_load this will not check to see if a symbol is already
         *                   contained in a register, and will force the loading
         *                   of a symbol into a register (think loops)
         * @return a pointer to a register which has been marked used and will contain
         *         the value of the evaluated expression during runtime
         */
        Register * expression (ASTreeExpr * expression, bool force_load = false);
        
        /**
         * Evaluates a condition and jumps to label_true if the condition is
         * true, or label_false if the condition is false
         * @param condition the condition to evaluate
         * @param label_true the label to branch to if the condition is true
         * @param label_false the label to branch to if the condition is false
         */
        void condition (ASTreeCondition * condition,
                        std::string label_true,
                        std::string label_false,
                        bool force_load = false);
        
        /**
         * Evaluates an if statement
         * @param treeIf the if statement node in the ASTree
         */
        void treeIf (ASTreeIf * treeIf);
        
        /**
         * Like treeIf there is no trickery here. behaves like it looks
         * @param treeWhile the tree node in the ASTree
         */
        void treeWhile (ASTreeWhile * treeWhile);
        
        void assign (ASTreeAssign * assign);
        
        void       statement  (ASTreeStatement * statement);
        
    public :
        Compiler (ASTree * tree);
        void compile();
        
        std::list <MemoryDefinition> g_definitions ();
        std::list <Instruction *> g_instructions ();
        
        /**
         * Because compiler will sometimes call compiler on sub-parts of
         * itself and then use the instructions from those instances, we need
         * fine-grained control over when we will delete/free the instructions
         * of an assembler.
         * This should only be called when the instructions are no longer needed
         */
        void free_instructions();

};

#endif
