#ifndef MIPS_SIMULATOR_H
#define MIPS_SIMULATOR_H


#include <parse/parser.h>
#include "coordinator.h"

class Simulator {
private:
	void init_map();
	Coordinator *steward;
	Parser *parser;

public:
	Simulator();

	~Simulator();

	/**
	 * use parser to read in stdin and parse them to structured instructions.
	 * And initialize pc to be main function's position.
	 * */
	void parse_src();

	int pipeline();

	void display_all_label();
};


#endif //MIPS_SIMULATOR_H
