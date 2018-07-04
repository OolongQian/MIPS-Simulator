//
// Created by 钱苏澄 on 2018/7/3.
//

#ifndef MYMIPSRE_SCANNER_H
#define MYMIPSRE_SCANNER_H

#include <cstring>
#include <iostream>
#include <vector>
#include "MemoryHW.h"

using namespace std;

/**
 * A class that is used to scan a line of code and interpret it into a vector of tokens.
 * It returns true if a line isn't trivial, false otherwise.
 * It is able to recognize comment, tab, comma...
 * */
class Scanner {
private:
    string *line;
    int index, len;
public:
    Scanner() {
        index = len = 0;
        line = nullptr;
    }
    /**
     * Specify a new line to process.
     * */
    void setLine(string &src_line) {
        line = &src_line;
        len = line->length();
        index = 0;
    }

    /**
     * process current line to end, and ends up with a tokens vector.
     * If this line is trivial, return false, otherwise return true.
     * */
    bool getTokens(vector<string> &tokens) {
        string token;
        while( (token = getNextToken()) != "") {
            tokens.push_back(token);
        }
        if( tokens.size() == 0) return false;
        else return true;
    }

private:
    /**
     * Get next token which is separated by " "、 ","、and tab.
     * If the token returned is "", line-processing is finished.
     * */
    string getNextToken() {
        string str = "";

        /// 跳过前面没有意义的。
        while(index < len && ((*line)[index] == ' ' || (*line)[index] == '\t' || (*line)[index] == ',')) {
        		// cout << (*line)[index] << endl;
            ++index;
        }
        /**
         * 注意，我们在爬字符串的时候要非常小心，否则就是完蛋。
         * 直接特判。
         * */

        if(index < len && (*line)[index] != '\"') {
		        while(index < len && (*line)[index] != ' ' && (*line)[index] != '\t' && (*line)[index] != ',') {
				        // cout << (*line)[index] << endl;
				        str += (*line)[index];
				        ++index;
		        }
        }
        else if(index < len && (*line)[index] == '\"'){
		        while(index < len) {
				        // cout << (*line)[index] << endl;
				        str += (*line)[index];
				        ++index;
		        }
        }
        return str;
    }
};

#endif //MYMIPSRE_SCANNER_H
