#include <list>
#include <string>
#include <iostream>

#include "exception.hpp"
#include "instruction.hpp"
#include "lexer.hpp"
#include "parser.hpp"
#include "token.hpp"


int test_input (std::string input) {    
    std::list <Instruction> instructions;
    std::list <Instruction> :: iterator instruction_it;

    try {
    
        Lexer lexer(input);
        lexer.lex();
    
        Parser parser(lexer.g_tokens());
        parser.parse();
        
        instructions = parser.g_instructions();
        for (instruction_it  = instructions.begin();
             instruction_it != instructions.end();
             instruction_it++) {
            std::cout << (*instruction_it).assembly() << "\n";
        }
    }
    catch (Exception e) {
        std::cout << e.g_description();
        return -1;
    }
    
    return 0;
}



int main () {
    
    if (test_input("var = 7;"))
        return -1;
    if (test_input("var = 4 + 7;"))
        return -1;
    if (test_input("a = 1 + 2; b = 3 + 4; a = a + b;"))
        return -1;
    
    return 0;
    
}
