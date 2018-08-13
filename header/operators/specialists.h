#ifndef MIPS_BaseS_H
#define MIPS_BaseS_H

#include "base.h"
#include "../simulator/processor.h"
#include "utility/common.h"

/**
 * various kinds of Bases.
 * */
class Adder : public Base {
public:
	explicit Adder(Processor *procs);

	void decode() override;

	void execute() override;

	void memory() override;

	void writeBack() override;
};

class Suber : public Base {
public:
	explicit Suber(Processor *procs);

	void decode() override;

	void execute() override;

	void memory() override;

	void writeBack() override;
};

class Muler : public Base {
public:
	explicit Muler(Processor *procs);

	void decode() override;

	void execute() override;

	void memory() override;

	void writeBack() override;
};

class Diver : public Base {
private:
	int remainder;
public:
	explicit Diver(Processor *procs);

	void decode() override;

	void execute() override;

	void memory() override;

	void writeBack() override;
};

class XorRemer : public Base {
public:
	explicit XorRemer(Processor *procs);

	void decode() override;

	void execute() override;

	void memory() override;

	void writeBack() override;
};

class Neger : public Base {
public:
	explicit Neger(Processor *procs);

	void decode() override;

	void execute() override;

	void memory() override;

	void writeBack() override;
};

class Lier : public Base {
public:
	explicit Lier(Processor *procs);

	void decode() override;

	void execute() override;

	void memory() override;

	void writeBack() override;
};

class Comparer : public Base {
public:
	explicit Comparer (Processor *procs);

	void decode() override;

	void execute() override;

	void memory() override;

	void writeBack() override;
};

class Beer : public Base {
public:
	explicit Beer (Processor *procs);

	void decode() override;

	void execute() override;

	void memory() override;

	void writeBack() override;
};

class Jer : public Base {
public:
	explicit Jer (Processor *procs);

	void decode() override;

	void execute() override;

	void memory() override;

	void writeBack() override;
};

class Loader : public Base {
private:
	int offset;
	char LMD_temp[4];
public:
	explicit Loader (Processor *procs);

	void decode() override;

	void execute() override;

	void memory() override;

	void writeBack() override;
};

class Storer : public Base {
private:
	int offset;
public:
	explicit Storer (Processor *procs);

	void decode() override;

	void execute() override;

	void memory() override;

	void writeBack() override;
};

class Mover : public Base {
private:
	int offset;
public:
	explicit Mover (Processor *procs);

	void decode() override;

	void execute() override;

	void memory() override;

	void writeBack() override;
};

class Noper : public Base {
public:
	explicit Noper (Processor *procs) {
		this->procs = procs;
	}

	void decode() override {}

	void execute() override {}

	void memory() override {}

	void writeBack() override {}
};

class Syser : public Base {
private:
	int opCode;
	int stdin_temp;
	int heap_address;
public:
	explicit Syser (Processor *procs);

	void decode() override;

	void execute() override;

	void memory() override;

	void writeBack() override;
};

#endif //MIPS_BaseS_H
