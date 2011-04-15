#include <list>
#include <string>
#include <iostream>

#include "exception.hpp"
#include "lexer.hpp"
#include "parser.hpp"
#include "astree.hpp"


int test_input (std::string input) {    
    std::list <ASTree *> tree;
    std::list <ASTree *> :: iterator tree_it;
    try {
        
        std::cout << "\nRUNNING TEST\n" << input << "\n";
    
        Lexer lexer(input);
        lexer.lex();
    
        Parser parser(lexer.g_tokens());
        parser.parse();
        
        tree = parser.g_tree();
        for (tree_it = tree.begin(); tree_it != tree.end(); tree_it++) {
            (*tree_it)->debug(0);
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
    
    if (test_input("var = 7 + 4;"))
        return -1;
    
    if (test_input("{var = 1 + 2 + (3 + 4);}"))
        return -1;
    
    if (test_input("if (4 == 5) {a = 3;}"))
        return -1;
    
    return 0;
    
}
