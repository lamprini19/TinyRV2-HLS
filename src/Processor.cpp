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
    ac_int<32,false> op_destination;
    ac_int<32,false> op_operand_1;
    ac_int<32,false> op_operand_2;
    ac_int<3,false> op_control;
    
    switch(opcode) {
        case 51:
            {
            // arithmetic
            ac_int<3,false> func3 = instruction.slc<3>(12);
    	    ac_int<7,false> func7 = instruction.slc<7>(25);
    	    ac_int<32,false> rd = instruction.slc<5>(7);
            ac_int<32,false> rs1 = instruction.slc<5>(15);
    	    ac_int<32,false> rs2 = instruction.slc<5>(20);
            
            op_destination = rd;
            op_operand_1 = R[rs1];
            op_operand_2 = R[rs2];
            op_control = 1;

            if(func3 == 0) {
                if(func7 == 0) {
                    std::cout << "ADD instruction" << std::endl;
                    ALU_opcode = 0;
    	        } else if(func7[5] == 1) {
                    std::cout << "SUB instruction" << std::endl;
                    ALU_opcode = 1;
                } else if(func7[0] == 1) {
                    std::cout << "MUL instruction" << std::endl;
                    ALU_opcode = 2;
                } else {
                    invalid_instruction = 1;
                    ALU_opcode = 0;
                }
            } else if(func3 == 7) {
                std::cout << "AND instruction" << std::endl;
                ALU_opcode = 3;
            } else if(func3 == 6) {
                std::cout << "OR instruction" << std::endl;
                ALU_opcode = 4;
            } else if(func3 == 4) {
                std::cout << "XOR instruction" << std::endl;
                ALU_opcode = 5;
            } else if(func3 == 2) {
                std::cout << "SLT instruction" << std::endl;
                ALU_opcode = 11;
            } else if(func3 == 3) {
                std::cout << "SLTU instruction" << std::endl;
                ALU_opcode = 12;
            } else if(func3 == 5) {
                if(func7[5] == 1) {
                    std::cout << "SRA instruction" << std::endl;
                    ALU_opcode = 7;
                } else if(func7 == 0) {
                    std::cout << "SRL instruction" << std::endl;
                    ALU_opcode = 8;
                } else {
                    invalid_instruction = 1;
                    ALU_opcode = 0;
                }
            } else if(func3 == 1) {
                std::cout << "SLL instruction" << std::endl;
                ALU_opcode = 6;
            } else {
                invalid_instruction = 1;
                ALU_opcode = 0;
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

            op_destination = rd;
            op_operand_1 = R[rs1];
            op_operand_2 = sext_imm;
            op_control = 1;

            if(func3 == 0) {
                std::cout<< "ADDI instruction" << std::endl;
                ALU_opcode = 0;
            } else if(func3 == 7) {
                std::cout << "ANDI instruction" << std::endl;
                ALU_opcode = 3;
            } else if(func3 == 6) {
                std::cout << "ORI instruction" << std::endl;
                ALU_opcode = 4;
            } else if(func3 == 4) {
                std::cout << "XORI instruction" << std::endl;
                ALU_opcode = 5;
            } else if(func3 == 2) {
                std::cout << "SLTI instruction" << std::endl;
                ALU_opcode = 11;
            } else if(func3 == 3) {
                std::cout << "SLTIU instruction" << std::endl;
                ALU_opcode = 12;
            } else if(func3 == 5) {
                if(imm[10] == 1) {
                    std::cout << "SRAI instruction" << std::endl;
                    ALU_opcode = 7;
                    op_operand_2 = imm.slc<5>(0);
                } else if(imm.slc<7>(5) == 0) {
                    std::cout << "SRLI instruction" << std::endl;
                    ALU_opcode = 8;
                    op_operand_2 = imm.slc<5>(0);
                } else {
                    invalid_instruction = 1;
                    ALU_opcode = 0;
                }           
            } else if(func3 == 1) {
                std::cout << "SLLI instruction" << std::endl;
                ALU_opcode = 6;
                Operation op(ALU_opcode, rd, R[rs1], imm, 1);
                return op;           
            } else {
                invalid_instruction = 1;
                ALU_opcode = 0;
                Operation op(ALU_opcode, 0, 0, 0, 1);
                return op;
            }
            break;
            }
        case 55:
            {
            // lui
            std::cout << "LUI instruction" << std::endl;
            ac_int<32,true> rd = instruction.slc<5>(7);
            ac_int<32,true> imm = 0;
            ac_int<20,true> imm_part = instruction.slc<20>(12);
            imm.set_slc(12,imm_part);

            ALU_opcode = 0;
            op_destination = rd;
            op_operand_1 = imm;
            op_operand_2 = 0;
            op_control = 1;

            break;
            }
        case 23:
            {
            // auipc
            std::cout << "AUIPC instruction" << std::endl;
            ac_int<32,true> rd = instruction.slc<5>(7);
            ac_int<32,true> imm = 0;
            ac_int<20,true> imm_part = instruction.slc<20>(12);
            imm.set_slc(12,imm_part);

            ALU_opcode = 0;
            op_destination = rd;
            op_operand_1 = imm;
            op_operand_2 = PC;
            op_control = 1;

            break;
            }
        case 3:
            {
            // lw
            std::cout << "LW instruction" << std::endl;
            ac_int<32,true> rd = instruction.slc<5>(7);
            ac_int<32,true> rs1 = instruction.slc<5>(15);
            ac_int<32,true> imm = instruction.slc<12>(20);
            // sign extend
            ac_int<20,false> temp = -1;
            ac_int<32,true> sext_imm = imm;
            if(imm[11] == 1) { sext_imm.set_slc(12,temp); }

            ALU_opcode = 0;
            op_destination = rd;
            op_operand_1 = imm;
            op_operand_2 = R[rs1];
            op_control = 2;

            Operation op(ALU_opcode, rd, imm, R[rs1], 2);
            return op;
            break;
            }
        case 35:
            {
            // sw
            std::cout << "SW instruction" << std::endl;
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

            ALU_opcode = 0;
            op_destination = R[rs2];
            op_operand_1 = R[rs1];
            op_operand_2 = sext_imm;
            op_control = 3;
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

            ALU_opcode = 0;
            op_destination = rd;
            op_operand_1 = PC;
            op_operand_2 = sext_imm;
            op_control = 4;
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
            op_destination = rd;
            op_operand_1 = R[rs1];
            op_operand_2 = sext_imm;
            op_control = 4;
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

            op_destination = sext_imm;
            op_operand_1 = R[rs1];
            op_operand_2 = R[rs2];
            op_control = 0;

            if(func3 == 0) {
                ALU_opcode = 9;
                std::cout << "If " << R[rs1] << " == " << R[rs2]
                          << " then PC <- PC + " << sext_imm << std::endl;
            } else if(func3 == 1) {
                std::cout << "If " << R[rs1] << " != " << R[rs2]
                          << " then PC <- PC + " << sext_imm << std::endl;
                ALU_opcode = 10;
            } else if(func3 == 4) {
                std::cout << "If " << R[rs1] << " <s " << R[rs2]
                          << " then PC <- PC + " << sext_imm << std::endl;
                ALU_opcode = 11;
            } else if(func3 == 5) {
                std::cout << "If " << R[rs1] << " >=s " << R[rs2]
                          << " then PC <- PC + " << sext_imm << std::endl;
                ALU_opcode = 13;
            } else if(func3 == 6) {
                std::cout << "If " << R[rs1] << " <u " << R[rs2]
                          << " then PC <- PC + " << sext_imm << std::endl;
                ALU_opcode = 12;
            } else if(func3 == 7) {
                std::cout << "If " << R[rs1] << " >=u " << R[rs2]
                          << " then PC <- PC + " << sext_imm << std::endl;
                ALU_opcode = 14;
            } else {
                invalid_instruction = 1;
                ALU_opcode = 0;
            }
            break;
            }
        default:
            invalid_instruction = 1;
            ALU_opcode = 0;
        }
    Operation op(ALU_opcode, op_destination, op_operand_1, op_operand_2, op_control);
    return op;
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
        std::cout << "Store " << PC+4 << " in " << reg_addr << std::endl;

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
    if(instruction == 0) {
        std::cout << "No instruction found, exit.\n";
        return 0;
    }

    Operation operation = decode_read(instruction);
    if(invalid_instruction) {
        std::cout << "Invalid instruction, exit.\n";
        return 0;
    }

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
        std::cout << "Wrote " << operation.destination
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
