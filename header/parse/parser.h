#ifndef MIPS_PARSER_H
#define MIPS_PARSER_H

#include "scanner.h"
#include "utility/common.h"
#include "utility/utility.h"
#include "utility/memory.h"

#include <vector>
#include <string>

/**
 * This is a class that parse the text source code and fill instructionPool.
 * It also allocates static_variable.
 * */
class Parser {
	enum statusType {
		DATA, TEXT
	};
private:
	Scanner scanner;
	statusType status;

private:

	/**
	 * This tokens of instruction is for static_data_init, simulate in memory.
	 * */
	void static_data_init(std::vector<std::string> &tokens);

public:
	Parser();
	/**
	 * This function processes the whole source code read from cin.
	 * It creates instructions for TEXT, allocates data for DATA.
	 * Records positions for labels: TEXT label with std::vector index, DATA label with heap pointer offset.
	 * And finally replace labels with their index in instructionPool.
	 *
	 * And finally return the line index of main function.
	 * */
	int parse();

	/**
	 * Parse the code to be the god damn shit.
	 * */
	void code_analyzer();
};

#endif //MIPS_PARSER_H
