//
// Created by 钱苏澄 on 2018/7/3.
//

#include "limits.h"
#include "Utility.h"

//////////
Adder::Adder(Processor *procs) {
    this->procs = procs;
}

void Adder::decode(bool &dataHazard) {
    int *ic = procs->instruction->ins_code;

    Rdest = ic[0];
    Rsrc1 = ic[1];

    Rsrc2 = -1;
    if(procs->instruction->not_imm) Rsrc2 = ic[2];
    else Imm = ic[2];

    /// 触发 data hazard
    if(reg_in_use[Rsrc1] != 0 || (Rsrc2 != -1 && reg_in_use[Rsrc2] != 0)) {
				addStall(2, 1);
		    // stallPos = 1;
		    // stallCycle = 1;
		    dataHazard = true;
		    return;
    }
    else {
    		++reg_in_use[Rdest];
    }
    /*
    vector<string> &tokens = instruction->tokens;
    bool flag = tokens[3][0] == '$';

    Rdest = regTable[tokens[1]];
    Rsrc1 = regTable[tokens[2]];
    /// 特判立即数
    if(flag) Rsrc2 = regTable[tokens[3]];
    else Imm = str2int(tokens[3]);
    */

    A = registers[Rsrc1];
    if(procs->instruction->not_imm) B = registers[Rsrc2];
    else B = Imm;
}

void Adder::execute() {
    if(procs->instruction->op == ADD) ALUoutput = A + B;
    else ALUoutput = (unsigned) A + (unsigned) B;
}

void Adder::memory() {
    ;
}

void Adder::writeBack() {
    registers[Rdest] = (int)ALUoutput;
    --reg_in_use[Rdest];
}

//////////
Suber::Suber(Processor *procs) {
    this->procs = procs;
}

void Suber::decode(bool &dataHazard) {
    int *ic = procs->instruction->ins_code;

    Rdest = ic[0];
    Rsrc1 = ic[1];
    Rsrc2 = -1;
    if(procs->instruction->not_imm) Rsrc2 = ic[2];
    else Imm = ic[2];

		/// 触发 data hazard
		if(reg_in_use[Rsrc1] != 0 || (Rsrc2 != -1 && reg_in_use[Rsrc2] != 0)) {
				addStall(2, 1);
				// stallPos = 1;
				// stallCycle = 1;
				dataHazard = true;
				return;
		}
		else {
				++reg_in_use[Rdest];
		}
    /*
    vector<string> &tokens = procs->instruction->tokens;
    bool flag = tokens[3][0] == '$';

    Rdest = regTable[tokens[1]];
    Rsrc1 = regTable[tokens[2]];
    /// 特判立即数
    if(flag) Rsrc2 = regTable[tokens[3]];
    else Imm = str2int(tokens[3]);
    */

    A = registers[Rsrc1];
    if(procs->instruction->not_imm) B = registers[Rsrc2];
    else B = Imm;
}

void Suber::execute() {
    if(procs->instruction->op == SUB) ALUoutput = A - B;
    else ALUoutput = (unsigned) A - (unsigned) B;
}

void Suber::memory() {
    ;
}

void Suber::writeBack() {
    registers[Rdest] = (int)ALUoutput;
		--reg_in_use[Rdest];
}

//////////
Muler::Muler(Processor *procs) {
    this->procs = procs;
}

