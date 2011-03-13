#include "token.hpp"

#include <iostream>
using namespace std;


Token :: Token () {
    this->type = -1;
    this->text = "";
}


Token :: Token (int type) {
    this->type = type;
    this->text = "";
}


Token :: Token (std::string text) {
    this->type = -1;
    this->text = text;
}


Token :: Token (int type, std::string text) {
    this->type = type;
    this->text = text;
}


bool Token :: valid_symbol_name (std::string name) {
    std::string::iterator it;
    
    if ((name[0] >= '0') && (name[0] <= '9'))
        return false;
    
    for (it = name.begin(); it != name.end(); it++) {
        if (! ((*it >= 'a') && (*it <= 'z'))
              || ((*it >= 'A') && (*it <= 'Z'))
              || ((*it >= '0') && (*it <= '9')))
            return false;
    }
    return true;
}


bool Token :: is_number (std::string name) {
    std::string::iterator it;
    
    for (it = name.begin(); it != name.end(); it++) {
        if (! ((*it >= '0') && (*it <= '9')))
            return false;
    }
    return true;
}


void Token :: choose_type () {
    if (this->text == std::string("if"))
        this->type = TOKEN_IF;
    else if (this->valid_symbol_name(this->text))
        this->type = TOKEN_SYMBOL;
    else if (this->is_number(this->text))
        this->type = TOKEN_NUMBER;
    else if (this->text.size() == 1) {
        switch (this->text.c_str()[0]) {
            case '+' :
                this->type = TOKEN_PLUS;
                break;
            case '-' :
                this->type = TOKEN_MINUS;
                break;
            case '=' :
                this->type = TOKEN_EQUAL;
                break;
            case ';' :
                this->type = TOKEN_TERMINATOR;
                break;
            case '{' :
                this->type = TOKEN_BLOCK_OPEN;
                break;
            case '}' :
                this->type = TOKEN_BLOCK_CLOSE;
                break;
            case '(' :
                this->type = TOKEN_PAREN_OPEN;
                break;
            case ')' :
                this->type = TOKEN_PAREN_CLOSE;
                break;
            default :
                throw Exception("could not tokenize input: " + this->text);
        }
    }
}

#define TOKEN_PLUS        1
#define TOKEN_MINUS       2
#define TOKEN_EQUALS      3
#define TOKEN_TERMINATOR  4
#define TOKEN_SYMBOL      5
#define TOKEN_DECLARATION 6
std::string Token :: type_textual_description () {

    switch (this->type) {
        case TOKEN_PLUS :
            return "TOKEN_PLUS";
        case TOKEN_MINUS :
            return "TOKEN_MINUS";
        case TOKEN_EQUALS :
            return "TOKEN_EQUALS";
        case TOKEN_TERMINATOR :
            return "TOKEN_TERMINATOR";
        case TOKEN_SYMBOL :
            return "TOKEN_SYMBOL";
        case TOKEN_DECLARATION :
            return "TOKEN_DECLARATION";
    }
    
    return "UNKNOWN ";

}


void Token :: s_text (std::string text) { this->text = text; }
void Token :: s_type (int type)         { this->type = type; }

std::string Token :: g_text () { return this->text; }
int         Token :: g_type () { return this->type; }
