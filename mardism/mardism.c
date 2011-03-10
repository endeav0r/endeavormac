#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DEBUG 0

#define OP_NOP   0x0000000A
#define OP_SUBX  0x00000009
#define OP_ORX   0x00000008
#define OP_JMP   0x00000007
#define OP_ADDX  0x00000006
#define OP_ANDX  0x00000005
#define OP_NOTX  0x00000004
#define OP_SRLX  0x00000003
#define OP_SLLX  0x00000002
#define OP_LD    0x00000001
#define OP_ST    0x00000000

#define OP_HLT   0x00000000
#define OP_RET   0x00000001
#define OP_ADDI  0x00000003
#define OP_BA    0x00000004
#define OP_BN    0x00000005
#define OP_BZ    0x00000006
#define OP_SETHI 0x00000007

#define swap_endianness(x) (x >> 8) | (x << 8)


char register_name_c[4];
char constant_c[16];


struct instruction_s {
	int MD;
	int MB;
	int RD;
	int RS1;
	int RS2;
	int OP;
	uint16_t constant;
};




/**
* shortened substitute for strncat(dest, src, strlen(dest) - dest_size - 1)
* @param dest the character array to append characters to
* @param src  the character array which will be appeneded to dest
* @param dest_size the size of dest, in bytes
* @return always returns 0
*/
int cat (char * dest, char * src, int dest_size);

/**
* @param reg the value of a register
* @return a char array containing the ascii name of the passed register
*/
char * register_name (int reg);

/**
* @param constant the value of a 16 bit constant
* @return pointer to a char array containing ascii representation of
*         constant in hex
*/
char * constant_string (uint16_t constant);

/**
* sets the relevant parts of the instruction_s struct based on the value
* of an instruction word
* @param word the 16 bit instruction word to parse
* @param instruction the instruction_s struct to set
* @return always returns 0
*/
int disasm_word (uint16_t word, struct instruction_s * instruction);

/**
* fills out a character array with a textual description of the contents of
*   an instruction
* @param instruction the instruction_s struct containing information about this
*                    this instruction
* @param buf the character array to place the textual description into
* @param buf_size the size of buf in bytes
* @return returns -1 if the given instruction doesn't make sense, 0 on success
*/
int instruction_to_string (struct instruction_s * instruction, char * buf, int buf_size);

/**
* opens a file containing instrutions for the MARC, disassembles them and sends
*   the result to stdout
* @param filename name of a file containing MARC instructions
* @param instruction_location the location of the first instruction in memory.
*                             this is used to create some pretty output
* @return returns -1 on file/parsing errors, 0 on success
*/
int parse_file (char * filename, uint16_t instruction_location);




int cat (char * dest, char * src, int dest_size) {
    strncat(dest, src, strlen(dest) - dest_size - 1);
    return 0;
}


char * register_name (int reg) {
    snprintf(register_name_c, 4, "R%d", reg);
    return register_name_c;
}


char * constant_string (uint16_t constant) {
    // yes, ok, theoretically impossible to overflow
    snprintf(constant_c, 16, "0x%04hX", constant);
    return constant_c;
}


char * signed_constant_string (uint16_t constant, int bits) {
    int i;
    uint16_t mask = 0x0000;
    
    if ((constant >> (bits - 1)) & 0x0001) {
        constant -= 1;
        for (i = 0; i < bits; i++)
            mask |= 1 << i;
        constant ^= mask;
        snprintf(constant_c, 16, "-0x%04hX", constant);
    }
    else
        return constant_string(constant);
    return constant_c;
}

void debug_instruction (struct instruction_s * instruction) {
    printf("MD:    %d\n", instruction->MD);
    printf("MB:    %d\n", instruction->MB);
    printf("RD:    %d\n", instruction->RD);
    printf("RS1:   %d\n", instruction->RS1);
    printf("RS2:   %d\n", instruction->RS2);
    printf("OP:    %02X\n", instruction->OP);
    printf("CONST: %04X\n", instruction->constant);
}

void debug_word (uint16_t word) {
    uint16_t tmp;
    int i;
    
    tmp = word;
    printf("%04hX\n", tmp);
    
    printf("00RD OP  RS1 RS2 *\n");
    printf("01RD OP CONSTANT--\n");
    printf("1-----------------\n");
    for (i = 0; i < 16; i++) {
        if (tmp & 0x8000)
            printf("1");
        else
            printf("0");
        tmp <<= 1;
    }
    printf("\n");
}


int disasm_word (uint16_t word, struct instruction_s * instruction) {
	
	instruction->MD  = (int) ((word >> 15) & 0x01);
	instruction->MB  = (int) ((word >> 14) & 0x01);	
	instruction->RD  = (int) ((word >> 11) & 0x07);
	instruction->RS1 = (int) ((word >> 04) & 0x07);
	instruction->RS2 = (int) ((word >> 01) & 0x07);
	
	if (instruction->MD) {
		instruction->constant = word & 0x7FFF;
		instruction->OP = 0;
	}
	else {
		if (instruction->MB) {
			instruction->constant = (int) (word & 0x00ff);
			instruction->OP = (int) ((word >> 8) & 0x0007);
		}
		else {
			instruction->constant = 0;
            instruction->OP = (int) ((word >> 7) & 0x000F);
        }
	}
	
	return 0;
	
}


