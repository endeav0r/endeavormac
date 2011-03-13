#include "parserstack.hpp"


ParserStack :: ParserStack (int type) {
    this->type = type;
}


ParserStack :: ParserStack (int type, unsigned int constant) {
    this->type     = type;
    this->constant = constant;
}


ParserStack :: ParserStack (int type, int reg) {
    this->type = type;
    switch (type) {
        case PS_BLOCK_OPEN :
        case PS_BLOCK_CLOSE :
        case PS_BLOCK :
            this->branch_address = reg;
            break;
        case PS_REGISTER :
            this->reg = reg;
            break;
    }
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


void ParserStack :: s_constant       (unsigned int constant) { this->constant = constant; }
void ParserStack :: s_register       (int          reg)      { this->reg = reg; }
void ParserStack :: s_name           (std::string  name)     { this->name = name; }
void ParserStack :: s_branch_address (int branch_address)    { this->branch_address = branch_address; }

int          ParserStack :: g_type           () { return this->type;     }
unsigned int ParserStack :: g_constant       () { return this->constant; }
int          ParserStack :: g_register       () { return this->reg;      }
std::string  ParserStack :: g_name           () { return this->name;     }
int          ParserStack :: g_branch_address () { return this->branch_address; }
