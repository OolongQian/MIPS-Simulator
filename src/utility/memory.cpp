#include "utility/memory.h"
#include <cstring>
#include <iostream>


MemoryHW::MemoryHW() {
	storage = new char[4 * 1024 * 1024];
	hp = 0;
}

void MemoryHW::display() {
	std::cout << "there are " << hp << " bytes have been allocated" << std::endl;
	std::cout << "ASCII: " << std::endl;
	for (int i = 0; i < hp; ++i) {
		std::cout << (int)storage[i] << ' ';
	}
	std::cout << std::endl;
	std::cout << "CHAR: " << std::endl;
	for (int i = 0; i < hp; ++i) {
		std::cout << (char) storage[i] << ' ';
	}
	std::cout << std::endl;
}

void MemoryHW::memGet(const int pos, const size_t length, char *mem) {
	memcpy(mem, storage + pos, length);
}

void MemoryHW::memSet(const int pos, const size_t length, char *mem) {
	memcpy(storage + pos, mem, length);
}

