//
// Created by 钱苏澄 on 2018/7/3.
//

#ifndef MYMIPSRE_SIMULATOR_H
#define MYMIPSRE_SIMULATOR_H

#include <vector>
#include <map>
#include <fstream>
#include "Instruction.h"
#include "MemoryHW.h"
#include "Parser.h"
#include "Processors.h"

using namespace std;


ifstream fp;
/**
 * myLexer parses the source code file.
 * */
Parser *parser;

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

class Simulator {
private:
    void init_map() {
        regTable["$0"] = 0;    regTable["$zero"]   = 0;
        regTable["$1"] = 1;    regTable["$at"]     = 1;
        regTable["$2"] = 2;    regTable["$v0"]     = 2;
        regTable["$3"] = 3;    regTable["$v1"]     = 3;
        regTable["$4"] = 4;    regTable["$a0"]     = 4;
        regTable["$5"] = 5;    regTable["$a1"]     = 5;
        regTable["$6"] = 6;    regTable["$a2"]     = 6;
        regTable["$7"] = 7;    regTable["$a3"]     = 7;
        regTable["$8"] = 8;    regTable["$t0"]     = 8;
        regTable["$9"] = 9;    regTable["$t1"]     = 9;
        regTable["$10"] = 10;    regTable["$t2"]     = 10;
        regTable["$11"] = 11;    regTable["$t3"]     = 11;
        regTable["$12"] = 12;    regTable["$t4"]     = 12;
        regTable["$13"] = 13;    regTable["$t5"]     = 13;
        regTable["$14"] = 14;    regTable["$t6"]     = 14;
        regTable["$15"] = 15;    regTable["$t7"]     = 15;
        regTable["$16"] = 16;    regTable["$s0"]     = 16;
        regTable["$17"] = 17;    regTable["$s1"]     = 17;
        regTable["$18"] = 18;    regTable["$s2"]     = 18;
        regTable["$19"] = 19;    regTable["$s3"]     = 19;
        regTable["$20"] = 20;    regTable["$s4"]     = 20;
        regTable["$21"] = 21;    regTable["$s5"]     = 21;
        regTable["$22"] = 22;    regTable["$s6"]     = 22;
        regTable["$23"] = 23;    regTable["$s7"]     = 23;
        regTable["$24"] = 24;    regTable["$t8"]     = 24;
        regTable["$25"] = 25;    regTable["$t9"]     = 25;
        regTable["$28"] = 28;    regTable["$gp"]     = 28;
        regTable["$29"] = 29;    regTable["$sp"]     = 29;
        regTable["$30"] = 30;    regTable["$fp"]     = 30;
        regTable["$31"] = 31;    regTable["$ra"]     = 31;
        regTable["$32"] = 32;    regTable["$hi"]     = 32;
        regTable["$33"] = 33;    regTable["$lo"]     = 33;
        regTable["$34"] = 34;    regTable["$pc"]     = 34;

        /**
         * use opTable to handle comparison and branch_jump load, store, move command.
         * so that we can handle instruction with the same format once and for all.
         * */
        opTable["add"] = ADD;
        opTable["addu"] = ADDU;
        opTable["addiu"] = ADDIU;
        opTable["sub"] = SUB;
        opTable["subu"] = SUBU;
        opTable["mul"] = MUL;
        opTable["mulu"] = MULU;
        opTable["div"] = DIV;
        opTable["divu"] = DIVU;
        opTable["xor"] = XOR;
        opTable["xoru"] = XORU;
        opTable["neg"] = NEG;
        opTable["negu"] = NEGU;
        opTable["rem"] = REM;
        opTable["remu"] = REMU;
        opTable["li"] = LI;
        opTable["seq"] = SEQ;
        opTable["sge"] = SGE;
        opTable["sgt"] = SGT;
        opTable["sle"] = SLE;
        opTable["slt"] = SLT;
        opTable["sne"] = SNE;
        opTable["b"] = BB;
        opTable["beq"] = BEQ;
        opTable["bne"] = BNE;
        opTable["bge"] = BGE;
        opTable["ble"] = BLE;
        opTable["bgt"] = BGT;
        opTable["blt"] = BLT;
        opTable["beqz"] = BEQZ;
        opTable["bnez"] = BNEZ;
        opTable["blez"] = BLEZ;
        opTable["bgez"] = BGEZ;
        opTable["bgtz"] = BGTZ;
        opTable["bltz"] = BLTZ;
        opTable["j"] = J;
        opTable["jr"] = JR;
        opTable["jal"] = JAL;
        opTable["jalr"] = JALR;
        opTable["la"] = LA;
        opTable["lb"] = LB;
        opTable["lh"] = LH;
        opTable["lw"] = LW;
        opTable["sb"] = SB;
        opTable["sh"] = SH;
        opTable["sw"] = SW;
        opTable["move"] = MOVE;
        opTable["mfhi"] = MFHI;
        opTable["mflo"] = MFLO;
        opTable["nop"] = NOP;
        opTable["syscall"] = SYSCALL;
    }

    Processor procs;
public:
    Simulator() {
        parser = new Parser();
        instructionPool = new vector<Instruction>;
        mainMemory = new MemoryHW();
        memset(registers, 0, sizeof(registers));
        registers[29] = 4 * 1024 * 1024 - 1;
        var2mem.clear();
        lab2src.clear();
        init_map();
        returnFlag = false;
    }

    ~Simulator() {
        delete parser;
        delete instructionPool;
        delete mainMemory;
    }

    /**
     * use parser to read in stdin and parse them to structured instructions.
     * And initialize pc to be main function's position.
     * */
    void parse_src() {
        registers[34] = parser->parse(*instructionPool, *mainMemory, var2mem, lab2src);
        // display_all_label();
    }

    int pipeline() {
            int step_counter = 0;

            while (procs.step()) {
                    if (step_counter == 0) {
                            if(procs.instruction->op == LA) {
                                    int a = 1;
                            }// &&
                                // procs.instruction->tokens[1] == "$a0" &&
                                // procs.instruction->tokens[2] == "_skip_set_less_than_zero")

                            // procs.display_intruction();
                    }
                    step_counter = (step_counter + 1) % 5;
                    if (step_counter == 0) {
                            // registers_display();
                    }
            }
            if(returnFlag) return registers[regTable["$a0"]];
            else return 0;
    }

    /**
     * display memory content, debug function.
     * */
    /*
	  void memory_content() {
		  mainMemory->display();
	  }
	   */

    void registers_display() {
            cout << "display:" << endl;
            for (int i = 0; i < 35; ++i) {
                    cout << i << ": " << registers[i] << ' ';
            }
            cout << endl;
    }

    void display_all_label() {
            for(pair<string,int> elem : lab2src) {
                    cout << elem.first << endl;
            }
            cout << endl;
    }
};

#endif //MYMIPSRE_SIMULATOR_H
