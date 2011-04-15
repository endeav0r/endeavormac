#ifndef assembler_HEADER
#define assembler_HEADER

#include "abstractsyntaxtree.hpp"

class Assembler {

    private :
    
        std::list <Instruction> instructions;
        AbstractSyntaxTree tree;
        
        void push_register  (int reg);
        void pop_register   (int reg);
        void load_immediate (int reg, int imm);
        void mov            (int reg_dest, int reg_src);
        void addx           (int reg_dest, int reg_1, int reg_2);
        void subx           (int reg_dest, int reg_1, int reg_2);
        void bn             (int imm);
        void ba             (int imm);
        void bz             (int imm);
        
        // shortcuts to load/store symbols from/to memory
        // requires the symboltable
        void symbol_st (std::string name, int reg);
        void symbol_ld (std::string name, int reg);
        
        // returns the id of a register containing the value of symbol
        // this is better than symbol_ld because it checks if the symbol is
        // already in a register first
        int symbol_to_register (std::string name);

    public :
        Assembler (AbstractSyntaxTree tree);

};

#endif
