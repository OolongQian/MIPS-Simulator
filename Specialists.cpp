//
// Created by 钱苏澄 on 2018/7/3.
//
#include "Processors.h"
#include "Instruction.h"
#include "Simulator.h"
#include "limits.h"


extern Parser *parser;

extern vector<Instruction> *instructionPool;

extern MemoryHW *mainMemory;

extern int registers[35];

extern map<string, int> var2mem;
extern map<string, int> lab2src;

extern map<string, short> regTable;
extern map<string, opType> opTable;

extern bool returnFlag;

//////////
Adder::Adder(Instruction *instruct) {
    instruction = instruct;
}

void Adder::decode() {
    vector<string> &tokens = instruction->tokens;
    bool flag = tokens[3][0] == '$';

    Rdest = regTable[tokens[1]];
    Rsrc1 = regTable[tokens[2]];
    /// 特判立即数
    if(flag) Rsrc2 = regTable[tokens[3]];
    else Imm = str2int(tokens[3]);

    A = registers[Rsrc1];
    if(flag) B = registers[Rsrc2];
    else B = Imm;
}

void Adder::execute() {
    if(instruction->op == ADD) ALUoutput = A + B;
    else ALUoutput = (unsigned) A + (unsigned) B;
}

void Adder::memory() {
    ;
}

void Adder::writeBack() {
    registers[Rdest] = (int)ALUoutput;
}

//////////
Suber::Suber(Instruction *instruct) {
    instruction = instruct;
}

void Suber::decode() {
    vector<string> &tokens = instruction->tokens;
    bool flag = tokens[3][0] == '$';

    Rdest = regTable[tokens[1]];
    Rsrc1 = regTable[tokens[2]];
    /// 特判立即数
    if(flag) Rsrc2 = regTable[tokens[3]];
    else Imm = str2int(tokens[3]);

    A = registers[Rsrc1];
    if(flag) B = registers[Rsrc2];
    else B = Imm;
}

void Suber::execute() {
    if(instruction->op == SUB) ALUoutput = A - B;
    else ALUoutput = (unsigned) A - (unsigned) B;
}

void Suber::memory() {
    ;
}

void Suber::writeBack() {
    registers[Rdest] = (int)ALUoutput;
}

//////////
Muler::Muler(Instruction *instruct) {
    instruction = instruct;
}

void Muler::decode() {
    vector<string> &tokens = instruction->tokens;
    if(tokens.size() == 4) {
        bool flag = tokens[3][0] == '$';

        Rdest = regTable[tokens[1]];
        Rsrc1 = regTable[tokens[2]];
        /// 特判立即数
        if(flag) Rsrc2 = regTable[tokens[3]];
        else Imm = str2int(tokens[3]);

        A = registers[Rsrc1];
        if(flag) B = registers[Rsrc2];
        else B = Imm;
    }
    else if(tokens.size() == 3) {
        bool flag = tokens[2][0] == '$';

        Rdest = regTable[tokens[1]];
        if(flag) Rsrc1 = regTable[tokens[2]];
        else Imm = str2int(tokens[2]);

        A = registers[Rdest];
        if(flag) B = registers[Rsrc1];
        else B = Imm;
    }
    else {
        cerr << "muler decode bug" << endl;
    }
}

void Muler::execute() {
    if(instruction->op == MUL) ALUoutput = A * B;
    else ALUoutput = (unsigned) A * (unsigned) B;
}

void Muler::memory() {
    ;
}

void Muler::writeBack() {
    vector<string> &tokens = instruction->tokens;
    if(tokens.size() == 4) registers[Rdest] = (int) ALUoutput;
    else {
        // hi 32 lo 33
        memcpy(registers + 33, &ALUoutput, sizeof(int));
        memcpy(registers + 32, ((int *)&ALUoutput) + 1, sizeof(int));
    }
}

//////////
Diver::Diver(Instruction *instruct) {
    instruction = instruct;
}

void Diver::decode() {
    vector<string> &tokens = instruction->tokens;
    if(tokens.size() == 4) {
        bool flag = tokens[3][0] == '$';

        Rdest = regTable[tokens[1]];
        Rsrc1 = regTable[tokens[2]];
        /// 特判立即数
        if(flag) Rsrc2 = regTable[tokens[3]];
        else Imm = str2int(tokens[3]);

        A = registers[Rsrc1];
        if(flag) B = registers[Rsrc2];
        else B = Imm;
    }
    else if(tokens.size() == 3) {
        bool flag = tokens[2][0] == '$';

        Rdest = regTable[tokens[1]];
        if(flag) Rsrc1 = regTable[tokens[2]];
        else Imm = str2int(tokens[2]);

        A = registers[Rdest];
        if(flag) B = registers[Rsrc1];
        else B = Imm;
    }
    else {
        cerr << "diver decode bug" << endl;
    }
}

