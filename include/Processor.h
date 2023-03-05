#ifndef PROCESSOR_H
#define PROCESSOR_H

#include "ac_int.h"
#include "Operation.h"
#include "ALU.h"

class Processor {
    ac_int<32,true> R[32];
    ac_int<32,false> PC;
    ac_int<32,false> next_PC;
    ALU alu;

public:
    ac_int<1,false> invalid_instruction;

private:
    ac_int<32,false> read_instruction(ac_int<32,false> instr_mem[256]);
    Operation decode_read(ac_int<32,false> instruction);
    ac_int<32,false> update_pc(Operation);
    ac_int<32,true> execute(Operation);
    void memory_write(ac_int<32,true> memory[256],
                      ac_int<32,false> address,
                      ac_int<32,true> value);
    ac_int<32,true> memory_read(ac_int<32,true> memory[256],
                                ac_int<32,false> address);
    void write_back(ac_int<32,false> destination_reg, ac_int<32,true> value);

public:
    Processor() {
        next_PC = 200;
        std::cout << "Initial PC is: " << next_PC << std::endl;
        R[0] = 0;
        invalid_instruction = 0;
    }
    bool run(ac_int<32,false> instr_mem[256], ac_int<32,true> data_mem[256]);
};

#endif
