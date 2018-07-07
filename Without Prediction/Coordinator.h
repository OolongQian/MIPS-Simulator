//
// Created by 钱苏澄 on 2018/7/5.
//

#ifndef MIPSNEW_COORDINATOR_H
#define MIPSNEW_COORDINATOR_H

#include <queue>
#include "Utility.h"

enum stageType {
		FETCH, DECODE, EXECUTE, MEMORY, WRITEBACK
};

class Processor;

void addStall(const int pos, const int cycle);
void registers_display();

class Specialist {
protected:
		Processor *procs;
		int Rdest, Rsrc1, Rsrc2;
		int Imm, label_address;
		int A, B;
		long long ALUoutput;
		bool Cond;
public:
		int Npc;

		Specialist() {
				label_address = Rdest = Rsrc1 = Rsrc2 = Imm = A = B = Npc = 0;
				ALUoutput = 0;
				Cond = false;
				procs = nullptr;
		}
		virtual ~Specialist() {}

		virtual void decode(bool &dataHazard) = 0;
		virtual void execute() = 0;
		virtual void memory() = 0;
		virtual void writeBack() = 0;
};


/**
 * various kinds of specialists.
 * */
class Adder : public Specialist {
public:
		explicit Adder(Processor *procs);

		void decode(bool &dataHazard) override;

		void execute() override;

		void memory() override;

		void writeBack() override;
};

class Suber : public Specialist {
public:
		explicit Suber(Processor *procs);

		void decode(bool &dataHazard) override;

		void execute() override;

		void memory() override;

		void writeBack() override;
};

class Muler : public Specialist {
public:
		explicit Muler(Processor *procs);

		void decode(bool &dataHazard) override;

		void execute() override;

		void memory() override;

		void writeBack() override;
};

class Diver : public Specialist {
private:
		int remainder;
public:
		explicit Diver(Processor *procs);

		void decode(bool &dataHazard) override;

		void execute() override;

		void memory() override;

		void writeBack() override;
};

class XorRemer : public Specialist {
public:
		explicit XorRemer(Processor *procs);

		void decode(bool &dataHazard) override;

		void execute() override;

		void memory() override;

		void writeBack() override;
};

class Neger : public Specialist {
public:
		explicit Neger(Processor *procs);

		void decode(bool &dataHazard) override;

		void execute() override;

		void memory() override;

		void writeBack() override;
};

class Lier : public Specialist {
public:
		explicit Lier(Processor *procs);

		void decode(bool &dataHazard) override;

		void execute() override;

		void memory() override;

		void writeBack() override;
};

class Comparer : public Specialist {
public:
		explicit Comparer (Processor *procs);

		void decode(bool &dataHazard) override;

		void execute() override;

		void memory() override;

		void writeBack() override;
};

class Beer : public Specialist {
public:
		explicit Beer (Processor *procs);

		void decode(bool &dataHazard) override;

		void execute() override;

		void memory() override;

		void writeBack() override;
};

class Jer : public Specialist {
public:
		explicit Jer (Processor *procs);

		void decode(bool &dataHazard) override;

		void execute() override;

		void memory() override;

		void writeBack() override;
};

class Loader : public Specialist {
private:
		int offset;
		char LMD_temp[4];
public:
		explicit Loader (Processor *procs);

		void decode(bool &dataHazard) override;

		void execute() override;

		void memory() override;

		void writeBack() override;
};

class Storer : public Specialist {
private:
		int offset;
public:
		explicit Storer (Processor *procs);

		void decode(bool &dataHazard) override;

		void execute() override;

		void memory() override;

		void writeBack() override;
};

class Mover : public Specialist {
private:
		int offset;
public:
		explicit Mover (Processor *procs);

		void decode(bool &dataHazard) override;

		void execute() override;

		void memory() override;

		void writeBack() override;
};

class Noper : public Specialist {
public:
		explicit Noper (Processor *procs) {
				this->procs = procs;
		}

