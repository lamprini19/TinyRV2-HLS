#include "ac_int.h"
#include "Operation.h"
#include "ALU.h"
#include "Processor.h"

#include <iostream>

ac_int<32,false> Processor::read_instruction(ac_int<32,false> instr_mem[256]) {
    return instr_mem[this->PC];
}

Operation Processor::decode_read(ac_int<32,false> instruction) {
    ac_int<7,false> opcode = instruction.slc<7>(0);
    std::cout << "Instruction opcode is: " << opcode
              << " or in binary " << opcode.to_string(AC_BIN,false,true)
              << std::endl;
    ac_int<32,true> (ALU::*operation)(ac_int<32,true>, ac_int<32,true>);
    switch(opcode) {
        case 1100111:
            {
            // controls
            ac_int<3,false> func3 = instruction.slc<3>(12);
	        ac_int<32,false> rd = instruction.slc<5>(7);
	        break;
            }
        case 51:
            {
	        // arithmetics
	        ac_int<3,false> func3 = instruction.slc<3>(12);
    	    ac_int<7,false> func7 = instruction.slc<7>(25);
    	    ac_int<32,false> rd = instruction.slc<5>(7);
            ac_int<32,false> rs1 = instruction.slc<5>(15);
    	    ac_int<32,false> rs2 = instruction.slc<5>(20);

            if(func3 == 0) {
                if(func7 == 0) {
                    std::cout << "ADD instruction" << std::endl;
                    operation = &ALU::add;
                    Operation op(operation, rd, R[rs1], R[rs2], 0);

                    return op;
    	        }  // else if ...
            } // else if ...
            break;
            }
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
   
}

void Processor::update_pc(Operation operation) {
    if(operation.control == 1) {
        // call jump function
    } else {
        this->PC = this->PC + 1;
    }
}

ac_int<32,true> Processor::execute(Operation op) {
    return (alu.*(op.operation))(op.operand_1, op.operand_2);
}

void Processor::run(ac_int<32,false> instr_mem[256], ac_int<32,true> data_mem[256]) {
    PC = 50;
    std::cout << "Initial PC: " << PC << std::endl;
    std::cout << std::endl;

    R[4] = 2;
    R[7] = 6;

    ac_int<32,false> instruction = read_instruction(instr_mem);
    Operation operation = decode_read(instruction);
    update_pc(operation);
    ac_int<32,true> result = execute(operation);

    std::cout << std::endl;

    // test read_instruction
    std::cout << "Instruction read: " 
              << instruction.to_string(AC_BIN,false,true)
              << std::endl;

    std::cout << std::endl;

    // test decode_read
    std::cout << "Instruction type: " << operation.control << std::endl
              << "Destination Register: " << operation.destination
              << " or in binary: " << operation.destination.to_string(AC_BIN,false,false) << std::endl
              << "Operands: " << operation.operand_1
              << " and " << operation.operand_2
              << std::endl;

    std::cout << std::endl;

    // test update_pc
    std::cout << "New PC is: " << PC 
              << std::endl;

    std::cout << std::endl;

    // test execute
    std::cout << "Result of adding " << operation.operand_1
              << " and " << operation.operand_2
              << " is " << result
              << " or in binary: " << result.to_string(AC_BIN,false,true)
              << std::endl;
}

