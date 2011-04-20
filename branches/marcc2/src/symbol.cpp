#include "symbol.hpp"



Symbol :: Symbol (std::string name) {
    this->name     = name;
    this->offset   = 0x00000000;
    this->reg      = NULL;
}


Symbol :: Symbol () {
    this->name     = "";
    this->offset   = 0x00000000;
    this->reg      = NULL;
}

bool Symbol :: register_isset  () { return this->reg == NULL ? false : true; }
void Symbol :: register_free ()   { this->reg = NULL; }

std::string Symbol :: g_name         () { return this->name;        }
int         Symbol :: g_offset       () { return this->offset;      }
Register *  Symbol :: g_register_ptr () { return this->reg; }

void Symbol :: s_name     (std::string name) { this->name     = name;     }
void Symbol :: s_offset   (int       offset) { this->offset   = offset;   }

void Symbol :: s_register_ptr (Register * reg) { 
	this->reg = reg;
	this->reg->s_symbol(this);
}


Symbol& Symbol :: operator= (Symbol symbol) {
    this->name     = symbol.g_name();
    this->offset   = symbol.g_offset();
    return *this;
}
