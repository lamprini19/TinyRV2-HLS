#include "ac_int.h"
#include "Operation.h"
#include "ALU.h"
#include "Processor.h"

#include <iostream>

ac_int<32,false> Processor::read_instruction(ac_int<32,false> instr_mem[256]) {
    return instr_mem[PC];
}

Operation Processor::decode_read(ac_int<32,false> instruction) {
    ac_int<7,false> opcode = instruction.slc<7>(0);
    ac_int<32,true> (ALU::*operation)(ac_int<32,true>, ac_int<32,true>);
    
    switch(opcode) {
        case 115:
            {
            // controls
            ac_int<3,false> func3 = instruction.slc<3>(12);
	        ac_int<32,false> rd = instruction.slc<5>(7);

            if(func3 == 2) {
                std::cout << "CSRR" << std::endl;
            } else if(func3 == 1) {
                std::cout << "CSRW" << std::endl;
            } else {
                operation = &ALU::add;
                Operation op(operation, 0, 0, 0, 1);
                return op;
            }
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
                    Operation op(operation, rd, R[rs1], R[rs2], 1);
                    return op;
    	        } else if(func7[5] == 1) {
                    std::cout << "SUB instruction" << std::endl;
                    operation = &ALU::sub;
                    Operation op(operation, rd, R[rs1], R[rs2], 1);
                    return op;
                } else if(func7[0] == 1) {
                    std::cout << "MUL operation" << std::endl;
                    operation = &ALU::mul;
                    Operation op(operation, rd, R[rs1], R[rs2], 1);
                    return op;
                } else {
                    std::cout << "Wrong operation (add)" << std::endl;
                    operation = &ALU::add;
                    Operation op(operation, 0, 0, 0, 1);
                    return op;
                }
            } else if(func3 == 7) {
                std::cout << "AND operation" << std::endl;
                operation = &ALU::bwand;
                Operation op(operation, rd, R[rs1], R[rs2], 1);
                return op;
            } else if(func3 == 6) {
                std::cout << "OR operation" << std::endl;
                operation = &ALU::bwor;
                Operation op(operation, rd, R[rs1], R[rs2], 1);
                return op;
            } else if(func3 == 4) {
                std::cout << "XOR operation" << std::endl;
                operation = &ALU::bwxor;
                Operation op(operation, rd, R[rs1], R[rs2], 1);
                return op;
            } else if(func3 == 2) {             
                std::cout << "SLT operation" << std::endl;
                operation = &ALU::less_than;
                Operation op(operation, rd, R[rs1], R[rs2], 1);
                return op;
            } else if(func3 == 3) {             
                std::cout << "SLTU operation" << std::endl;
                operation = &ALU::less_than_u;
                Operation op(operation, rd, R[rs1], R[rs2], 1);
                return op;
            } else if(func3 == 5) {
                if(func7[5] == 1) {            
                    std::cout << "SRA operation" << std::endl;
                    operation = &ALU::shift_right_a;
                    Operation op(operation, rd, R[rs1], R[rs2], 1);
                    return op;
                } else if(func7 == 0) {
                    std::cout << "SRL operation" << std::endl;
                    operation = &ALU::shift_right_l;
                    Operation op(operation, rd, R[rs1], R[rs2], 1);
                    return op;
                } else {             
                    std::cout << "Wrong operation (right shift)" << std::endl;
                    operation = &ALU::add;
                    Operation op(operation, 0, 0, 0, 1);
                    return op;
                }
            } else if(func3 == 1) {             
                std::cout << "SLL operation" << std::endl;
                operation = &ALU::shift_left;
                Operation op(operation, rd, R[rs1], R[rs2], 1);
                return op;           
            } else {
                std::cout << "Wrong operation (arithm)" << std::endl;
                operation = &ALU::add;
                Operation op(operation, 0, 0, 0, 1);
                return op;
            }
            break;
            }
        case 19:
            {
            // arithmetics with immediate
            ac_int<3,false> func3 = instruction.slc<3>(12);
    	    ac_int<32,true> imm = instruction.slc<12>(20);
    	    ac_int<32,false> rd = instruction.slc<5>(7);
            ac_int<32,false> rs1 = instruction.slc<5>(15);

            // sign extend of immediate
            ac_int<20,false> temp = -1;
            std::cout << "imm: " << imm.to_string(AC_BIN,false,true) << std::endl;
            std::cout << "temp: " << temp.to_string(AC_BIN,false,true) << std::endl;
            ac_int<32,true> sext_imm = imm;
            if(imm[11] == 1) { sext_imm.set_slc(12,temp); }

            if(func3 == 0) {
                std::cout<< "ADDI operation" << std::endl;
                operation = &ALU::add;
                Operation op(operation, rd, R[rs1], sext_imm, 1);
                return op;
            } else if(func3 == 7) {
                std::cout << "ANDI operation" << std::endl;
                operation = &ALU::bwand;
                Operation op(operation, rd, R[rs1], sext_imm, 1);
                return op;
            } else if(func3 == 6) {
                std::cout << "ORI operation" << std::endl;
                operation = &ALU::bwor;
                Operation op(operation, rd, R[rs1], sext_imm, 1);
                return op;
            } else if(func3 == 4) {             
                std::cout << "XORI operation" << std::endl;
                operation = &ALU::bwxor;
                Operation op(operation, rd, R[rs1], sext_imm, 1);
                return op;
            } else if(func3 == 2) {             
                std::cout << "SLTI operation" << std::endl;
                operation = &ALU::less_than;
                Operation op(operation, rd, R[rs1], sext_imm, 1);
                return op;
            } else if(func3 == 3) {             
                std::cout << "SLTIU operation" << std::endl;
                operation = &ALU::less_than_u;
                Operation op(operation, rd, R[rs1], sext_imm, 1);
                return op;           
            } else if(func3 == 5) {
                if(imm[10] == 1) {             
                    std::cout << "SRAI operation" << std::endl;
                    operation = &ALU::shift_right_a;
                    Operation op(operation, rd, R[rs1], imm.slc<5>(0), 1);
                    return op;
                } else if(imm.slc<7>(5) == 0) {
                    std::cout << "SRLI operation" << std::endl;
                    operation = &ALU::shift_right_l;
                    Operation op(operation, rd, R[rs1], imm.slc<5>(0), 1);
                    return op;
                } else {
                    std::cout << "Wrong operation (right shift imm)" << std::endl;
                    operation = &ALU::add;
                    Operation op(operation, 0, 0, 0, 1);
                    return op;
                }           
            } else if(func3 == 1) {             
                std::cout << "SLLI operation" << std::endl;
                operation = &ALU::shift_left;
                Operation op(operation, rd, R[rs1], imm, 1);
                return op;           
            } else {
                std::cout << "Wrong operation (arithm imm)" << std::endl;
                operation = &ALU::add;
                Operation op(operation, 0, 0, 0, 1);
                return op;
            }
            break;
            }
        case 55: 
            {
            // lui
            std::cout << "LUI operation" << std::endl;
            ac_int<32,true> rd = instruction.slc<5>(7);
            ac_int<32,true> imm = instruction.slc<20>(12);
            operation = &ALU::shift_left;
            Operation op(operation, rd, imm, 12, 1);
            return op;
            break;
            }
        case 23:
            {
            // auipc
            std::cout << "AUIPC operation" << std::endl;
            ac_int<32,true> rd = instruction.slc<5>(7);
            ac_int<32,true> imm = instruction.slc<20>(12);
            operation = &ALU::shift12_add;
            Operation op(operation, rd, imm, PC, 1);
            return op;
            break;
            }
        case 3:
            {
            // lw
            std::cout << "LW operation" << std::endl;
            ac_int<32,true> rd = instruction.slc<5>(7);
            ac_int<32,true> rs1 = instruction.slc<5>(15);
            ac_int<32,true> imm = instruction.slc<12>(20);
            // sign extend
            ac_int<20,false> temp = -1;
            ac_int<32,true> sext_imm = imm;
            if(imm[11] == 1) { sext_imm.set_slc(12,temp); }

            operation = &ALU::add;
            Operation op(operation, rd, imm, R[rs1], 2);
            return op;
            break;
            }
        case 35:
            {
            // sw
            std::cout << "SW operation" << std::endl;
            ac_int<32,true> rs1 = instruction.slc<5>(15);
            ac_int<32,true> rs2 = instruction.slc<5>(20);
            // reconstruct S-type immediate
            ac_int<32,true> imm = 0;
            ac_int<5,true> imm_part_1 = instruction.slc<5>(7);
            ac_int<7,true> imm_part_2 = instruction.slc<7>(25);
            imm.set_slc(0,imm_part_1);
            imm.set_slc(5,imm_part_2);
            // sign extend
            ac_int<20,false> temp = -1;
            ac_int<32,true> sext_imm = imm;
            if(imm[11] == 1) { sext_imm.set_slc(12,temp); }
            // create Operation object
            operation = &ALU::add;
            Operation op(operation, rs2, R[rs1], sext_imm, 3);
            return op;
            break;
            }
        case 111:
            {
            // jal
            ac_int<32,true> rd = instruction.slc<5>(7);
            ac_int<20,true> imm = instruction.slc<20>(12);
            // sign extend
            ac_int<12,false> temp = -1;
            ac_int<32,true> sext_imm = imm;
            if(imm[19] == 1) { sext_imm.set_slc(19,temp); }
            // create Operation object
            operation = &ALU::add;
            Operation op(operation, rd, PC, sext_imm, 4);
            return op;
            break;
            }
        case 103:
            {
            // jalr (+jr)
            ac_int<32,true> rd = instruction.slc<5>(7);
            ac_int<32,true> imm = instruction.slc<12>(20);
            ac_int<32,true> rs1 = instruction.slc<5>(15);
            // sign extend
            ac_int<20,false> temp = -1;
            ac_int<32,true> sext_imm = imm;
            if(imm[11] == 1) { sext_imm.set_slc(20,temp); }
            // create Operation object
            operation = &ALU::add;
            Operation op(operation, rd, R[rs1], sext_imm, 4);
            return op;
            break;
            }
        case 99:
            {
            // conditional branch
            ac_int<3,false> func3 = instruction.slc<3>(12);
    	    ac_int<32,false> rs1 = instruction.slc<5>(15);
            ac_int<32,false> rs2 = instruction.slc<5>(20);
            // reconstruct B-type immediate
            ac_int<32,true> imm = 0;
            ac_int<4,true> imm_part_1 = instruction.slc<4>(8);
            ac_int<6,true> imm_part_2 = instruction.slc<6>(25);
            ac_int<1,false> imm_part_3 = instruction[7];
            std::cout << "Imm1: " << imm_part_1.to_string(AC_BIN,false,true)
                      << "  Imm2: " << imm_part_2.to_string(AC_BIN,false,true)
                      << "  Imm3: " << imm_part_3.to_string(AC_BIN,false,true) << std::endl;
            imm.set_slc(1,imm_part_1);
            std::cout << "Imm after 1 addition: " << imm.to_string(AC_BIN,false,true) << std::endl;
            imm.set_slc(5,imm_part_2);
            imm[11] = imm_part_3;
            // sign extend of immediate
            ac_int<20,false> temp = -1;
            ac_int<32,true> sext_imm = imm;
            if(imm[11] == 1) { sext_imm.set_slc(12,temp); }
            // create Operation object
            if(func3 == 0) {
                std::cout<< "BEQ operation"
                          << " sext_imm: " << sext_imm.to_string(AC_BIN,false,true) << std::endl;
                operation = &ALU::equal;
                Operation op(operation, sext_imm, R[rs1], R[rs2], 0);
                return op;
            } else if(func3 == 1) {
                std::cout << "BNE operation" << std::endl;
                operation = &ALU::not_equal;
                Operation op(operation, sext_imm, R[rs1], R[rs2], 0);
                return op;
            } else if(func3 == 4) {
                std::cout << "BLT operation" << std::endl;
                operation = &ALU::less_than;
                Operation op(operation, sext_imm, R[rs1], R[rs2], 0);
                return op;
            } else if(func3 == 5) {             
                std::cout << "BGE operation (if " << R[rs1] << ">" << R[rs2]
                          << " then PC <- PC + " << sext_imm << std::endl;
                operation = &ALU::greater_than;
                Operation op(operation, sext_imm, R[rs1], R[rs2], 0);
                return op;
            } else if(func3 == 6) {             
                std::cout << "BTLU operation" << std::endl;
                operation = &ALU::less_than_u;
                Operation op(operation, sext_imm, R[rs1], R[rs2], 0);
                return op;
            } else if(func3 == 7) {             
                std::cout << "BGEU operation" << std::endl;
                operation = &ALU::greater_than_u;
                Operation op(operation, sext_imm, R[rs1], R[rs2], 0);
                return op;           
            } else {
                std::cout << "Wrong operation (branch)" << std::endl;
                operation = &ALU::add;
                Operation op(operation, 0, 0, 0, 1);
                return op;
            }
            break;
            }
        default: 
            std::cout << "Wrong operation (opcode)" << std::endl;
            operation = &ALU::add;
            Operation op(operation, 0, 0, 0, 1);
        }
    //return op;
}

