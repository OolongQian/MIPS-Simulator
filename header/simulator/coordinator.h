#ifndef MIPS_COORDINATOR_H
#define MIPS_COORDINATOR_H


#include "processor.h"

/**
 * Coordinator is a class that organizes five processors.
 * It assigns instruction, deal with instruction ending, and system call terminating.
 * It also deals with hazards.
 * */
class Coordinator {
private:
	/**
	 * five processors
	 * */
	Processor *cores[5];

public:
	Coordinator();

	void commence();
};

#endif //MIPS_COORDINATOR_H
