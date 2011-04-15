#ifndef lexer_HEADER
#define lexer_HEADER

#include <list>
#include <string>

#include "token.hpp"

class Lexer {

    private :
        std::list <Token> tokens;
        std::string text;
        
        bool is_whitespace (char c);
        bool is_special (char c);
        
    public :
        Lexer (std::string text);
        
        void lex ();
        
        std::list <Token> g_tokens();
};

#endif
