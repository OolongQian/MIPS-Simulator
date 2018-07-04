//
// Created by 钱苏澄 on 2018/7/3.
//

#ifndef MYMIPSRE_MEMORYHW_H
#define MYMIPSRE_MEMORYHW_H

#include <iostream>
#include <map>

using namespace std;

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

    MemoryHW() {
        storage = new char[4 * 1024 * 1024];
        hp = 0;
    }

    /**
     * debug function, display all contents in heap.
     * */
    void display() {
        cout << "there are " << hp << " bytes have been allocated" << endl;
        cout << "ASCII: " << endl;
        for (int i = 0; i < hp; ++i) {
            cout << (int)storage[i] << ' ';
        }
        cout << endl;
        cout << "CHAR: " << endl;
        for (int i = 0; i < hp; ++i) {
            cout << (char) storage[i] << ' ';
        }
        cout << endl;
    }

    /**
     * manually implement memGet and memSet function.
     * */
    void memGet(const int pos, const size_t length, char *mem) {
        memcpy(mem, storage + pos, length);
    }

    void memSet(const int pos, const size_t length, char *mem) {
        memcpy(storage + pos, mem, length);
    }
};

#endif //MYMIPSRE_MEMORYHW_H
