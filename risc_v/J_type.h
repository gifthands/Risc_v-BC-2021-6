#pragma once
#include <stdio.h>
#include <stdlib.h>
#include "risc_v.h"

#define JAL (OPCODE==0b1101111)
#define J		(OPCODE==0b1101111)&&(RD==0b0)

//ÎÞ·ûºÅÀ©Õ¹
#define offset_nB (imm_4_1 << 1) | (imm_10_5 << 5) | (imm_B_11 << 11) | (imm_12 << 12)
//·ûºÅÀ©Õ¹
#define offset_sB (imm_20 == 1) ? (offset_nB | 0xffe00000) : offset_nB
long INSN_JAL(long pc)
{
	if(RD)
		reg32[RD] = pc + 4;
	unsigned int a = (imm_10_1 << 1) | (imm_J_11 << 11) | (imm_19_12 << 12) | (imm_20 << 20);
	int offset = (imm_20 == 1) ? (a | 0xffe00000) : a;
	return pc + offset;
}

long INSN_J(long pc)
{
	unsigned int a = (imm_10_1 << 1) | (imm_J_11 << 11) | (imm_19_12 << 12) | (imm_20 << 20);
	int offset = (imm_20 == 1) ? (a | 0xffe00000) : a;
	return pc + offset;
}
