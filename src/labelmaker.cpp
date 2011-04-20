#include "labelmaker.hpp"

LabelMaker :: LabelMaker () {
    this->next_label = 0;
}

std::string LabelMaker :: label () {
    std::stringstream s;
    
    s << "L_" << this->next_label++;
    
    return s.str();
}
