#include "symboltable.hpp"


SymbolTable :: SymbolTable () {
    int i;
    for (i = 0; i < GENERAL_PURPOSE_REGISTERS; i++)
        this->registers[i].s_id(i+1);
    this->previous = NULL;
    this->next = NULL;
    this->last = this;
    this->next_free_offset = 0;
    
    this->r0.s_id(0);
    this->stack_pointer.s_id(REG_STACK_POINTER);
    this->base_pointer.s_id(REG_BASE_POINTER);
}


Register * SymbolTable :: get_free_register_ptr () {
    int i;
    
    // first find a register without a symbol
    for (i = 0; i < GENERAL_PURPOSE_REGISTERS; i++) {
        if ((this->registers[i].g_free())
            && (this->registers[i].has_symbol() == false))
            return &(this->registers[i]);
    }
    
    // find any free register
    for (i = 0; i < GENERAL_PURPOSE_REGISTERS; i++) {
        if ((this->registers[i].g_free())
            && (this->registers[i].has_symbol() == false))
            return &(this->registers[i]);
    }
    throw Exception("no registers free");
    return NULL;
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


void SymbolTable :: add_symbol (std::string name, int stack_space) {
    this->last->symbols[name] = Symbol(name);
    this->last->symbols[name].s_offset(this->last->next_free_offset + stack_space);
    this->last->next_free_offset += stack_space;
}

    
bool SymbolTable :: symbol_exists (std::string name) {
    if (this->last->symbols.count(name) == 1)
        return true;
    return false;
}


int        SymbolTable :: g_symbol_offset   (std::string name) {
       	    return this->last->symbols[name].g_offset();   }
        
Symbol *   SymbolTable :: g_symbol_ptr      (std::string name) {
            return &(this->last->symbols[name]);           }

Register * SymbolTable :: g_register_ptr    (int reg) {
			return &(this->registers[reg - 1]);            }

Register * SymbolTable :: g_r0_ptr () {
            return &(this->r0); }

Register * SymbolTable :: g_base_pointer_ptr () {
            return &(this->base_pointer); }

Register * SymbolTable :: g_stack_pointer_ptr () {
            return &(this->stack_pointer); }
