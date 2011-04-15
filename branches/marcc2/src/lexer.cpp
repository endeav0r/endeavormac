#include "lexer.hpp"

#include <iostream>
using namespace std;

bool Lexer :: is_whitespace (char c) {
    switch (c) {
        case ' '  :
        case '\t' :
        case '\n' :
        case '\r' :
            return true;
        default :
            return false;
    }
}

bool Lexer :: is_special (char c) {
    switch (c) {
        case '+' :
        case '-' :
        case ';' :
        case '=' :
        case '{' :
        case '}' :
        case '[' :
        case ']' :
        case '(' :
        case ')' :
            return true;
        default :
            return false;
    }
}


Lexer :: Lexer (std::string text) {
    this->text = text;
}


void Lexer :: lex () {
    std::string buf = "";
    std::string::iterator text_it;
    std::list <Token> :: iterator token_it;
    
    for (text_it = this->text.begin(); text_it != this->text.end(); text_it++) {
        if (this->is_whitespace(*text_it)) {
            if (buf.size() > 0)
                this->tokens.push_back(Token(buf));
            buf = "";
        }
        else if (this->is_special(*text_it)) {
            if (buf.size() > 0)
                this->tokens.push_back(Token(buf));
            this->tokens.push_back(Token(std::string("") + *text_it));
            buf = "";
        }
        else {
            buf += *text_it;
        }
    }
    if (buf.size() > 0)
        this->tokens.push_back(Token(buf));
    
    for (token_it = this->tokens.begin(); token_it != tokens.end(); token_it++)
        (*token_it).choose_type();
}


std::list <Token> Lexer :: g_tokens () { return this->tokens; }
