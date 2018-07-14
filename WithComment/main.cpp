#include <iostream>
#include <fstream>
#include "Simulator.h"
#include "Utility.h"
#include "Global.h"
#include <vector>
using namespace std;

int main(int argc, char **argv) {
    int retCode;
#ifdef RUN
    string filename = argv[1];
    // cout << filename << endl;
	  // string filename = "heapsort-5100379110-daibo.s";

    fp.open(filename);
#endif
    Simulator sim;
    sim.parse_src();
#ifdef RUN
    fp.close();
#endif
    // cout << instructionPool->size() << endl;
    retCode = sim.pipeline();

    // printf("预测了%d次，正确了%d次\n", predictTimes, predictCorrectTimes);
    return retCode;
}
