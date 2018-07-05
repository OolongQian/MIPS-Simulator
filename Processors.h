//
// Created by 钱苏澄 on 2018/7/3.
//

#ifndef MYMIPSRE_PROCESSORS_H
#define MYMIPSRE_PROCESSORS_H

#include "Utility.h"

extern vector<Instruction> *instructionPool;
extern int registers[];
enum stageType {
    FETCH, DECODE, EXECUTE, MEMORY, WRITEBACK, EMPTY
};

class Processor;

class Specialist {
protected:
    Processor *procs;
    int Rdest, Rsrc1, Rsrc2;
    int Imm, label_address;
    int A, B;
    long long ALUoutput;
    bool Cond;
public:
    int Npc;
    bool OK;

    Specialist() {
        label_address = Rdest = Rsrc1 = Rsrc2 = Imm = A = B = Npc = 0;
        ALUoutput = 0;
        Cond = false;
        procs = nullptr;
        OK = true;
    }
    virtual ~Specialist() {}

    virtual void decode() = 0;
    virtual void execute() = 0;
    virtual void memory() = 0;
    virtual void writeBack() = 0;
};


/**
 * various kinds of specialists.
 * */
class Adder : public Specialist {
public:
    explicit Adder(Processor *procs);

    void decode() override;

    void execute() override;

    void memory() override;

    void writeBack() override;
};

class Suber : public Specialist {
public:
    explicit Suber(Processor *procs);

    void decode() override;

    void execute() override;

    void memory() override;

    void writeBack() override;
};

class Muler : public Specialist {
public:
    explicit Muler(Processor *procs);

    void decode() override;

    void execute() override;

    void memory() override;

    void writeBack() override;
};

class Diver : public Specialist {
private:
    int remainder;
public:
    explicit Diver(Processor *procs);

    void decode() override;

    void execute() override;

    void memory() override;

    void writeBack() override;
};

class XorRemer : public Specialist {
public:
    explicit XorRemer(Processor *procs);

    void decode() override;

    void execute() override;

    void memory() override;

    void writeBack() override;
};

class Neger : public Specialist {
public:
    explicit Neger(Processor *procs);

    void decode() override;

    void execute() override;

    void memory() override;

    void writeBack() override;
};

class Lier : public Specialist {
public:
    explicit Lier(Processor *procs);

    void decode() override;

    void execute() override;

    void memory() override;

    void writeBack() override;
};

class Comparer : public Specialist {
public:
    explicit Comparer (Processor *procs);

    void decode() override;

    void execute() override;

    void memory() override;

    void writeBack() override;
};

class Beer : public Specialist {
public:
    explicit Beer (Processor *procs);

    void decode() override;

    void execute() override;

    void memory() override;

    void writeBack() override;
};

class Jer : public Specialist {
public:
    explicit Jer (Processor *procs);

    void decode() override;

    void execute() override;

    void memory() override;

    void writeBack() override;
};

class Loader : public Specialist {
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

class Storer : public Specialist {
private:
    int offset;
public:
    explicit Storer (Processor *procs);

    void decode() override;

    void execute() override;

    void memory() override;

    void writeBack() override;
};

class Mover : public Specialist {
private:
    int offset;
public:
    explicit Mover (Processor *procs);

    void decode() override;

    void execute() override;

    void memory() override;

    void writeBack() override;
};

class Noper : public Specialist {
public:
    explicit Noper (Processor *procs) {
        this->procs = procs;
    }

    void decode() override {}

    void execute() override {}

    void memory() override {}

