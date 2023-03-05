#include "ac_int.h"
#include "Operation.h"
#include "ALU.h"
#include "Processor.h"

#include <iostream>

ac_int<32,false> Processor::read_instruction(ac_int<32,false> instr_mem[256]) {
    return instr_mem[PC.slc<30>(2)];
}

Operation Processor::decode_read(ac_int<32,false> instruction) {
    ac_int<7,false> opcode = instruction.slc<7>(0);
    ac_int<4,false> ALU_opcode;
    
    switch(opcode) {
        case 51:
            {
            // arithmetic
            ac_int<3,false> func3 = instruction.slc<3>(12);
    	    ac_int<7,false> func7 = instruction.slc<7>(25);
    	    ac_int<32,false> rd = instruction.slc<5>(7);
            ac_int<32,false> rs1 = instruction.slc<5>(15);
    	    ac_int<32,false> rs2 = instruction.slc<5>(20);

            if(func3 == 0) {
                if(func7 == 0) {
                    std::cout << "ADD instruction" << std::endl;
                    ALU_opcode = 0;
                    Operation op(ALU_opcode, rd, R[rs1], R[rs2], 1);
                    return op;
    	        } else if(func7[5] == 1) {
                    std::cout << "SUB instruction" << std::endl;
                    ALU_opcode = 1;
                    Operation op(ALU_opcode, rd, R[rs1], R[rs2], 1);
                    return op;
                } else if(func7[0] == 1) {
                    std::cout << "MUL operation" << std::endl;
                    ALU_opcode = 2;
                    Operation op(ALU_opcode, rd, R[rs1], R[rs2], 1);
                    return op;
                } else {
                    std::cout << "Wrong operation (add)" << std::endl;
                    ALU_opcode = 0;
                    Operation op(ALU_opcode, 0, 0, 0, 1);
                    return op;
                }
            } else if(func3 == 7) {
                std::cout << "AND operation" << std::endl;
                ALU_opcode = 3;
                Operation op(ALU_opcode, rd, R[rs1], R[rs2], 1);
                return op;
            } else if(func3 == 6) {
                std::cout << "OR operation" << std::endl;
                ALU_opcode = 4;
                Operation op(ALU_opcode, rd, R[rs1], R[rs2], 1);
                return op;
            } else if(func3 == 4) {
                std::cout << "XOR operation" << std::endl;
                ALU_opcode = 5;
                Operation op(ALU_opcode, rd, R[rs1], R[rs2], 1);
                return op;
            } else if(func3 == 2) {
                std::cout << "SLT operation" << std::endl;
                ALU_opcode = 11;
                Operation op(ALU_opcode, rd, R[rs1], R[rs2], 1);
                return op;
            } else if(func3 == 3) {
                std::cout << "SLTU operation" << std::endl;
                ALU_opcode = 12;
                Operation op(ALU_opcode, rd, R[rs1], R[rs2], 1);
                return op;
            } else if(func3 == 5) {
                if(func7[5] == 1) {
                    std::cout << "SRA operation" << std::endl;
                    ALU_opcode = 7;
                    Operation op(ALU_opcode, rd, R[rs1], R[rs2], 1);
                    return op;
                } else if(func7 == 0) {
                    std::cout << "SRL operation" << std::endl;
                    ALU_opcode = 8;
                    Operation op(ALU_opcode, rd, R[rs1], R[rs2], 1);
                    return op;
                } else {
                    std::cout << "Wrong operation (right shift)" << std::endl;
                    ALU_opcode = 0;
                    Operation op(ALU_opcode, 0, 0, 0, 1);
                    return op;
                }
            } else if(func3 == 1) {
                std::cout << "SLL operation" << std::endl;
                ALU_opcode = 6;
                Operation op(ALU_opcode, rd, R[rs1], R[rs2], 1);
                return op;           
            } else {
                std::cout << "Wrong operation (arithm)" << std::endl;
                ALU_opcode = 0;
                Operation op(ALU_opcode, 0, 0, 0, 1);
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
            ac_int<32,true> sext_imm = imm;
            if(imm[11] == 1) { sext_imm.set_slc(12,temp); }

            if(func3 == 0) {
                std::cout<< "ADDI operation" << std::endl;
                ALU_opcode = 0;
                Operation op(ALU_opcode, rd, R[rs1], sext_imm, 1);
                return op;
            } else if(func3 == 7) {
                std::cout << "ANDI operation" << std::endl;
                ALU_opcode = 3;
                Operation op(ALU_opcode, rd, R[rs1], sext_imm, 1);
                return op;
            } else if(func3 == 6) {
                std::cout << "ORI operation" << std::endl;
                ALU_opcode = 4;
                Operation op(ALU_opcode, rd, R[rs1], sext_imm, 1);
                return op;
            } else if(func3 == 4) {
                std::cout << "XORI operation" << std::endl;
                ALU_opcode = 5;
                Operation op(ALU_opcode, rd, R[rs1], sext_imm, 1);
                return op;
            } else if(func3 == 2) {
                std::cout << "SLTI operation" << std::endl;
                ALU_opcode = 11;
                Operation op(ALU_opcode, rd, R[rs1], sext_imm, 1);
                return op;
            } else if(func3 == 3) {
                std::cout << "SLTIU operation" << std::endl;
                ALU_opcode = 12;
                Operation op(ALU_opcode, rd, R[rs1], sext_imm, 1);
                return op;           
            } else if(func3 == 5) {
                if(imm[10] == 1) {
                    std::cout << "SRAI operation" << std::endl;
                    ALU_opcode = 7;
                    Operation op(ALU_opcode, rd, R[rs1], imm.slc<5>(0), 1);
                    return op;
                } else if(imm.slc<7>(5) == 0) {
                    std::cout << "SRLI operation" << std::endl;
                    ALU_opcode = 8;
                    Operation op(ALU_opcode, rd, R[rs1], imm.slc<5>(0), 1);
                    return op;
                } else {
                    std::cout << "Wrong operation (right shift imm)" << std::endl;
                    ALU_opcode = 0;
                    Operation op(ALU_opcode, 0, 0, 0, 1);
                    return op;
                }           
            } else if(func3 == 1) {
                std::cout << "SLLI operation" << std::endl;
                ALU_opcode = 6;
                Operation op(ALU_opcode, rd, R[rs1], imm, 1);
                return op;           
            } else {
                std::cout << "Wrong operation (arithm imm)" << std::endl;
                ALU_opcode = 0;
                Operation op(ALU_opcode, 0, 0, 0, 1);
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
            ALU_opcode = 6;
            Operation op(ALU_opcode, rd, imm, 12, 1);
            return op;
            break;
            }
        case 23:
            {
            // auipc
            std::cout << "AUIPC operation" << std::endl;
            ac_int<32,true> rd = instruction.slc<5>(7);
            ac_int<32,true> imm = instruction.slc<20>(12);
            ALU_opcode = 15;
            Operation op(ALU_opcode, rd, imm, PC, 1);
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

            ALU_opcode = 0;
            Operation op(ALU_opcode, rd, imm, R[rs1], 2);
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
            ALU_opcode = 0;
            Operation op(ALU_opcode, rs2, R[rs1], sext_imm, 3);
            return op;
            break;
            }
        case 111:
            {
            // jal
            ac_int<32,true> rd = instruction.slc<5>(7);
            // reconstruct J-type immediate
            ac_int<32,true> imm = 0;
            ac_int<10,false> imm_part_1 = instruction.slc<10>(21);
            ac_int<1,false> imm_part_2 = instruction[20];
            ac_int<8,false> imm_part_3 = instruction.slc<8>(12);
            ac_int<1,false> imm_part_4 = instruction[31];
            imm.set_slc(1,imm_part_1);
            imm.set_slc(11,imm_part_2);
            imm.set_slc(12,imm_part_3);
            imm.set_slc(20,imm_part_4);
            // sign extend
            ac_int<12,false> temp = -1;
            ac_int<32,true> sext_imm = imm;
            if(imm[20] == 1) { sext_imm.set_slc(20,temp); }
            // create Operation object
            ALU_opcode = 0;
            Operation op(ALU_opcode, rd, PC, sext_imm, 4);
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
            ALU_opcode = 0;
            Operation op(ALU_opcode, rd, R[rs1], sext_imm, 4);
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
            imm.set_slc(1,imm_part_1);
            imm.set_slc(5,imm_part_2);
            imm[11] = imm_part_3;
            // sign extend of immediate
            ac_int<20,false> temp = -1;
            ac_int<32,true> sext_imm = imm;
            if(imm[11] == 1) { sext_imm.set_slc(12,temp); }
            // create Operation object
            if(func3 == 0) {
                ALU_opcode = 9;
                std::cout << "If " << R[rs1] << " == " << R[rs2]
                          << " then PC <- PC + " << sext_imm << std::endl;
                Operation op(ALU_opcode, sext_imm, R[rs1], R[rs2], 0);
                return op;
            } else if(func3 == 1) {
                std::cout << "If " << R[rs1] << " != " << R[rs2]
                          << " then PC <- PC + " << sext_imm << std::endl;
                ALU_opcode = 10;
                Operation op(ALU_opcode, sext_imm, R[rs1], R[rs2], 0);
                return op;
            } else if(func3 == 4) {
                std::cout << "If " << R[rs1] << " <s " << R[rs2]
                          << " then PC <- PC + " << sext_imm << std::endl;
                ALU_opcode = 11;
                Operation op(ALU_opcode, sext_imm, R[rs1], R[rs2], 0);
                return op;
            } else if(func3 == 5) {
                std::cout << "If " << R[rs1] << " >=s " << R[rs2]
                          << " then PC <- PC + " << sext_imm << std::endl;
                ALU_opcode = 13;
                Operation op(ALU_opcode, sext_imm, R[rs1], R[rs2], 0);
                return op;
            } else if(func3 == 6) {
                std::cout << "If " << R[rs1] << " <u " << R[rs2]
                          << " then PC <- PC + " << sext_imm << std::endl;
                ALU_opcode = 12;
                Operation op(ALU_opcode, sext_imm, R[rs1], R[rs2], 0);
                return op;
            } else if(func3 == 7) {
                std::cout << "If " << R[rs1] << " >=u " << R[rs2]
                          << " then PC <- PC + " << sext_imm << std::endl;
                ALU_opcode = 14;
                Operation op(ALU_opcode, sext_imm, R[rs1], R[rs2], 0);
                return op;  
            } else {
                std::cout << "Wrong operation (branch)" << std::endl;
                ALU_opcode = 0;
                Operation op(ALU_opcode, 0, 0, 0, 1);
                return op;
            }
            break;
            }
        default:
            std::cout << "Wrong operation (opcode)" << std::endl;
            ALU_opcode = 0;
            Operation op(ALU_opcode, 0, 0, 0, 1);
        }
    //return op;
}

ac_int<32,false> Processor::update_pc(Operation op) {
    if(op.control == 0) {
        // call branch function
        ac_int<32,false> result = alu.operation(op.operand_1,
                                                op.operand_2,
                                                op.ALU_opcode);
        if(result[0]) {
            return PC + op.destination;
        } else {
            return PC + 4;
        }
    } else if(op.control == 4) {
        // call jump function
        ac_int<5,false> reg_addr = op.destination.slc<5>(0);
        R[reg_addr] = PC + 4;

        ac_int<32,false> result = alu.operation(op.operand_1,
                                                op.operand_2,
                                                op.ALU_opcode);

        std::cout << "Jump to instruction address: " << result << std::endl;

        return result;
    } else {
        return PC + 4;
    }
}

ac_int<32,true> Processor::execute(Operation op) {
    return alu.operation(op.operand_1, op.operand_2, op.ALU_opcode);
}

void Processor::memory_write(ac_int<32,true> memory[256],
                             ac_int<32,false> address,
                             ac_int<32,true> value) {
    memory[address.slc<30>(2)] = value;
}

ac_int<32,true> Processor::memory_read(ac_int<32,true> memory[256],
                                       ac_int<32,false> address) {
    return memory[address.slc<30>(2)];
}

void Processor::write_back(ac_int<32,false> destination_reg, ac_int<32,true> value) {
    ac_int<5,false> reg_addr = destination_reg.slc<5>(0);
    R[reg_addr] = value;
}

bool Processor::run(ac_int<32,false> instr_mem[256], ac_int<32,true> data_mem[256]) {

    R[0] = 0;
    std::cout << std::string(72,'-');
    std::cout << std::endl;
    PC = next_PC;
    std::cout << "Current PC: " << PC << std::endl;
    ac_int<32,false> instruction = read_instruction(instr_mem);
    if(instruction == 0) { std::cout << "No instruction found, exit.\n"; return 0; }
    Operation operation = decode_read(instruction);
    next_PC = update_pc(operation);
    if(operation.control == 0 || operation.control == 4) { return 1; }
    ac_int<32,true> result = execute(operation);
    if(operation.control == 2) {
        write_back(operation.destination, memory_read(data_mem, result));
    } else if(operation.control == 3) {
        memory_write(data_mem, result, operation.destination);
    } else if(operation.control == 1) {
        write_back(operation.destination, result);
    }


    // tests
    std::cout << std::endl;

    std::cout << "Instruction type: " << operation.control << std::endl
              << "Operation: " << operation.ALU_opcode << std::endl
              << "Destination Register: " << operation.destination
              << std::endl;

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

    return 1;
}