ac_int<32,false> Processor::update_pc(Operation op) {
    if(op.control == 0) {
        // call branch function
        ac_int<32,false> result = (alu.*(op.operation))(op.operand_1, op.operand_2);
        if(result[0]) {
            return PC + op.destination;
        } else {
            return PC + 1;
        }
    } else if(op.control == 4) {
        // call jump function
        ac_int<5,false> reg_addr = op.destination.slc<5>(0);
        R[reg_addr] = PC+1;
        std::cout << PC+1 << " will be stored to register " << reg_addr << std::endl;
        ac_int<32,true> result =  (alu.*(op.operation))(op.operand_1, op.operand_2);
        std::cout << "addition result of " << op.operand_1
                  << "and " << op.operand_2 << " is " 
                  << result.to_string(AC_BIN,false,true) << " (" << result << ")\n";
        return result;
    } else {
        return PC + 1;
    }
}

ac_int<32,true> Processor::execute(Operation op) {
    return (alu.*(op.operation))(op.operand_1, op.operand_2);
}

void Processor::memory_write(ac_int<32,true> memory[256],
                             ac_int<32,false> address,
                             ac_int<32,true> value) {
    memory[address] = value;
}

ac_int<32,true> Processor::memory_read(ac_int<32,true> memory[256],
                                       ac_int<32,false> address) {
    return memory[address];
}

