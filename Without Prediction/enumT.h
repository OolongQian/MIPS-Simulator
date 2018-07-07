//
// Created by 钱苏澄 on 2018/7/6.
//

#ifndef MIPSNEW_ENUMT_H
#define MIPSNEW_ENUMT_H
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
#endif //MIPSNEW_ENUMT_H
