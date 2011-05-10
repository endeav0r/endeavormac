#include <list>
#include <string>

#include <iostream>
#include <fstream>

#include "compiler.hpp"
#include "astree.hpp"
#include "exception.hpp"
#include "instruction.hpp"
#include "lexer.hpp"
#include "parser.hpp"


int test_input (std::string input) {    
    std::list <Instruction *> instructions;
    std::list <Instruction *> :: iterator instruction_it;
    std::list <MemoryDefinition> definitions;
    std::list <MemoryDefinition> :: iterator definition_it;
    
    ASTree * tree;
    try {
        
        std::cout << "\nRUNNING TEST\n" << input << "\n";
        
        Lexer lexer(input);
        lexer.lex();
    
        Parser parser(lexer.g_tokens());
        parser.parse();
        
        tree = parser.g_tree();
        tree->debug(0);
        
        Compiler compiler(parser.g_tree());
        compiler.compile();
        
        instructions = compiler.g_instructions();
        for (instruction_it  = instructions.begin();
             instruction_it != instructions.end();
             instruction_it++) {
            std::cout << (*instruction_it)->assembly() << "\n";
        }
        
        definitions = compiler.g_definitions();
        for (definition_it = definitions.begin();
             definition_it != definitions.end();
             definition_it++) {
            std::cout << (*definition_it).assembly() << "\n";
        }
        
        compiler.free_instructions();
        
        std::cout << "\n";
    }
    catch (Exception e) {
        std::cout << e.g_description() << "\n";
        return -1;
    }
    
    return 0;
}



int main () {
    char buf[10240];
    std::ifstream file;
        /*
    if (test_input("int var = 7 + 4;"))
        return -1;
    
    if (test_input("int a = 7 + 4; int b = 2 + 9; if (a == b) { if (a == b) {}int c = 4; }"))
        return -1;
    
    if (test_input("int i = 0; while (i < 5) { i = i + 1; }"))
        return -1;
    
    if (test_input("int i[2]; int j; i[0] = 7; j = i; i[1] = j;"))
        return -1;

    if (test_input("int keystream[256]; int i = 0; while (i < 256) { keystream[i] = i; i = i + 1; }"))
        return -1;
            */
    file.open("tests/rc4.mc", std::ifstream::in);
    file.read(buf, 10240);
    file.close();
    std::cout << buf;
    if (test_input(buf))
        return -1;
    

    return 0;
    
}
