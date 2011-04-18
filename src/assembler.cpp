#include "assembler.hpp"

Assembler :: Assembler (AbstractSyntaxTree tree) {
    this->tree = tree;
}


void Assembler :: push_register (int reg) {
    Instruction store(OP_ST);
    Instruction decrement(OP_ADDI);
    
    store.s_RD(REG_STACK_POINTER);
    store.s_RS1(reg);
    
    decrement.s_RD(REG_STACK_POINTER);
    decrement.s_IMM((0x00000001 ^ 0xFFFFFFFF) + 1);
    
    this->instructions.push_back(store);
    this->instructions.push_back(decrement);
}


void Assembler :: pop_register (int reg) {
    Instruction increment(OP_ADDI);
    Instruction load(OP_LD);
    
    increment.s_RD(REG_STACK_POINTER);
    increment.s_IMM(0x00000001);
    
    load.s_RD(reg);
    load.s_RS1(REG_STACK_POINTER);
    
    this->instructions.push_back(increment);
    this->instructions.push_back(load);
}


void Assembler :: load_immediate (int reg, int imm) {
    Instruction comment(OP_COMMENT);
    Instruction zero(OP_SUBX);
    Instruction sethi(OP_SETHI);
    Instruction addi(OP_ADDI);
    
    comment.s_COMMENT("load_immediate");
    
    zero.s_RD(reg);
    zero.s_RS1(reg);
    zero.s_RS2(reg);
    
    sethi.s_RD(reg);
    sethi.s_IMM((imm >> 8) & 0x000000FF);
    
    addi.s_RD(reg);
    addi.s_IMM(imm & 0x000000FF);
    
    this->instructions.push_back(comment);
    this->instructions.push_back(zero);
    this->instructions.push_back(sethi);
    this->instructions.push_back(addi);
}


void Assembler :: mov (int reg_dest, int reg_src) {
    Instruction orx(OP_ORX);
    
    orx.s_RD(reg_dest);
    orx.s_RS1(reg_src);
    orx.s_RS2(0);
    
    this->instructions.push_back(orx);
}


void Assembler :: addx (int reg_dest, int reg_1, int reg_2) {
    Instruction add(OP_ADDX);
    
    add.s_RD(reg_dest);
    add.s_RS1(reg_1);
    add.s_RS2(reg_2);
    
    this->instructions.push_back(add);
}


void Assembler :: subx (int reg_dest, int reg_1, int reg_2) {
    Instruction sub(OP_SUBX);
    
    sub.s_RD(reg_dest);
    sub.s_RS1(reg_1);
    sub.s_RS2(reg_2);
    
    this->instructions.push_back(sub);
}


void Assembler :: bn (int imm) {
    Instruction bn(OP_BN);
    bn.s_IMM(imm);
    this->instructions.push_back(bn);
}


void Assembler :: ba (int imm) {
    Instruction ba(OP_BA);
    ba.s_IMM(imm);
    this->instructions.push_back(ba);
}


void Assembler :: bz (int imm) {
    Instruction bz(OP_BZ);
    bz.s_IMM(imm);
    this->instructions.push_back(bz);
}


void Assembler :: symbol_st (std::string name, int reg) {
    Instruction comment(OP_COMMENT);
    Instruction st(OP_ST);
    int reg1, reg2;
    
    Symbol & symbol = this->table.g_symbol(name);
    symbol.s_register(this->table.g_register_ptr(reg));
    
    comment.s_COMMENT("symbol st");
    // the value of symbol <name> is in <reg>, set it up make it so
    this->table.g_symbol(name).s_register(this->table.g_register_ptr(reg));
    
    if (! this->table.symbol_exists(name))
        throw Exception(std::string("tried to set symbol ") + name + " but it wasn't found");
    
    if (! this->table.g_symbol_absolute(name)) {
        // we need a couple general purpose registers
        reg1 = this->table.get_free_register();
        this->table.use_register(reg1);
        reg2 = this->table.get_free_register();
        this->table.use_register(reg2);
        
        // reg1 gets symbol offset
        // reg2 gets bp + reg1
        // reg2 => the address of the symbol
        this->load_immediate(reg1, this->table.g_symbol_offset(name));
        
        this->instructions.push_back(comment);
        this->addx(reg2, reg1, REG_BASE_POINTER);
        
        st.s_RD(reg2);
        st.s_RS1(reg);
        this->instructions.push_back(st);
        
        this->table.free_register(reg1);
        this->table.free_register(reg2);
    }
}


void Assembler :: symbol_ld (std::string name, int reg) {
    Instruction comment(OP_COMMENT);
    Instruction ld(OP_LD);
    int reg1, reg2;
    
    Symbol & symbol = this->table.g_symbol(name);
    symbol.s_register(this->table.g_register_ptr(reg));
    
    comment.s_COMMENT("symbol ld");
    
    if (! this->table.symbol_exists(name))
        throw Exception(std::string("tried to get symbol ") + name + " but it wasn't found");
    
    if (! this->table.g_symbol_absolute(name)) {
        // set registers as used
        reg1 = this->table.get_free_register();
        this->table.use_register(reg1);
        reg2 = this->table.get_free_register();
        this->table.use_register(reg2);
        
        // put offset into reg1
        this->load_immediate(reg1, this->table.g_symbol_offset(name));
        
        this->instructions.push_back(comment);
        // add offset to base pointer, place in reg2
        this->addx(reg2, reg1, REG_BASE_POINTER);
        
        // ld instruction, loading memory at reg2 into reg
        ld.s_RD(reg);
        ld.s_RS1(reg2);
        this->instructions.push_back(ld);
        
        // free tmp registers
        this->table.free_register(reg1);
        this->table.free_register(reg2);
    }
}


int Assembler :: symbol_to_register (std::string name) {
    int reg;
    Instruction comment(OP_COMMENT);
    Symbol & symbol = this->table.g_symbol(name);
    
    if (symbol.register_isset()) {
        reg = symbol.g_register();
        this->table.use_register(reg);
        comment.s_COMMENT(std::string("symbol " + name + " in reg " + (char) ('0' + reg)));
        this->instructions.push_back(comment);
    }
    else {
        reg = this->table.get_free_register();
        this->table.use_register(reg);
        comment.s_COMMENT(std::string("loading symbol " + name + " into reg " + (char) ('0' + reg)));
        this->instructions.push_back(comment);
        this->symbol_ld(name, reg);
    }
    return reg;
}

void Assembler :: assemble () {
    ASTree * tree = this->tree;
    
}
