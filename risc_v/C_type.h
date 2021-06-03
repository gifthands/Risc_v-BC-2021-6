#pragma once
#include <stdio.h>
#include <stdlib.h>
#include "risc_v.h"

#define CMV (CFUNCT3==0b100)&&(cimm_12==0)&&(CRD!=0)&&(CRS2!=0)&&(COPCODE==0b10)
#define CADD (CFUNCT3==0b100)&&(cimm_12==1)&&(CRD!=0)&&(CRS2!=0)&&(COPCODE==0b10)
#define CSLLI (CFUNCT3==0)&&(CRD!=0)&&(COPCODE==0b10)
#define CSRLI (CFUNCT3==0b100)&&((CRD>>3)==0)&&(COPCODE==0b01)
#define CLI	(CFUNCT3==0b010)&&(CRD!=0)&&(COPCODE==0b01)
#define CFLD	(CFUNCT3==0b001)&&(COPCODE==0b00)
#define CSW	(CFUNCT==0b110)&&(COPCODE==0b00)
#define CSW  ((COPCODE == 0b00) && (CFUNCT3 == 0b110))

#define CLW ((COPCODE == 0) && (CFUNCT3 == 0b010))
#define COR ((COPCODE == 1) && (CFUNCT3 == 0b100) &&(((insn_16>>5)&0b11)==0b10) &&(((insn_16>>10)&0b111)==0b11))
#define CADDI ((COPCODE == 1) && (CFUNCT3 == 0) && ((insn_16>>7) & 0b11111)!=0)
#define CJ ((COPCODE == 1) && (CFUNCT3 == 0b101))
#define CFSD ((COPCODE == 0) && (CFUNCT3 == 0b101))
#define RET (insn_16==0x8082)
#define CADDI_16sp (COPCODE==0b01)&&(CFUNCT3==0b011)&&(CRD==2)
#define CLW_sp (COPCODE==0b10)&&(CFUNCT3==0b010)&&(CRD!=0)
#define CSW_sp (COPCODE==0b10)&&(CFUNCT3==0b110)
#define CFSD_sp ((COPCODE == 0b10) && (CFUNCT3 == 0b101))
#define CFLD_sp ((COPCODE == 0b10) && (CFUNCT3 == 0b001))
#define CADDI_4spn (COPCODE==0b0)&&(CFUNCT3==0)
#define CLUI (COPCODE==0b01)&&(CFUNCT3==0b011)&&(CRD!=0)&&(CRD!=2)

#define CJAL (COPCODE == 0b1) && (CFUNCT3 == 0b1)

#define CSUB (COPCODE == 0b1) && (((insn_16>>5)&0b11) == 0) && (((insn_16>>10)&0b111111) == 0b100011)
#define CBNEZ (COPCODE == 0b1) && (CFUNCT3 == 0b111)
#define CBEQZ (COPCODE == 0b1) && (CFUNCT3 == 0b110)
#define CANDI (COPCODE == 0b1) && (((insn_16>>10)&0b11)==0b10) && (CFUNCT3 == 0b100)
#define CJALR ((insn_16 & 0b111)==0b10) && ((insn_16>>12)==0b1001)
#define CSRAI (COPCODE==0b01)&&(CFUNCT3==0b100)&&(((insn_16>>10)&0b11)==0b1)
#define CAND (COPCODE==0b01)&&(CFUNCT6==0b100011)&&(((insn_16>>5)&0b11)==0b11)
#define CNOP (insn_16 == 1)
#define CJR ((insn_16&0b1111111)==0b10)&&(((insn_16>>12)&0b1111)==0b1000)&&(((insn_16>>7)&0b11111)!=0)

long INSN_CMV(long pc)
{
	if(CRD)
		reg32[CRD] = reg32[CRS2];
	return pc + 2;
}
long INSN_CADD(long pc)
{
	if(CRD)
		reg32[CRD] = reg32[CRD] + reg32[CRS2];
	return pc + 2;
}
long INSN_CSLLI(long pc)
{
	unsigned int uimm = (cimm_12 << 5) + (cimm_6_2);
	if(CRD)
		reg32[CRD] = reg32[CRD] << uimm;
	return pc + 2;
}
long INSN_CSRLI(long pc)
{
	unsigned int uimm = (cimm_12 << 5) + (cimm_6_2);
	reg32[8 + CRD_] = ((unsigned int)reg32[8 + CRD_]) >> uimm;
	return pc + 2;
}
long INSN_CSW(long pc)
{
	unsigned int uimm = (cimm_12_10 << 3) + ((cimm_6_5 & 0b1) << 6) + (((cimm_6_5 >> 1) & 0b1) << 2);
	write_mem_word(reg32[8 + CRS1_] + uimm, reg32[8 + CRS2_]);
	return pc + 2;
}

