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
    instr_mem[79] = 0b00000000000000000000000100010111; // AUIPC R2, 00000000000000000000
    instr_mem[80] = 0b00000001100000010000000111100111; // JALR R3, R2, 16
    instr_mem[81] = 0b00000000010100100110001001100011; // BLTU R4, R5, 8
    instr_mem[82] = 0b00000000010100100100101001100011; // BLT  R4, R5, 20
    instr_mem[83] = 0b00000000010100100000001000110011; // ADD  R4, R4, R5
    instr_mem[84] = 0b0;
    instr_mem[85] = 0b00000000010100100000110001100011; // BEQ  R4, R5, 24
    instr_mem[86] = 0b11111110010100100001011011100011; // BNE  R4, R5, -20
    instr_mem[87] = 0b11111110010100100111100011100011; // BGEU R4, R5, -16

    Processor processor = Processor();

    while(processor.run(instr_mem, data_mem));

    /** Test 2 - Wrong Opcode **/
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
              << std::endl << std::endl << std::endl;

    /** Test 3 - Fibonacci **/
    std::cout << std::endl << std::string(72,'-') << std::endl
              << "Test 3: Fibonacci numbers"
              << std::endl << std::string(72,'-') << std::endl;
    // main
    instr_mem[50] = 0b00000000000000000000011000010011; // ADDI R12, R0 , 0
    instr_mem[51] = 0b00000000000100000000010110010011; // ADDI R11, R0 , 1
    instr_mem[52] = 0b00000000000000000000010000010011; // ADDI R8 , R0 , 0
    instr_mem[53] = 0b00001101110000000000100100010011; // ADDI R18, R0 , loop (220)
    instr_mem[54] = 0b00000000111100000000010010010011; // ADDI R9 , R0 , 15
    // loop
    instr_mem[55] = 0b00001000100101000101011001100011; // BGE  R8 , R9 , 140
    instr_mem[56] = 0b00010010110000000000000011100111; // JALR R1 , R0 , fibo (300)
    instr_mem[57] = 0b00000000001001000001001110010011; // SLLI R7 , R8 , 2
    instr_mem[58] = 0b00000000101000111010101000100011; // SW   R10, R7 , bgn (20)
    instr_mem[59] = 0b00000000000101000000010000010011; // ADDI R8 , R8 , 1
    instr_mem[60] = 0b00000000000010010000100111100111; // JALR R19, R18, 0
    // end of program
    instr_mem[61] = 0b0;
    // fibonacci calculations
    instr_mem[75] = 0b00000000110001011000010100110011; // ADD  R10, R11, R12
    instr_mem[76] = 0b00000000000001011000011000110011; // ADD  R12, R11, R0
    instr_mem[77] = 0b00000000000001010000010110110011; // ADD  R11, R10, R0
    instr_mem[78] = 0b00000000000000001000100111100111; // JALR R19, R1,  0   
 
    Processor processor_3 = Processor();
    while(processor_3.run(instr_mem, data_mem));
    std::cout << std::endl << "Fibonacci numbers read from memory:" << std::endl;
    int expected_fibo[15] = {1,2,3,5,8,13,21,34,55,89,144,233,377,610,987};
    for(int i=0; i<15; i++) {
        std::cout << data_mem[5+i] << " ("
                  << ((data_mem[5+i]==expected_fibo[i]) ? "correct)" : "false)")
                  << std::endl;

    }

    return 0;
}



