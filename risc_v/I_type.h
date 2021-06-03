#pragma once
#include <stdio.h>
#include <stdlib.h>
#include "risc_v.h"

//imm_11_0  RS1  FUNCT3  RD  OPCODE
#define ADDI ((OPCODE == 0b0010011) && (FUNCT3 == 0))  	//若指令为ADDI，则该值为1，否则为0
#define ANDI ((OPCODE == 0b0010011) && (FUNCT3 == 0b111))
#define XORI ((OPCODE == 0b0010011) && (FUNCT3 == 0b100))
#define ORI ((OPCODE == 0b0010011) && (FUNCT3 == 0b110))
#define LW ((OPCODE == 0b0000011) && (FUNCT3 == 0b010))
#define LB ((OPCODE == 0b0000011) && (FUNCT3 == 0b000))
#define LBU ((OPCODE == 0b0000011) && (FUNCT3 == 0b100))
#define LH ((OPCODE == 0b0000011) && (FUNCT3 == 0b001))
#define LHU ((OPCODE == 0b0000011) && (FUNCT3 == 0b101))

#define SLLI ((OPCODE == 0b0010011) && (FUNCT3 == 0b001) && ((imm_11_0>>6) == 0))
#define SRAI ((OPCODE == 0b0010011) && (FUNCT3 == 0b101) && ((imm_11_0>>6) == 0b10000))
#define SRLI ((OPCODE == 0b0010011) && (FUNCT3 == 0b101) && ((imm_11_0>>6) == 0))

#define JALR ((OPCODE == 0b1100111) && (FUNCT3 == 0b000))
#define FLW ((OPCODE == 0b0000111) && (FUNCT3 == 0b010))
#define FLD ((OPCODE == 0b0000111) && (FUNCT3 == 0b011))
#define SLTI ((OPCODE == 0b0010011) && (FUNCT3 == 0b010))
#define SLTIU ((OPCODE == 0b0010011) && (FUNCT3 == 0b011))



#define shamt imm_11_0 & 0x3f
int sext(unsigned int imm_11_0)
{
	int sign = imm_11_0>>11;
	int res;
	if(sign)
		res = imm_11_0 | 0xfffff000;
	else	res = imm_11_0; 
	return res;
}

long INSN_ADDI(long pc) {
	if(RD)
		reg32[RD] = reg32[RS1] + sext(imm_11_0);
	return pc + 4;
}

long INSN_ANDI(long pc) {
	if(RD)
		reg32[RD] = reg32[RS1] & sext(imm_11_0);
	return pc + 4;
}

long INSN_XORI(long pc) {
	if(RD)
		reg32[RD] = reg32[RS1] ^ sext(imm_11_0);
	return pc + 4;
}

long INSN_ORI(long pc) {
	if(RD)
		reg32[RD] = reg32[RS1] | sext(imm_11_0);
	return pc + 4;
}

long INSN_LW(long pc) {
	long addr = reg32[RS1] + sext(imm_11_0);
	if(RD)
		reg32[RD] = read_mem_word(addr);
	return pc + 4;
}

long INSN_LB(long pc) {
	long addr = reg32[RS1] + sext(imm_11_0);
	if (RD) 
		reg32[RD] = (int)(read_mem_byte(addr));  

	return pc + 4;
}

long INSN_LBU(long pc) {
	long addr = reg32[RS1] + sext(imm_11_0);
	char b = (read_mem_byte(addr)); 
	if(RD)
		reg32[RD] = ((int)b) & 0xff; //为实现零扩展,通过与运算将高3个字节都置为0 
	return pc + 4;
}

long INSN_LH(long pc) {
	long addr = reg32[RS1] + sext(imm_11_0);
	if(RD)
		reg32[RD] = (int)(read_mem_halfword(addr)); //符号扩展 
	return pc + 4;
}

long INSN_LHU(long pc) {
	long addr = reg32[RS1] + sext(imm_11_0);
	int h = (int)(read_mem_halfword(addr));
	if(RD)
		reg32[RD] = h & 0xffff; //为实现零扩展,通过与运算将高2个字节都置为0 
	return pc + 4;
}

long INSN_SLLI(long pc) {
	if(RD)
		reg32[RD] = (reg32[RS1] << shamt);
	return pc + 4;
}

long INSN_SRAI(long pc) {
	if(RD)
		reg32[RD] = (reg32[RS1] >> shamt);
	return pc + 4;
}

long INSN_SRLI(long pc) {
	if(RD)
		reg32[RD] = (int)((unsigned int)(reg32[RS1]) >> shamt); //逻辑右移，高位补0 
	return pc + 4;
}

long INSN_JALR(long pc){
	if(RD)
		reg32[RD] = pc+4;
	int sst = (reg32[RS1] + sext(imm_11_0)) & ~1;
	return (reg32[RS1]+sext(imm_11_0)) & ~1;
}

long INSN_FLW(long pc) {
	long addr = reg32[RS1] + sext(imm_11_0);
	reg64[RD] = (double)(read_mem_float(addr));
	return pc + 4;
}

long INSN_FLD(long pc) {
	long addr = reg32[RS1] + sext(imm_11_0);
	reg64[RD] = (read_mem_double(addr));
	return pc + 4;
}

long INSN_SLTI(long pc) {
	if(RD)
		reg32[RD] = (reg32[RS1] < sext(imm_11_0))?1:0; 
	return pc + 4;
}

long INSN_SLTIU(long pc) {
	if(RD)
		reg32[RD] = ((unsigned int)(reg32[RS1]) < (unsigned int)(sext(imm_11_0)))?1:0; 
	return pc + 4;
}

