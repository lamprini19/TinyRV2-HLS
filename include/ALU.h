#ifndef ALU_H
#define ALU_H

class ALU {
public:
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
    ac_int<32,true> shift_right(ac_int<32,true>, ac_int<32,true>, bool);

    // comparison operations
    ac_int<32,true> less_than(ac_int<32,true>, ac_int<32,true>, bool);
};
#endif
