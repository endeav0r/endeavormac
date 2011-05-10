#include "instruction.hpp"


Instruction :: Instruction ()              { this->OP = OP_NOP; }
Instruction :: Instruction (int operation) {
    this->OP = operation;
    this->RD = NULL;
    this->RS1 = NULL;
    this->RS2 = NULL;
    this->comment = "";
    this->label = "";
    this->IMM = 0;
}


void Instruction :: s_RD      (Register * RD)       { this->RD  = RD;  }
void Instruction :: s_RS1     (Register * RS1)      { this->RS1 = RS1; }
void Instruction :: s_RS2     (Register * RS2)      { this->RS2 = RS2; }
void Instruction :: s_IMM     (int IMM)             { this->IMM = IMM; }
void Instruction :: s_COMMENT (std::string comment) { this->comment = comment; }
void Instruction :: s_LABEL   (std::string label  ) { this->label = label; }

int Instruction :: g_OP () { return this->OP; }

std::string Instruction :: assembly () {

    std::stringstream out_string;
    
    switch (this->OP) {
        case OP_NOP :
            return "\tNOP";
        case OP_SUBX :
            out_string << "\tSUBX  r" << this->RD->g_id() << ", r" << this->RS1->g_id() << ", r" << this->RS2->g_id();
            break;
        case OP_ORX :
            out_string << "\tORX   r" << this->RD->g_id() << ", r" << this->RS1->g_id() << ", r" << this->RS2->g_id();
            break;
        case OP_JMP :
            out_string << "\tJMP   r" << this->RS2->g_id();
            break;
        case OP_ADDX :
            out_string << "\tADDX  r" << this->RD->g_id() << ", r" << this->RS1->g_id() << ", r" << this->RS2->g_id();
            break;
        case OP_ANDX :
            out_string << "\tANDX  r" << this->RD->g_id() << ", r" << this->RS1->g_id() << ", r" << this->RS2->g_id();
            break;
        case OP_NOTX :
            out_string << "\tNOTX  r" << this->RD->g_id() << ", r" << this->RS1->g_id() << ", r" << this->RS2->g_id();
            break;
        case OP_SRLX :
            out_string << "\tSRLX  r" << this->RD->g_id() << ", r" << this->RS1->g_id();
            break;
        case OP_SLLX :
            out_string << "\tSLLX  r" << this->RD->g_id() << ", r" << this->RS1->g_id();
            break;
        case OP_LD :
            out_string << "\tLD    r" << this->RD->g_id() << ", [r" << this->RS1->g_id() << "]";
            break;
        case OP_ST :
            out_string << "\tST    [r" << this->RD->g_id() << "], r" << this->RS1->g_id();
            break;
        case OP_HLT :
            out_string << "HLT";
            break;
        case OP_RET :
            out_string << "RET";
            break;
        case OP_ADDI :
            out_string << "\tADDI  r" << this->RD->g_id() << ", " << this->IMM;
            break;
        case OP_BA :
            out_string << "\tBA    " << this->label;
            break;
        case OP_BN :
            out_string << "\tBN    " << this->label;
            break;
        case OP_BZ :
            out_string << "\tBZ    " << this->label;
            break;
        case OP_SETHI :
            out_string << "\tSETHI r" << this->RD->g_id() << ", " << this->IMM;
            break;
        case OP_CALL :
            out_string << "\tCALL  " << this->label;
            break;
        case OP_LDI :
            out_string << "\tLDI   r" << this->RD->g_id() << ", ";
            if (this->label != "")
                out_string << this->label;
            else
                out_string << this->IMM;
            break;
        case OP_LABEL :
            out_string << this->label << ":";
            break;
        case OP_COMMENT :
            break;
        default :
            throw Exception("Error creating assembly from instruction");
    }
    if (this->comment != "")
        out_string << " ; " << this->comment;
    return out_string.str();
}
