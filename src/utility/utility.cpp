#include <utility/common.h>
#include "utility/utility.h"

std::string getString(const std::string &input_str) {
	std::string ret = "";
	int index = 1;
	for (; index < input_str.length() - 1; ++index) {
		if (input_str[index] == '\\') {
			++index;
			switch (input_str[index]) {
				case 'a':
					ret += '\a';
					break;
				case 'b':
					ret += '\b';
					break;
				case 'f':
					ret += '\f';
					break;
				case 'n':
					ret += '\n';
					break;
				case 'r':
					ret += '\r';
					break;
				case 't':
					ret += '\t';
					break;
				case 'v':
					ret += '\v';
					break;
				case '\\':
					ret += '\\';
					break;
				case '?':
					ret += '\?';
					break;
				case '\'':
					ret += '\'';
					break;
				case '\"':
					ret += '\\';
					break;
				case '0':
					ret += '\0';
					break;
				default:
					break;
			}
		} else
			ret += input_str[index];
	}
	return ret;
}

int str2int(const std::string &str) {
	bool neg = false;
	int index = 0;
	if (str[0] == '-') {
		neg = true;
		++index;
	}

	int ret = 0;
	for (; index < str.length(); ++index) {
		ret = ret * 10 + str[index] - '0';
	}
	if (neg) ret = -ret;
	return ret;
}

bool ch_in_string(const std::string &str, const char &ch) {
	int len = str.length();
	for (int i = 0; i < len; ++i) {
		if (str[i] == ch) return true;
	}
	return false;
}

void displacement_extractor(std::map<std::string, short> &local_regTable, const std::string &str, int &offset,
														int &register_id) {
	bool minus = false;
	int len = str.length();
	int index = 0;
	std::string register_name = "";
	offset = 0;

	if (str[0] == '-') {
		minus = true;
		++index;
	}
	while (str[index] != '(' && index < len) {
		offset = offset * 10 + (str[index] - '0');
		++index;
	}
	if (minus) offset = -offset;
	++index;
	while (str[index] != '(' && index < len - 1) {
		register_name += str[index];
		++index;
	}
	register_id = local_regTable[register_name];
}

void registers_display() {
	std::cout << "display:" << std::endl;
	for (int i = 0; i < 35; ++i) {
		std::cout << i << ": " << registers[i] << ' ';
	}
	std::cout << std::endl;
}

void flush(int low, int high) {
	for (; low <= high; ++low) {
		if (converter[low] != -1) {
			reserve.push(converter[low]);
			converter[low] = -1;
		}
	}
}
