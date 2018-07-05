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
    Specialist *specialist;
private:
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
                new (specialist) Adder(this);
                // specialist = (Specialist*) new Adder(instruction);
                break;
            case SUB: case SUBU:
                new (specialist) Suber(this);
                // specialist = (Specialist*) new Suber(instruction);
                break;
            case MUL: case MULU:
                new (specialist) Muler(this);
                // specialist = (Specialist*) new Muler(instruction);
                break;
            case DIV: case DIVU:
                new (specialist) Diver(this);
                // specialist = (Specialist*) new Diver(instruction);
                break;
            case XOR: case XORU: case REM: case REMU:
                new (specialist) XorRemer(this);
                // specialist = (Specialist*) new XorRemer(instruction);
                break;
            case NEG: case NEGU:
                new (specialist) Neger(this);
                // specialist = (Specialist*) new Neger(instruction);
                break;
            case LI:
                new (specialist) Lier(this);
                // specialist = (Specialist*) new Lier(instruction);
                break;
            case SEQ: case SGE: case SGT: case SLE: case SLT: case SNE:
                new (specialist) Comparer(this);
                // specialist = (Specialist*) new Comparer(instruction);
                break;
            case BB: case BEQ: case BNE: case BGE: case BLE: case BGT: case BLT:
            case BEQZ: case BNEZ: case BLEZ: case BGEZ: case BGTZ: case BLTZ:
                new (specialist) Beer(this);
                // specialist = (Specialist*) new Beer(instruction);
                break;
            case J: case JR: case JAL: case JALR:
                new (specialist) Jer(this);
                // specialist = (Specialist*) new Jer(instruction);
                break;
            case LA: case LB: case LH: case LW:
                new (specialist) Loader(this);
                // specialist = (Specialist*) new Loader(instruction);
                break;
            case SB: case SH: case SW:
                new (specialist) Storer(this);
                // specialist = (Specialist*) new Storer(instruction);
                break;
            case MOVE: case MFHI: case MFLO:
                new (specialist) Mover(this);
                // specialist = (Specialist*) new Mover(instruction);
                break;
            case NOP:
                new (specialist) Noper(this);
                // specialist = (Specialist*) new Noper(instruction);
                break;
            case SYSCALL:
                new (specialist) Syser(this);
                // specialist = (Specialist*) new Syser(instruction);
                break;
		        default:
		        		break;
        }
        specialist->Npc = ++registers[34];
        return true;
    }

public:
    explicit Processor() {
        instruction = nullptr;
        stage = FETCH;
        // specialist = nullptr;
        specialist = (Specialist *) malloc(200);
    }

    bool step() {
        bool Contin = true;

        switch (stage) {
            case FETCH:
                if(!fetch()) Contin = false;
                stage = DECODE;
                break;
            case DECODE:
                specialist->decode();
                stage = EXECUTE;
                break;
            case EXECUTE:
                specialist->execute();
                stage = MEMORY;
                break;
            case MEMORY:
                specialist->memory();
                stage = WRITEBACK;
                break;
            case WRITEBACK:
                specialist->writeBack();
                stage = FETCH;
                break;
            default:
                break;
        }
        if(!specialist->OK) Contin = false;
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
