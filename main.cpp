#include <iostream>
#include <fstream>
#include "Simulator.h"

#define RUN
using namespace std;

int main(int argc, char **argv) {
    int retCode;
#ifdef RUN
    string filename = argv[1];
    fp.open(filename);
#endif
    Simulator sim;
    sim.parse_src();
#ifdef RUN
    fp.close();
#endif
    retCode = sim.pipeline();
    return retCode;
}
