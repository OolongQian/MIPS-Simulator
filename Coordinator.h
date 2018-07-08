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

		virtual void decode() = 0;
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

		void decode() override;

		void execute() override;

		void memory() override;

		void writeBack() override;
};

class Suber : public Specialist {
public:
		explicit Suber(Processor *procs);

		void decode() override;

		void execute() override;

		void memory() override;

		void writeBack() override;
};

class Muler : public Specialist {
public:
		explicit Muler(Processor *procs);

		void decode() override;

		void execute() override;

		void memory() override;

		void writeBack() override;
};

class Diver : public Specialist {
private:
		int remainder;
public:
		explicit Diver(Processor *procs);

		void decode() override;

		void execute() override;

		void memory() override;

		void writeBack() override;
};

class XorRemer : public Specialist {
public:
		explicit XorRemer(Processor *procs);

		void decode() override;

		void execute() override;

		void memory() override;

		void writeBack() override;
};

class Neger : public Specialist {
public:
		explicit Neger(Processor *procs);

		void decode() override;

		void execute() override;

		void memory() override;

		void writeBack() override;
};

class Lier : public Specialist {
public:
		explicit Lier(Processor *procs);

		void decode() override;

		void execute() override;

		void memory() override;

		void writeBack() override;
};

class Comparer : public Specialist {
public:
		explicit Comparer (Processor *procs);

		void decode() override;

		void execute() override;

		void memory() override;

		void writeBack() override;
};

class Beer : public Specialist {
public:
		explicit Beer (Processor *procs);

		void decode() override;

		void execute() override;

		void memory() override;

		void writeBack() override;
};

class Jer : public Specialist {
public:
		explicit Jer (Processor *procs);

		void decode() override;

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

		void decode() override;

		void execute() override;

		void memory() override;

		void writeBack() override;
};

class Storer : public Specialist {
private:
		int offset;
public:
		explicit Storer (Processor *procs);

		void decode() override;

		void execute() override;

		void memory() override;

		void writeBack() override;
};

class Mover : public Specialist {
private:
		int offset;
public:
		explicit Mover (Processor *procs);

		void decode() override;

		void execute() override;

		void memory() override;

		void writeBack() override;
};

class Noper : public Specialist {
public:
		explicit Noper (Processor *procs) {
				this->procs = procs;
		}

		void decode() override {}

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

