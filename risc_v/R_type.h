#pragma once
#include <stdio.h>
#include <stdlib.h>
#include "risc_v.h"

#define R_type  0b0110011

#define ADD  ((OPCODE == R_type) && (FUNCT7 == 0) && (FUNCT3 == 0))	//若指令为ADD，则该值为1，否则为0
#define SUB  ((OPCODE == R_type) && (FUNCT7 == 0b0100000) && (FUNCT3 == 0))
#define SLL  ((OPCODE == R_type) && (FUNCT7 == 0) && (FUNCT3 == 1))
#define SLT  ((OPCODE == R_type) && (FUNCT7 == 0) && (FUNCT3 == 0b010))
#define SLTU  ((OPCODE == R_type) && (FUNCT7 == 0) && (FUNCT3 == 0b011))
#define XOR  ((OPCODE == R_type) && (FUNCT7 == 0) && (FUNCT3 == 0b100))
#define SRL  ((OPCODE == R_type) && (FUNCT7 == 0) && (FUNCT3 == 0b101))
#define SRA  ((OPCODE == R_type) && (FUNCT7 == 0b0100000) && (FUNCT3 == 0b101))
#define OR  ((OPCODE == R_type) && (FUNCT7 == 0) && (FUNCT3 == 0b110))
#define AND  ((OPCODE == R_type) && (FUNCT7 == 0) && (FUNCT3 == 0b111))
#define DIVU  ((OPCODE == R_type) && (FUNCT7 == 1) && (FUNCT3 == 0b101))


#define MUL  ((OPCODE == R_type) && (FUNCT7 == 1) && (FUNCT3 == 0))
#define REMU  ((OPCODE == R_type) && (FUNCT7 == 1) && (FUNCT3 == 0b111))
#define FLTD  ((OPCODE == 0b1010011) && (FUNCT7 == 0b1010001) && (FUNCT3 == 1))
#define FEQD  ((OPCODE == 0b1010011) && (FUNCT7 == 0b1010001) && (FUNCT3 == 0b010))
#define REM  ((OPCODE == R_type) && (FUNCT7 == 1) && (FUNCT3 == 0b110))
#define SLTU  ((OPCODE == R_type) && (FUNCT7 == 0) && (FUNCT3 == 0b011))
#define MULHU ((OPCODE == R_type) && (FUNCT7 == 1) && (FUNCT3 == 0b011))

#define FMULD  (OPCODE == 0b1010011) && (FUNCT7 == 0b0001001)
#define FADDD  (OPCODE == 0b1010011) && (FUNCT7 == 1)

#define DIV  ((OPCODE == R_type) && (FUNCT7 == 1) && (FUNCT3 == 0b100))
#define FCVTDW  (OPCODE == 0b1010011) && (FUNCT7 == 0b1101001)


long INSN_FCVTDW(long pc){
	reg64[RD] = (double)reg32[RS1];
	return pc + 4;
}

long INSN_FMULD(long pc){
	reg64[RD] = reg64[RS1] * reg64[RS2];
	return pc + 4;
}

long INSN_DIV(long pc){
	if(RD)
		reg32[RD] = reg32[RS1] / reg32[RS2];
	return pc + 4;
}

long INSN_MULHU(long pc){
	if(RD)
		reg32[RD] = (unsigned int)(((unsigned int)reg32[RS1]) * ((unsigned int)reg32[RS1])) >> 32;
	return pc + 4;
}

long INSN_SLTU(long pc){
	if(RD)
		reg32[RD] = (((unsigned int)reg32[RS1]) < ((unsigned int)reg32[RS2])) ? 1 : 0;
	return pc + 4;
}

long INSN_REM(long pc){
	if(RD)
		reg32[RD] = reg32[RS1] % reg32[RS2];
	return pc + 4;
}

long INSN_DIVU(long pc){
	if(RD)
		reg32[RD] = ((unsigned int)reg32[RS1]) / ((unsigned int)reg32[RS2]);
	return pc + 4;
}

long INSN_FEQD(long pc){
	if(RD)
		reg32[RD] = reg64[RS1] == reg64[RS2] ? 1 : 0;
	return pc + 4;
}

long INSN_FLTD(long pc){
	if(RD)
		reg32[RD] = reg64[RS1] > reg64[RS2] ? 1 : 0;
	return pc + 4;
}

long INSN_FADDD(long pc){
	reg64[RD] = reg64[RS1] + reg64[RS2];
	return pc + 4;
}

long INSN_REMU(long pc){
	if(RD)
		reg32[RD] = ((unsigned int)reg32[RS1]) % ((unsigned int) reg32[RS2]);
	return pc + 4;
}


long INSN_MUL(long pc){
	if(RD)
		reg32[RD] = reg32[RS1] * reg32[RS2];
	return pc + 4;
}




long INSN_ADD(long pc) {
	if(RD)
		reg32[RD] = reg32[RS1] + reg32[RS2];
	return pc + 4;
}

long INSN_SUB(long pc) {
	if(RD)
		reg32[RD] = reg32[RS1] - reg32[RS2];
	return pc + 4;
}

long INSN_SLL(long pc){
	if(RD)
		reg32[RD] = reg32[RS1] << ((unsigned int)(reg32[RS2] & 0x1f));
	return pc + 4;
}

long INSN_SLT(long pc){
	if(RD)
		reg32[RD] = (reg32[RS1] < reg32[RS2]) ? 1 : 0;
	return pc + 4;
}


long INSN_XOR(long pc){
	if(RD)
		reg32[RD] = (reg32[RS1] ^ reg32[RS2]);
	return pc + 4;
}

long INSN_SRL(long pc){
	if(RD)
		reg32[RD] = (unsigned int)reg32[RS1] >> ((unsigned int)(reg32[RS2] & 0x1f));
	return pc + 4;
}

long INSN_SRA(long pc){
	if(RD)
		reg32[RD] = reg32[RS1] >> ((unsigned int)(reg32[RS2] & 0x1f));
	return pc + 4;	
}

long INSN_OR(long pc){
	if(RD)
		reg32[RD] = reg32[RS1] | reg32[RS2];
	return pc + 4;
}

long INSN_AND(long pc){
	if(RD)
		reg32[RD] = reg32[RS1] & reg32[RS2];
	return pc + 4;
}

