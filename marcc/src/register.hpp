#ifndef register_HEADER
#define register_HEADER


class Register {

    private :
        bool free;
    
    public :
        Register ();
        
        void s_free (bool free);
        bool g_free ();

};

#endif