		void decode() override;

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
		int designated;             /// chosen specialist.
private:
		/**
 		 * processor will do nothing in pipeline if being locked.
 		 * */
		bool lock;
		Specialist *specialist[15];
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
								break;
						case J: case JR: case JAL: case JALR:
								designated = 9;
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
										break;
								case DECODE:
										specialist[designated]->decode();
										break;
								case EXECUTE:
										specialist[designated]->execute();
										break;
								case MEMORY:
										specialist[designated]->memory();
										break;
								case WRITEBACK:
										specialist[designated]->writeBack();
										break;
								default:
										break;
						}
				}
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

		public:
		Coordinator() {
				for(int i = 0; i < 35; ++i)
						reg_in_use[i] = 0;
				for(int i = 0; i < 5; ++i) {
						cores[i] = new Processor();     /// processors are constructed to be FETCH
						cores[i]->stage = FETCH;
						//cores[i]->set_lock();
						reserve.push(i);
						converter[i] = -1;
				}
				dataHazard = false;
				structureHazard = false;
				unpredictedJumpNum = 0;
				while(!predictedAddress.empty()) predictedAddress.pop();
				while(!predictJInstructions.empty()) predictJInstructions.pop();
				BTB.clear();
				flushSignal = false;
				predicting = 0;
				predictTable.clear();
				globalPredHistory = 0;
		}


		void commence() {
				/// In case of instructionPool is empty
				code_end = instructionPool->empty();
				// cout << int(code_end) << endl;
				syscall_terminate = false;
				int convIdx = 4;
				/// 一开始所有处理器的状态都必定是对的。
				while(!code_end && !syscall_terminate) {
						if(convIdx == 4 && converter[convIdx] != -1) {
								cores[converter[convIdx]]->step();
								/// 为下一次执行调整对状态。
								cores[converter[convIdx]]->stage = FETCH;
								/// 存入队列中
								reserve.push(converter[convIdx]);
								/// 取消当前位置的处理器
								converter[convIdx] = -1;

								/// 处理一下分支预测，进行flush，flush之后只有一个还未取指的处理器，所以相当于重新构造一遍coordinator
								if(flushSignal) {
										/// 注意重制信号
										flushSignal = false;
										/// flush converter当中的 1，2，3 号处理器
										for(int i = 1; i <= 3; ++i) {
												if(converter[i] != -1) {
														cores[converter[i]]->stage = FETCH;
														reserve.push(converter[i]);
														converter[i] = -1;
												}
										}
										/// 清空所有的 recordedNpc和predictedAddress
										while(!predictJInstructions.empty()) predictJInstructions.pop();
										while(!predictedAddress.empty()) predictedAddress.pop();
										/// 清空所有的 reg_in_use
										memset(reg_in_use, 0, sizeof(reg_in_use));
										/// 重制一些容易遗忘的东西
										dataHazard = false;
										structureHazard = false;
										unpredictedJumpNum = 0;
										predicting = 0;
								}
						}
						else if(convIdx == 3 && converter[convIdx] != -1) {
								/// 如果最后一个是预测的，不能进行load和store
								/*
								if(predicting > 0 &&
								   (cores[converter[3]]->designated == 10 || cores[converter[3]]->designated == 11)) {

										if(converter[4] != -1 && predictJInstructions.front() == cores[converter[4]]->ins_idx) {
												convIdx = 4;
												continue;
										}
								}
								 */
								cores[converter[convIdx]]->step();
								cores[converter[convIdx]]->stage = WRITEBACK;
								/// 把当前处理器存入下一个位置
								converter[convIdx + 1] = converter[convIdx];
								converter[convIdx] = -1;
						}
						else if(convIdx == 2 && converter[convIdx] != -1) {
								cores[converter[convIdx]]->step();
								cores[converter[convIdx]]->stage = MEMORY;
								/// 把当前处理器存入下一个位置
								converter[convIdx + 1] = converter[convIdx];
								converter[convIdx] = -1;
						}
						else if(convIdx == 1 && converter[convIdx] != -1) {
								/// 只有 decode 阶段的处理器能够上报 data hazard
								cores[converter[convIdx]]->step();
								if(dataHazard) {
										/// 重制data hazard
										// converter[convIdx + 1] = -1;
										/// 直接跳过 FETCH，如果有的话，因为，拜托，反正你得停一回合，而且多停不会错啊
										/// 直接跳回原点，但在跳回原点的过程当中要清空hazard
										convIdx = 4;
										structureHazard = false;
										dataHazard = false;
										continue;
								}
								else {
										/// 为下一次执行调整状态
										cores[converter[convIdx]]->stage = EXECUTE;
										/// 将处理器存入下一个地点
										converter[convIdx + 1] = converter[convIdx];
										/// 清除当前处理器
										converter[convIdx] = -1;
								}
						}
						else if(convIdx == 0 && converter[convIdx] != -1) {
								/// 如果memory已经被占用，那么我没有办法取指令了，跳回原点，不更新自己，清空hazard
								/// 如果当前流水线中存在没有写入的跳转指令，那么也没有办法取指令了，我不愿意进行分支预测。
								if(structureHazard || unpredictedJumpNum > 0) {
										convIdx = 4;
										structureHazard = false;
										dataHazard = false;
										continue;
								}
								else {
										/// 如果可以执行，那么就执行，并且往前进一步
										cores[converter[convIdx]]->step();
										if(predicting > 0 &&
										   (cores[converter[convIdx]]->designated == 10 ||
										    cores[converter[convIdx]]->designated == 11 ||
										    cores[converter[convIdx]]->designated == 14)) {
												convIdx = 4;
												--registers[34];
												continue;
										}
										/// 调整好状态
										cores[converter[convIdx]]->stage = DECODE;
										/// 将处理器存入下一个地点
										converter[convIdx + 1] = converter[convIdx];
										/// 清除当前处理器
										converter[convIdx] = -1;
								}
						}
						/// 只要有处理器空余，我就应该给它分配一个，这样才真实
						if(converter[0] == -1) {
								converter[0] = reserve.front();
								reserve.pop();
						}
						convIdx = (convIdx - 1 + 5) % 5;
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

						registers_display();

						if(flushSignal) cout << "flushSignal" << endl;
						if(dataHazard) cout << "dataHazard" << endl;
						if(structureHazard) cout << "structureHazard" << endl;
						cout << "unpredictedJumpNum: " << unpredictedJumpNum << endl;
#endif
				}
		}
};

#include "Specialists.cpp"

#endif //MIPSNEW_COORDINATOR_H
