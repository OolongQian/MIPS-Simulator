#include <iostream>
#include <fstream>
#include "Simulator.h"

// #define RUN
using namespace std;

int main(int argc, char **argv) {
#ifdef RUN
    string filename = argv[1];
    cout << filename << endl;
    fp.open(filename);
#endif
    Simulator sim;
    sim.parse_src();
#ifdef RUN
    fp.close();
#endif
    return sim.pipeline();
}
