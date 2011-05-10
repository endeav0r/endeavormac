#ifndef memorydefinition_header
#define memorydefinition_header

#include <string>

enum { MEMORYDEFINITION_STRING };

class MemoryDefinition {
    private :
        int type;
        std::string string;
        std::string label;
    public :
        MemoryDefinition (int type);
        MemoryDefinition (int type, std::string label, std::string string);
        int         g_type   ();
        std::string g_label  ();
        std::string g_string ();
        std::string assembly ();
};

#endif
