#include "operators/base.h"

Base::Base() {
	label_address = Rdest = Rsrc1 = Rsrc2 = Imm = A = B = Npc = 0;
	ALUoutput = 0;
	Cond = false;
	procs = nullptr;
}
