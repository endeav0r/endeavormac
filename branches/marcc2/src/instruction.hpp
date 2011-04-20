#ifndef instruction_HEADER
#define instruction_HEADER

#include <string>
#include <sstream>
#include <inttypes.h>

#include "exception.hpp"
#include "register.hpp"

enum { OP_ADDI,
       OP_ADDX,
       OP_ANDX,
       OP_BA,
       OP_BN,
       OP_BZ,
       OP_CALL,
       OP_HLT,
       OP_JMP,
       OP_LD,
       OP_NOP,
       OP_NOTX,
       OP_ORX,
       OP_RET,
       OP_SETHI,
       OP_SLLX,
       OP_SRLX,
       OP_ST,
       OP_SUBX,
       // these are special "pseudo" instructions which are really more
       // just assembler directives
       OP_COMMENT,
       OP_LABEL };

class Instruction {

    private :
        int FORMAT;
        int OP;
        Register * RD;
        Register * RS1;
        Register  *RS2;
        std::string comment;
        std::string label;
        int IMM;
    
    public :
        Instruction ();
        Instruction (int operation);
        
        void s_RD  (Register * RD);
        void s_RS1 (Register * RS1);
        void s_RS2 (Register * RS2);
        void s_IMM (int IMM);
        void s_COMMENT (std::string comment);
        void s_LABEL   (std::string label);
        
        int g_OP ();
        
        uint16_t machine_code();
        std::string assembly();
        
};

#endif
