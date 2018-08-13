#ifndef MIPS_UTILITY_H
#define MIPS_UTILITY_H


#include <string>
#include <map>
#include <iostream>

/**
 * parse a actual 'std::string' from an input raw std::string.
 * */
std::string getString(const std::string &input_str);

/**
 * convert data type.
 * */
int str2int(const std::string &str);

/**
 * whether a character is contained by this std::string.
 * */
bool ch_in_string(const std::string &str, const char &ch);

/**
 * Given std::string as "100($fp), extract register_id number and the offset integer in the front "
 * */
void displacement_extractor(std::map<std::string, short> &local_regTable, const std::string &str, int &offset, int &register_id);


void registers_display();

/**
 * pos is the position we start flushing till the beginning.
 * */
void flush(int low, int high);

#endif //MIPS_UTILITY_H
