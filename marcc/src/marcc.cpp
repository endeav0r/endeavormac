#include <list>
#include <string>
#include <iostream>

#include "exception.hpp"
#include "instruction.hpp"
#include "lexer.hpp"
#include "parser.hpp"
#include "token.hpp"


int main () {
    
    std::string test_input = "result = 7;";
    std::list <Token> tokens;
    std::list <Token> :: iterator it;
    
    std::list <Instruction> instructions;
    std::list <Instruction> :: iterator instruction_it;
    
    
    try {
    
        Lexer lexer(test_input);
        lexer.lex();
        
        tokens = lexer.g_tokens();
        for (it = tokens.begin(); it != tokens.end(); it++) {
            std::cout << (*it).type_textual_description()
                      << "\t" 
                      << (*it).g_text()
                      << "\n";
        }
    
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
        return 0;
    }
    

    
    return 0;
    
}