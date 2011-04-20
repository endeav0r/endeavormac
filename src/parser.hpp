#ifndef parser_HEADER
#define parser_HEADER

#include <cstdlib>
#include <iostream>
#include <list>
#include <stack>
#include <string>

#include "astree.hpp"
#include "exception.hpp"
#include "token.hpp"

class Parser {

    private :
        
        // though this is used like a stack, we use a list so that
        // we can peek several tokens down
        // (we're not creating no stinking state table)
        std::list <ASTree *> stack;
        
        ASTree * tree;
        
        std::list <Token> tokens;
        std::list <Token> :: iterator tokens_it;
        Token g_next_token();
        
        void reduce ();
    
    public :
        Parser (std::list <Token> tokens);
        ~Parser ();
        
        void     parse ();
        ASTree * g_tree ();
        
};

#endif
