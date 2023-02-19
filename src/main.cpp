#include "ac_int.h"
#include "Processor.h"

int main() {
    // Initialize memories
    ac_int<32,false> instr_mem[256];
    ac::init_array<AC_VAL_0>(instr_mem, 256);
    ac_int<32,true> data_mem[256];
    ac::init_array<AC_VAL_0>(data_mem, 256);
    
    // Write program
    instr_mem[50] = 0b00000000010000111000001010110011;

    Processor processor = Processor();
    processor.run(instr_mem, data_mem);

    return 0;

}

