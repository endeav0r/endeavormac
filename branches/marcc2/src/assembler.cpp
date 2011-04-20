#include "assembler.hpp"

Assembler :: Assembler (ASTree * tree) {
    this->tree = tree;
}


void Assembler :: free_instructions () {
    std::list <Instruction *> :: iterator instruction_it;
    
    for (instruction_it  = this->instructions.begin();
         instruction_it != this->instructions.end();
         instruction_it++) {
        delete *instruction_it;
    }
}


/**
 * loads the value immediate into Register * pointed to by reg
 * @param reg A pointer to a register WHICH HAS ALREADY been marked as used
 * @param immediate Value to load into register (these bits: 0x0000FFFF)
 */
void Assembler :: load_immediate (Register * reg, int immediate) {
    std::cout << "load_immediate\n";
    Instruction * instruction;
    instruction = new Instruction(OP_SUBX);
    instruction->s_RD(reg);
    instruction->s_RS1(reg);
    instruction->s_RS2(reg);
    this->instructions.push_back(instruction);
    instruction = new Instruction(OP_SETHI);
    instruction->s_RD(reg);
    instruction->s_IMM((immediate >> 8) & 0x000000FF);
    this->instructions.push_back(instruction);
    instruction = new Instruction(OP_ADDI);
    instruction->s_RD(reg);
    instruction->s_IMM(immediate & 0x000000FF);
    this->instructions.push_back(instruction);
}


void Assembler :: declare (ASTreeExprVar * var) {
    std::cout << "declare\n";
    ASTreeSymbol * symbol;
    symbol = var->g_symbol();
    // everything for now is just a 2 byte var
    if (this->symbolTable.symbol_exists(symbol->g_symbol()))
        throw Exception ("tried to declare already existing symbol");
    this->symbolTable.add_symbol(symbol->g_symbol());
}


void Assembler :: condition (ASTreeCondition * condition,
                             std::string label_true,
                             std::string label_false,
                             bool force_load) {
    Instruction * instruction;
    Register * left;
    Register * right;
    
    // evaluate the right and left side
    left = this->expression(condition->g_left());
    right = this->expression(condition->g_right());
    
    // now what are we evaluating
    switch (condition->g_op()) {
        case AST_EQUALS :
            instruction = new Instruction(OP_SUBX);
            instruction->s_RD(left);
            instruction->s_RS1(left);
            instruction->s_RS2(right);
            this->instructions.push_back(instruction);
            instruction = new Instruction(OP_BZ);
            instruction->s_LABEL(label_true);
            this->instructions.push_back(instruction);
            instruction = new Instruction(OP_BA);
            instruction->s_LABEL(label_false);
            this->instructions.push_back(instruction);
            break;
        case AST_LESS_THAN :
            instruction = new Instruction(OP_SUBX);
            instruction->s_RD(left);
            instruction->s_RS1(left);
            instruction->s_RS2(right);
            this->instructions.push_back(instruction);
            instruction = new Instruction(OP_BN);
            instruction->s_LABEL(label_true);
            this->instructions.push_back(instruction);
            instruction = new Instruction(OP_BA);
            instruction->s_LABEL(label_false);
            this->instructions.push_back(instruction);
            break;
        default :
            throw Exception ("assembler encountered unknown condition");
    }
    
    left->free();
    right->free();
}

    
void Assembler :: treeIf (ASTreeIf * treeIf) {
    Instruction * instruction;
    std::string label_true;
    std::string label_false;
    
    label_true  = this->labelMaker.label();
    label_false = this->labelMaker.label();
    
    this->condition(treeIf->g_condition(), label_true, label_false);
    
    instruction = new Instruction(OP_LABEL);
    instruction->s_LABEL(label_true);
    this->instructions.push_back(instruction);
    
    this->statement(treeIf->g_statement());
    
    instruction = new Instruction(OP_LABEL);
    instruction->s_LABEL(label_false);
    this->instructions.push_back(instruction);
}
            
    
void Assembler :: treeWhile (ASTreeWhile * treeWhile) {
    Instruction * instruction;
    std::string label_true; // if condition is true
    std::string label_false; // if condition is false
    std::string label_jump; // jump back to beginning of while (it is a loop :p)
    
    label_true  = this->labelMaker.label();
    label_false = this->labelMaker.label();
    label_jump  = this->labelMaker.label();
    
    instruction = new Instruction(OP_LABEL);
    instruction->s_LABEL(label_jump);
    this->instructions.push_back(instruction);
    
    this->condition(treeWhile->g_condition(),
                    label_true,
                    label_false,
                    true);
    
    instruction = new Instruction(OP_LABEL);
    instruction->s_LABEL(label_true);
    this->instructions.push_back(instruction);
    
    this->statement(treeWhile->g_statement());
    
    instruction = new Instruction(OP_BA);
    instruction->s_LABEL(label_jump);
    this->instructions.push_back(instruction);
    
    instruction = new Instruction(OP_LABEL);
    instruction->s_LABEL(label_false);
    this->instructions.push_back(instruction);
}


