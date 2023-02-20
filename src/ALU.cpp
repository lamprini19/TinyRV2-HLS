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

// Shift Operations
ac_int<32, true> ALU::shift_left(ac_int<32,true> op1, ac_int<32,true> op2) {
    ac_int<5, false> op2 = op2.slc<5>(0);
    return op1 << op2;
}

ac_int<32, true> ALU::shift_left(ac_int<32,true> op1, ac_int<32,true> op2, bool sign) {
    ac_int5, false> op2 = op2.slc<5>(0);
    if(!sign) {
        ac_int<32,false> uns1 = op1;
        return uns1 >> op2;
    } else {
        return op1 >> op2;
    }
}
