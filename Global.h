//
// Created by 钱苏澄 on 2018/7/6.
//

#ifndef MIPSNEW_GLOBAL_H
#define MIPSNEW_GLOBAL_H

#include "InsMem.h"
#include "Parser.h"
#include <cstring>

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

extern int stallCycle[5];

#include "Global.cpp"

#endif //MIPSNEW_GLOBAL_H
