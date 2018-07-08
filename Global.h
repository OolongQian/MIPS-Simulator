//
// Created by 钱苏澄 on 2018/7/6.
//

#ifndef MIPSNEW_GLOBAL_H
#define MIPSNEW_GLOBAL_H

#include "InsMem.h"
#include "Parser.h"
#include <cstring>
#include <queue>

using namespace std;

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

extern queue<int> predictInstructions;

extern map<int, int> BTB;

extern bool flushSignal;

extern int predicting;

extern map<int, int> predictTable;

extern int globalPredHistory;

extern int predictTimes;
extern int predictCorrectTimes;

extern queue<int> predictIndices;

#include "Global.cpp"

#endif //MIPSNEW_GLOBAL_H
