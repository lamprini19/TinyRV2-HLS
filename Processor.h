#ifndef PROCESSOR_H
#define PROCESSOR_H

#include "ac_int.h"
#include "Operation.h"
#include "ALU.h"

#include "mc_verify.h"

class Processor {
    ac_int<32,true> R[32];
    ac_int<32,false> PC;
    ac_int<32,false> next_PC;
    ALU alu;

public:
    ac_int<1,false> invalid_instruction;

private:
    ac_int<32,false> read_instruction(ac_int<32,false> instr_mem[256]) {
        return instr_mem[next_PC.slc<30>(2)];
    }

    Operation decode_read(ac_int<32,false> instruction) {
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
                        ALU_opcode = 0;
                    } else if(func7[5] == 1) {
                        ALU_opcode = 1;
                    } else if(func7[0] == 1) {
                        ALU_opcode = 2;
                    } else {
                        invalid_instruction = 1;
                        ALU_opcode = 0;
                    }
                } else if(func3 == 7) {
                    ALU_opcode = 3;
                } else if(func3 == 6) {
                    ALU_opcode = 4;
                } else if(func3 == 4) {
                    ALU_opcode = 5;
                } else if(func3 == 2) {
                    ALU_opcode = 11;
                } else if(func3 == 3) {
                    ALU_opcode = 12;
                } else if(func3 == 5) {
                    if(func7[5] == 1) {
                        ALU_opcode = 7;
                    } else if(func7 == 0) {
                        ALU_opcode = 8;
                    } else {
                        invalid_instruction = 1;
                        ALU_opcode = 0;
                    }
                } else if(func3 == 1) {
                    ALU_opcode = 6;
                } else {
                    invalid_instruction = 1;
                    ALU_opcode = 0;
                }

                // update PC
                next_PC = PC + 4;

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
                    ALU_opcode = 0;
                } else if(func3 == 7) {
                    ALU_opcode = 3;
                } else if(func3 == 6) {
                    ALU_opcode = 4;
                } else if(func3 == 4) {
                    ALU_opcode = 5;
                } else if(func3 == 2) {
                    ALU_opcode = 11;
                } else if(func3 == 3) {
                    ALU_opcode = 12;
                } else if(func3 == 5) {
                    if(imm[10] == 1) {
                        ALU_opcode = 7;
                        op_operand_2 = imm.slc<5>(0);
                    } else if(imm.slc<7>(5) == 0) {
                        ALU_opcode = 8;
                        op_operand_2 = imm.slc<5>(0);
                    } else {
                        invalid_instruction = 1;
                        ALU_opcode = 0;
                    }           
                } else if(func3 == 1) {
                    ALU_opcode = 6;
                    op_operand_2 = imm;
                } else {
                    invalid_instruction = 1;
                    ALU_opcode = 0;
                }

                // update PC
                next_PC = PC + 4;

                break;
                }
            case 55:
                {
                // lui
                ac_int<32,true> rd = instruction.slc<5>(7);
                ac_int<32,true> imm = 0;
                ac_int<20,true> imm_part = instruction.slc<20>(12);
                imm.set_slc(12,imm_part);

                ALU_opcode = 0;
                op_destination = rd;
                op_operand_1 = imm;
                op_operand_2 = 0;
                op_control = 1;

                // update PC
                next_PC = PC + 4;

                break;
                }
            case 23:
                {
                // auipc
                ac_int<32,true> rd = instruction.slc<5>(7);
                ac_int<32,true> imm = 0;
                ac_int<20,true> imm_part = instruction.slc<20>(12);
                imm.set_slc(12,imm_part);

                ALU_opcode = 0;
                op_destination = rd;
                op_operand_1 = imm;
                op_operand_2 = PC;
                op_control = 1;

                // update PC
                next_PC = PC + 4;

                break;
                }
            case 3:
                {
                // lw
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

                // update PC
                next_PC = PC + 4;

                break;
                }
            case 35:
                {
                // sw
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
                
                // update PC
                next_PC = PC + 4;

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

                // update PC
                ac_int<5,false> reg_addr = op_destination.slc<5>(0);
                R[reg_addr] = PC + 4;
                next_PC = op_operand_1 + op_operand_2;

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
                if(imm[11] == 1) { sext_imm.set_slc(12,temp); }
                // create Operation object
                ALU_opcode = 0;
                op_destination = rd;
                op_operand_1 = R[rs1];
                op_operand_2 = sext_imm;
                op_control = 4;

                // update PC
                ac_int<5,false> reg_addr = op_destination.slc<5>(0);
                R[reg_addr] = PC + 4;
                next_PC = op_operand_1 + op_operand_2;
                
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
                ac_int<1,false> result;

                if(func3 == 0) {
                    ALU_opcode = 9;
                    result = (op_operand_1 == op_operand_2);
                } else if(func3 == 1) {
                    ALU_opcode = 10;
                    result = (op_operand_1 != op_operand_2);
                } else if(func3 == 4) {
                    ALU_opcode = 11;
                    result = (op_operand_1 < op_operand_2);
                } else if(func3 == 5) {
                    ALU_opcode = 13;
                    result = (op_operand_1 >= op_operand_2);
                } else if(func3 == 6) {
                    ALU_opcode = 12;
                    result = (op_operand_1 < op_operand_2);
                } else if(func3 == 7) {
                    ALU_opcode = 14;
                    result = (op_operand_1 >= op_operand_2);
                } else {
                    invalid_instruction = 1;
                    ALU_opcode = 0;
                }

                // update PC
                if(result[0]) {
                    next_PC = PC + op_destination;
                } else {
                    next_PC = PC + 4;
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

    ac_int<32,true> execute(Operation op) {
        return alu.operation(op.operand_1, op.operand_2, op.ALU_opcode);
    }

    void memory_write(ac_int<32,true> memory[256], ac_int<32,false> address, ac_int<32,true> value) {
        memory[address.slc<30>(2)] = value;
    }

    ac_int<32,true> memory_read(ac_int<32,true> memory[256], ac_int<32,false> address) {
        return memory[address.slc<30>(2)];
    }

    void write_back(ac_int<32,false> destination_reg, ac_int<32,true> value) {
        ac_int<5,false> reg_addr = destination_reg.slc<5>(0);
        R[reg_addr] = value;
    }

public:
    Processor() {
        next_PC = 200;
        std::cout << "Initial PC is: " << next_PC << std::endl;
        R[0] = 0;
        invalid_instruction = 0;
    }

    bool CCS_BLOCK(run)(ac_int<32,false> instr_mem[256], ac_int<32,true> data_mem[256]) {

        R[0] = 0;
        PC = next_PC;

        #ifndef _SYNTHESIS_
        std::cout << std::string(72,'-');
        std::cout << std::endl;
        std::cout << "Current PC: " << PC << std::endl;
        #endif

        ac_int<32,false> instruction = read_instruction(instr_mem);

        Operation operation = decode_read(instruction);
        if(invalid_instruction) {
            #ifndef _SYNTHESIS_
            std::cout << "Invalid instruction, exit.\n";
            #endif
            return 0;
        }

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
        #ifndef _SYNTHESIS_
        std::cout << std::endl;

        std::cout << "Instruction type: " << operation.control << std::endl
                << "Operation: " << operation.ALU_opcode << std::endl
                << "Destination Register: " << operation.destination
                << std::endl;

        std::cout << "Result of operating with " << operation.operand_1
                << " and " << operation.operand_2
                << " is " << result
                << " or in binary: "
                << result.to_string(AC_BIN,false)
                << std::endl;

        if(operation.control == 3) {
            std::cout << "Wrote " << operation.destination
                    << " on memory address " << result
                    << " (" << result.to_string(AC_HEX,false)
                    << ")" << std::endl;
        }

        if(operation.control == 2) {
            std::cout << "Read value " << memory_read(data_mem,result)
                    << "from memory address " << result
                    << " (" << result.to_string(AC_HEX,false) << ")" 
                    << " in register " << operation.destination
                    << std::endl;
        }

        // test write_back
        std::cout << "Value of register "
                << operation.destination
                << ": " << R[operation.destination]
                << std::endl;
        #endif

        return 1;
    }
};

#endif
