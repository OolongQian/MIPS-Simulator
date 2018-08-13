#include <iostream>
#include <fstream>
#include <vector>
#include <simulator/simulator.h>
using namespace std;

int main(int argc, char **argv) {
	int retCode;
	 string filename = "heapsort-5100379110-daibo.s";

	fp.open(filename);
	Simulator sim;
	sim.parse_src();

	fp.close();
	retCode = sim.pipeline();

	printf("预测了%d次，正确了%d次\n", predictTimes, predictCorrectTimes);
	printf("分支预测正确率%.2f%%\n", (double) predictCorrectTimes / (double) predictTimes * 100);
	return retCode;
}
