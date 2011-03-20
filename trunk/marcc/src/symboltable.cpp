#include "symboltable.hpp"


SymbolTable :: SymbolTable () {
    int i;
    for (i = 0; i < GENERAL_PURPOSE_REGISTERS; i++)
        this->registers[i].s_id(i+1);
    this->previous = NULL;
    this->next = NULL;
    this->last = this;
    this->next_free_offset = 0;
}


int SymbolTable :: get_free_register () {
    int i;
    
    // first find a register without a symbol
    for (i = 0; i < GENERAL_PURPOSE_REGISTERS; i++) {
        if ((this->registers[i].g_free())
            && (this->registers[i].has_symbol() == false))
            return i+1;
    }
    
    // find any free register
    for (i = 0; i < GENERAL_PURPOSE_REGISTERS; i++) {
        if ((this->registers[i].g_free())
            && (this->registers[i].has_symbol() == false))
            return i+1;
    }
    throw Exception("no registers free");
    return -1;
}


void SymbolTable :: use_register (int reg) {
    if (! this->registers[reg - 1].g_free())
        throw Exception("tried to use an already used register");
    this->registers[reg - 1].use();
}


void SymbolTable :: free_register (int reg) {
    if (this->registers[reg - 1].g_free())
        throw Exception("tried to free already free register");
    this->registers[reg - 1].free();
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


int        SymbolTable :: g_symbol_offset   (std::string name) {
       	    return this->symbols[name].g_offset();   }
       	
bool       SymbolTable :: g_symbol_absolute (std::string name) {
            return this->symbols[name].g_absolute(); }
        
int        SymbolTable :: g_symbol_address  (std::string name) {
            return this->symbols[name].g_address();  }
        
Symbol &   SymbolTable :: g_symbol          (std::string name) {
            return this->symbols[name];              }

Register * SymbolTable :: g_register_ptr    (int reg) {
			return &(this->registers[reg - 1]);      }
