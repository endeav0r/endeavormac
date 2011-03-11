#include "parserstack.hpp"


ParserStack :: ParserStack (int type) {
    this->type = type;
}


ParserStack :: ParserStack (int type, unsigned int constant) {
    this->type = type;
    this->constant = constant;
}


ParserStack :: ParserStack (int type, int reg) {
    this->type = type;
    this->reg = reg;
}


ParserStack :: ParserStack (int type, std::string name) {
    this->type = type;
    switch (type) {
        case PS_SYMBOL :
            this->name = name;
            break;
        case PS_NUMBER :
            this->constant = (unsigned int) atoi(name.c_str());
            break;
    }
}


void ParserStack :: s_constant (unsigned int constant) { this->constant = constant; }
void ParserStack :: s_register (int reg)               { this->reg = reg; }
void ParserStack :: s_name     (std::string name)      { this->name = name; }

int          ParserStack :: g_type     () { return this->type;     }
unsigned int ParserStack :: g_constant () { return this->constant; }
int          ParserStack :: g_register () { return this->reg;      }
std::string  ParserStack :: g_name     () { return this->name;     }
