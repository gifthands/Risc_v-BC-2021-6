#pragma once
#include <stdio.h>
#include <stdlib.h>

char* mem; 		             // 主存起始地址指针
int   mem_size = 0x10000000;//0x080000;	 // 主存的大小

/*寄存器文件*/
int reg32[32];
double reg64[32];

long cycles = 0;
unsigned int insn;
unsigned short insn_16;
long pc;
//译码后的各个数据字段              
unsigned int OPCODE;
unsigned int RD;
unsigned int RS1;
unsigned int RS2;
unsigned int RS3;

unsigned int FUNCT2;
unsigned int FUNCT3;
unsigned int FUNCT7;

unsigned int imm_11_0;  //I类型指令涉及到的立即数

unsigned int imm_4_0;   //S类型指令涉及到的立即数
unsigned int imm_11_5;

unsigned int imm_4_1;   //B类型指令涉及到的立即数
unsigned int imm_12;
unsigned int imm_B_11;
unsigned int imm_10_5;

unsigned int imm_31_12; //U类型指令涉及到的立即数

unsigned int imm_19_12; //J类型指令涉及到的立即数
unsigned int imm_10_1;
unsigned int imm_J_11;
unsigned int imm_20;

unsigned int pc_add;    //默认的每次pc增量，如果是C压缩指令，则加2，否则加4

/*压缩指令相关定义*/
unsigned int COPCODE;

unsigned int CRD;           //CR,CI,CSS的相关寄存器
unsigned int CRS1;
unsigned int CRS2;

unsigned int CRD_;          //CIW, CL, CS, CA, CB相关的寄存器，参照表格，_代表'，此外CA中rd'/rs1'认为这个位置是RS1_
unsigned int CRS1_;
unsigned int CRS2_;

unsigned int CFUNCT2;       //FUNCT操作数字段
unsigned int CFUNCT3;
unsigned int CFUNCT4;
unsigned int CFUNCT6;

unsigned int cimm_12;           //CI相关
unsigned int cimm_6_2;

unsigned int cimm_12_7;      //CSS相关

unsigned int cimm_12_5;         //CIW相关

unsigned int cimm_12_10;        //CL, CS相关
unsigned int cimm_6_5;

unsigned int coffset_12_10;         //CB相关
unsigned int coffset_6_2;

unsigned int cimm_j;        //CJ里面的jump target


char read_mem_byte(long addr) {
    return *((char*)(mem + addr));
}

void write_mem_byte(long addr, char ch) {
    *((char*)(mem + addr)) = ch;
}

short read_mem_halfword(long addr) {
    return *((short*)(mem + addr));
}

void write_mem_halfword(long addr, short ch) {
    *((short*)(mem + addr)) = ch;
}

unsigned short read_mem_uhalfword(long addr) {
    return *((unsigned short*)(mem + addr));
}

void write_mem_uhalfword(long addr, unsigned short ch) {
    *((unsigned short*)(mem + addr)) = ch;
}

int read_mem_word(long addr) {
    return *((int*)(mem + addr));
}

void write_mem_word(long addr, int ch) {
    *((int*)(mem + addr)) = ch;
}

unsigned int read_mem_uword(long addr) {
    return *((unsigned int*)(mem + addr));
}

void write_mem_uword(long addr, unsigned int ch) {
    *((unsigned int*)(mem + addr)) = ch;
}

float read_mem_float(long addr) {
    return *((float*)(mem + addr));
}

void write_mem_float(long addr, float a) {
    *((float*)(mem + addr)) = a;
}

double read_mem_double(long addr) {
    return *((double*)(mem + addr));
}

void write_mem_double(long addr, double a) {
    *((double*)(mem + addr)) = a;
}