int csext(unsigned int cimm_12, unsigned int cimm_6_2) {
	int res;
	if (cimm_12)
		res = cimm_6_2 | 0xffffffe0;
	else	res = cimm_6_2;
	return res;
}

long INSN_CLI(long pc) {
	if(CRD)
		reg32[CRD] = csext(cimm_12, cimm_6_2);
	return pc + 2;
}

long INSN_CFLD(long pc) {
	unsigned int uimm = (cimm_12_10 << 3) | (cimm_6_5 << 6);
	long addr = reg32[8 + CRS1_] + uimm;
	//printf("CFLD: CRD_:%d  CRS1_:%d uimm:%d  reg:%#X", CRD_, CRS1_, uimm, reg32[8 + CRS1_]);
	reg64[8 + CRD_] = read_mem_double(addr);
	return pc + 2;
}

int INSN_CLW(int pc) {
	int RD_ = ((insn_16) >> 2) & 0b111;
	int RS1_ = ((insn_16) >> 7) & 0b111;
	int imm = (((insn_16 >> 5) & 1) << 6) + (((insn_16 >> 10) & 0b111) << 3) + (((insn_16 >> 6) & 1) << 2);
	reg32[RD_+8] = read_mem_word(reg32[RS1_+8] + imm);
	return pc + 2;
}

int INSN_COR(int pc) {
	int crs2 = ((insn_16) >> 2) & 0b111;
	int crd = ((insn_16) >> 7) & 0b111;
	reg32[8 + crd] |= reg32[8 + crs2];
	return pc + 2;
}

int INSN_CADDI(int pc) {
	int rs1 = (insn_16 >> 7) & 0b11111;
	int imm = ((insn_16 >> 2) & 0b11111) + (((insn_16 >> 12) & 1) << 5);
	if (((insn_16 >> 12) & 1))
		imm |= 0xffffffc0;
	if(rs1)
		reg32[rs1] += imm;
	return pc + 2;
}

int INSN_CJ(int pc) {
	int imm = (((insn_16 >> 3) & 0b111) << 1) + (((insn_16 >> 11) & 0b1) << 4) + (((insn_16 >> 2) & 1) << 5) + (((insn_16 >> 7) & 1) << 6) + (((insn_16 >> 6) & 1) << 7) + (((insn_16 >> 9) & 0b11) << 8) + (((insn_16 >> 8) & 1) << 10) + (((insn_16 >> 12) & 1) << 11);
	if ((((insn_16 >> 12) & 1) << 11))
		imm |= 0xfffff000;
	return pc + imm;
}

int INSN_CFSD(int pc) {
	int imm = (((insn_16 >> 10) & 0b111) << 3) + (((insn_16 >> 5) & 0b11) << 6);
	int rs2 = ((insn_16 >> 2) & 0b111);
	int rs1 = ((insn_16 >> 7) & 0b111);
	write_mem_double(reg32[rs1+8] + imm, reg64[rs2+8]);
	return pc + 2;
}

long INSN_RET(long pc)
{
	//printf("RET中的ret_pc:%#X\n", reg32[1]);
	return reg32[1];
}

long INSN_CADDI_16sp(long pc)
{
	unsigned imm4 = (cimm_6_2 >> 4) & 0b1;
	unsigned imm5 = (cimm_6_2) & 0b1;
	unsigned imm6 = (cimm_6_2 >> 3) & 0b1;
	unsigned imm87 = (cimm_6_2 >> 1) & 0b11;
	unsigned int imm_ = (cimm_12 << 9) + (imm4 << 4) + (imm5 << 5) + (imm6 << 6) + (imm87 << 7);
	int n = (cimm_12 == 1) ? (imm_ | 0xfffffc00) : imm_;
	reg32[2] += n;
	return pc + 2;
}

long INSN_CLW_sp(long pc)
{
	unsigned int n5 = cimm_12;
	unsigned int n76 = cimm_6_2 & 0b11;
	unsigned int n42 = (cimm_6_2 >> 2) & 0b111;
	unsigned int n = (n5 << 5)+(n76<<6)+(n42<<2);
	//printf("LW中的 rd:%d    uimm:%#X\n", CRD,n);
	if(CRD)
		reg32[CRD] = read_mem_word(reg32[2] + n);
	return pc+2;
}

long INSN_CSW_sp(long pc)
{
	unsigned int n52 = (cimm_12_7 >> 2) & 0b1111;
	unsigned int n76 = (cimm_12_7 ) & 0b11;
	unsigned int n = (n52 << 2) + (n76 << 6);
	write_mem_word(reg32[2] + n, reg32[CRS2]);
	return pc+2;
}

