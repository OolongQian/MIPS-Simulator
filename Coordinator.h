//
// Created by 钱苏澄 on 2018/7/5.
//

#ifndef MIPSNEW_COORDINATOR_H
#define MIPSNEW_COORDINATOR_H

#include <queue>
#include "Processors.h"

/**
 * If this bool value is true, it means instruction vector is read to the end,
 * or system call for program end is received.
 *
 * No more instruction will be fetched.
 * */
bool code_end;
bool syscall_terminate;

/**
 * Coordinator is a class that organizes five processors.
 * It assigns instruction, deal with instruction ending, and system call terminating.
 * It also deals with hazards.
 * */

/**
 * Development log: 我需要模拟的架构是一条五个工序的流水线，但是我现在有的是五个人。这其中需要一个转换。
 * 我假定有一条固定的五个工序的流水线，用一个整型数组 int pipeline[5] 来表示，我将五个 processor 看作五个人，
 * 将他们分别编号为 0-4 。则 pipeline 当中的整形数值代表的含义为 现在哪一个 processor 正在这条工序上跑。
 * 这样我就模拟了流水线架构。
 * */

/**
 * 后来我想不需要像上文一样繁琐，我可以为所有的 processor 设置一个锁，被锁上的 processor 在 step 时
 * 不能运行。然后每个 processor 记录自己的命令的行号。然后维护一个队列，把所有无所事事的 processor 都堆放
 * 在里面，在队列里面的 processor 都是上锁的。
 *
 * 每个时钟周期，我们检查队列是否为空，如果不为空，就出队一个 processor，并将它的锁解开。
 * 每个时钟周期为所有的 processor 进行step，因为有锁，所以 processor 的调度都是很合理的。
 *
 * 印象中有一些 hazard 的处理需要让一些工序暂停几个时钟周期，但是不会 flush 它们。因此我在 lock 的时候仅仅
 * 让这些 processor 暂停，而不会 reset 它们的空间，这个工作由 restart 内置方法来完成。
 *
 * 这样再一想，我们发现 lock 仅仅对被 flush 的 processor 有效，而对暂停几个时钟周期的 processor 没有什么作用，
 * 这个时候应该加入一个内置变量叫做 stall 来管理这个事情。
 *
 * 当 stall 为 0 时，未被加锁的 processor 可以行动，在 stall 不为 0 时，每个 step 未被加锁的 processor 不行动，
 * 而是去减少 stall 的数值。人为规定被加锁的 processor 的 stall 数值一定为0。（不一定，针对需要调整）。
 *
 *
 * */
class Coordinator {
private:
		/**
		 * five processors
		 * */
		Processor *cores[5];

		/**
		 * A index queue for coordinating
		 * */
		queue<int> reserve;

public:
		Coordinator() {
				/// If instructionPool is empty
				code_end = instructionPool->empty();
				syscall_terminate = false;

				for(int i = 0; i < 5; ++i) {
						cores[i] = new Processor();     /// processors are constructed to be FETCH
						cores[i]->set_lock();
						reserve.push(i);
				}
		}

		void run() {
				while(!code_end && !syscall_terminate) {
						if(!reserve.empty()) {
								int new_core_idx = reserve.front();
								reserve.pop();
								cores[new_core_idx]->unlock();
						}
						for(int i = 0; i < 5; ++i) {
								cores[i]->step();
						}
				}

				while(reserve.size() != 5 && !syscall_terminate) {
						for(int i = 0; i < 5; ++i) {
								if(cores[i]->stage == FETCH) {
										cores[i]->set_lock();
										reserve.push(i);
								}
								else {
										cores[i]->step();
								}
						}
				}

				if(reserve.size() == 5 && !syscall_terminate) {
						cerr << "source code ends without system call to terminate" << endl;
				}
		}
};
#endif //MIPSNEW_COORDINATOR_H
