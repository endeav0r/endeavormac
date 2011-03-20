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
        
        // when we have a code block { } preceeded by a conditional,
        // we keep a pointer to the branch instruction that needs to
        // point after the block. when the block closes, we can set the
        // branch up right
        std::stack <Instruction *> jmp_stack;
        
        SymbolTable table;
        
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
        
        // returns the id of a register containing the value of symbol
        // this is better than symbol_ld because it checks if the symbol is
        // already in a register first
        int symbol_to_register (std::string name);
        
        void reduce(Token next);
        
        // currently comments in the instructions are messing things up
        // this is a temporary quick fix, ugly as it may be
        int instructions_size ();
    
    public :
        Parser (std::list <Token> tokens);
        void parse ();
        
        std::list <Instruction> g_instructions ();
        
};

#endif
