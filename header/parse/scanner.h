#ifndef MIPS_SCANNER_H
#define MIPS_SCANNER_H

#include <string>
#include <vector>

class Scanner {
private:
	std::string *line;
	int index, len;
public:
	Scanner();
	/**
	 * Specify a new line to process.
	 * */
	void setLine(std::string &src_line);
	/**
	 * process current line to end, and ends up with a tokens std::vector.
	 * If this line is trivial, return false, otherwise return true.
	 * */
	bool getTokens(std::vector<std::string> &tokens);
private:
	/**
	 * Get next token which is separated by " "、 ","、and tab.
	 * If the token returned is "", line-processing is finished.
	 * */
	std::string getNextToken();
};


#endif //MIPS_SCANNER_H
