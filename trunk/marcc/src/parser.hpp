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


class Parser {

    private :
        // though this is used like a stack, we use a list so that
        // we can peek several tokens down
        // (we're not creating no stinking state table)
        std::list <ParserStack> stack;
        std::list <Token> tokens;
        std::list <Instruction> instructions;
        
        SymbolTable table;
        
        void declare_variable (int bytes);
        
        
        void push_register  (int reg);
        void pop_register   (int reg);
        void load_immediate (int reg, int imm);
        void mov            (int reg_dest, int reg_src);
        void addx           (int reg_dest, int reg_1, int reg_2);
        
        
        
        void set_symbol   (std::string name, int value);
        
        void reduce(Token next);
    
    public :
        Parser (std::list <Token> tokens);
        void parse ();
        
        std::list <Instruction> g_instructions ();
        
};

#endif