		void decode(bool &dataHazard) override {}

		void execute() override {}

		void memory() override {}

		void writeBack() override {}
};

class Syser : public Specialist {
private:
		int opCode;
		int stdin_temp;
		int heap_address;
public:
		explicit Syser (Processor *procs);

		void decode(bool &dataHazard) override;

		void execute() override;

		void memory() override;

		void writeBack() override;
};

/**
 * A processor is one of the five building blocks in the pipeline.
 * It can simulate all five stages of instructions.
 *
 * It needs adequate interfaces to communicate with upper coordinator, which
 * comprises reporting status, hazards, processing log.
 *
 * It also needs to be maneuvered by coordinator.
 *
 * 注意了，当 processor 处于解锁状态，但是程序已经terminate的情况。
 * */
class Processor {
private:
public:
		Instruction *instruction;

		/**
		 * instruction line number.
		 * */
		int ins_idx;

		stageType stage;
private:
		/**
 		 * processor will do nothing in pipeline if being locked.
 		 * */
		bool lock;
		Specialist *specialist[15];
		int designated;             /// chosen specialist.
		bool dataHazard;
private:
		/**
		 * return whether the instructionPool has ended.
		 * */
		void fetch() {
				if(registers[34] >= instructionPool->size()) {
						code_end = true;
						return;
				}
				instruction = &instructionPool->at(registers[34]);
#ifdef INS
				display_intruction();
#endif
				if(instruction->op == SYSCALL && registers[2] == 5) {
						int a = 1;
				}
				ins_idx = registers[34];
				switch (instruction->op) {
						case ADD: case ADDU: case ADDIU:
								designated = 0;
								break;
						case SUB: case SUBU:
								designated = 1;
								break;
						case MUL: case MULU:
								designated = 2;
								break;
						case DIV: case DIVU:
								designated = 3;
								break;
						case XOR: case XORU: case REM: case REMU:
								designated = 4;
								break;
						case NEG: case NEGU:
								designated = 5;
								break;
						case LI:
								designated = 6;
								break;
						case SEQ: case SGE: case SGT: case SLE: case SLT: case SNE:
								designated = 7;
								break;
						case BB: case BEQ: case BNE: case BGE: case BLE: case BGT: case BLT:
						case BEQZ: case BNEZ: case BLEZ: case BGEZ: case BGTZ: case BLTZ:
								designated = 8;
								/// 跳转寄存器正在被修改。
								addStall(0, 3);
								++reg_in_use[34];
								break;
						case J: case JR: case JAL: case JALR:
								designated = 9;
								addStall(0, 3);
								++reg_in_use[34];
								break;
						case LA: case LB: case LH: case LW:
								designated = 10;
								break;
						case SB: case SH: case SW:
								designated = 11;
								break;
						case MOVE: case MFHI: case MFLO:
								designated = 12;
								break;
						case NOP:
								designated = 13;
								break;
						case SYSCALL:
								designated = 14;
								break;
						default:
								break;
				}
				specialist[designated]->Npc = ++registers[34];
		}

public:
		explicit Processor() {
				instruction = nullptr;
				stage = FETCH;
				designated = 0;
				lock = false;
				specialist[0] = new Adder(this);
				specialist[1] = new Suber(this);
				specialist[2] = new Muler(this);
				specialist[3] = new Diver(this);
				specialist[4] = new XorRemer(this);
				specialist[5] = new Neger(this);
				specialist[6] = new Lier(this);
				specialist[7] = new Comparer(this);
				specialist[8] = new Beer(this);
				specialist[9] = new Jer(this);
				specialist[10] = new Loader(this);
				specialist[11] = new Storer(this);
				specialist[12] = new Mover(this);
				specialist[13] = new Noper(this);
				specialist[14] = new Syser(this);
		}

		~Processor() {
				for(int i = 0; i < 15; ++i) {
						delete specialist[i];
				}
		}

