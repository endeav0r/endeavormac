#ifndef instruction_HEADER
#define instruction_HEADER

#include <string>
#include <sstream>
#include <inttypes.h>

#include "exception.hpp"

#define OP_NOP   1
#define OP_SUBX  2
#define OP_ORX   3
#define OP_JMP   4
#define OP_ADDX  5
#define OP_ANDX  6
#define OP_NOTX  7
#define OP_SRLX  8
#define OP_SLLX  9
#define OP_LD    10
#define OP_ST    11
#define OP_HLT   12
#define OP_RET   13
#define OP_ADDI  14
#define OP_BA    15
#define OP_BN    16
#define OP_BZ    17
#define OP_SETHI 18
#define OP_CALL  19

#define OPCODE_NOP   0x0A
#define OPCODE_SUBX  0x09
#define OPCODE_ORX   0x08
#define OPCODE_JMP   0x07
#define OPCODE_ADDX  0x06
#define OPCODE_ANDX  0x05
#define OPCODE_NOTX  0x04
#define OPCODE_SRLX  0x03
#define OPCODE_SLLX  0x02
#define OPCODE_LD    0x01
#define OPCODE_ST    0x00

#define OPCODE_HLT   0x00
#define OPCODE_RET   0x01
#define OPCODE_ADDI  0x03
#define OPCODE_BA    0x04
#define OPCODE_BN    0x05
#define OPCODE_BZ    0x06
#define OPCODE_SETHI 0x07

#define REG_STACK_POINTER 7
#define REG_BASE_POINTER  6

class Instruction {

    private :
        int FORMAT;
        int OP;
        int RD;
        int RS1;
        int RS2;
        unsigned int IMM;
    
    public :
        Instruction ();
        Instruction (int operation);
        
        void s_RD  (int RD);
        void s_RS1 (int RS1);
        void s_RS2 (int RS2);
        void s_IMM (unsigned int IMM);
        
        uint16_t machine_code();
        std::string assembly();
        
};

#endif