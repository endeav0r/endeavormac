#ifndef token_HEADER
#define token_HEADER

#include <string>

#include "exception.hpp"

#define TOKEN_PLUS        1
#define TOKEN_MINUS       2
#define TOKEN_EQUAL       3
#define TOKEN_TERMINATOR  4
#define TOKEN_SYMBOL      5
#define TOKEN_DECLARATION 6
#define TOKEN_NUMBER      7


class Token {

    private :
        std::string text;
        int type;
        
        bool valid_symbol_name (std::string name);
        bool is_number (std::string name);
    
    public :
        Token ();
        Token (int type);
        Token (std::string text);
        Token (int type, std::string text);
        
        void   s_text(std::string text);
        void   s_type(int type);
        
        std::string g_text();
        int         g_type();
        
        void choose_type ();
        std::string type_textual_description ();
};

#endif