#ifndef instruction_HEADER
#define instruction_HEADER

#include <string>
#include <sstream>
#include <inttypes.h>

#include "exception.hpp"

enum { OP_NOP,
       OP_SUBX,
       OP_ORX,
       OP_JMP,
       OP_ADDX,
       OP_ANDX,
       OP_NOTX,
       OP_SRLX,
       OP_SLLX,
       OP_LD,
       OP_ST,
       OP_HLT,
       OP_RET,
       OP_ADDI,
       OP_BA,
       OP_BN,
       OP_BZ,
       OP_SETHI,
       OP_CALL,
       OP_COMMENT,
       OP_LABEL };

#define REG_STACK_POINTER 7
#define REG_BASE_POINTER  6

class Instruction {

    private :
        int FORMAT;
        int OP;
        int RD;
        int RS1;
        int RS2;
        std::string comment;
        std::string label;
        int IMM;
    
    public :
        Instruction ();
        Instruction (int operation);
        
        void s_RD  (int RD);
        void s_RS1 (int RS1);
        void s_RS2 (int RS2);
        void s_IMM (int IMM);
        void s_COMMENT (std::string comment);
        void s_LABEL   (std::string label);
        
        int g_OP ();
        
        uint16_t machine_code();
        std::string assembly();
        
};

#endif