		/**
		 * step or reset the specialist.
		 * only do things when unlocked.
		 * */
		void step() {
				if(!lock) {
						switch (stage) {
								case FETCH:
										fetch();
										stage = DECODE;
										break;
								case DECODE:
										specialist[designated]->decode(dataHazard);
										if(!dataHazard) stage = EXECUTE;
										dataHazard = false;
										break;
								case EXECUTE:
										specialist[designated]->execute();
										stage = MEMORY;
										break;
								case MEMORY:
										specialist[designated]->memory();
										stage = WRITEBACK;
										break;
								case WRITEBACK:
										specialist[designated]->writeBack();
										stage = FETCH;
										break;
								default:
										break;
						}
				}
		}

		void set_lock() {
				lock = true;
		}

		void unlock() {
				lock = false;
		}

		void restart() {
				stage = FETCH;
		}

		void display_intruction() {
				cout << registers[34] << ". ";
				for (int i = 0; i < instruction->tokens.size(); ++i)
						cout << instruction->tokens[i] << ' ';
				cout << endl;
		}
};

/**
 * Coordinator is a class that organizes five processors.
 * It assigns instruction, deal with instruction ending, and system call terminating.
 * It also deals with hazards.
 * */

/**
 * Development log: 我需要模拟的架构是一条五个工序的流水线，但是我现在有的是五个人。这其中需要一个转换。
 * 我假定有一条固定的五个工序的流水线，用一个整型数组 int pipeline[5] 来表示，我将五个 processor 看作五个人，
 * 将他们分别编号为 0-4 。则 pipeline 当中的整形数值代表的含义为 现在哪一个 processor 正在这条工序上跑。
 * 这样我就模拟了流水线架构。
 * */

/**
 * 后来我想不需要像上文一样繁琐，我可以为所有的 processor 设置一个锁，被锁上的 processor 在 step 时
 * 不能运行。然后每个 processor 记录自己的命令的行号。然后维护一个队列，把所有无所事事的 processor 都堆放
 * 在里面，在队列里面的 processor 都是上锁的。
 *
 * 每个时钟周期，我们检查队列是否为空，如果不为空，就出队一个 processor，并将它的锁解开。
 * 每个时钟周期为所有的 processor 进行step，因为有锁，所以 processor 的调度都是很合理的。
 *
 * 印象中有一些 hazard 的处理需要让一些工序暂停几个时钟周期，但是不会 flush 它们。因此我在 lock 的时候仅仅
 * 让这些 processor 暂停，而不会 reset 它们的空间，这个工作由 restart 内置方法来完成。
 *
 * 这样再一想，我们发现 lock 仅仅对被 flush 的 processor 有效，而对暂停几个时钟周期的 processor 没有什么作用，
 * 这个时候应该加入一个内置变量叫做 stall 来管理这个事情。
 *
 * 当 stall 为 0 时，未被加锁的 processor 可以行动，在 stall 不为 0 时，每个 step 未被加锁的 processor 不行动，
 * 而是去减少 stall 的数值。人为规定被加锁的 processor 的 stall 数值一定为0。（不一定，针对需要调整）。
 * */
class Coordinator {
private:
		/**
		 * five processors
		 * */
		Processor *cores[5];

		/**
		 * A index queue for coordinating
		 * */
		queue<int> reserve;

		int converter[5];
public:
		Coordinator() {
				memset(stallCycle, 0, sizeof(stallCycle));
				for(int i = 0; i < 35; ++i)
						reg_in_use[i] = 0;
				for(int i = 0; i < 5; ++i) {
						cores[i] = new Processor();     /// processors are constructed to be FETCH
						//cores[i]->set_lock();
						reserve.push(i);
						converter[i] = -1;
				}
		}

