#include "compiler.hpp"

Compiler :: Compiler (ASTree * tree) {
    this->tree = tree;
}


void Compiler :: free_instructions () {
    std::list <Instruction *> :: iterator instruction_it;
    std::list <MemoryDefinition *> :: iterator def_it;
    
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
void Compiler :: load_immediate (Register * reg, int immediate) {
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


void Compiler :: declare (ASTreeExprVar * var) {
    std::cout << "declare " << var->g_symbol()->g_symbol() << "\n";
    int size = 1;
    ASTreeExprConstant * constant;
    ASTreeSymbol * symbol;
    symbol = var->g_symbol();
    
    if (this->symbolTable.symbol_exists(symbol->g_symbol()))
        throw Exception ("tried to declare already existing symbol");
    if (var->g_expr() != NULL) {
        if ((constant = dynamic_cast<ASTreeExprConstant *>(var->g_expr()))
            == false)
            throw Exception ("array declaration sizes must be static!");
        else {
            std::cout << "declare " << var->g_symbol()->g_symbol() 
                      << " " << constant->g_constant() << "\n";
            size = constant->g_constant();
        }
    }
        
    this->symbolTable.add_symbol(symbol->g_symbol(), size);
}


void Compiler :: condition (ASTreeCondition * condition,
                             std::string label_true,
                             std::string label_false,
                             bool force_load) {
    Instruction * instruction;
    Register * reg;
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
            break;
        case AST_GREATER_THAN :
            instruction = new Instruction(OP_SUBX);
            instruction->s_RD(right);
            instruction->s_RS1(right);
            instruction->s_RS2(left);
            this->instructions.push_back(instruction);
            instruction = new Instruction(OP_BN);
            instruction->s_LABEL(label_true);
            this->instructions.push_back(instruction);
            break;
        default :
            throw Exception ("Compiler encountered unknown condition");
    }
    
    left->free();
    right->free();
    
    // these jmps tend to be large, so we use the 16-bit register JMP instead of
    // the 8-bit BA
    reg = this->symbolTable.get_free_register_ptr();
    reg->use();
    instruction = new Instruction(OP_LDI);
    instruction->s_RD(reg);
    instruction->s_LABEL(label_false);
    this->instructions.push_back(instruction);
    instruction = new Instruction(OP_JMP);
    instruction->s_RS2(reg);
    this->instructions.push_back(instruction);
    reg->free();
    

}

    
void Compiler :: treeIf (ASTreeIf * treeIf) {
    Instruction * instruction;
    std::string label_true;
    std::string label_false;
    
    label_true  = this->labelMaker.label();
    label_false = this->labelMaker.label();
    
    this->condition(treeIf->g_condition(), label_true, label_false);
    
    instruction = new Instruction(OP_LABEL);
    instruction->s_LABEL(label_true);
    instruction->s_COMMENT(treeIf->g_comment());
    this->instructions.push_back(instruction);
    
    this->statement(treeIf->g_statement());
    
    instruction = new Instruction(OP_LABEL);
    instruction->s_LABEL(label_false);
    this->instructions.push_back(instruction);
}
            
    
void Compiler :: treeWhile (ASTreeWhile * treeWhile) {
    Register * reg;
    Instruction * instruction;
    std::string label_true; // if condition is true
    std::string label_false; // if condition is false
    std::string label_jump; // jump back to beginning of while (it is a loop :p)
    
    label_true  = this->labelMaker.label();
    label_false = this->labelMaker.label();
    label_jump  = this->labelMaker.label();
    
    instruction = new Instruction(OP_LABEL);
    instruction->s_LABEL(label_jump);
    instruction->s_COMMENT(treeWhile->g_comment());
    this->instructions.push_back(instruction);
    
    this->condition(treeWhile->g_condition(),
                    label_true,
                    label_false,
                    true);
    
    instruction = new Instruction(OP_LABEL);
    instruction->s_LABEL(label_true);
    this->instructions.push_back(instruction);
    
    this->statement(treeWhile->g_statement());
    
    // these tend to be very long jumps, so we'll use the jmp instruction
    // which allows us to jmp to a 16-bit register instead of BA which
    // restricts us to 8-bit constants
    reg = this->symbolTable.get_free_register_ptr();
    reg->use();
    instruction = new Instruction(OP_LDI);
    instruction->s_RD(reg);
    instruction->s_LABEL(label_jump);
    this->instructions.push_back(instruction);
    instruction = new Instruction(OP_JMP);
    instruction->s_RS2(reg);
    this->instructions.push_back(instruction);
    reg->free();
    
    instruction = new Instruction(OP_LABEL);
    instruction->s_LABEL(label_false);
    this->instructions.push_back(instruction);
}


Register * Compiler :: variable_address (ASTreeExprVar * var, bool force_load) {
    std::cout << "variable_address\n";
    int offset;
    ASTreeSymbol * symbol;
    Register * reg;
    Register * reg2;
    Instruction * instruction;
    
    symbol = var->g_symbol();
    if (this->symbolTable.symbol_exists(symbol->g_symbol()) == false)
        throw Exception("tried to get address of non-existant symbol");
        
    // register to hold variable address
    reg = this->symbolTable.get_free_register_ptr();
    reg->use();
    
    // start with base pointer
    instruction = new Instruction(OP_ORX);
    instruction->s_RD(reg);
    instruction->s_RS1(this->symbolTable.g_base_pointer_ptr());
    instruction->s_RS2(this->symbolTable.g_r0_ptr());
    instruction->s_COMMENT(var->g_comment());
    this->instructions.push_back(instruction);
    
    // subtract offset from base pointer
    offset = this->symbolTable.g_symbol_offset(symbol->g_symbol());
    // if offset >= 256, then we can't load it as an immediate and have to put
    // it in a register first
    if (offset >= 256) {
        reg2 = this->symbolTable.get_free_register_ptr();
        reg2->use();
        this->load_immediate(reg2, offset);
        instruction = new Instruction(OP_SUBX);
        instruction->s_RD(reg);
        instruction->s_RS1(reg);
        instruction->s_RS2(reg2);
        this->instructions.push_back(instruction);
        reg2->free();
    }
    else {
        instruction = new Instruction(OP_ADDI);
        instruction->s_RD(reg);
        instruction->s_IMM(0 - this->symbolTable.g_symbol_offset(symbol->g_symbol()));
        this->instructions.push_back(instruction);
    }
    
    // add expr if any (arrays)
    if (var->g_expr() != NULL) {
        reg2 = this->expression(var->g_expr(), force_load);
        instruction = new Instruction(OP_ADDX);
        instruction->s_RD(reg);
        instruction->s_RS1(reg);
        instruction->s_RS2(reg2);
        this->instructions.push_back(instruction);
        reg2->free();
    }
        
    return reg;
}


Register * Compiler :: expression (ASTreeExpr * expression, bool force_load) {
    std::cout << "expression\n";
    ASTreeExprArithmetic * arithmetic;
    ASTreeExprConstant * constant;
    ASTreeExprString * string;
    ASTreeExprVar * var;
    ASTreeSymbol * treeSymbol;
    Instruction * instruction;
    Register * reg;
    Register * reg2;
    Symbol * symbol;
    
    std::string label;
    
    // if this is just a constant, put it in a register
    if ((constant = dynamic_cast<ASTreeExprConstant *>(expression))) {
        std::cout << " ASTreeExprConstant\n";
        reg = this->symbolTable.get_free_register_ptr();
        reg->use();
        this->load_immediate(reg, constant->g_constant());
        return reg;
    }
    // if this is a variable, we need to do some symboltable stuff
    else if ((var = dynamic_cast<ASTreeExprVar *>(expression))) {
        std::cout << " ASTreeExprVar\n";
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
            reg = this->variable_address(var, force_load);
            
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
        std::cout << " ASTreeExprArithmetic\n";
        switch (arithmetic->g_operation()) {
            case AST_ADD :
                instruction = new Instruction(OP_ADDX);
                break;
            case AST_SUBTRACT :
                instruction = new Instruction(OP_SUBX);
                break;
            default :
                throw Exception("compiler found unknown arithmetic operation ");
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
    else if ((string = dynamic_cast<ASTreeExprString *>(expression))) {
        std::cout << " ASTreeExprString\n";
        label = this->labelMaker.label();
        this->definitions.push_back(MemoryDefinition(MEMORYDEFINITION_STRING, 
                                                     label,
                                                     string->g_string()));
        reg = this->symbolTable.get_free_register_ptr();
        reg->use();
        instruction = new Instruction(OP_LDI);
        instruction->s_RD(reg);
        instruction->s_LABEL(label);
        this->instructions.push_back(instruction);
        return reg;
    }
    throw (Exception("compiler could not evaluate expression"));
    return NULL;
}


void Compiler :: assign (ASTreeAssign * assign) {
    std::cout << "assign\n";
    Instruction * instruction;
    Register * rhs;
    Register * lhs_address;
    
    instruction = new Instruction(OP_COMMENT);
    instruction->s_COMMENT(assign->g_comment());
    this->instructions.push_back(instruction);
    
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
    

void Compiler :: statement (ASTreeStatement * statement) {
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
        else if ((dynamic_cast<ASTreeExprVar *>(*node_it))
                 && (dynamic_cast<ASTreeExprVar *>(*node_it)->g_decl() != NULL))
            this->declare(dynamic_cast<ASTreeExprVar *>(*node_it));
    }
}


void Compiler :: compile () {
    std::cout << "compile\n";
    if (dynamic_cast<ASTreeStatement *>(this->tree))
        this->statement(dynamic_cast<ASTreeStatement *>(this->tree));
    else
        throw Exception("assembly tree wasn't statement");
}

std::list <Instruction *> Compiler :: g_instructions () {
    return this->instructions; }
std::list <MemoryDefinition> Compiler :: g_definitions () {
    return this->definitions; }