void Diver::execute() {
    if(instruction->op == DIV) {
        ALUoutput = A / B;
        remainder = A % B;
    }
    else {
        ALUoutput = (unsigned) A / (unsigned) B;
        remainder = (unsigned) A % (unsigned) B;
    }
}

void Diver::memory() {
    ;
}

void Diver::writeBack() {
    vector<string> &tokens = instruction->tokens;
    if(tokens.size() == 4) registers[Rdest] = (int) ALUoutput;
    else {
        // hi 32 lo 33
        registers[33] = (int) ALUoutput;
        registers[32] = remainder;
        // memcpy(registers + 33, &ALUoutput, sizeof(int));
        // memcpy(registers + 32, &remainder, sizeof(int));
    }
}

/////////
XorRemer::XorRemer(Instruction *instruct) {
    instruction = instruct;
}

void XorRemer::decode() {
    vector<string> &tokens = instruction->tokens;
    bool flag = tokens[3][0] == '$';

    Rdest = regTable[tokens[1]];
    Rsrc1 = regTable[tokens[2]];
    /// 特判立即数
    if(flag) Rsrc2 = regTable[tokens[3]];
    else Imm = str2int(tokens[3]);

    A = registers[Rsrc1];
    if(flag) B = registers[Rsrc2];
    else B = Imm;
}

void XorRemer::execute() {
    switch (instruction->op) {
        case XOR:
            ALUoutput = A ^ B;
            break;
        case XORU:
            ALUoutput = (unsigned) A ^ (unsigned) B;
            break;
        case REM:
            ALUoutput = A % B;
            break;
        case REMU:
            ALUoutput = (unsigned) A % (unsigned) B;
            break;
        default:
            break;
    }
}

void XorRemer::memory() {
    ;
}

void XorRemer::writeBack() {
    registers[Rdest] = (int)ALUoutput;
}

/////////
Neger::Neger(Instruction *instruct) {
    instruction = instruct;
}

void Neger::decode() {
    vector<string> &tokens = instruction->tokens;
    /*
    vector<string> &tokens = instruction->tokens;
    bool flag = tokens[2][0] == '$';

    Rdest = regTable[tokens[1]];
    if(flag) Rsrc1 = regTable[tokens[2]];
    else Imm = str2int(tokens[2]);

    if(flag) A = registers[Rsrc1];
    else A = Imm;
     */
    Rdest = regTable[tokens[1]];
    Rsrc1 = regTable[tokens[2]];
    A = registers[Rsrc1];
}

/** This is undefined. */
void Neger::execute() {
    if(instruction->op == NEG) ALUoutput = -A;
    else {
        // cerr << "neger is undefined" << endl;
        ALUoutput = ~A;
    }
}

void Neger::memory() {
    ;
}

void Neger::writeBack() {
    registers[Rdest] = (int) ALUoutput;
}

/////////
Lier::Lier(Instruction *instruct) {
    instruction = instruct;
}

void Lier::decode() {
    vector<string> &tokens = instruction->tokens;
    Rdest = regTable[tokens[1]];
    Imm = str2int(tokens[2]);
}

void Lier::execute() {
    ;
}

void Lier::memory() {
    ;
}

void Lier::writeBack() {
    registers[Rdest] = Imm;
}

/////////
Comparer::Comparer(Instruction *instruct) {
    instruction = instruct;
}

void Comparer::decode() {
    vector<string> &tokens = instruction->tokens;
    bool flag = tokens[3][0] == '$';

    Rdest = regTable[tokens[1]];
    Rsrc1 = regTable[tokens[2]];
    /// 特判立即数
    if(flag) Rsrc2 = regTable[tokens[3]];
    else Imm = str2int(tokens[3]);

    A = registers[Rsrc1];
    if(flag) B = registers[Rsrc2];
    else B = Imm;
}

