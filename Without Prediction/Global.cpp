//
// Created by 钱苏澄 on 2018/7/6.
//

ifstream fp;

/**
 * instructions have linear structure.
 * */
vector<Instruction> *instructionPool;

/**
 * a main memory for static, stack, heap.
 * */
MemoryHW *mainMemory;

/**
 * simulator 35 registers.
 * */
int registers[35];

/**
 * two label maps.
 * */
map<string, int> var2mem;
map<string, int> lab2src;

map<string, short> regTable;
map<string, opType> opTable;

bool returnFlag;


/**
 * If this bool value is true, it means instruction vector is read to the end,
 * or system call for program end is received.
 *
 * No more instruction will be fetched.
 * */
bool code_end;
bool syscall_terminate;

int stallCycle[5];

int reg_in_use[35];
