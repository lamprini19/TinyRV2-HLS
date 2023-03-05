#ifndef ALU_H
#define ALU_H

class ALU {
    // arithmetic operations
    ac_int<32,true> add(ac_int<32,true>, ac_int<32,true>);
    ac_int<32,true> sub(ac_int<32,true>, ac_int<32,true>);
    ac_int<32,true> mul(ac_int<32,true>, ac_int<32,true>);

    // bitwise logical operations
    ac_int<32,true> bwand(ac_int<32,true>, ac_int<32,true>);
    ac_int<32,true> bwor(ac_int<32,true>, ac_int<32,true>);
    ac_int<32,true> bwxor(ac_int<32,true>, ac_int<32,true>);

    // shift operations
    ac_int<32,true> shift_left(ac_int<32,true>, ac_int<32,true>);
    ac_int<32,true> shift_right_a(ac_int<32,true>, ac_int<32,true>);
    ac_int<32,true> shift_right_l(ac_int<32,true>, ac_int<32,true>);

    // comparison operations
    ac_int<32,true> equal(ac_int<32,true>, ac_int<32,true>);
    ac_int<32,true> not_equal(ac_int<32,true>, ac_int<32,true>);
    ac_int<32,true> less_than(ac_int<32,true>, ac_int<32,true>);
    ac_int<32,true> less_than_u(ac_int<32,true>, ac_int<32,true>);
    ac_int<32,true> greater_than(ac_int<32,true>, ac_int<32,true>);
    ac_int<32,true> greater_than_u(ac_int<32,true>, ac_int<32,true>);

public:
    ac_int<32,true> operation(ac_int<32,true> operand_1,
                              ac_int<32,true> operand_2,
                              ac_int<4,false> opcode);
};
#endif
