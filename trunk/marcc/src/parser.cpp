#include "parser.hpp"





Parser :: Parser (std::list <Token> tokens) {
    this->tokens = tokens;
}


void Parser :: declare_variable (int bytes) {
    Instruction instruction(OP_ADDI);
    
    instruction.s_RD(REG_STACK_POINTER);
    instruction.s_IMM(bytes);
    this->instructions.push_back(instruction);
}


void Parser :: push_register (int reg) {
    Instruction store(OP_ST);
    Instruction decrement(OP_ADDI);
    
    store.s_RD(REG_STACK_POINTER);
    store.s_RS1(reg);
    
    decrement.s_RD(REG_STACK_POINTER);
    decrement.s_IMM((0x00000001 ^ 0xFFFFFFFF) + 1);
    
    this->instructions.push_back(store);
    this->instructions.push_back(decrement);
}


void Parser :: pop_register (int reg) {
    Instruction increment(OP_ADDI);
    Instruction load(OP_LD);
    
    increment.s_RD(REG_STACK_POINTER);
    increment.s_IMM(0x00000001);
    
    load.s_RD(reg);
    load.s_RS1(REG_STACK_POINTER);
    
    this->instructions.push_back(increment);
    this->instructions.push_back(load);
}


void Parser :: load_immediate (int reg, int imm) {
    Instruction zero(OP_SUBX);
    Instruction sethi(OP_SETHI);
    Instruction addi(OP_ADDI);
    
    zero.s_RD(reg);
    zero.s_RS1(reg);
    zero.s_RS2(reg);
    
    sethi.s_RD(reg);
    sethi.s_IMM((imm >> 8) & 0x000000FF);
    
    addi.s_RD(reg);
    addi.s_IMM(imm & 0x000000FF);
    
    this->instructions.push_back(zero);
    this->instructions.push_back(sethi);
    this->instructions.push_back(addi);
}


void Parser :: mov (int reg_dest, int reg_src) {
    Instruction orx(OP_ORX);
    
    orx.s_RD(reg_dest);
    orx.s_RS1(reg_src);
    orx.s_RS2(0);
    
    this->instructions.push_back(orx);
}


void Parser :: addx (int reg_dest, int reg_1, int reg_2) {
    Instruction add(OP_ADDX);
    
    add.s_RD(reg_dest);
    add.s_RS1(reg_1);
    add.s_RS2(reg_2);
    
    this->instructions.push_back(add);
}


void Parser :: set_symbol (std::string name, int value) {
    Instruction st(OP_ST);
    
    if (! this->table.symbol_exists(name))
        throw Exception(std::string("tried to set symbol ") + name + " but it wasn't found");
    
    if (! this->table.g_symbol_absolute(name)) {
        // push registers R4 and R5 onto the stack
        this->push_register(4);
        this->push_register(5);
        
        // r5 gets symbol offset
        // r4 gets base pointer + r4
        // r4 => the address of the symbol
        this->load_immediate(5, this->table.g_symbol_offset(name));
        this->addx(4, 5, REG_BASE_POINTER);
        
        // r5 gets the value
        std::cout << "setting 5 to " << value << "\n";
        this->load_immediate(5, value);
        st.s_RD(4);
        st.s_RS1(5);
        this->instructions.push_back(st);
        
        this->pop_register(5);
        this->pop_register(4);
    }
}


void Parser :: reduce (Token next) {
    int constant;
    
    bool reduce = true;
    std::list <ParserStack> :: iterator stack_it;
    
    while (reduce) {
        reduce = false;
        
        stack_it = this->stack.begin();
        // number
        if ((*stack_it).g_type() == PS_NUMBER) {
            constant = (*stack_it).g_constant();
            stack_it++;
            // assign number terminator
            if (((*stack_it).g_type() == PS_ASSIGN) 
                && (next.g_type() == TOKEN_TERMINATOR)) {
                // symbol assign number terminator
                stack_it++;
                if ((*stack_it).g_type() == PS_SYMBOL) {
                    this->set_symbol((*stack_it).g_name(), constant);
                    this->stack.pop_front();
                    this->stack.pop_front();
                    this->stack.pop_front();
                    reduce = true;
                }
            }
        }
        // terminator
        if ((*stack_it).g_type() == PS_TERMINATOR) {
            this->stack.pop_front();
            reduce = true;
        }
    }
}


void Parser :: parse () {

    Token next;
    std::list <Token> :: iterator token_it;
    std::list <Token> :: iterator stack_it;
    
    for (token_it  = this->tokens.begin();
         token_it != this->tokens.end();
         token_it++) {
        
        // all tokens are immediately shifted
        switch ((*token_it).g_type()) {
            case TOKEN_SYMBOL :
                // if this symbol doesn't exist, create it and zero it
                if (! this->table.symbol_exists((*token_it).g_text())) {
                    this->table.add_symbol((*token_it).g_text(), false);
                    //this->set_symbol((*token_it).g_text(), 0);
                }
                this->stack.push_front(ParserStack(PS_SYMBOL, (*token_it).g_text()));
                break;
                
            case TOKEN_EQUAL :
                this->stack.push_front(ParserStack(PS_ASSIGN));
                break;
            case TOKEN_NUMBER :
                this->stack.push_front(ParserStack(PS_NUMBER, (*token_it).g_text()));
                break;
            case TOKEN_TERMINATOR :
                this->stack.push_front(ParserStack(PS_TERMINATOR));
                break;
        }
        
        // grab the next token
        next = *(token_it++);
        token_it--;
        
        this->reduce(next);

    }
    
}


std::list <Instruction> Parser :: g_instructions () {
    return this->instructions;
}