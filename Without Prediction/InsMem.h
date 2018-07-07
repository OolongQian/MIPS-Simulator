//
// Created by 钱苏澄 on 2018/7/6.
//

#ifndef MIPSNEW_INSMEM_H
#define MIPSNEW_INSMEM_H

#include <cstring>
#include <cstdio>
#include <vector>
#include <map>
#include "enumT.h"

using namespace std;

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


/**
 * This class is memory simulator, comprising an array and two pointers.
 * The size of mainMemory is set to be 4M by default.
 * */
class MemoryHW {
		friend class Parser;

private:
		/// temporary data member reserved for debug.
public:
		char *storage;
		int hp;     /// pointer for static and heap.

		MemoryHW() {
				storage = new char[4 * 1024 * 1024];
				hp = 0;
		}

		/**
		 * debug function, display all contents in heap.
		 * */
		void display() {
				cout << "there are " << hp << " bytes have been allocated" << endl;
				cout << "ASCII: " << endl;
				for (int i = 0; i < hp; ++i) {
						cout << (int)storage[i] << ' ';
				}
				cout << endl;
				cout << "CHAR: " << endl;
				for (int i = 0; i < hp; ++i) {
						cout << (char) storage[i] << ' ';
				}
				cout << endl;
		}

		/**
		 * manually implement memGet and memSet function.
		 * */
		void memGet(const int pos, const size_t length, char *mem) {
				memcpy(mem, storage + pos, length);
		}

		void memSet(const int pos, const size_t length, char *mem) {
				memcpy(storage + pos, mem, length);
		}
};
#endif //MIPSNEW_INSMEM_H
