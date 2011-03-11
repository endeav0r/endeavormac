#ifndef parser_HEADER
#define parser_HEADER

#include <iostream>
#include <list>
#include <stack>
#include <string>

#include "instruction.hpp"
#include "parserstack.hpp"
#include "symboltable.hpp"
#include "token.hpp"

// general purpose registers will be 1 through GENERAL_PURPOSE_REGISTERS - 1
// if this is 4, then general purpose registers will be 1, 2 and 3
#define GENERAL_PURPOSE_REGISTERS 6

class Parser {

    private :
        // though this is used like a stack, we use a list so that
        // we can peek several tokens down
        // (we're not creating no stinking state table)
        std::list <ParserStack> stack;
        
        std::list <Token> tokens;
        std::list <Instruction> instructions;
        
        // when we have a code block { } preceeded by a conditional,
        // we keep a pointer to the branch instruction that needs to
        // point after the block. when the block closes, we can set the
        // branch up right
        std::stack <Instruction *> jmp_stack;
        
        SymbolTable table;
        
        // corresponds to register 1-3. 0 unused
        bool registers_free[GENERAL_PURPOSE_REGISTERS];
        
        // the purpose of these register functions is to allow us to
        // eventually dynamically store register values on the stack
        // in current form registers will exhaust
        int  get_free_register ();
        void use_register  (int reg);
        void free_register (int reg);
        
        void declare_variable (int bytes);
        
        void push_register  (int reg);
        void pop_register   (int reg);
        void load_immediate (int reg, int imm);
        void mov            (int reg_dest, int reg_src);
        void addx           (int reg_dest, int reg_1, int reg_2);
        void subx           (int reg_dest, int reg_1, int reg_2);
        void bn             (int imm);
        void ba             (int imm);
        void bz             (int imm);
        
        void symbol_st (std::string name, int reg);
		void symbol_ld (std::string name, int reg);
        
        void reduce(Token next);
    
    public :
        Parser (std::list <Token> tokens);
        void parse ();
        
        std::list <Instruction> g_instructions ();
        
};

#endif