Register * Assembler :: variable_address (ASTreeExprVar * var) {
    std::cout << "variable_address\n";
    ASTreeSymbol * symbol;
    Register * reg;
    Instruction * instruction;
    
    symbol = var->g_symbol();
    if (this->symbolTable.symbol_exists(symbol->g_symbol()) == false)
        throw Exception("tried to get address of non-existant symbol");
    reg = this->symbolTable.get_free_register_ptr();
    reg->use();
    
    instruction = new Instruction(OP_ORX);
    instruction->s_RD(reg);
    instruction->s_RS1(this->symbolTable.g_base_pointer_ptr());
    instruction->s_RS2(this->symbolTable.g_r0_ptr());
    this->instructions.push_back(instruction);
    
    instruction = new Instruction(OP_ADDI);
    instruction->s_RD(reg);
    instruction->s_IMM(this->symbolTable.g_symbol_offset(symbol->g_symbol()));
    this->instructions.push_back(instruction);
    
    return reg;
}


Register * Assembler :: expression (ASTreeExpr * expression, bool force_load) {
    std::cout << "expression\n";
    ASTreeExprArithmetic * arithmetic;
    ASTreeExprConstant * constant;
    ASTreeExprVar * var;
    ASTreeSymbol * treeSymbol;
    Instruction * instruction;
    Register * reg;
    Register * reg2;
    Symbol * symbol;
    
    // if this is just a constant, put it in a register
    if ((constant = dynamic_cast<ASTreeExprConstant *>(expression))) {
        reg = this->symbolTable.get_free_register_ptr();
        reg->use();
        this->load_immediate(reg, constant->g_constant());
        return reg;
    }
    // if this is a variable, we need to do some symboltable stuff
    else if ((var = dynamic_cast<ASTreeExprVar *>(expression))) {
        treeSymbol = var->g_symbol();
        
        // this is not an appropriate place for variable declarations
        if (var->g_decl() != NULL)
            throw Exception ("Tried to evaluate variable declaration");
        
        // let's make sure this symbol actualyl exists
        if (this->symbolTable.symbol_exists(treeSymbol->g_symbol()) == false)
            throw Exception (std::string("Tried to evaluate variable but ")
                                         + "symbol was not in symbol table:"
                                         + treeSymbol->g_symbol());
        
        symbol = this->symbolTable.g_symbol_ptr(treeSymbol->g_symbol());
        // if this symbol is currently loaded into a register, then our job
        // becomes very easy
        if ((symbol->register_isset()) && (! force_load)) {
            reg = symbol->g_register_ptr();
            reg->use();
            reg->s_symbol(symbol);
            return reg;
        }
        else {
            // find offset from base pointer
            reg = this->variable_address(var);
            
            // load memory at that address into register
            instruction = new Instruction(OP_LD);
            instruction->s_RD(reg);
            instruction->s_RS1(reg);
            this->instructions.push_back(instruction);
            
            // link this register and symbol
            symbol->s_register_ptr(reg);
            
            return reg;
        }
    }
    else if ((arithmetic = dynamic_cast<ASTreeExprArithmetic *>(expression))) {
        switch (arithmetic->g_operation()) {
            case AST_ADD :
                instruction = new Instruction(OP_ADDX);
                break;
            case AST_SUBTRACT :
                instruction = new Instruction(OP_SUBX);
                break;
            default :
                throw Exception("assembler found unknown arithmetic operation ");
        }
        reg = this->expression(arithmetic->g_left());
        instruction->s_RD(reg);
        instruction->s_RS1(reg);
        reg2 = this->expression(arithmetic->g_right());
        instruction->s_RS2(reg2);
        reg2->free();
        this->instructions.push_back(instruction);
        return reg;
    }
    throw ("assembler could not evaluate expression");
    return NULL;
}


void Assembler :: assign (ASTreeAssign * assign) {
    std::cout << "assign\n";
    Instruction * instruction;
    Register * rhs;
    Register * lhs_address;
    
    // if this is a declaration, let's declare that variable
    if (assign->g_left()->g_decl() != NULL)
        this->declare(assign->g_left());
    
    // now we need to evaluate the rhs and place it in a register
    rhs = this->expression(assign->g_right());
    
    // and get the address to where we're going to store the lhs
    lhs_address = this->variable_address(assign->g_left());
    
    instruction = new Instruction(OP_ST);
    instruction->s_RD(lhs_address);
    instruction->s_RS1(rhs);
    this->instructions.push_back(instruction);
    
    // shouldn't need those registers anymore
    rhs->free();
    lhs_address->free();
}
    

void Assembler :: statement (ASTreeStatement * statement) {
    std::cout << "statement\n";
    std::list <ASTree *> nodes;
    std::list <ASTree *> :: iterator node_it;
    
    nodes = statement->g_nodes();
    
    for (node_it  = nodes.begin();
         node_it != nodes.end();
         node_it++) {
        if (dynamic_cast<ASTreeStatement *>(*node_it))
            this->statement(dynamic_cast<ASTreeStatement *>(*node_it));
        else if (dynamic_cast<ASTreeAssign *>(*node_it))
            this->assign(dynamic_cast<ASTreeAssign *>(*node_it));
        else if (dynamic_cast<ASTreeIf *>(*node_it))
            this->treeIf(dynamic_cast<ASTreeIf *>(*node_it));
        else if (dynamic_cast<ASTreeWhile *>(*node_it))
            this->treeWhile(dynamic_cast<ASTreeWhile *>(*node_it));
    }
}


void Assembler :: assemble () {
    std::cout << "assemble\n";
    if (dynamic_cast<ASTreeStatement *>(this->tree))
        this->statement(dynamic_cast<ASTreeStatement *>(this->tree));
    else
        throw Exception("assembly tree wasn't statement");
}

std::list <Instruction *> Assembler :: g_instructions () {
    return this->instructions; }