void Processor::write_back(ac_int<32,false> destination_reg, ac_int<32,true> value) {
    ac_int<5,false> reg_addr = destination_reg.slc<5>(0);
    R[reg_addr] = value;
}

void Processor::run(ac_int<32,false> instr_mem[256], ac_int<32,true> data_mem[256]) {
    ac_int<32,false> next_PC = 50;
    std::cout << "Initial PC: " << next_PC << std::endl;
    std::cout << std::endl;

    while(true) {
        R[0] = 0;
        std::cout << std::string(72,'-');
        std::cout << std::endl;
        PC = next_PC;
        std::cout << "Current PC: " << PC << std::endl;
        ac_int<32,false> instruction = read_instruction(instr_mem);
        if(instruction == 0) { std::cout << "No instruction found, exit.\n"; break; }
        Operation operation = decode_read(instruction);
        next_PC = update_pc(operation);
        if(operation.control == 0 || operation.control == 4) { continue; }
        ac_int<32,true> result = execute(operation);
        if(operation.control == 2) {
            write_back(operation.destination, memory_read(data_mem, result));
        } else if(operation.control == 3) {
            memory_write(data_mem, result, operation.destination);
        } else if(operation.control == 1) {
            write_back(operation.destination, result);
        }

        // test read_instruction
        std::cout << "Instruction read: "
                  << instruction.to_string(AC_BIN,false,true)
                  << std::endl;

        std::cout << std::endl;

        // test decode_read
        std::cout << "Instruction type: " << operation.control << std::endl
                  << "Destination Register: " << operation.destination
                  << " or in binary: "
                  << operation.destination.to_string(AC_BIN,false,false)
                  << std::endl
                  << "Operands: " << operation.operand_1
                  << " and " << operation.operand_2
                  << std::endl;

        std::cout << std::endl;

        // test update_pc
        std::cout << "Next PC is: " << next_PC
                  << std::endl;

        std::cout << std::endl;

        // test execute
        std::cout << "Result of operating with " << operation.operand_1
                  << " and " << operation.operand_2
                  << " is " << result
                  << " or in binary: "
                  << result.to_string(AC_BIN,false,true)
                  << std::endl;

        if(operation.control == 3) {
            std::cout << "Wrote " << operation.operand_2
                    << " on memory address " << result
                    << " (" << result.to_string(AC_HEX,false,false)
                    << ")" << std::endl;
        }

        if(operation.control == 2) {
            std::cout << "Read value " << memory_read(data_mem,result)
                      << "from memory address " << result
                      << " (" << result.to_string(AC_HEX,false,false) << ")" 
                      << " in register " << operation.destination
                      << std::endl;
        }

        // test write_back
        std::cout << "Value of register "
                  << operation.destination
                  << ": " << R[operation.destination]
                  << std::endl;

        std::cout << std::endl;
    }
}
