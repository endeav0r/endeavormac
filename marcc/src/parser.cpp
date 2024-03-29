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


void Parser :: subx (int reg_dest, int reg_1, int reg_2) {
    Instruction sub(OP_SUBX);
    
    sub.s_RD(reg_dest);
    sub.s_RS1(reg_1);
    sub.s_RS2(reg_2);
    
    this->instructions.push_back(sub);
}


void Parser :: bn (int imm) {
    Instruction bn(OP_BN);
    bn.s_IMM(imm);
    this->instructions.push_back(bn);
}


void Parser :: ba (int imm) {
    Instruction ba(OP_BA);
    ba.s_IMM(imm);
    this->instructions.push_back(ba);
}


void Parser :: bz (int imm) {
    Instruction bz(OP_BZ);
    bz.s_IMM(imm);
    this->instructions.push_back(bz);
}


void Parser :: symbol_st (std::string name, int reg) {
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


void Parser :: symbol_ld (std::string name, int reg) {
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


int Parser :: symbol_to_register (std::string name) {
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
        


void Parser :: reduce (Token next) {
    int constant;
    int reg, reg2;
    
    bool reduce = true;
    std::list <ParserStack> :: iterator stack_it;
    
    while (reduce) {
        reduce = false;
        
        stack_it = this->stack.begin();
        // register := number
        if ((*stack_it).g_type() == PS_NUMBER) {
            constant = (*stack_it).g_constant();
            reg = this->table.get_free_register();
            this->table.use_register(reg);
            this->stack.pop_front();
            this->load_immediate(reg, constant);
            this->stack.push_front(ParserStack(PS_REGISTER, reg));
            
            reduce = true;
        }
        // register
        else if ((*stack_it).g_type() == PS_REGISTER) {
            reg = (*stack_it).g_register();
            stack_it++;
            // add register
            if ((*stack_it).g_type() == PS_ADD) {
                stack_it++;
                // register := register add register
                if ((*stack_it).g_type() == PS_REGISTER) {
                    this->addx(reg, reg, (*stack_it).g_register());
                    this->table.free_register((*stack_it).g_register());
                    this->stack.pop_front();
                    this->stack.pop_front();
                    this->stack.pop_front();
                    this->stack.push_front(ParserStack(PS_REGISTER, reg));
                    reduce = true;
                }
                // register add register := symbol add register
                else if ((*stack_it).g_type() == PS_SYMBOL) {
                    reg2 = this->symbol_to_register((*stack_it).g_name());
                    this->stack.pop_front();
                    this->stack.pop_front();
                    this->stack.pop_front();
                    this->stack.push_front(ParserStack(PS_REGISTER, reg2));
                    this->stack.push_front(ParserStack(PS_ADD));
                    this->stack.push_front(ParserStack(PS_REGISTER, reg));
                    reduce = true;
                }
            }
            // equals register [+]
            else if (((*stack_it).g_type() == PS_EQUALS)
                     && (next.g_type() != TOKEN_PLUS)) {
                stack_it++;
                // condition := register equals register [+]
                if ((*stack_it).g_type() == PS_REGISTER) {
                    reg2 = (*stack_it).g_register();
                    this->subx(reg, reg, reg2);
                    this->bz(1);
                    this->ba(1);
                    this->jmp_stack.push(&(this->instructions.back()));
                    this->stack.pop_front();
                    this->stack.pop_front();
                    this->stack.pop_front();
                    this->stack.push_front(ParserStack(PS_CONDITION));
                    this->table.free_register(reg);
                    this->table.free_register(reg2);
                    reduce = true;
                }
                // register equals register := symbol equals register [+]
                else if ((*stack_it).g_register() == PS_SYMBOL) {
                    reg2 = this->table.get_free_register();
                    this->table.use_register(reg2);
                    this->symbol_ld((*stack_it).g_name(), reg2);
                    this->stack.pop_front();
                    this->stack.pop_front();
                    this->stack.pop_front();
                    this->stack.push_front(ParserStack(PS_REGISTER, reg2));
                    this->stack.push_front(ParserStack(PS_EQUALS));
                    this->stack.push_front(ParserStack(PS_REGISTER, reg));
                    reduce = true;
                }
            }
            // lessthan register [+]
            else if (((*stack_it).g_type() == PS_LESS_THAN)
                     && (next.g_type() != TOKEN_PLUS)) {
                stack_it++;
                // condition := register lessthan register [+]
                if ((*stack_it).g_type() == PS_REGISTER) {
                    reg2 = (*stack_it).g_register();
                    this->subx(reg, reg, reg2);
                    this->bn(1);
                    this->ba(1);
                    this->jmp_stack.push(&(this->instructions.back()));
                    this->stack.pop_front();
                    this->stack.pop_front();
                    this->stack.pop_front();
                    this->stack.push_front(ParserStack(PS_CONDITION));
                    this->table.free_register(reg);
                    this->table.free_register(reg2);
                    reduce = true;
                }
                // register lessthan register := symbol lessthan register [+]
                else if ((*stack_it).g_type() == PS_SYMBOL) {
                    reg2 = this->table.get_free_register();
                    this->table.use_register(reg2);
                    this->symbol_ld((*stack_it).g_name(), reg2);
                    this->stack.pop_front();
                    this->stack.pop_front();
                    this->stack.pop_front();
                    this->stack.push_front(ParserStack(PS_REGISTER, reg2));
                    this->stack.push_front(ParserStack(PS_LESS_THAN));
                    this->stack.push_front(ParserStack(PS_REGISTER, reg));
                    reduce = true;
                }
            }
        }
        // block_close
        else if ((*stack_it).g_type() == PS_BLOCK_CLOSE) {
            stack_it++;
            // block := block_open block_close
            if ((*stack_it).g_type() == PS_BLOCK_OPEN) {
                constant = (*stack_it).g_branch_address();
                this->stack.pop_front();
                this->stack.pop_front();
                this->stack.push_front(ParserStack(PS_BLOCK, constant));
                reduce = true;
            }
        }
        //block
        else if ((*stack_it).g_type() == PS_BLOCK) {
            constant = (*stack_it).g_branch_address();
            stack_it++;
            // condition block
            if ((*stack_it).g_type() == PS_CONDITION) {
                // if condition block
                stack_it++;
                if ((*stack_it).g_type() == PS_IF) {
                    // adjust ba from opening of this block
                    (*(this->jmp_stack.top())).s_IMM(this->instructions_size()
                                                     - constant);
                    this->jmp_stack.pop();
                    this->stack.pop_front();
                    this->stack.pop_front();
                    this->stack.pop_front();
                    reduce = true;
                }
                else if ((*stack_it).g_type() == PS_WHILE) {
                    std::cout << "debug branch address " << (*stack_it).g_branch_address() 
                    << " :: " << this->instructions_size() << "\n";
                    this->ba(0 - (this->instructions_size() - (*stack_it).g_branch_address()));
                    // adjust ba from opening of this block
                    (*(this->jmp_stack.top())).s_IMM(this->instructions_size()
                                                     - constant);
                    this->jmp_stack.pop();
                    this->stack.pop_front();
                    this->stack.pop_front();
                    this->stack.pop_front();
                    reduce = true;
                }
            }
        }
        // symbol 
        else if ((*stack_it).g_type() == PS_SYMBOL) {
            stack_it++;
            // add       register := add    symbol
            if ((*stack_it).g_type() == PS_ADD) {
                stack_it--;
                reg = this->symbol_to_register((*stack_it).g_name());
                this->stack.pop_front();
                this->stack.push_front(ParserStack(PS_REGISTER, reg));
                reduce = true;
            }
            // equals    register := equals symbol
            // less_than register := less_than symbol
            if (((*stack_it).g_type() == PS_EQUALS)
                || ((*stack_it).g_type() == PS_LESS_THAN)) {
                stack_it--;
                reg = this->table.get_free_register();
                this->table.use_register(reg);
                this->symbol_ld((*stack_it).g_name(), reg);
                this->stack.pop_front();
                this->stack.push_front(ParserStack(PS_REGISTER, reg));
                reduce = true;
            }
        }
        // assign
        if ((*stack_it).g_type() == PS_ASSIGN) {
            stack_it++;
            // equals := assign assign
            if ((*stack_it).g_type() == PS_ASSIGN) {
                this->stack.pop_front();
                this->stack.pop_front();
                this->stack.push_front(ParserStack(PS_EQUALS));
                reduce = true;
            }
        }
        // paren_close
        else if ((*stack_it).g_type() == PS_PAREN_CLOSE) {
            stack_it++;
            // register paren_close
            if ((*stack_it).g_type() == PS_REGISTER) {
                reg = (*stack_it).g_register();
                stack_it++;
                // register := paren_open register paren_close
                if ((*stack_it).g_type() == PS_PAREN_OPEN) {
                    this->stack.pop_front();
                    this->stack.pop_front();
                    this->stack.pop_front();
                    this->stack.push_front(ParserStack(PS_REGISTER, reg));
                    reduce = true;
                }
            }
            // condition paren_close
            else if ((*stack_it).g_type() == PS_CONDITION) {
                stack_it++;
                // condition := paren_open condition paren_close
                if ((*stack_it).g_type() == PS_PAREN_OPEN) {
                    this->stack.pop_front();
                    this->stack.pop_front();
                    this->stack.pop_front();
                    this->stack.push_front(ParserStack(PS_CONDITION));
                    reduce = true;
                }
            }
        }
        // terminator
        else if ((*stack_it).g_type() == PS_TERMINATOR) {
            stack_it++;
            // register terminator
            if ((*stack_it).g_type() == PS_REGISTER) {
                reg = (*stack_it).g_register();
                stack_it++;
                // assign register terminator
                if ((*stack_it).g_type() == PS_ASSIGN) {
                    stack_it++;
                    // symbol assign register terminator
                    if ((*stack_it).g_type() == PS_SYMBOL) {
                        // symbol assign register terminator
                        this->symbol_st((*stack_it).g_name(), reg);
                        this->table.free_register(reg);
                        this->stack.pop_front();
                        this->stack.pop_front();
                        this->stack.pop_front();
                        this->stack.pop_front();
                        reduce = true;
                    }
                }
            }
        }
    }
}


int Parser :: instructions_size () {
    int size = 0;
    std::list <Instruction> :: iterator it;
    for (it = this->instructions.begin(); it != this->instructions.end(); it++) {
        if ((*it).g_OP() != OP_COMMENT)
            size++;
    }
    return size;
}


void Parser :: parse () {

    Token next;
    std::list <Token> :: iterator token_it;
    std::list <ParserStack> :: iterator stack_it;
    Instruction hlt(OP_HLT);
    
    for (token_it  = this->tokens.begin();
         token_it != this->tokens.end();
         token_it++) {
        
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
            case TOKEN_PLUS :
                this->stack.push_front(ParserStack(PS_ADD));
                break;
            case TOKEN_BLOCK_OPEN :
                this->stack.push_front(ParserStack(PS_BLOCK_OPEN, (int) this->instructions_size()));
                break;
            case TOKEN_BLOCK_CLOSE :
                this->stack.push_front(ParserStack(PS_BLOCK_CLOSE));
                break;
            case TOKEN_PAREN_OPEN :
                this->stack.push_front(ParserStack(PS_PAREN_OPEN));
                break;
            case TOKEN_PAREN_CLOSE :
                this->stack.push_front(ParserStack(PS_PAREN_CLOSE));
                break;
            case TOKEN_IF :
                this->stack.push_front(ParserStack(PS_IF));
                break;
            case TOKEN_WHILE :
                this->stack.push_front(ParserStack(PS_WHILE, (int) this->instructions_size()));
                break;
            case TOKEN_LESS_THAN :
                this->stack.push_front(ParserStack(PS_LESS_THAN));
                break;
            default :
                throw Exception(std::string("Unknown token ") + (*token_it).g_text());
        }
        
        // grab the next token
        next = *(token_it++);
        token_it--;
        
        this->reduce(next);

    }
    
    if (this->stack.size() > 0) {
        std::cout << "left on the parse stack: ";
        for (stack_it = this->stack.begin(); stack_it !=this->stack.end();
             stack_it++) {
            std::cout << (*stack_it).g_type() << " ";
         }
         std::cout << "\n";
     }
     else
        this->instructions.push_back(hlt);
    
}


std::list <Instruction> Parser :: g_instructions () {
    return this->instructions;
}
