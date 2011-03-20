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
        
        std::cout << "\nRUNNING TEST\n" << input << "\n";
    
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
        std::cout << "\n";
    }
    catch (Exception e) {
        std::cout << e.g_description() << "\n";
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
	if (test_input("if (1 == 1) { a = 2; } a = a + 1;"))
		return -1;
	if (test_input("i = 0; while (i < 5) {i = i + 1;}"))
		return -1;
    
    return 0;
    
}
