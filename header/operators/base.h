#ifndef MIPS_BASE_H
#define MIPS_BASE_H

class Processor;

class Base {
protected:
	Processor *procs;
	int Rdest, Rsrc1, Rsrc2;
	int Imm, label_address;
	int A, B;
	long long ALUoutput;
	bool Cond;
public:
	int Npc;

	Base();

	virtual ~Base() = default;

	virtual void decode() = 0;
	virtual void execute() = 0;
	virtual void memory() = 0;
	virtual void writeBack() = 0;
};


#endif //MIPS_BASE_H
