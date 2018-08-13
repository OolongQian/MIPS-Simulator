#include "simulator/coordinator.h"

Coordinator::Coordinator() {
	for(int i = 0; i < 35; ++i)
		reg_in_use[i] = 0;
	for(int i = 0; i < 5; ++i) {
		cores[i] = new Processor();     /// processors are constructed to be FETCH
		cores[i]->stage = FETCH;
		//cores[i]->set_lock();
		reserve.push(i);
		converter[i] = -1;
	}
	dataHazard = false;
	structureHazard = false;
	unpredictedJumpNum = 0;
	while(!predictedAddress.empty()) predictedAddress.pop();
	while(!predictJInstructions.empty()) predictJInstructions.pop();
	BTB.clear();
	flushSignal = false;
	predicting = 0;
	predictTable.clear();
	globalPredHistory = 0;
}

void Coordinator::commence() {
/// In case of instructionPool is empty
	code_end = instructionPool->empty();
	// cout << int(code_end) << endl;
	syscall_terminate = false;
	int convIdx = 4;
	/// 一开始所有处理器的状态都必定是对的。
	while(!code_end && !syscall_terminate) {
		if(convIdx == 4 && converter[convIdx] != -1) {
			cores[converter[convIdx]]->step();
			/// 为下一次执行调整对状态。
			cores[converter[convIdx]]->stage = FETCH;
			/// 存入队列中
			reserve.push(converter[convIdx]);
			/// 取消当前位置的处理器
			converter[convIdx] = -1;

			/// 处理一下分支预测，进行flush，flush之后只有一个还未取指的处理器，所以相当于重新构造一遍coordinator
			if(flushSignal) {
				/// 注意重制信号
				flushSignal = false;
				/// flush converter当中的 1，2，3 号处理器
				for(int i = 1; i <= 3; ++i) {
					if(converter[i] != -1) {
						cores[converter[i]]->stage = FETCH;
						reserve.push(converter[i]);
						converter[i] = -1;
					}
				}
				/// 清空所有的 recordedNpc和predictedAddress
				while(!predictJInstructions.empty()) predictJInstructions.pop();
				while(!predictedAddress.empty()) predictedAddress.pop();
				while(!predictIndices.empty()) predictIndices.pop();
				/// 清空所有的 reg_in_use
				memset(reg_in_use, 0, sizeof(reg_in_use));
				/// 重制一些容易遗忘的东西
				dataHazard = false;
				structureHazard = false;
				unpredictedJumpNum = 0;
				predicting = 0;
			}
		}
		else if(convIdx == 3 && converter[convIdx] != -1) {
			/// 如果最后一个是预测的，不能进行load和store
			/*
			if(predicting > 0 &&
				 (cores[converter[3]]->designated == 10 || cores[converter[3]]->designated == 11)) {

					if(converter[4] != -1 && predictJInstructions.front() == cores[converter[4]]->ins_idx) {
							convIdx = 4;
							continue;
					}
			}
			 */
			cores[converter[convIdx]]->step();
			cores[converter[convIdx]]->stage = WRITEBACK;
			/// 把当前处理器存入下一个位置
			converter[convIdx + 1] = converter[convIdx];
			converter[convIdx] = -1;
		}
		else if(convIdx == 2 && converter[convIdx] != -1) {
			cores[converter[convIdx]]->step();
			cores[converter[convIdx]]->stage = MEMORY;
			/// 把当前处理器存入下一个位置
			converter[convIdx + 1] = converter[convIdx];
			converter[convIdx] = -1;
		}
		else if(convIdx == 1 && converter[convIdx] != -1) {
			/// 只有 decode 阶段的处理器能够上报 data hazard
			cores[converter[convIdx]]->step();
			if(dataHazard) {
				/// 重制data hazard
				// converter[convIdx + 1] = -1;
				/// 直接跳过 FETCH，如果有的话，因为，拜托，反正你得停一回合，而且多停不会错啊
				/// 直接跳回原点，但在跳回原点的过程当中要清空hazard
				convIdx = 4;
				structureHazard = false;
				dataHazard = false;
				continue;
			}
			else {
				/// 为下一次执行调整状态
				cores[converter[convIdx]]->stage = EXECUTE;
				/// 将处理器存入下一个地点
				converter[convIdx + 1] = converter[convIdx];
				/// 清除当前处理器
				converter[convIdx] = -1;
			}
		}
		else if(convIdx == 0 && converter[convIdx] != -1) {
			/// 如果memory已经被占用，那么我没有办法取指令了，跳回原点，不更新自己，清空hazard
			/// 如果当前流水线中存在没有写入的跳转指令，那么也没有办法取指令了，我不愿意进行分支预测。
			if(structureHazard || unpredictedJumpNum > 0) {
				convIdx = 4;
				structureHazard = false;
				dataHazard = false;
				continue;
			}
			else {
				/// 如果可以执行，那么就执行，并且往前进一步
				cores[converter[convIdx]]->step();
				if(predicting > 0 &&
					 (cores[converter[convIdx]]->designated == 10 ||
						cores[converter[convIdx]]->designated == 11 ||
						cores[converter[convIdx]]->designated == 14)) {
					convIdx = 4;
					--registers[34];
					continue;
				}
				/// 调整好状态
				cores[converter[convIdx]]->stage = DECODE;
				/// 将处理器存入下一个地点
				converter[convIdx + 1] = converter[convIdx];
				/// 清除当前处理器
				converter[convIdx] = -1;
			}
		}
		/// 只要有处理器空余，我就应该给它分配一个，这样才真实
		if(converter[0] == -1) {
			converter[0] = reserve.front();
			reserve.pop();
		}
		convIdx = (convIdx - 1 + 5) % 5;
#ifdef DISPLAY
		cout << "converter: ";
						for(int i = 0; i < 5; ++i)
								cout << converter[i] << ' ';
						cout << endl;

						cout << "locked registers: ";
						for(int i = 0; i < 35; ++i) {
								if(reg_in_use[i] != 0) cout << i << '(' << reg_in_use[i] << ')' << ' ';
						}
						cout << endl;

						registers_display();

						if(flushSignal) cout << "flushSignal" << endl;
						if(dataHazard) cout << "dataHazard" << endl;
						if(structureHazard) cout << "structureHazard" << endl;
						cout << "unpredictedJumpNum: " << unpredictedJumpNum << endl;
#endif
	}
}
