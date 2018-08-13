#ifndef MIPS_MEMORY_H
#define MIPS_MEMORY_H

#include <cstdio>

/**
 * This class is memory simulator, comprising an array and two pointers.
 * The size of mainMemory is set to be 4M by default.
 * */
class MemoryHW {
	friend class Parser;

private:
	/// temporary data member reserved for debug.
public:
	char *storage;
	int hp;     /// pointer for static and heap.

	MemoryHW();

	/**
	 * debug function, display all contents in heap.
	 * */
	void display();

	/**
	 * manually implement memGet and memSet function.
	 * */
	void memGet(const int pos, const size_t length, char *mem);

	void memSet(const int pos, const size_t length, char *mem);
};

#endif //MIPS_MEMORY_H
