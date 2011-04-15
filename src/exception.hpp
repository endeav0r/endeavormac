#ifndef exception_HEADER
#define exception_HEADER

#include <string>

class Exception {
    
    private :
        std::string description;
        
    public :
        Exception (std::string description);
        
        std::string g_description();
        
};

#endif