int instruction_to_string (struct instruction_s * instruction, char * buf, int buf_size) {
    
    buf[0] = 0x00;
    
    if ((instruction->MD | instruction->MB) == 0) {
        switch (instruction->OP) {
            case OP_NOP :
                cat(buf, "NOP   ", buf_size);
                break;
            case OP_SUBX :
                cat(buf, "SUBX  ", buf_size);
                break;
            case OP_ORX :
                cat(buf, "ORX   ", buf_size);
                break;
            case OP_ADDX :
                cat(buf, "ADDX  ", buf_size);
                break;
            case OP_ANDX :
                cat(buf, "ANDX  ", buf_size);
                break;
            case OP_NOTX :
                cat(buf, "NOTX  ", buf_size);
                break;
            case OP_SRLX :
                cat(buf, "SRLX  ", buf_size);
                break;
            case OP_SLLX :
                cat(buf, "SLLX  ", buf_size);
                break;
            case OP_JMP :
                cat(buf, "JMP   ", buf_size);
                cat(buf, register_name(instruction->RS2), buf_size);
                break;
            case OP_LD :
                cat(buf, "LD "                          , buf_size);
                cat(buf, register_name(instruction->RD) , buf_size);
                cat(buf, ", M["                         , buf_size);
                cat(buf, register_name(instruction->RS1), buf_size);
                cat(buf, "]",                             buf_size);
                break;
            case OP_ST :
                cat(buf, "ST M["                        , buf_size);
                cat(buf, register_name(instruction->RS1), buf_size);
                cat(buf, "], "                          , buf_size);
                cat(buf, register_name(instruction->RS2), buf_size);
                break;
            default :
                return -1;
        }
        switch (instruction->OP) {
            case OP_SUBX :
            case OP_ORX :
            case OP_ADDX :
            case OP_ANDX :
                cat(buf, register_name(instruction->RD) , buf_size);
                cat(buf, ", "                           , buf_size);
                cat(buf, register_name(instruction->RS1), buf_size);
                cat(buf, ", "                           , buf_size);
                cat(buf, register_name(instruction->RS2), buf_size);
                break;
            case OP_NOTX :
            case OP_SRLX :
            case OP_SLLX :
                cat(buf, register_name(instruction->RD) , buf_size);
                cat(buf, ", "                           , buf_size);
                cat(buf, register_name(instruction->RS1), buf_size);
        }
        
        return 0;
    }
    else if (instruction->MD == 0) {
        switch (instruction->OP) {
            case OP_HLT :
                cat(buf, "HLT", buf_size);
                break;
            case OP_RET :
                cat(buf, "RET", buf_size);
                break;
            case OP_ADDI :
                cat(buf, "ADDI  ", buf_size);
                cat(buf, register_name(instruction->RD)        , buf_size);
                cat(buf, ", "                                  , buf_size);
                cat(buf, signed_constant_string(instruction->constant, 8), buf_size);
                break;
            case OP_BA :
                cat(buf, "BA    ", buf_size);
                break;
            case OP_BN :
                cat(buf, "BN    ", buf_size);
                break;
            case OP_BZ :
                cat(buf, "BZ    ", buf_size);
                break;
            case OP_SETHI :
                cat(buf, "SETHI ", buf_size);
                cat(buf, register_name(instruction->RD)        , buf_size);
                cat(buf, ", "                                  , buf_size);
                cat(buf, signed_constant_string(instruction->constant, 8), buf_size);
                break;
            default :
                return -1;
        }
        switch (instruction->OP) {
            case OP_BA :
            case OP_BN :
            case OP_BZ :
                cat(buf, signed_constant_string(instruction->constant, 8), buf_size);
                break;
        }
                
    }
    else {
        cat(buf, "CALL ", buf_size);
        cat(buf, constant_string(instruction->constant), buf_size);
    }
    
    return 0;
}


int parse_file (char * filename, uint16_t instruction_location) {
    uint16_t word;
    size_t bytes_read;
    char description[64];
    FILE * fh;
    struct instruction_s instruction;
    
    fh = fopen(filename, "rb");
    if (fh == NULL) {
        fprintf(stderr, "error opening file %s\n", filename);
        return -1;
    }
    
    while (1) {
        bytes_read = fread(&word, 2, 1, fh);
        if (bytes_read != 1) {
            if (feof(fh))
                return 0;
            fprintf(stderr, "error reading instruction. malformed file?\n");
            return -1;
        }
        word = swap_endianness(word);
        #if DEBUG == 1
            debug_word(word);
        #endif
        if (disasm_word(word, &instruction)) {
            fprintf(stderr, "error parsing instruction %04hX\n", word);
            return -1;
        }
        #if DEBUG == 1
            debug_instruction(&instruction);
        #endif
        if (instruction_to_string(&instruction, description, 64)) {
            fprintf(stderr, "error creating description for instruction %04hX\n", word);
            return -1;
        }
        printf("%04hX:   %04hX    %s\n", instruction_location, word, description);
        instruction_location++;
        #if DEBUG == 1
            printf("\n");
        #endif
    }
    
    fclose(fh);
    
    return 0;
}
        


int main (int argc, char * argv[]) {
	if ((argc != 2) && (argc != 3)) {
		printf("usage: %s <filename> [location_first_instruction]\n", argv[0]);
		exit(0);
	}
	
    if (argc == 2)
        return parse_file(argv[1], 0);
    else if (argc == 3)
        return parse_file(argv[1], (uint16_t) strtol(argv[2], (char **) NULL, 16));
	
}