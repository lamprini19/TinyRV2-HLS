#include "ac_int.h"
#include "Operation.h"
#include "ALU.h"
#include "Processor.h"


ac_int<32,false> Processor::read_instruction(ac_int<32,false> instr_mem[256]) {
    return instr_mem[this->PC];
}

void Processor::run(ac_int<32,false> instr_mem[256], ac_int<32,true> data_mem[256]) {
    this->PC = 50;
    ac_int<32,false> instruction = read_instruction(instr_mem);
    
    // test read_instruction
    std::cout << "Instruction read: " 
              << instruction.to_string(AC_BIN,false,true)
              << std::endl;
}


Operation Processor::decode_read(ac_int<32,false> instruction) {
    ac_int<7,false> opcode = instruction[0:6];
    switch(opcode) {
        case 1100111:
	    // controls
	    ac_int<3,false> func3 = instruction[12:14];
	    ac_int<5,false> rd = instruction[7:11];
	    break;
	
        case 1100110:
	    // arithmetics
	    ac_int<3,false> func3 = instruction[12:14];
	    ac_int<7,false> func7 = instruction[25:31];
	    ac_int<5,false> rd = instruction[7:11];
	    ac_int<5,false> rs1 = instruction[15:19];
	    ac_int<5,false> rs2 = instruction[20:24];
	    std::cout << "Add operation\n" ;

            if(func3==0) {
		if(func7==0) {
		    Operation op(operation = add,
				 destination = rd,
				 operand_1 = rs1,
				 operand_2 = rs2,
				 control = 0)
		}
		// else if ...
	    // else if ...
	    }
	    break;

	case 1100100:
	    // arithmetics with immediate
	    break;

	case 1110110:
	    // lui
	    break;
	
	case 1110100:
	    // auipc
	    break;
	
	case 1100000:
	    // lw
	    break;
	
	case 1100010:
	    // sw
	    break;

	case 1111011:
	    // jal
	    break;
	
	case 1110011:
	    // jalr (+jr)
	    break;
	
	case 1100011:
	    // conditional branch
	    break;

	default:
	    std::cout << "Wrong\n" ;
	    break;
    }
    return op;
}

void Processor::update_pc(Operation operation) {
    if(operation->control==1) {
	// call jump function
    } else {
	this->PC = this->PC + 4;
}
