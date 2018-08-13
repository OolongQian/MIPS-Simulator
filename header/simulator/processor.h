#ifndef MIPS_PROCESSOR_H
#define MIPS_PROCESSOR_H

#include <utility/common.h>
#include "../operators/base.h"
#include "../operators/specialists.h"

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
	Base *specialist[15];
	bool dataHazard;
private:
	/**
	 * return whether the instructionPool has ended.
	 * */
	void fetch();

public:
	explicit Processor();

	~Processor();

	/**
	 * step or reset the specialist.
	 * only do things when unlocked.
	 * */
	void step();

	void display_intruction();
};

#endif //MIPS_PROCESSOR_H
