#include <iostream>
#include <fstream>
#include "Simulator.h"
#include "Utility.h"
#include "Global.h"
#include <vector>
using namespace std;

/*
DCBAE
4ssfsdf
 */
int main(int argc, char **argv) {
    int retCode;
#ifdef RUN
    string filename = argv[1];
    // cout << filename << endl;
		// string filename = "mytest.s";

    fp.open(filename);
#endif
    Simulator sim;
    sim.parse_src();
#ifdef RUN
    fp.close();
#endif
    // cout << instructionPool->size() << endl;
    retCode = sim.pipeline();
    return retCode;
}
