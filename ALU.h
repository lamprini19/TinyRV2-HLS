#ifndef ALU_H
#define ALU_H

#include "ac_int.h"

class ALU {
    // arithmetic operations
    ac_int<32,true> add(ac_int<32,true> op1, ac_int<32,true> op2) {
        return op1 + op2;
    }
    ac_int<32,true> sub(ac_int<32,true> op1, ac_int<32,true> op2) {
        return op1 - op2;
    }
    ac_int<32,true> mul(ac_int<32,true> op1, ac_int<32,true> op2) {
        return op1 * op2;
    }

    // bitwise logical operations
    ac_int<32,true> bwand(ac_int<32,true> op1, ac_int<32,true> op2) {
        return op1 & op2;
    }
    ac_int<32,true> bwor(ac_int<32,true> op1, ac_int<32,true> op2) {
        return op1 | op2;
    }
    ac_int<32,true> bwxor(ac_int<32,true> op1, ac_int<32,true> op2) {
        return op1 ^ op2;
    }

    // shift operations
    ac_int<32,true> shift_left(ac_int<32,true> op1, ac_int<32,true> op2) {
        ac_int<5, false> op2_bottom = op2.slc<5>(0);
        return op1 << op2_bottom;
    }
    ac_int<32,true> shift_right_a(ac_int<32,true> op1, ac_int<32,true> op2) {
        ac_int<5, false> op2_bottom = op2.slc<5>(0);
        return op1 >> op2_bottom;
    }
    ac_int<32,true> shift_right_l(ac_int<32,true> op1, ac_int<32,true> op2) {
        ac_int<5, false> op2_bottom = op2.slc<5>(0);
        ac_int<32,false> uns1 = op1;
        return uns1 >> op2_bottom;
    }

    // comparison operations
    ac_int<32,true> equal(ac_int<32,true> op1, ac_int<32,true> op2) {
        return op1 == op2;
    }
    ac_int<32,true> not_equal(ac_int<32,true> op1, ac_int<32,true> op2) {
        return equal(op1, op2).slc<1>(0).bit_complement();
    }
    ac_int<32,true> less_than(ac_int<32,true> op1, ac_int<32,true> op2) {
        return op1 < op2;
    }
    ac_int<32,true> less_than_u(ac_int<32,true> op1, ac_int<32,true> op2) {
        ac_int<32,false> uns1 = op1;
        ac_int<32,false> uns2 = op2;
        return uns1 < uns2;
    }
    ac_int<32,true> greater_than(ac_int<32,true> op1, ac_int<32,true> op2) {
        return op1 >= op2;
    }
    ac_int<32,true> greater_than_u(ac_int<32,true> op1, ac_int<32,true> op2) {
        ac_int<32,false> uns1 = op1;
        ac_int<32,false> uns2 = op2;
        return uns1 >= uns2;
    }

public:
    ac_int<32,true> operation(ac_int<32,true> operand_1,
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
            default:
                return 0;
        }
    }
};
#endif
