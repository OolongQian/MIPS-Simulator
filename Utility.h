//
// Created by 钱苏澄 on 2018/7/3.
//

#ifndef MYMIPSRE_UTILITY_H
#define MYMIPSRE_UTILITY_H

#include <cstring>
#include <iostream>
#include <map>

#include "Instruction.h"

using namespace std;

extern map<string, short> regTable;
extern map<string, opType> opTable;

/**
 * parse a actual 'string' from an input raw string.
 * */
string getString(const string &input_str) {
		string ret = "";
		int index = 1;
		for(; index < input_str.length() - 1; ++index) {
				if(input_str[index] == '\\') {
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
				}
				else
						ret += input_str[index];
		}
		return ret;
}

/**
 * convert data type.
 * */
int str2int(const string &str) {
		bool neg = false;
		int index = 0;
		if(str[0] == '-') {
				neg = true;
				++index;
		}

    int ret = 0;
    for(; index < str.length(); ++index) {
        ret = ret * 10 + str[index] - '0';
    }
    if(neg) ret = -ret;
    return ret;
}

/**
 * whether a character is contained by this string.
 * */
bool ch_in_string(const string &str, const char &ch) {
    int len = str.length();
    for(int i = 0; i < len; ++i) {
        if(str[i] == ch) return true;
    }
    return false;
}

/**
 * Given string as "100($fp), extract register_id number and the offset integer in the front "
 * */
void displacement_extractor(const string &str, int &offset, int &register_id) {
    bool minus = false;
    int len = str.length();
    int index = 0;
    string register_name = "";
    offset = 0;

    if(str[0] == '-') {
        minus = true;
        ++index;
    }
    while(str[index] != '(' && index < len) {
        offset = offset * 10 + (str[index] - '0');
        ++index;
    }
    if(minus) offset = -offset;
    ++index;
    while(str[index] != '(' && index < len - 1) {
        register_name += str[index];
        ++index;
    }
    register_id = regTable[register_name];
}

#endif //MYMIPSRE_UTILITY_H
