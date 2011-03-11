#ifndef parserstack_HEADER
#define parserstack_HEADER

#include <iostream>
#include <string>
#include <cstdlib>

#define PS_SYMBOL      1
#define PS_NUMBER      2
#define PS_ADD         3
#define PS_TERMINATOR  4
#define PS_ASSIGN      5
#define PS_REGISTER    6
#define PS_PAREN_OPEN  7
#define PS_PAREN_CLOSE 8
#define PS_IF          9
#define PS_EQUALS      10
#define PS_BLOCK_OPEN  11
#define PS_BLOCK_CLOSE 12
#define PS_BLOCK       13
#define PS_CONDITION   14


class ParserStack {

    private :
        int          type;
        unsigned int constant;
        int          reg;
        int          branch_address;
        std::string  name;
    
    public :
        ParserStack (int type);
        ParserStack (int type, unsigned int constant);
        ParserStack (int type, int reg);
        ParserStack (int type, std::string name);
        
        void s_constant       (unsigned int constant);
        void s_register       (int reg);
        void s_name           (std::string name);
        void s_branch_address (int branch_address);
        
        int          g_type     ();
        unsigned int g_constant ();
        int          g_register ();
        std::string  g_name     ();
        int          g_branch_address ();
};

#endif
