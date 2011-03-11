#include "parser.hpp"


Parser :: Parser (std::list <Token> tokens) {
    int i;
    for (i = 0; i < GENERAL_PURPOSE_REGISTERS; i++)
        this->registers_free[i] = true;
    this->tokens = tokens;
}


int Parser :: get_free_register () {
    int i;
    for (i = 1; i < GENERAL_PURPOSE_REGISTERS; i++) {
        if (this->registers_free[i])
            return i;
    }
    throw Exception("no registers free");
    return -1;
}


void Parser :: use_register (int reg) {
    if (! this->registers_free[reg])
        throw Exception("tried to use an already used register");
    this->registers_free[reg] = false;
}


void Parser :: free_register (int reg) {
    if (this->registers_free[reg])
        throw Exception("tried to free already free register");
    this->registers_free[reg] = true;
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


void Parser :: symbol_st (std::string name, int reg) {
	Instruction comment(OP_COMMENT);
    Instruction st(OP_ST);
    int reg1, reg2;
    
	comment.s_COMMENT("symbol st");
    
    if (! this->table.symbol_exists(name))
        throw Exception(std::string("tried to set symbol ") + name + " but it wasn't found");
    
    if (! this->table.g_symbol_absolute(name)) {
        // we need a couple general purpose registers
        reg1 = this->get_free_register();
        this->use_register(reg1);
        reg2 = this->get_free_register();
        this->use_register(reg2);
        
        // reg1 gets symbol offset
        // reg2 gets bp + reg1
        // reg2 => the address of the symbol
        this->load_immediate(reg1, this->table.g_symbol_offset(name));
        
        this->instructions.push_back(comment);
        this->addx(reg2, reg1, REG_BASE_POINTER);
        
        st.s_RD(reg2);
        st.s_RS1(reg);
        this->instructions.push_back(st);
        
        this->free_register(reg1);
        this->free_register(reg2);
    }
}


void Parser :: symbol_ld (std::string name, int reg) {
	Instruction comment(OP_COMMENT);
	Instruction ld(OP_LD);
	int reg1, reg2;
	
	comment.s_COMMENT("symbol ld");
	
	if (! this->table.symbol_exists(name))
		throw Exception(std::string("tried to get symbol ") + name + " but it wasn't found");
	
	if (! this->table.g_symbol_absolute(name)) {
		reg1 = this->get_free_register();
		this->use_register(reg1);
		reg2 = this->get_free_register();
		this->use_register(reg2);
		
		this->load_immediate(reg1, this->table.g_symbol_offset(name));
		
		this->instructions.push_back(comment);
		this->addx(reg2, reg1, REG_BASE_POINTER);
		
		ld.s_RD(reg);
		ld.s_RS1(reg2);
		this->instructions.push_back(ld);
		this->free_register(reg1);
		this->free_register(reg2);
	}
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
            reg = this->get_free_register();
            this->use_register(reg);
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
					this->free_register((*stack_it).g_register());
					this->stack.pop_front();
					this->stack.pop_front();
					this->stack.pop_front();
					this->stack.push_front(ParserStack(PS_REGISTER, reg));
					reduce = true;
				}
				// register := symbol add register
				else if ((*stack_it).g_type() == PS_SYMBOL) {
					reg2 = this->get_free_register();
					this->use_register(reg2);
					this->symbol_ld((*stack_it).g_name(), reg2);
					this->addx(reg, reg, reg2);
					this->free_register(reg2);
					this->stack.pop_front();
					this->stack.pop_front();
					this->stack.pop_front();
					this->stack.push_front(ParserStack(PS_ADD, reg));
					reduce = true;
				}
			}
		}
		// symbol 
		else if ((*stack_it).g_type() == PS_SYMBOL) {
			stack_it++;
			// add register := add symbol
			if ((*stack_it).g_type() == PS_ADD) {
				stack_it--;
				reg = this->get_free_register();
				this->use_register(reg);
				this->symbol_ld((*stack_it).g_name(), reg);
				this->stack.pop_front();
				this->stack.push_front(ParserStack(PS_REGISTER, reg));
				reduce = true;
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
						this->free_register(reg);
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


void Parser :: parse () {

    Token next;
    std::list <Token> :: iterator token_it;
    std::list <ParserStack> :: iterator stack_it;
    
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
			case TOKEN_PLUS :
				this->stack.push_front(ParserStack(PS_ADD));
				break;
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
    
}


std::list <Instruction> Parser :: g_instructions () {
    return this->instructions;
}
