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
    ac_int<5, false> op2_bottom = op2.slc<5>(0);
    return op1 << op2_bottom;
}

ac_int<32, true> ALU::shift_right_a(ac_int<32,true> op1, ac_int<32,true> op2) {
    ac_int<5, false> op2_bottom = op2.slc<5>(0);
    return op1 >> op2_bottom;
}

ac_int<32, true> ALU::shift_right_l(ac_int<32,true> op1, ac_int<32,true> op2) {
    ac_int<5, false> op2_bottom = op2.slc<5>(0);
    ac_int<32,false> uns1 = op1;
    return uns1 >> op2_bottom;
}

// Comparison Operations
ac_int<32,true> ALU::equal(ac_int<32,true> op1, ac_int<32,true> op2) {
    return op1 == op2;
}

ac_int<32,true> ALU::not_equal(ac_int<32,true> op1, ac_int<32,true> op2) {
    return equal(op1, op2).slc<1>(0).bit_complement();
}

ac_int<32, true> ALU::less_than(ac_int<32,true> op1, ac_int<32,true> op2) {
    return op1 < op2;
}

ac_int<32, true> ALU::less_than_u(ac_int<32,true> op1, ac_int<32,true> op2) {
    ac_int<32,false> uns1 = op1;
    ac_int<32,false> uns2 = op2;
    return uns1 < uns2;
}

ac_int<32, true> ALU::greater_than(ac_int<32,true> op1, ac_int<32,true> op2) {
    return op1 >= op2;
}

ac_int<32, true> ALU::greater_than_u(ac_int<32,true> op1, ac_int<32,true> op2) {
    ac_int<32,false> uns1 = op1;
    ac_int<32,false> uns2 = op2;
    return uns1 >= uns2;
}


ac_int<32,true> ALU::operation(ac_int<32,true> operand_1,
                               ac_int<32,true> operand_2,
                               ac_int<4,false> opcode) {
     switch(opcode) {
        case  0:
            return add(operand_1, operand_2);
        case  1:
            return sub(operand_1, operand_2);
        case  2:
            return mul(operand_1, operand_2);
        case  3:
            return bwand(operand_1, operand_2);
        case  4:
            return bwor(operand_1, operand_2);
        case  5:
            return bwxor(operand_1, operand_2);
        case  6:
            return shift_left(operand_1, operand_2);
        case  7:
            return shift_right_a(operand_1, operand_2);
        case  8:
            return shift_right_l(operand_1, operand_2);
        case  9:
            return equal(operand_1, operand_2);
        case 10:
            return not_equal(operand_1, operand_2);
        case 11:
            return less_than(operand_1, operand_2);
        case 12:
            return less_than_u(operand_1, operand_2);
        case 13:
            return greater_than(operand_1, operand_2);
        case 14:
            return greater_than_u(operand_1, operand_2);
     }
}
