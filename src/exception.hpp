#ifndef error_HEADER
#define error_HEADER

#include <string>

class Exception {
    
    private :
        std::string description;
        
    public :
        Exception (std::string description);
        
        std::string g_description();
        
};

#endif