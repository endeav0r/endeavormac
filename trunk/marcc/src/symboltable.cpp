#include "symboltable.hpp"


Symbol :: Symbol (std::string name) {
    this->name = name;
    this->absolute = true;
    this->address = 0x00000000;
    this->offset  = 0x00000000;
}


Symbol :: Symbol () {
    this->name = name;
    this->absolute = true;
    this->address = 0x00000000;
    this->offset  = 0x00000000;
}


std::string Symbol :: g_name     () { return this->name;     }
bool        Symbol :: g_absolute () { return this->absolute; }
int         Symbol :: g_offset   () { return this->offset;   }
int         Symbol :: g_address  () { return this->address;  }


void Symbol :: s_name     (std::string name) { this->name = name; }
void Symbol :: s_absolute (bool absolute)    { this->absolute = absolute; }
void Symbol :: s_offset   (int offset)       { this->offset = offset; }
void Symbol :: s_address  (int address)      { this->address = address; }


Symbol& Symbol :: operator= (Symbol symbol) {
    this->name = symbol.g_name();
    this->absolute = symbol.g_absolute();
    this->address = symbol.g_address();
    this->offset = symbol.g_offset();
    return *this;
}


SymbolTable :: SymbolTable () {
    this->previous = NULL;
    this->next = NULL;
    this->last = this;
    this->next_free_offset = 0;
}


void SymbolTable :: push () {
    SymbolTable * next = this;
    while (next->next != NULL) {
        next = next->next;
    }
    next->next = new SymbolTable();
    next->next->previous = next;
    this->last = next->next;
}


void SymbolTable :: pop () {
    SymbolTable * tmp;
    
    if (this->last == this)
        throw Exception("tried to pop top layer of symbol table");
    
    tmp = this->last;
    this->last = this->last->previous;
    this->last->next = NULL;
    
    delete tmp;
}


int SymbolTable :: add_symbol (std::string name, bool absolute) {
    if (! absolute) {
        this->symbols[name] = Symbol(name);
        this->symbols[name].s_absolute(false);
        this->symbols[name].s_offset(this->next_free_offset);
        this->next_free_offset++;
        return 1;
    }
    return 0;
}

    
bool SymbolTable :: symbol_exists (std::string name) {
    // is it in the last table, IE the stack table?
    // static variables not implemented yet
    if (this->last->symbols.count(name) == 1)
        return true;
    return false;
}


int SymbolTable :: g_symbol_offset (std::string name) {
    return this->symbols[name].g_offset();
}


bool SymbolTable :: g_symbol_absolute (std::string name) {
    return this->symbols[name].g_absolute();
}


int SymbolTable :: g_symbol_address (std::string name) {
    return this->symbols[name].g_address();
}