long INSN_CFSD_sp(long pc) {
	unsigned int uimm8_6 = cimm_12_7 & 0b111;
	unsigned int uimm5_3 = (cimm_12_7>>3) & 0b111;
	unsigned int uimm = (uimm8_6 << 6) | (uimm5_3 << 3);
	write_mem_double(reg32[2] + uimm, reg64[CRS2]);
	return pc + 2;
}

long INSN_CFLD_sp(long pc) {
	unsigned int uimm5 = cimm_12;
	unsigned int uimm8_6 = cimm_6_2 & 0b111;
	unsigned int uimm4_3 = (cimm_6_2>>3) & 0b11;
	unsigned int uimm = ((uimm8_6 << 6) | (uimm5<<5)) | (uimm4_3 << 3);
	long addr = reg32[2] + uimm;
	reg64[CRD] = read_mem_double(addr);
	return pc + 2;
}

long INSN_CADDI_4spn(long pc)
{
	unsigned int n3 = cimm_12_5 & 0b1;
	unsigned int n2 = (cimm_12_5 >> 1) & 0b1;
	unsigned int n96 = (cimm_12_5 >> 2) & 0b1111;
	unsigned int n54 = (cimm_12_5 >> 6) & 0b11;
	unsigned n = (n2 << 2) + (n3 << 3) + (n96 << 6) + (n54 << 4);
	reg32[8 + CRD_] = reg32[2] + n;
	return pc + 2;
}

long INSN_CLUI(long pc) {
	unsigned int imm17 = cimm_12;
	unsigned int imm16_12 = cimm_6_2;
	unsigned int imm = (imm17 << 17) + (imm16_12 << 12);
	int res;
	if (imm17) {
		res = imm | 0xfffe0000;
	}
	else res = imm;
	if(CRD)
		reg32[CRD] = res;
	return pc + 2;
}

long INSN_CJAL(long pc) {
	int imm = (((insn_16 >> 3) & 0b111) << 1) + (((insn_16 >> 11) & 0b1) << 4) + (((insn_16 >> 2) & 1) << 5) + (((insn_16 >> 7) & 1) << 6) + (((insn_16 >> 6) & 1) << 7) + (((insn_16 >> 9) & 0b11) << 8) + (((insn_16 >> 8) & 1) << 10) + (((insn_16 >> 12) & 1) << 11);
	if ((((insn_16 >> 12) & 1) << 11))
		imm |= 0xfffff000;
	reg32[1] = pc + 2;
	return pc + imm;
}

long INSN_CJALR(long pc) {
	int rs1 = (insn_16 >> 7) & 0b11111;
	reg32[1] = pc + 2;
	return ((reg32[rs1])&~1);
}

long INSN_CSUB(long pc) {
	int crs2 = ((insn_16) >> 2) & 0b111;
	int crd = ((insn_16) >> 7) & 0b111;
	reg32[8 + crd] -= reg32[8 + crs2];
	return pc + 2;
}

long INSN_CBNEZ(long pc) {
	int imm = (((insn_16 >> 3) & 0b11) << 1) + (((insn_16 >> 10) & 0b11) << 3) + (((insn_16 >> 2) & 1) << 5) + (((insn_16 >> 5) & 0b11) << 6) + (((insn_16 >> 12) & 1) << 8);
	if (imm >> 8)
		imm |= 0xffffff00;
	if (reg32[CRS1_ + 8]) return pc + imm;
	return pc + 2;
}

long INSN_CBEQZ(long pc) {
	int imm = (((insn_16 >> 3) & 0b11) << 1) + (((insn_16 >> 10) & 0b11) << 3) + (((insn_16 >> 2) & 1) << 5) + (((insn_16 >> 5) & 0b11) << 6) + (((insn_16 >> 12) & 1) << 8);
	if (imm >> 8)
		imm |= 0xffffff00;
	if (reg32[CRS1_ + 8]) return pc + 2;
	return pc + imm;
}



long INSN_CANDI(long pc) {
	int imm = (((insn_16 >> 2) & 0b11111)) + (((insn_16 >> 12) & 1) << 5);
	if (((insn_16 >> 12) & 1))
		imm |= 0xffffffc0;
	int crd_ = (insn_16 >> 7) & 0b111;
	reg32[crd_ + 8] = reg32[crd_ + 8] & imm;
	return pc + 2;
}
long INSN_CSRAI(long pc)
{
	unsigned int n = (cimm_12 << 5) + (cimm_6_2);
	reg32[8 + CRD_] = reg32[8 + CRD_] >> n;
	return pc + 4;
}

long INSN_CAND(long pc)
{
	reg32[8 + CRD_] = reg32[8 + CRD_] & reg32[8 + CRS2_];
	return pc + 2;
}

long INSN_CNOP(long pc) {
	return pc + 2;
}
long INSN_CJR(long pc) {
	int cs = (insn_16 >> 7) & 0b11111;
	return (reg32[cs]&(~1));
}