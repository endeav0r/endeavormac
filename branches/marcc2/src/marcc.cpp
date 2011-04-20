#include <list>
#include <string>
#include <iostream>

#include "assembler.hpp"
#include "astree.hpp"
#include "exception.hpp"
#include "instruction.hpp"
#include "lexer.hpp"
#include "parser.hpp"


int test_input (std::string input) {    
    std::list <Instruction *> instructions;
    std::list <Instruction *> :: iterator instruction_it;
    ASTree * tree;
    try {
        
        std::cout << "\nRUNNING TEST\n" << input << "\n";
    
        Lexer lexer(input);
        lexer.lex();
    
        Parser parser(lexer.g_tokens());
        parser.parse();
        
        tree = parser.g_tree();
        tree->debug(0);
        
        Assembler assembler(parser.g_tree());
        assembler.assemble();
        
        instructions = assembler.g_instructions();
        for (instruction_it  = instructions.begin();
             instruction_it != instructions.end();
             instruction_it++) {
            std::cout << (*instruction_it)->assembly() << "\n";
        }
        
        assembler.free_instructions();
        
        std::cout << "\n";
    }
    catch (Exception e) {
        std::cout << e.g_description() << "\n";
        return -1;
    }
    
    return 0;
}



int main () {
    
    if (test_input("int var = 7 + 4;"))
        return -1;
    
    if (test_input("int a = 7 + 4; int b = 2 + 9; if (a == b) { if (a == b) {}int c = 4; }"))
        return -1;
    
    if (test_input("int i = 0; while (i < 5) { i = i + 1; }"))
        return -1;
    
    return 0;
    
}
