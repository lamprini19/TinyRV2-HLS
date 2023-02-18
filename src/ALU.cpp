#include "ac_int.h"
#include "ALU.h"

ac_int<32,true> ALU::add(ac_int<32,true> operand_1, ac_int<32,true> operand_2) {
    return operand_1 + operand_2;
}
