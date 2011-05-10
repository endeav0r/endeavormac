#include "memorydefinition.hpp"

MemoryDefinition :: MemoryDefinition (int type) {
    this->type = type;
}


MemoryDefinition :: MemoryDefinition (int type,
                                      std::string label,
                                      std::string string) {
    this->type = type;
    this->label = label;
    this->string = string;
}


int         MemoryDefinition :: g_type   () { return this->type; }
std::string MemoryDefinition :: g_label  () { return this->label; }
std::string MemoryDefinition :: g_string () { return this->string; }

std::string MemoryDefinition :: assembly () {
    std::string result;
    switch (this->type) {
        case MEMORYDEFINITION_STRING :
            result = this->label + " : string \"" + this->string + "\"";
            return result;
    }
    return "";
}
