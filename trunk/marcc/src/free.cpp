#include "register.hpp"


Register :: Register () {
    this->free = true;
}


void Register :: s_free (bool free) { this->free = free; }
bool Register :: g_free ()          { return this->free; }