//
// Created by 钱苏澄 on 2018/7/3.
//

#ifndef MYMIPSRE_INSTRUCTION_H
#define MYMIPSRE_INSTRUCTION_H

#include <cstring>
#include <cstdio>
#include <vector>

using namespace std;

/**
 * This is an op type, which can be different from source.
 * Note that ADD has multiple types, but we make it identical.
 * Note that div has 2 cases, we distinguish them from opType.
 * */
enum opType {
    ADD, ADDU, ADDIU, SUB, SUBU, MUL, MULU, DIV, DIVU,
    XOR, XORU, NEG, NEGU, REM, REMU, LI, SEQ, SGE, SGT,
    SLE, SLT, SNE, BB, BEQ, BNE, BGE, BLE, BGT, BLT, BEQZ, BNEZ,
    BLEZ, BGEZ, BGTZ, BLTZ, J, JR, JAL, JALR, LA, LB, LH, LW,
    SB, SH, SW, MOVE, MFHI, MFLO, NOP, SYSCALL
};

/**
 * A class that simulates each line of mips source code.
 * It stores all necessary information for a line of source code.
 * */
struct Instruction {
    opType op;
    vector<string> tokens;
    int ins_code[4];
    bool not_imm;
    bool ch_in_str;
    Instruction(opType opT, vector<string> vec): op(opT), tokens(vec) {
    		memset(ins_code, 0, sizeof(ins_code));
		    not_imm = false;
		    ch_in_str = false;
    }
};

#endif //MYMIPSRE_INSTRUCTION_H