void Comparer::execute() {
    switch (instruction->op) {
        case SEQ:
            ALUoutput = A == B;
            break;
        case SGE:
            ALUoutput = A >= B;
            break;
        case SGT:
            ALUoutput = A > B;
            break;
        case SLE:
            ALUoutput = A <= B;
            break;
        case SLT:
            ALUoutput = A < B;
            break;
        case SNE:
            ALUoutput = A != B;
            break;
        default:
            cerr << "comparer error" << endl;
            break;
    }
}

void Comparer::memory() {
    ;
}

void Comparer::writeBack() {
    registers[Rdest] = (int) ALUoutput;
    if(ALUoutput == 1) registers[Rdest] = 1;
    else registers[Rdest] = 0;
}

/////////
Beer::Beer(Instruction *instruct) {
    instruction = instruct;
}

void Beer::decode() {
    vector<string> &tokens = instruction->tokens;
    if(tokens.size() == 4) {
        bool flag = tokens[2][0] == '$';

        Rsrc1 = regTable[tokens[1]];
        if(flag) Rsrc2 = regTable[tokens[2]];
        else Imm = str2int(tokens[2]);

        A = registers[Rsrc1];
        if(flag) B = registers[Rsrc2];
        else B = Imm;

        label_address = lab2src[tokens[3]];
    }
    else if(tokens.size() == 3) {
        Rsrc1 = regTable[tokens[1]];
        A = registers[Rsrc1];
        B = 0;
        label_address = lab2src[tokens[2]];
    }
    else if(tokens.size() == 2) {
        label_address = lab2src[tokens[1]];
    }
}

void Beer::execute() {
    ALUoutput = label_address;
    switch (instruction->op) {
        case BB:
            Cond = true;
            break;
        case BEQ: case BEQZ:
            Cond = A == B;
            break;
        case BNE: case BNEZ:
            Cond = A != B;
            break;
        case BGE: case BGEZ:
            Cond = A >= B;
            break;
        case BLE: case BLEZ:
            Cond = A <= B;
            break;
        case BGT: case BGTZ:
            Cond = A > B;
            break;
        case BLT: case BLTZ:
            Cond = A < B;
            break;
        default:
            cerr << "bug in beer" << endl;
            break;
    }
}

void Beer::memory() {
    if(Cond) {
        registers[34] = (int) ALUoutput;
    }
}

void Beer::writeBack() {
    ;
}

/////////
Jer::Jer(Instruction *instruct) {
    instruction = instruct;
}

void Jer::decode() {
    switch (instruction->op) {
    case J: case JAL:
        label_address = lab2src[instruction->tokens[1]];
        break;
    case JR: case JALR:
        Rsrc1 = regTable[instruction->tokens[1]];
        break;
    default:
        cerr << "Jer bug" << endl;
        break;
    }
}

void Jer::execute() {
    switch (instruction->op) {
    case JR: case JALR:
        ALUoutput = registers[Rsrc1];
        break;
    case J: case JAL:
        ALUoutput = label_address;
        break;
    default:
        break;
    }
}

void Jer::memory() {
    ;
}

void Jer::writeBack() {
    switch (instruction->op) {
        case JAL: case JALR:
            registers[31] = Npc;
            break;
        default:
            break;
    }
    registers[34] = (int) ALUoutput;
}

//////////
Loader::Loader(Instruction *instruct) {
    instruction = instruct;
    offset = INT_MAX;
}

/**
 * Note that here address can be displacement addressing.
 * */
void Loader::decode() {
    vector<string> &tokens = instruction->tokens;
    Rdest = regTable[tokens[1]];
    /// if this line is label.
    if (!ch_in_string(tokens[2], '$')) {
        label_address = var2mem[tokens[2]];
    }
    /// if this line is displacement address.
    else {
        displacement_extractor(tokens[2], offset, Rsrc1);
    }
}

void Loader::execute() {
    if(offset == INT_MAX) ALUoutput = label_address;
    else ALUoutput = registers[Rsrc1] + offset;
}

void Loader::memory() {
    switch (instruction->op) {
        case LB:
            mainMemory->memGet((int) ALUoutput, 1, LMD_temp);
            break;
        case LH:
            mainMemory->memGet((int) ALUoutput, 2, LMD_temp);
            break;
        case LW:
            mainMemory->memGet((int) ALUoutput, 4, LMD_temp);
            break;
        default:
            break;
    }
}