void Muler::decode(bool &dataHazard) {
    int *ic = procs->instruction->ins_code;

    if(procs->instruction->tokens.size() == 4) {
        Rdest = ic[0];
        Rsrc1 = ic[1];
        Rsrc2 = -1;
        if(procs->instruction->not_imm) Rsrc2 = ic[2];
        else Imm = ic[2];

		    /// 触发 data hazard
		    if(reg_in_use[Rsrc1] != 0 || (Rsrc2 != -1 && reg_in_use[Rsrc2] != 0 )) {
				    addStall(2, 1);
				    // stallPos = 1;
				    // stallCycle = 1;
				    dataHazard = true;

				    cout << "data hazard" << endl;

				    return;
		    }
		    else {
				    ++reg_in_use[Rdest];
		    }
        /*
        bool flag = tokens[3][0] == '$';

        Rdest = regTable[tokens[1]];
        Rsrc1 = regTable[tokens[2]];
        /// 特判立即数
        if(flag) Rsrc2 = regTable[tokens[3]];
        else Imm = str2int(tokens[3]);
         */
        A = registers[Rsrc1];
        if(procs->instruction->not_imm) B = registers[Rsrc2];
        else B = Imm;
    }
    else if(procs->instruction->tokens.size() == 3) {
        Rsrc1 = ic[1];
        Rsrc2 = -1;
        if(procs->instruction->not_imm) Rsrc2 = ic[2];
        else Imm = ic[2];

		    /// 触发 data hazard
		    if(reg_in_use[Rsrc1] != 0 || (Rsrc2 != -1 && reg_in_use[Rsrc2] != 0 )) {
				    addStall(2, 1);
		    		// stallPos = 1;
				    // stallCycle = 1;
				    dataHazard = true;
 
				    cout << "data hazard" << endl;
				    return;
		    }
		    else {
		    		++reg_in_use[32];
				    ++reg_in_use[33];
		    }
        /*
        bool flag = tokens[2][0] == '$';

        Rsrc1 = regTable[tokens[1]];
        if(flag) Rsrc2 = regTable[tokens[2]];
        else Imm = str2int(tokens[2]);
        */
        A = registers[Rsrc1];
        if(procs->instruction->not_imm) B = registers[Rsrc2];
        else B = Imm;
    }
    else {
        cerr << "muler decode bug" << endl;
    }
}

void Muler::execute() {
    if(procs->instruction->op == MUL) ALUoutput = A * B;
    else ALUoutput = (unsigned) A * (unsigned) B;
}

void Muler::memory() {
    ;
}

void Muler::writeBack() {
    vector<string> &tokens = procs->instruction->tokens;
    if(tokens.size() == 4) {
		    registers[Rdest] = (int) ALUoutput;
		    --reg_in_use[Rdest];
    }
    else {
        // hi 32 lo 33
        memcpy(registers + 33, &ALUoutput, sizeof(int));
        memcpy(registers + 32, ((int *)&ALUoutput) + 1, sizeof(int));
		    --reg_in_use[32];
		    --reg_in_use[33];
    }
}

//////////
Diver::Diver(Processor *procs) {
    this->procs = procs;
}

void Diver::decode(bool &dataHazard) {
    int *ic = procs->instruction->ins_code;

    if(procs->instruction->tokens.size() == 4) {
        Rdest = ic[0];
        Rsrc1 = ic[1];
        Rsrc2 = -1;
        if(procs->instruction->not_imm) Rsrc2 = ic[2];
        else Imm = ic[2];

		    /// 触发 data hazard
		    if(reg_in_use[Rsrc1] != 0 || (Rsrc2 != -1 && reg_in_use[Rsrc2] != 0 )) {
				    addStall(2, 1);
				    // stallPos = 1;
				    // stallCycle = 1;
				    dataHazard = true;
				    cout << "data hazard" << endl;
				    return;
		    }
		    else {
				    ++reg_in_use[Rdest];
		    }

        A = registers[Rsrc1];
        if(procs->instruction->not_imm) B = registers[Rsrc2];
        else B = Imm;
    }
    else if(procs->instruction->tokens.size() == 3) {
        Rsrc1 = ic[1];
        Rsrc2 = -1;
        if(procs->instruction->not_imm) Rsrc2 = ic[2];
        else Imm = ic[2];

		    /// 触发 data hazard
		    if(reg_in_use[Rsrc1] != 0 || (Rsrc2 != -1 && reg_in_use[Rsrc2] != 0 )) {
				    addStall(2, 1);
				    // stallPos = 1;
				    // stallCycle = 1;
				    dataHazard = true;

				    cout << "data hazard" << endl;

				    return;
		    }
		    else {
				    ++reg_in_use[32];
				    ++reg_in_use[33];
		    }

        A = registers[Rsrc1];
        if(procs->instruction->not_imm) B = registers[Rsrc2];
        else B = Imm;
    }
    else {
        cerr << "diver decode bug" << endl;
    }
}

