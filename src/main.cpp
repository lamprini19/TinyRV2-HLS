#include "ac_int.h"
#include "Processor.h"

int main() {
    /** Test 1 **/
    std::cout << std::string(72,'-') << std::endl
              << "Test 1: Simple program to test individual commands."
              << std::endl << std::string(72,'-') << std::endl;

    // Initialize memories
    ac_int<32,false> instr_mem[256];
    ac::init_array<AC_VAL_0>(instr_mem, 256);
    ac_int<32,true> data_mem[256];
    ac::init_array<AC_VAL_0>(data_mem, 256);

    // Initialize some memory addresses
    data_mem[71] = -600;

    // Write program
    instr_mem[50] = 0b00000100011000000000001000010011; // ADDI R4, R0, 70
    instr_mem[51] = 0b00000000010000100010001010000011; // LW   R5, mem[ R4 + 1 ]
    instr_mem[52] = 0b00000010010000101000001100110011; // MUL  R6, R5, R4
    instr_mem[53] = 0b00000000011000100010010000100011; // SW   mem[r4 + 2] = R6
    instr_mem[54] = 0b00000000000000000000000110010011; // ADDI R3, R0, 0
    instr_mem[55] = 0b00000000000000110000001110010011; // ADDI R7, R6, 0
    instr_mem[56] = 0b00000000011000111111001110110011; // AND  R7, R7, R6
    instr_mem[57] = 0b00001110001100101111001100010011; // ANDI R6, R5, 000011100011
    instr_mem[58] = 0b00001110001100101110010000010011; // ORI  R8, R5, 000011100011
    instr_mem[59] = 0b00001110001100000000001000010011; // ADDI R4, R0, 000011100011
    instr_mem[60] = 0b00000000011000100100001000110011; // XOR  R4, R4, R6
    instr_mem[61] = 0b01110000100100110100001010010011, // XORI R5, R6, 011100001001
    instr_mem[62] = 0b11000000111100000000001000010011; // ADDI  R4, R0, 110000001111
    instr_mem[63] = 0b00000000001100000000001010010011; // ADDI  R5, R0, 000011111111
    instr_mem[64] = 0b00000000010100100010001100110011; // SLT   R6, R4, R5
    instr_mem[65] = 0b00000000010100100011001110110011; // SLTU  R7, R4, R5
    instr_mem[66] = 0b00111100010100100010010000010011; // SLTI  R8, R4, 001111000101
    instr_mem[67] = 0b11111100010100101011010010010011; // SLTIU R9, R5, 111111000101
    instr_mem[68] = 0b01000000010100100101001100110011; // SRA  R6,  R4, R5
    instr_mem[69] = 0b00000000010100100101001110110011; // SRL  R7,  R4, R5
    instr_mem[70] = 0b00000000010100100001010000110011; // SLL  R8,  R4, R5
    instr_mem[71] = 0b01000000001100100101010010010011; // SRAI R9,  R4, 00011
    instr_mem[72] = 0b00000000001100100101010100010011; // SRLI R10, R4, 00011
    instr_mem[73] = 0b00000000001100100001010110010011; // SLLI R11, R4, 00011
    instr_mem[74] = 0b10101010100000011011001100110111; // LUI   R6, 10101010100000011011
    instr_mem[75] = 0b00000000000000000000001110010111; // AUIPC R7, 00000000000000000000
    instr_mem[76] = 0b01000000011100011000000100110011; // SUB   R2, R3, R7
    instr_mem[77] = 0b00000000000000010101010001100011; // BGE   R2, R0, 000000001000
    instr_mem[78] = 0b11111111010111111111000111101111; // JAL  R3, -12
    instr_mem[79] = 0b00000001000000010000000111100111; // JALR R3, R2, 16
    instr_mem[80] = 0b0;
    instr_mem[81] = 0b0;
    instr_mem[82] = 0b0;
    instr_mem[83] = 0b0;
    instr_mem[84] = 0b0;
    instr_mem[85] = 0b0;
    instr_mem[86] = 0b0;
    instr_mem[87] = 0b0;
    instr_mem[88] = 0b0;

    Processor processor = Processor();

    while(processor.run(instr_mem, data_mem));

    /** Test 2 **/
    std::cout << std::endl << std::string(72,'-') << std::endl
              << "Test 2: Test invalid instruction response."
              << std::endl << std::string(72,'-') << std::endl;

    // Initialize memories
    ac_int<32,false> instr_mem_2[256];
    ac::init_array<AC_VAL_0>(instr_mem_2, 256);
    ac_int<32,true> data_mem_2[256];
    ac::init_array<AC_VAL_0>(data_mem_2, 256);

    instr_mem_2[50] = 0b00000100011000000000001001111111;

    Processor processor_2 = Processor();
    std::cout << "Initial Processor's invalid_instruction signal: "
              << processor_2.invalid_instruction
              << std::endl;

    while(processor_2.run(instr_mem_2, data_mem_2));

    std::cout << "Processor's invalid_instruction signal: "
              << processor_2.invalid_instruction
              << std::endl;

    return 0;
}

