#ifndef labelmaker_HEADER
#define labelmaker_HEADER

#include <string>
#include <sstream>

/**
 * Originally i thought it would be a bit ridiculous to waste the time, space,
 * files and effort in order to create an entirely new class that would do
 * nothing but create strings used for labels by the assembler just so I could
 * have a class called Label Maker. It was at that precise moment I knew this
 * class had to be created.
 */

class LabelMaker {
    
    private :
        int next_label;
    
    public :
        LabelMaker ();
        std::string label();

};

#endif
