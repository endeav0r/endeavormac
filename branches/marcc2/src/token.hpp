#ifndef token_HEADER
#define token_HEADER

#include <string>

#include "exception.hpp"

enum { TOKEN_PLUS, TOKEN_MINUS, TOKEN_EQUAL, TOKEN_TERMINATOR, TOKEN_SYMBOL,
       TOKEN_DECLARATION, TOKEN_NUMBER, TOKEN_PAREN_OPEN, TOKEN_PAREN_CLOSE,
       TOKEN_IF, TOKEN_BLOCK_OPEN, TOKEN_BLOCK_CLOSE, TOKEN_WHILE,
       TOKEN_LESS_THAN };



class Token {

    private :
        std::string text;
        int         type;
        
        bool valid_symbol_name (std::string name);
        bool is_number         (std::string name);
    
    public :
        Token ();
        Token (int type);
        Token (std::string text);
        Token (int type, std::string text);
        
        void s_text(std::string text);
        void s_type(int type);
        
        std::string g_text();
        int         g_type();
        
        void        choose_type              ();
        std::string type_textual_description ();
};

#endif
