#ifndef OPERATION_H
#define OPERATION_H

#include "ac_int.h"

class Operation {
public:
    ac_int<32,true> (*operation)(ac_int<32,true>, ac_int<32,true>);
    ac_int<32,true> destination;
    ac_int<32,true> operand_1, operand_2;
    ac_int<3,false> control;

    Operation(ac_int<32,true> (*o)(ac_int<32,true>, ac_int<32,true>),
              ac_int<32,true> d,
              ac_int<32,true> op_1,
              ac_int<32,true> op_2,
              ac_int<3,false> c) {
        operation = &o;
        destination = d;
        operand_1 = op_1;
        operand_2 = op_2;
        control = c;
    }
}

#endif
