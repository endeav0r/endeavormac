#include "exception.hpp"


Exception :: Exception (std::string description) {
    this->description = description;
}


std::string Exception :: g_description () { return this->description; }