void Loader::writeBack() {
    switch (instruction->op) {
        case LA:
            registers[Rdest] = (int) ALUoutput;
            break;
        case LB:
            memset(&registers[Rdest], 0, sizeof(registers[Rdest]));
            memcpy(&registers[Rdest], LMD_temp, 1);
            break;
        case LH:
            memset(&registers[Rdest], 0, sizeof(registers[Rdest]));
            memcpy(&registers[Rdest], LMD_temp, 2);
            break;
        case LW:
            memset(&registers[Rdest], 0, sizeof(registers[Rdest]));
            memcpy(&registers[Rdest], LMD_temp, 4);
            break;
        default:
            break;
    }
}

/////////
Storer::Storer(Instruction *instruct) {
    instruction = instruct;
    offset = INT_MAX;
}

void Storer::decode() {
    vector<string> &tokens = instruction->tokens;
    Rdest = regTable[tokens[1]];
    /// if this line is label.
    if (!ch_in_string(tokens[2], '$')) {
        label_address = var2mem[tokens[2]];
    }
    /// if this line is displacement address.
    else {
        displacement_extractor(tokens[2], offset, Rsrc1);
    }
}

void Storer::execute() {
    if(offset == INT_MAX) ALUoutput = label_address;
    else ALUoutput = registers[Rsrc1] + offset;
}

void Storer::memory() {
    switch (instruction->op) {
        case SB:
            mainMemory->memSet((int) ALUoutput, 1, (char *)(registers + Rdest));
            break;
        case SH:
            mainMemory->memSet((int) ALUoutput, 2, (char *)(registers + Rdest));
            break;
        case SW:
            mainMemory->memSet((int) ALUoutput, 4, (char *)(registers + Rdest));
            break;
        default:
            break;
    }
}

void Storer::writeBack() {
    ;
}

/////////
Mover::Mover(Instruction *instruct) {
    instruction = instruct;
}

void Mover::decode() {
    Rdest = regTable[instruction->tokens[1]];
    if(instruction->op == MOVE)
        Rsrc1 = regTable[instruction->tokens[2]];

    switch (instruction->op) {
        case MOVE:
            A = registers[Rsrc1];
            break;
        case MFHI:
            A = registers[32];
            break;
        case MFLO:
            A = registers[33];
            break;
        default:
            break;
    }
}

void Mover::execute() {
    ALUoutput = A;
}

void Mover::memory() {
    ;
}

void Mover::writeBack() {
    registers[Rdest] = (int) ALUoutput;
}

//////////
Syser::Syser(Instruction *instruct) {
    instruction = instruct;
}

void Syser::decode() {
    opCode = registers[regTable["$v0"]];  /// 系统调用编号值。
    A = registers[regTable["$a0"]];
    B = registers[regTable["$a1"]];
}

void Syser::execute() {
    switch (opCode) {
        case 1:
            printf("%d", A);
            break;
        case 4:
            // cout << (int) mainMemory->storage[0] << endl;
            // mainMemory->display();
            printf("%s", mainMemory->storage + A);

            break;
        case 5:
            scanf("%d", &stdin_temp);
            break;
        case 8:
            // string str_temp;
            char tmp;
            do {
                tmp = cin.get();
            } while(tmp == '\n');
            cin.putback(tmp);

            /*
            char temp[100];
            cin.getline(temp, B);
            cout << temp << endl;
             */
            cin.getline(mainMemory->storage + A, B);
            /*
            int i;
            for(i = 0; i < str_temp.length(); ++i) {
                mainMemory->storage[A + i] = str_temp[i];
            }
            mainMemory->storage[A + i] = '\0';
             */
            /*
            char temp;
            do {
                temp = cin.get();
            } while(temp == '\n');
            cin.putback(temp);

            for(int i = 0; i < B; ++i) {
                temp = cin.get();
                // cout << (int) temp << endl;
                if(temp == '\n' || temp == ' ') break;
                mainMemory->storage[A + i] = temp;
            }
             */
            break;
        case 10:
            OK = false;
        case 17:
            returnFlag = true;
            OK = false;
        default:
            break;
    }
}

void Syser::memory() {
    if(opCode == 9) {
        // cerr << "heap memory allocation may be buggy" << endl;
        heap_address = mainMemory->hp;
        mainMemory->hp += A;
    }
}

void Syser::writeBack() {
    switch (opCode) {
        case 5:
            registers[regTable["$v0"]] = stdin_temp;
            break;
        case 9:
            registers[regTable["$v0"]] = heap_address;
            break;
        case 17:
            registers[regTable["$a0"]] = 0;
            break;
        default:
            break;
    }
}