    void writeBack() override {}
};

class Syser : public Specialist {
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

/**
 * Begin of Processor.
 * */
class Processor {
private:
public:
    Instruction *instruction;
private:
    stageType stage;
    Specialist *specialist[15];
    int designated;             /// chosen specialist.
private:
    /**
     * return whether the instructionPool has ended.
     * */
    bool fetch() {
        /*
        if(specialist != nullptr) {
            delete specialist;
            specialist = nullptr;
        }
        */
        if(registers[34] >= instructionPool->size())
            return false;
        instruction = &instructionPool->at(registers[34]);
        switch (instruction->op) {
            case ADD: case ADDU: case ADDIU:
                designated = 0;
                // new (specialist) Adder(instruction);
                // specialist = (Specialist*) new Adder(instruction);
                break;
            case SUB: case SUBU:
                designated = 1;
                // new (specialist) Suber(instruction);
                // specialist = (Specialist*) new Suber(instruction);
                break;
            case MUL: case MULU:
                designated = 2;
                // new (specialist) Muler(instruction);
                // specialist = (Specialist*) new Muler(instruction);
                break;
            case DIV: case DIVU:
                designated = 3;
                // new (specialist) Diver(instruction);
                // specialist = (Specialist*) new Diver(instruction);
                break;
            case XOR: case XORU: case REM: case REMU:
                designated = 4;
                // new (specialist) XorRemer(instruction);
                // specialist = (Specialist*) new XorRemer(instruction);
                break;
            case NEG: case NEGU:
                designated = 5;
                // new (specialist) Neger(instruction);
                // specialist = (Specialist*) new Neger(instruction);
                break;
            case LI:
                designated = 6;
                // new (specialist) Lier(instruction);
                // specialist = (Specialist*) new Lier(instruction);
                break;
            case SEQ: case SGE: case SGT: case SLE: case SLT: case SNE:
                designated = 7;
                // new (specialist) Comparer(instruction);
                // specialist = (Specialist*) new Comparer(instruction);
                break;
            case BB: case BEQ: case BNE: case BGE: case BLE: case BGT: case BLT:
            case BEQZ: case BNEZ: case BLEZ: case BGEZ: case BGTZ: case BLTZ:
                designated = 8;
                // new (specialist) Beer(instruction);
                // specialist = (Specialist*) new Beer(instruction);
                break;
            case J: case JR: case JAL: case JALR:
                designated = 9;
                // new (specialist) Jer(instruction);
                // specialist = (Specialist*) new Jer(instruction);
                break;
            case LA: case LB: case LH: case LW:
                designated = 10;
                // new (specialist) Loader(instruction);
                // specialist = (Specialist*) new Loader(instruction);
                break;
            case SB: case SH: case SW:
                designated = 11;
                // new (specialist) Storer(instruction);
                // specialist = (Specialist*) new Storer(instruction);
                break;
            case MOVE: case MFHI: case MFLO:
                designated = 12;
                // new (specialist) Mover(instruction);
                // specialist = (Specialist*) new Mover(instruction);
                break;
            case NOP:
                designated = 13;
                // new (specialist) Noper(instruction);
                // specialist = (Specialist*) new Noper(instruction);
                break;
            case SYSCALL:
                designated = 14;
                // new (specialist) Syser(instruction);
                // specialist = (Specialist*) new Syser(instruction);
                break;
		        default:
		        		break;
        }
        specialist[designated]->Npc = ++registers[34];
        return true;
    }

public:
    explicit Processor() {
        instruction = nullptr;
        stage = FETCH;
        designated = 0;
        // specialist = nullptr;
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

    ~Processor() {
        for(int i = 0; i < 15; ++i) {
            delete specialist[i];
        }
    }

    /**
     * step or reset the specialist.
     * */
    bool step() {
        bool Contin = true;

        switch (stage) {
            case FETCH:
                if(!fetch()) Contin = false;
                stage = DECODE;
                break;
            case DECODE:
                specialist[designated]->decode();
                stage = EXECUTE;
                break;
            case EXECUTE:
                specialist[designated]->execute();
                stage = MEMORY;
                break;
            case MEMORY:
                specialist[designated]->memory();
                stage = WRITEBACK;
                break;
            case WRITEBACK:
                specialist[designated]->writeBack();
                stage = FETCH;
                break;
            default:
                break;
        }
        if(!specialist[designated]->OK) Contin = false;
        return Contin;
    }

    void display_intruction() {
        cout << registers[34] << ". ";
        for(int i = 0; i < instruction->tokens.size(); ++i)
            cout << instruction->tokens[i] << ' ';
        cout << endl;
    }
};

#include "Specialists.cpp"

#endif //MYMIPSRE_PROCESSORS_H
