#include "symbol.hpp"



Symbol :: Symbol (std::string name) {
    this->name     = name;
    this->absolute = true;
    this->address  = 0x00000000;
    this->offset   = 0x00000000;
    this->reg      = NULL;
}


Symbol :: Symbol () {
    this->name     = name;
    this->absolute = true;
    this->address  = 0x00000000;
    this->offset   = 0x00000000;
    this->reg      = NULL;
}

bool Symbol :: register_isset  () { return this->reg == NULL ? false : true; }
void Symbol :: register_free () { this->reg = NULL; }

std::string Symbol :: g_name     () { return this->name;        }
bool        Symbol :: g_absolute () { return this->absolute;    }
int         Symbol :: g_offset   () { return this->offset;      }
int         Symbol :: g_address  () { return this->address;     }
int         Symbol :: g_register () { return this->reg->g_id(); }

void Symbol :: s_name     (std::string name) { this->name     = name;     }
void Symbol :: s_absolute (bool    absolute) { this->absolute = absolute; }
void Symbol :: s_offset   (int       offset) { this->offset   = offset;   }
void Symbol :: s_address  (int      address) { this->address  = address;  }

void Symbol :: s_register (Register * reg) { 
	this->reg = reg;
	this->reg->s_symbol(this);
}


Symbol& Symbol :: operator= (Symbol symbol) {
    this->name     = symbol.g_name();
    this->absolute = symbol.g_absolute();
    this->address  = symbol.g_address();
    this->offset   = symbol.g_offset();
    return *this;
}
