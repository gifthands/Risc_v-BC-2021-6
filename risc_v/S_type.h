#pragma once
#include <stdio.h>
#include <stdlib.h>
#include "risc_v.h"

#define S_type 0b100011
#define B_type 0b1100011

#define SB ((OPCODE == S_type) && (FUNCT3 == 0))
#define SH ((OPCODE == S_type) && (FUNCT3 == 1))
#define SW ((OPCODE == S_type) && (FUNCT3 == 2))
#define FSD ((OPCODE == 0b0100111) && (FUNCT3 == 0b11))

#define BEQ ((OPCODE == B_type) && (FUNCT3 == 0))
#define BNE ((OPCODE == B_type) && (FUNCT3 == 1))
#define BLT ((OPCODE == B_type) && (FUNCT3 == 0b100))
#define BLTU ((OPCODE == B_type) && (FUNCT3 == 0b110))
#define BGE ((OPCODE == B_type) && (FUNCT3 == 0b101))
#define BGEU ((OPCODE == B_type) && (FUNCT3 == 0b111))

long INSN_SW(long pc) {
	int imm = (imm_11_5 << 5) + imm_4_0;
	if ((imm >> 11)&1)
		imm |= 0xfffff800;
	write_mem_word(imm + reg32[RS1], reg32[RS2]);
	return pc + 4;
}

long INSN_SH(long pc) {
	int imm = (imm_11_5 << 5) + imm_4_0;
	if ((imm >> 11) & 1)
		imm |= 0xfffff800;
	write_mem_halfword(imm + reg32[RS1], reg32[RS2]);
	return pc + 4;
}
long INSN_SB(long pc) {
	int imm = (imm_11_5 << 5) + imm_4_0;
	if ((imm >> 11) & 1)
		imm |= 0xfffff800;
	write_mem_byte(imm + reg32[RS1], reg32[RS2]);
	return pc + 4;
}
long INSN_FSD(long pc) {
	int imm = (imm_11_5 << 5) + imm_4_0;
	if ((imm >> 11) & 1)
		imm |= 0xfffff800;
	write_mem_double(imm + reg32[RS1], reg64[RS2]);
	return pc + 4;
}
long INSN_BEQ(long pc) {
	int imm = ((imm_12 << 12) + (imm_B_11 << 11) + (imm_10_5 << 5) + (imm_4_1 << 1));
	if (imm_12 == 1)
		imm |= 0xffffe000;
	if (reg32[RS1] == reg32[RS2]) 
		return pc + imm;
	else 
		return pc + 4;
}

long INSN_BNE(long pc) {
	int imm = ((imm_12 << 12) + (imm_B_11 << 11) + (imm_10_5 << 5) + (imm_4_1 << 1));
	if (imm_12 == 1)
		imm |= 0xffffe000;
	if (reg32[RS1] != reg32[RS2])
		return pc + imm;
	else
		return pc + 4;
}

long INSN_BLTU(long pc) {
	int imm = ((imm_12 << 12) + (imm_B_11 << 11) + (imm_10_5 << 5) + (imm_4_1 << 1));
	if (imm_12 == 1)
		imm |= 0xffffe000;
	if ((unsigned int)reg32[RS1] < (unsigned int)reg32[RS2])
		return pc + imm;
	else
		return pc + 4;
}

long INSN_BLT(long pc) {
	int imm = ((imm_12 << 12) + (imm_B_11 << 11) + (imm_10_5 << 5) + (imm_4_1 << 1));
	if (imm_12 == 1)
		imm |= 0xffffe000;
	if (reg32[RS1] < reg32[RS2])
		return pc + imm;
	else
		return pc + 4;
}
long INSN_BGEU(long pc) {
	int imm = ((imm_12 << 12) + (imm_B_11 << 11) + (imm_10_5 << 5) + (imm_4_1 << 1));
	if (imm_12 == 1)
		imm |= 0xffffe000;
	if ((unsigned int)reg32[RS1] >= (unsigned int)reg32[RS2])
		return pc + imm;
	else
		return pc + 4;
}

long INSN_BGE(long pc) {
	int imm = ((imm_12 << 12) + (imm_B_11 << 11) + (imm_10_5 << 5) + (imm_4_1 << 1));
	if (imm_12 == 1)
		imm |= 0xffffe000;
	if (reg32[RS1] >= reg32[RS2])
		return pc + imm;
	else
		return pc + 4;
}