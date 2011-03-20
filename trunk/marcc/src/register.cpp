#include "register.hpp"


Register :: Register (int id) {
	this->id        = id;
	this->bool_free = true;
	this->symbol    = NULL;
}

Register :: Register () {
	this->bool_free = true;
	this->id        = -1;
	this->symbol    = NULL;
}

void Register :: free () {
	this->bool_free = true;
}

void Register :: use () { 
	this->bool_free = false;
	if (this->symbol != NULL) {
		this->symbol->register_free();
		this->symbol = NULL;
	}
}

bool Register :: has_symbol () { return this->symbol == NULL ? false : true; }

void Register :: s_id     (int id)          { this->id     = id;     }
void Register :: s_symbol (Symbol * symbol) { this->symbol = symbol; }

int  Register :: g_id   () { return this->id;        }
bool Register :: g_free () { return this->bool_free; }
