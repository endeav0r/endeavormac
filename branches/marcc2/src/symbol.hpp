#ifndef symbol_HEADER
#define symbol_HEADER

#include <string>

#include "exception.hpp"
#include "register.hpp"

class Register;

class Symbol {
    
    private :
        std::string name;
        bool absolute; // is this an absolute address, or relative to BP
                       // IE is it static/global or in the scope of the
                       // stack frame
        int  address;  // address or offset based on value of absolute
        int  offset;   // what's 4 bytes anyway, two names for clarity
        Register * reg;

    public :
        Symbol (std::string name);
        Symbol ();

		bool register_isset ();
		void register_free  ();

        std::string g_name     ();
        bool        g_absolute ();
        int         g_offset   ();
        int         g_address  ();
        int         g_register ();
        
        void s_name     (std::string name);
        void s_absolute (bool absolute);
        void s_offset   (int offset);
        void s_address  (int address);
        // sets this->reg to reg, and sets reg's symbol to this
        void s_register (Register * reg);
        
        Symbol& operator= (Symbol symbol);
        
};

#endif
