#include "parse/scanner.h"

Scanner::Scanner() {
	index = len = 0;
	line = nullptr;
}

void Scanner::setLine(std::string &src_line) {
	line = &src_line;
	len = line->length();
	index = 0;
}

bool Scanner::getTokens(std::vector<std::string> &tokens) {
	std::string token;
	while ((token = getNextToken()) != "") {
		tokens.push_back(token);
	}
	if (tokens.size() == 0) return false;
	else return true;
}

std::string Scanner::getNextToken() {
	std::string str = "";

	/// 跳过前面没有意义的。
	while (index < len && ((*line)[index] == ' ' || (*line)[index] == '\t' || (*line)[index] == ',')) {
		++index;
	}
	/**
	 * 注意，我们在爬字符串的时候要非常小心，否则就是完蛋。
	 * 直接特判。
	 * */

	if (index < len && (*line)[index] != '\"') {
		while (index < len && (*line)[index] != ' ' && (*line)[index] != '\t' && (*line)[index] != ',') {
			str += (*line)[index];
			++index;
		}
	} else if (index < len && (*line)[index] == '\"') {
		while (index < len) {
			str += (*line)[index];
			++index;
		}
	}
	return str;
}
