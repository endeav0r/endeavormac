#ifndef parserstack_HEADER
#define parserstack_HEADER

#include <iostream>
#include <string>
#include <cstdlib>

#define PS_SYMBOL     1
#define PS_NUMBER     2
#define PS_ADD        3
#define PS_TERMINATOR 4
#define PS_ASSIGN     5
#define PS_REGISTER   6


class ParserStack {

    private :
        int type;
        unsigned int constant;
        int reg;
        std::string name;
    
    public :
        ParserStack (int type);
        ParserStack (int type, unsigned int constant);
        ParserStack (int type, int reg);
        ParserStack (int type, std::string name);
        
        void s_constant (unsigned int constant);
        void s_register (int reg);
        void s_name     (std::string name);
        
        int          g_type     ();
        unsigned int g_constant ();
        int          g_register ();
        std::string  g_name     ();
};

#endif
