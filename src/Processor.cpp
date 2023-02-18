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