void Diver::execute() {
    if(procs->instruction->op == DIV) {
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
    vector<string> &tokens = procs->instruction->tokens;
    if(tokens.size() == 4) {
    		registers[Rdest] = (int) ALUoutput;
		    --reg_in_use[Rdest];
    }
    else {
        registers[33] = (int) ALUoutput;
        registers[32] = remainder;
		    --reg_in_use[33];
		    --reg_in_use[32];
    }
}

/////////
XorRemer::XorRemer(Processor *procs) {
    this->procs = procs;
}

void XorRemer::decode(bool &dataHazard) {
    int *ic = procs->instruction->ins_code;

    Rdest = ic[0];
    Rsrc1 = ic[1];
    Rsrc2 = -1;
    if(procs->instruction->not_imm) Rsrc2 = ic[2];
    else Imm = ic[2];

		/// 触发 data hazard
		if(reg_in_use[Rsrc1] != 0 || (Rsrc2 != -1 && reg_in_use[Rsrc2] != 0 )) {
				addStall(2, 1);
				// stallPos = 1;
				// stallCycle = 1;
				dataHazard = true;

				cout << "data hazard" << endl;

				return;
		}
		else {
				++reg_in_use[Rdest];
		}

    A = registers[Rsrc1];
    if(procs->instruction->not_imm) B = registers[Rsrc2];
    else B = Imm;
}

void XorRemer::execute() {
    switch (procs->instruction->op) {
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
		--reg_in_use[Rdest];
}

/////////
Neger::Neger(Processor *procs) {
    this->procs = procs;
}

void Neger::decode(bool &dataHazard) {
    int *ic = procs->instruction->ins_code;
    Rdest = ic[0];
    Rsrc1 = ic[1];

		/// 触发 data hazard
		if(reg_in_use[Rsrc1] != 0) {
				addStall(2, 1);
				// stallPos = 1;
				// stallCycle = 1;
				dataHazard = true;

				cout << "data hazard" << endl;

				return;
		}
		else {
				++reg_in_use[Rdest];
		}

    A = registers[Rsrc1];
}

/** This is undefined. */
void Neger::execute() {
    if(procs->instruction->op == NEG) ALUoutput = -A;
    else ALUoutput = ~A;
}

void Neger::memory() {
    ;
}

void Neger::writeBack() {
    registers[Rdest] = (int) ALUoutput;
		--reg_in_use[Rdest];
}

/////////
Lier::Lier(Processor *procs) {
    this->procs = procs;
}

void Lier::decode(bool &dataHazard) {
    int *ic = procs->instruction->ins_code;
    Rdest = ic[0];
    Imm = ic[1];

    ++reg_in_use[Rdest];
}

void Lier::execute() {
    ;
}

void Lier::memory() {
    ;
}

void Lier::writeBack() {
    registers[Rdest] = Imm;
		--reg_in_use[Rdest];

}

/////////
Comparer::Comparer(Processor *procs) {
    this->procs = procs;
}

void Comparer::decode(bool &dataHazard) {
    int *ic = procs->instruction->ins_code;

    Rdest = ic[0];
    Rsrc1 = ic[1];
    Rsrc2 = -1;
    if(procs->instruction->not_imm) Rsrc2 = ic[2];
    else Imm = ic[2];

		/// 触发 data hazard
		if(reg_in_use[Rsrc1] != 0 || (Rsrc2 != -1 && reg_in_use[Rsrc2] != 0 )) {
				addStall(2, 1);
				// stallPos = 1;
				// stallCycle = 1;
				dataHazard = true;

				cout << "data hazard" << endl;

				return;
		}
		else {
				++reg_in_use[Rdest];
		}

    A = registers[Rsrc1];
    if(procs->instruction->not_imm) B = registers[Rsrc2];
    else B = Imm;
}

void Comparer::execute() {
    switch (procs->instruction->op) {
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
		--reg_in_use[Rdest];
}

/////////
Beer::Beer(Processor *procs) {
    this->procs = procs;
}

void Beer::decode(bool &dataHazard) {
    int *ic = procs->instruction->ins_code;

    vector<string> &tokens = procs->instruction->tokens;
    if(tokens.size() == 4) {
        Rsrc1 = ic[0];
        Rsrc2 = -1;
        if(procs->instruction->not_imm) Rsrc2 = ic[1];
        else Imm = ic[1];

		    /// 触发 data hazard
		    if(reg_in_use[Rsrc1] != 0 || (Rsrc2 != -1 && reg_in_use[Rsrc2] != 0)) {
				    addStall(2, 1);
				    // stallPos = 1;
				    // stallCycle = 1;
				    dataHazard = true;

				    cout << "data hazard" << endl;

				    return;
		    }

        A = registers[Rsrc1];
        if(procs->instruction->not_imm)
            B = registers[Rsrc2];
        else
            B = Imm;

        label_address = ic[2];
    }
    else if(tokens.size() == 3) {
        Rsrc1 = ic[0];

		    /// 触发 data hazard
		    if(reg_in_use[Rsrc1] != 0) {
				    addStall(2, 1);
				    // stallPos = 1;
				    // stallCycle = 1;
				    dataHazard = true;

				    cout << "data hazard" << endl;

				    return;
		    }

        A = registers[Rsrc1];
        B = 0;
        label_address = ic[2];
    }
    else if(tokens.size() == 2) {
        label_address = ic[2];
    }
}

void Beer::execute() {
    ALUoutput = label_address;
    switch (procs->instruction->op) {
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
		;
}

void Beer::writeBack() {
		if(Cond) {
				registers[34] = (int) ALUoutput;
		}
		--reg_in_use[34];
}

/////////
Jer::Jer(Processor *procs) {
    this->procs = procs;
}

void Jer::decode(bool &dataHazard) {
    int *ic = procs->instruction->ins_code;
		switch (procs->instruction->op) {
				case J: case JAL:
						label_address = ic[0];
						break;
				case JR: case JALR:
						Rsrc1 = ic[0];
						A = registers[Rsrc1];
						break;
				default:
						cerr << "Jer bug" << endl;
						break;
		}
    /**
     * deal with hazard.
     * */
		switch (procs->instruction->op) {
				case J:
						break;
				case JAL:
						++reg_in_use[31];
						break;
				case JR:
						if(reg_in_use[Rsrc1] != 0) {
								addStall(2, 1);
								// stallPos = 1;
								// stallCycle = 1;
								dataHazard = true;

								cout << "data hazard" << endl;

								return;
						}
						break;
				case JALR:
						if(reg_in_use[Rsrc1] != 0) {
								addStall(2, 1);
								// stallPos = 1;
								// stallCycle = 1;
								dataHazard = true;

								cout << "data hazard" << endl;

								return;
						}
						else
								++reg_in_use[31];
						break;
				default:
						break;
		}

}

void Jer::execute() {
    switch (procs->instruction->op) {
    case JR: case JALR:
        ALUoutput = A;
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
    switch (procs->instruction->op) {
        case JAL: case JALR:
            registers[31] = Npc;
            --reg_in_use[31];
            break;
        default:
            break;
    }
    registers[34] = (int) ALUoutput;
    --reg_in_use[34];
}

//////////
Loader::Loader(Processor *procs) {
    this->procs = procs;
    offset = INT_MAX;
}

/**
 * Note that here address can be displacement addressing.
 * */
void Loader::decode(bool &dataHazard) {
    int *ic = procs->instruction->ins_code;
    Rdest = ic[0];
    if(!procs->instruction->ch_in_str) {
        label_address = ic[1];

        ++reg_in_use[Rdest];
    }
    else {
        offset = ic[1];
        Rsrc1 = ic[2];

		    /// 触发 data hazard
        if(reg_in_use[Rsrc1] != 0) {
		        addStall(2, 1);
        		// stallPos = 1;
		        // stallCycle = 1;
		        dataHazard = true;

		        cout << "data hazard" << endl;

		        return;
        }
        else {
		        ++reg_in_use[Rdest];
        }
        A = registers[Rsrc1];
    }
}

void Loader::execute() {
    if(offset == INT_MAX) ALUoutput = label_address;
    else ALUoutput = A + offset;

    /**
     * set offset to INT_MAX again, reset immediately.
     * */
    offset = INT_MAX;
}

void Loader::memory() {
    switch (procs->instruction->op) {
        case LB:
            mainMemory->memGet((int) ALUoutput, 1, LMD_temp);
				    addStall(1, 1);
            // stallPos = 0;
            // stallCycle = 1;
            break;
        case LH:
            mainMemory->memGet((int) ALUoutput, 2, LMD_temp);
				    addStall(1, 1);
            // stallPos = 0;
				    // stallCycle = 1;
				    break;
        case LW:
            mainMemory->memGet((int) ALUoutput, 4, LMD_temp);
				    addStall(1, 1);
            // stallPos = 0;
				    // stallCycle = 1;
				    break;
        default:
            break;
    }
}

void Loader::writeBack() {
    switch (procs->instruction->op) {
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
    --reg_in_use[Rdest];
}

/////////
Storer::Storer(Processor *procs) {
    this->procs = procs;
    offset = INT_MAX;
}

void Storer::decode(bool &dataHazard) {
    int *ic = procs->instruction->ins_code;
    Rsrc1 = ic[0];
    Rsrc2 = -1;
    if(!procs->instruction->ch_in_str) {
        label_address = ic[1];
    }
    else {
        offset = ic[1];
        Rsrc2 = ic[2];
    }
		/// 触发 data hazard
		if(reg_in_use[Rsrc1] != 0 || (Rsrc2 != -1 && reg_in_use[Rsrc2] != 0)) {
				addStall(2, 1);
				// stallPos = 1;
				// stallCycle = 1;
				dataHazard = true;

				cout << "data hazard" << endl;

				return;
		}
		A = registers[Rsrc2];
}

void Storer::execute() {
    if(offset == INT_MAX) ALUoutput = label_address;
    else ALUoutput = A + offset;

    /**
     * set offset to INT_MAX again, reset immediately.
     * */
    offset = INT_MAX;
}

void Storer::memory() {
    switch (procs->instruction->op) {
        case SB:
            mainMemory->memSet((int) ALUoutput, 1, (char *)(registers + Rsrc1));
				    addStall(1, 1);
            // stallPos = 0;
				    // stallCycle = 1;
				    break;
        case SH:
            mainMemory->memSet((int) ALUoutput, 2, (char *)(registers + Rsrc1));
				    addStall(1, 1);
            // stallPos = 0;
				    // stallCycle = 1;
				    break;
        case SW:
            mainMemory->memSet((int) ALUoutput, 4, (char *)(registers + Rsrc1));
				    addStall(1, 1);
            // stallPos = 0;
				    // stallCycle = 1;
				    break;
        default:
            break;
    }
}

void Storer::writeBack() {
    ;
}

/////////
Mover::Mover(Processor *procs) {
    this->procs = procs;
}

void Mover::decode(bool &dataHazard) {
    int *ic = procs->instruction->ins_code;
    Rdest = ic[0];
    if(procs->instruction->op == MOVE)
        Rsrc1 = ic[1];

    switch (procs->instruction->op) {
        case MOVE:
        		if(reg_in_use[Rsrc1] != 0) {
				        addStall(2, 1);
        				// stallPos = 1;
				        // stallCycle = 1;
				        dataHazard = true;


				        cout << "data hazard" << endl;

				        return;
        		}
        		else {
				        ++reg_in_use[Rdest];
		        }
            A = registers[Rsrc1];
            break;
        case MFHI:
		        if(reg_in_use[32] != 0) {
				        addStall(2, 1);
		        		// stallPos = 1;
				        // stallCycle = 1;
				        dataHazard = true;

				        cout << "data hazard" << endl;

				        return;
		        }
		        else {
				        ++reg_in_use[Rdest];
		        }
            A = registers[32];
            break;
        case MFLO:
		        if(reg_in_use[33] != 0) {
				        addStall(2, 1);
		        		// stallPos = 1;
				        // stallCycle = 1;
				        dataHazard = true;

				        cout << "data hazard" << endl;

				        return;
		        }
		        else {
				        ++reg_in_use[Rdest];
		        }
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
    --reg_in_use[Rdest];
}

//////////
Syser::Syser(Processor *procs) {
    this->procs = procs;
}

/**
 * If register 2 is in use, syser can't get opCode.
 * Given opCode, we need to further determine whether other related registers are in use.
 * */
void Syser::decode(bool &dataHazard) {

		if(reg_in_use[2] != 0) {
				addStall(2, 1);
				// stallPos = 1;
				// stallCycle = 1;
				dataHazard = true;

				cout << "data hazard" << endl;

				return;
		}
    opCode = registers[2];  /// 系统调用编号值。

    switch (opCode) {
        case 1: case 4: case 17:
		        if(reg_in_use[4] != 0) {
				        addStall(2, 1);
				        // stallPos = 1;
						    // stallCycle = 1;
						    dataHazard = true;

				        cout << "data hazard" << endl;

						    return;
		        }
		        break;
        case 5:
		        ++reg_in_use[2];
		        break;
    		case 8:
    				if(reg_in_use[4] != 0|| reg_in_use[5] != 0) {
						    addStall(2, 1);
						    // stallPos = 1;
						    // stallCycle = 1;
						    dataHazard = true;

						    cout << "data hazard" << endl;

						    return;
    				}
    				break;
    		case 9:
				    if(reg_in_use[4] != 0) {
						    addStall(2, 1);
						    // stallPos = 1;
						    // stallCycle = 1;
						    dataHazard = true;

						    cout << "data hazard" << endl;

						    return;
				    }
				    else {
				    		++reg_in_use[2];
				    }
				    break;
		    default:
		    		break;
    }
    A = registers[4];
    B = registers[5];
}

void Syser::execute() {
    switch (opCode) {
        case 1:
            printf("%d", A);
            break;
        case 5:
            scanf("%d", &stdin_temp);
            break;
        case 10:
            syscall_terminate = true;
        case 17:
            returnFlag = true;
            syscall_terminate = true;
        default:
            break;
    }
}

void Syser::memory() {
    switch (opCode) {
		    case 4:
				    printf("%s", mainMemory->storage + A);
				    /// this is structure hazard
				    addStall(1, 1);
				    // stallPos = 0;
				    // stallCycle = 1;
				    break;
		    case 8:
				    char tmp;
				    do {
						    tmp = cin.get();
				    } while(tmp == '\n');
				    cin.putback(tmp);
				    cin.getline(mainMemory->storage + A, B);
				    addStall(1, 1);
				    // stallPos = 0;
				    // stallCycle = 1;
				    break;
    		case 9:
				    heap_address = mainMemory->hp;
				    mainMemory->hp += A;
				    addStall(1, 1);
				    // stallPos = 0;
				    // stallCycle = 1;
				    break;
		    default:
		    		break;
    }
}

void Syser::writeBack() {
    switch (opCode) {
        case 5:
            registers[2] = stdin_temp;
            --reg_in_use[2];
            break;
        case 9:
            registers[2] = heap_address;
				    --reg_in_use[2];
				    break;
        case 17:
            registers[4] = 0;
				    --reg_in_use[4];
				    break;
        default:
            break;
    }
}
