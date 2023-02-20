#include "ac_int.h"
#include "ALU.h"

// Arithmetic Operations
ac_int<32,true> ALU::add(ac_int<32,true> op1, ac_int<32,true> op2) {
    return op1 + op2;
}

ac_int<32,true> ALU::sub(ac_int<32,true> op1, ac_int<32,true> op2) {
    return op1 - op2;
}

ac_int<32,true> ALU::mul(ac_int<32,true> op1, ac_int<32,true> op2) {
    return op1 * op2;
}

// Bitwise Logical Operations
ac_int<32,true> ALU::bwand(ac_int<32,true> op1, ac_int<32,true> op2) {
    return op1 & op2;
}

ac_int<32,true> ALU::bwor(ac_int<32,true> op1, ac_int<32,true> op2) {
    return op1 | op2;
}

ac_int<32,true> ALU::bwxor(ac_int<32,true> op1, ac_int<32,true> op2) {
    return op1 ^ op2;
}
