#ifndef MIPS_COMMON_H
#define MIPS_COMMON_H

#include <iostream>
#include <map>
#include <queue>
#include "memory.h"

using namespace std;

enum stageType {
	FETCH, DECODE, EXECUTE, MEMORY, WRITEBACK
};

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
	std::vector<std::string> tokens;
	int ins_code[4];
	bool not_imm;
	bool ch_in_str;
	Instruction(opType opT, std::vector<std::string> vec): op(opT), tokens(vec) {
		memset(ins_code, 0, sizeof(ins_code));
		not_imm = false;
		ch_in_str = false;
	}
};

extern ifstream fp;

extern vector<Instruction> *instructionPool;

extern MemoryHW *mainMemory;

extern int registers[35];

extern map<string, short> regTable;
extern map<string, opType> opTable;

extern map<string, int> var2mem;
extern map<string, int> lab2src;

extern bool returnFlag;

/// simulate queue.
extern int reg_in_use[35];

extern bool code_end;
extern bool syscall_terminate;

extern int converter[5];

/**
 * A index queue for coordinating
 * */
extern queue<int> reserve;

extern bool structureHazard;

extern bool dataHazard;

extern int unpredictedJumpNum;

extern queue<int> predictedAddress;

extern queue<int> predictJInstructions;

extern map<int, int> BTB;

extern bool flushSignal;

extern int predicting;

extern map<int, int> predictTable;

extern int globalPredHistory;

extern int predictTimes;
extern int predictCorrectTimes;

extern queue<int> predictIndices;


#endif //MIPS_COMMON_H
