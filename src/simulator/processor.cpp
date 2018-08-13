#include "simulator/processor.h"

void Processor::fetch() {
	if(registers[34] >= instructionPool->size()) {
		code_end = true;
		return;
	}
	instruction = &instructionPool->at(registers[34]);
#ifdef INS
	display_intruction();
#endif
	if(instruction->op == SYSCALL && registers[2] == 5) {
		int a = 1;
	}
	ins_idx = registers[34];
	switch (instruction->op) {
		case ADD: case ADDU: case ADDIU:
			designated = 0;
			break;
		case SUB: case SUBU:
			designated = 1;
			break;
		case MUL: case MULU:
			designated = 2;
			break;
		case DIV: case DIVU:
			designated = 3;
			break;
		case XOR: case XORU: case REM: case REMU:
			designated = 4;
			break;
		case NEG: case NEGU:
			designated = 5;
			break;
		case LI:
			designated = 6;
			break;
		case SEQ: case SGE: case SGT: case SLE: case SLT: case SNE:
			designated = 7;
			break;
		case BB: case BEQ: case BNE: case BGE: case BLE: case BGT: case BLT:
		case BEQZ: case BNEZ: case BLEZ: case BGEZ: case BGTZ: case BLTZ:
			designated = 8;
			break;
		case J: case JR: case JAL: case JALR:
			designated = 9;
			break;
		case LA: case LB: case LH: case LW:
			designated = 10;
			break;
		case SB: case SH: case SW:
			designated = 11;
			break;
		case MOVE: case MFHI: case MFLO:
			designated = 12;
			break;
		case NOP:
			designated = 13;
			break;
		case SYSCALL:
			designated = 14;
			break;
		default:
			break;
	}
	specialist[designated]->Npc = ++registers[34];
}

Processor::Processor() {
	instruction = nullptr;
	stage = FETCH;
	designated = 0;
	lock = false;
	specialist[0] = new Adder(this);
	specialist[1] = new Suber(this);
	specialist[2] = new Muler(this);
	specialist[3] = new Diver(this);
	specialist[4] = new XorRemer(this);
	specialist[5] = new Neger(this);
	specialist[6] = new Lier(this);
	specialist[7] = new Comparer(this);
	specialist[8] = new Beer(this);
	specialist[9] = new Jer(this);
	specialist[10] = new Loader(this);
	specialist[11] = new Storer(this);
	specialist[12] = new Mover(this);
	specialist[13] = new Noper(this);
	specialist[14] = new Syser(this);
}

Processor::~Processor() {
	for(int i = 0; i < 15; ++i) {
		delete specialist[i];
	}
}

void Processor::step() {
	if(!lock) {
		switch (stage) {
			case FETCH:
				fetch();
				break;
			case DECODE:
				specialist[designated]->decode();
				break;
			case EXECUTE:
				specialist[designated]->execute();
				break;
			case MEMORY:
				specialist[designated]->memory();
				break;
			case WRITEBACK:
				specialist[designated]->writeBack();
				break;
			default:
				break;
		}
	}
}

void Processor::display_intruction() {
	cout << registers[34] << ". ";
	for (int i = 0; i < instruction->tokens.size(); ++i)
		cout << instruction->tokens[i] << ' ';
	cout << endl;
}