		void takeStep() {
				for(int i = 4; i >= 0; --i) {
						/// WB 必做。

						/** stallCycle[i] = j 的意思是，converter 在位置 i 有 j 个时钟周期不被更新。
						 * 因此如果位置 i + 1 不被更新，那么位置 i 就不应该被做。 */
						if(converter[i] != -1 && (i == 4 || stallCycle[i + 1] == 0)) {
								cores[converter[i]]->step();
						}
				}
		}

		void updateConverter() {
				if(converter[4] != -1)
						reserve.push(converter[4]);

				int i;
				for(i = 4; i >= 1 && stallCycle[i] == 0; --i) {
						/// 如果会被更新。
						converter[i] = converter[i - 1];
				}


				if(i == 0 && stallCycle[0] == 0) {
						converter[0] = reserve.front();
						reserve.pop();
				}
				else if(i != 0 && stallCycle[i] != 0) {
						converter[i] = -1;
				}
				else {
						converter[i] = -1;
				}
				/*
				else {
						converter[0] = -1;
				}
				 */
				/// 给自然流水和hazard处理分别讨论。
				/*
				if(stallPos == -2) {
						if(converter[4] != -1)
								reserve.push(converter[4]);

						for(int i = 4; i > 0; --i)
								converter[i] = converter[i - 1];

						converter[0] = reserve.front();
						reserve.pop();
				}
				else {
						/// 不可能停止wb阶段的流水。
						if(converter[4] != -1)
								reserve.push(converter[4]);

 						//把 stallPos 之后的东西都往后面弄，如果 stall = -1 的情况，也兼容。
						for(int i = 3; i > stallPos && i >= 0; --i) {
								converter[i + 1] = converter[i];
						}
						converter[stallPos + 1] = -1;
				}
			*/
		}

		void updateDelay() {
				for(int i = 0; i < 5; ++i) {
						if(stallCycle[i] > 0)
								--stallCycle[i];
				}
				/*
				if(stallPos != -2) {
						--stallCycle;
						if(stallCycle == 0)
								stallPos = -2;
				}
				 */
		}

		void commence() {
				/// In case of instructionPool is empty
				code_end = instructionPool->empty();
				// cout << int(code_end) << endl;
				syscall_terminate = false;
				while(!code_end && !syscall_terminate) {
						updateConverter();
						updateDelay();
						takeStep();
#ifdef DISPLAY
						cout << "converter: ";
						for(int i = 0; i < 5; ++i)
								cout << converter[i] << ' ';
						cout << endl;

						cout << "locked registers: ";
						for(int i = 0; i < 35; ++i) {
								if(reg_in_use[i] != 0) cout << i << '(' << reg_in_use[i] << ')' << ' ';
						}
						cout << endl;

						cout << "delay: ";
						for(int i = 0; i < 5; ++i) {
								cout << stallCycle[i] << ' ';
						}
						cout << endl << endl;

						registers_display();
#endif
				}
				/*
				while(!code_end && !syscall_terminate) {
						if(!reserve.empty()) {
								int new_core_idx = reserve.front();
								reserve.pop();
								cores[new_core_idx]->unlock();
						}
						for(int i = 0; i < 5; ++i) {
								cores[i]->step();
						}
				}

				while(reserve.size() != 5 && !syscall_terminate) {
						for(int i = 0; i < 5; ++i) {
								if(cores[i]->stage == FETCH) {
										cores[i]->set_lock();
										reserve.push(i);
								}
								else {
										cores[i]->step();
								}
						}
				}

				if(reserve.size() == 5 && !syscall_terminate) {
						cerr << "source code ends without system call to terminate" << endl;
				}
				 */
		}
};

void registers_display() {
		cout << "display:" << endl;
		for (int i = 0; i < 35; ++i) {
				cout << i << ": " << registers[i] << ' ';
		}
		cout << endl;
}

void addStall(const int pos, const int cycle) {
		for(int i = 0; i <= pos; ++i)
				stallCycle[i] += cycle;
}

#include "Specialists.cpp"

#endif //MIPSNEW_COORDINATOR_H
