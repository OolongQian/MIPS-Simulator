//
// Created by 钱苏澄 on 2018/7/3.
//

#ifndef MYMIPSRE_PARSER_H
#define MYMIPSRE_PARSER_H

#include <cstring>
#include <vector>
#include <map>

#include "Scanner.h"
#include "Utility.h"
#include "Instruction.h"

#define RUN2
using namespace std;
extern map<string, short> regTable;
extern map<string, opType> opTable;
extern ifstream fp;

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
    void static_data_init(vector<string> &tokens, MemoryHW &memory) {
        /// align memory.hp pointer.
        if(tokens[0] == ".align") {
            int shift_exp = str2int(tokens[1]);
            int dist = (1 << shift_exp);
            int memPos = 0;
            while(memPos < memory.hp) memPos += dist;
            for(int i = memory.hp; i < memPos; ++i) {
                memory.storage[i] = 0;
            }
            memory.hp = memPos;
        }
        /// store string without \0.
        else if(tokens[0] == ".ascii") {
            /// 去除双引号。
            string static_str = getString(tokens[1]);
            for(int i = 0; i < static_str.length(); ++i) {
                memory.storage[memory.hp++] = static_str[i];
            }
        }
        /// store string with \0.
        else if(tokens[0] == ".asciiz") {
            string static_str = getString(tokens[1]);
            for(int i = 0; i < static_str.length(); ++i) {
                memory.storage[memory.hp++] = static_str[i];
            }
            // memory.display();
            memory.storage[memory.hp++] = '\0';
        }
            /// store the following byte in memory.
        else if(tokens[0] == ".byte") {
            char byte_val;
            for(int i = 1; i < tokens.size(); ++i) {
                byte_val = (char) str2int(tokens[i]);
                memory.storage[memory.hp++] = byte_val;
            }
        }
            /// store the following half word in memory.
        else if(tokens[0] == ".half") {
            short half_val;
            for(int i = 1; i < tokens.size(); ++i) {
                half_val = (short) str2int(tokens[i]);
                memcpy(memory.storage + memory.hp, &half_val, sizeof(short));
                memory.hp += sizeof(short);
            }
        }
            /// store the following word in memory.
        else if(tokens[0] == ".word") {
            int word_val;
            for(int i = 1; i < tokens.size(); ++i) {
                word_val = str2int(tokens[i]);
                memcpy(memory.storage + memory.hp, &word_val, sizeof(int));
                memory.hp += sizeof(int);
            }
        }
            /// alloc n byte in memory.
        else if(tokens[0] == ".space") {
            int alloc_space = str2int(tokens[1]);
            memset(memory.storage + memory.hp, 0, alloc_space * sizeof(char));
            memory.hp += alloc_space;
        }
    }


public:
    Parser() {
        status = TEXT;
    }

    /**
     * This function processes the whole source code read from cin.
     * It creates instructions for TEXT, allocates data for DATA.
     * Records positions for labels: TEXT label with vector index, DATA label with heap pointer offset.
     * And finally replace labels with their index in instructionPool.
     *
     * And finally return the line index of main function.
     * */
    int parse(vector<Instruction> &instructionPool, MemoryHW &memory, map<string, int> &var2mem,
              map<string, int> &lab2src) {
        string src_line;
        vector<string> tokens;

        /// get tokens first.
#ifdef RUN2
        while (getline(fp, src_line)) {
            if (fp.eof()) break;
#else
        while (getline(cin, src_line)) {
#endif
            if (src_line[0] == '@')
                break;
            tokens.clear();
            scanner.setLine(src_line);

            if (scanner.getTokens(tokens)) {
                /// distinguish among TEXT, DATA, and LABEL.

                /// if it's a status change.
                if (tokens[0] == ".data") {
                    status = DATA;
                } else if (tokens[0] == ".text") {
                    status = TEXT;
                }
                    /// if it creates a label.
                else if (tokens[0].back() == ':') {
                    if (status == TEXT) {
                        tokens[0].pop_back();
                        /**
                         * here may be bugs, if src code ends with a label, then bomb!
                         * */
                        lab2src.insert(pair<string, int>(tokens[0], instructionPool.size()));
                    } else if (status == DATA) {
                        tokens[0].pop_back();
                        var2mem.insert(pair<string, int>(tokens[0], memory.hp));
                    }
                }
                    /// if it does a normal instruction, distinguish it between ALU instruction and static_data creation.
                else {
                    if (status == DATA) {
                        static_data_init(tokens, memory);     /// initialize static_data.
                    } else if (status == TEXT) {
                        Instruction newInstruction(opTable[tokens[0]], tokens);
                        instructionPool.push_back(newInstruction);
                    }
                }
            }
        }
        return lab2src["main"];
    }
    /**
     * trace back and replace label placeholders with corresponding line number and memory address.
     * index2label 盛放着所有需要替换的地方，通过遍历它可以把所有的placeholder替换成直接的地址。
     * */
        // map<int, string>::iterator iter;
        // for(iter = index2label.begin(); iter != index2label.end(); ++iter) {
        //     instructionPool[(*iter).first].Address = label2index.at((*iter).second);
        // }
        /**
         * debug code, print all completed logs.
         * */
        // for(Instruction temp : instructionPool) {
        //     temp.display();
        // }
};

#endif //MYMIPSRE_PARSER_H
