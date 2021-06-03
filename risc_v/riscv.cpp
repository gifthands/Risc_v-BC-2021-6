#include <stdio.h>
#include <stdlib.h>
#include "risc_v.h"
#include "R_type.h"
#include "I_type.h"
#include "J_type.h"
#include "S_type.h"
#include "U_type.h"
#include "C_type.h"
FILE* outfile;
void print_info32(unsigned long instr)
{
    printf("未定义指令：pc:%#X\tpc_add:%d\tinsn:%#X\n", pc, pc_add, instr);
    fprintf(outfile, "未定义指令：pc:%#X\tpc_add:%d\tinsn:%#X\n", pc, pc_add, instr);
}
void print_info16(unsigned short instr)
{
    printf("未定义指令：pc:%#X\tpc_add:%d\tinsn_16:%#X\n", pc, pc_add, instr);
    fprintf(outfile, "未定义指令：pc:%#X\tpc_add:%d\tinsn_16:%#X\n", pc, pc_add, instr);
}
void Decoder() {
    OPCODE = insn & 0x7f;
    imm_4_0 = RD = (insn >> 7) & 0x1f;
    FUNCT3 = (insn >> 12) & 0x7;
    RS1 = (insn >> 15) & 0x1f;
    RS2 = (insn >> 20) & 0x1f;
    RS3 = (insn >> 27) & 0x1f;
    FUNCT2 = (insn >> 25) & 0x3;
    FUNCT7 = (insn >> 25) & 0x7f;
    imm_11_0 = (insn >> 20) & 0xfff;
    imm_11_5 = (insn >> 25) & 0x7f;
    imm_12 = (insn >> 31) & 0x1;
    imm_10_5 = (insn >> 25) & 0x3f;
    imm_4_1 = (insn >> 8) & 0xf;
    imm_B_11 = (insn >> 7) & 0x1;
    imm_31_12 = (insn >> 12) & 0xfffff;
    imm_19_12 = (insn >> 12) & 0xff;
    imm_J_11 = (insn >> 20) & 0x1;
    imm_10_1 = (insn >> 21) & 0x3ff;
    imm_20 = (insn >> 31) & 0x1;
}

void CDecoder()
{
    COPCODE = insn_16 & 0b11;
    CRD = CRS1 = (insn_16 >>7) & 0b11111;
    CRS2 = (insn_16 >> 2) & 0b11111;
    CRD_ = CRS2_ =  (insn_16 >> 2) & 0b111;
    CRS1_  = (insn_16 >> 7) & 0b111;
    CFUNCT2 = (insn_16 >> 5) & 0b11;
    CFUNCT3 = (insn_16 >> 13) & 0b111;
    CFUNCT4 = (insn_16 >> 12) & 0b1111;
    CFUNCT6 = (insn_16 >> 10) & 0b111111;
    cimm_12 = (insn_16 >> 12) & 0b1;
    cimm_6_2 = coffset_6_2 = (insn_16 >> 2) & 0b11111;
    cimm_12_7 = (insn_16 >> 7) & 0b111111;
    cimm_12_5 = (insn_16 >> 5) & 0b11111111;
    cimm_12_10 = coffset_12_10 = (insn_16 >> 10) & 0b111;
    cimm_6_5 = (insn_16 >> 5) & 0b11;
    cimm_j = (insn_16 >> 2) & 0x7ff;
}

unsigned short tran16(unsigned short instr)
{
    unsigned short n1 = instr & (0xff);
    unsigned short n2 = (instr>>8) & (0xff);
    unsigned short res = (n1) + (n2<<8);
    return res;
}

unsigned int tran32(unsigned int instr)
{
    unsigned int n1 = instr & (0xff);
    unsigned int n2 = (instr >> 8) & (0xff);
    unsigned int n3 = (instr >> 16) & (0xff);
    unsigned int n4 = (instr >> 24) & (0xff);
    unsigned res = (n1 ) + (n2 <<8) + (n3 << 16) + (n4<<24);
    return res;
}

void Execuction(void) {
    for (;;) {
        //unsigned char x = read_mem_byte(pc + 1);
        unsigned char x = read_mem_byte(pc);
        reg32[0] = 0;
        COPCODE = x & (0b11);
        printf("pc: %#4X\t读取的字节：%#4X", pc , x);
        fprintf(outfile, "pc: %#4X\t读取的字节：%#4X", pc, x);
        pc_add = (COPCODE != 0b11) ? 2 : 4;
        if (pc == 0x102b0) {
            //printf("%.4f\n", read_mem_double( - 32));
            printf("ss\n");
        }

        if (pc_add == 4)
        {
            insn = tran32(read_mem_uword(pc));  //取指令
            printf("\tinsn: %#010X\n", insn);
            fprintf(outfile,"\tinsn: %#010X\n", insn);
            Decoder();                  //译码
            if (ADD)
                pc = INSN_ADD(pc);
            else if (SUB)
                pc = INSN_SUB(pc);
            else if (SLL)
                pc = INSN_SLL(pc);
            else if (SLT)
                pc = INSN_SLT(pc);
            else if (SLTU)
                pc = INSN_SLTU(pc);
            else if (XOR)
                pc = INSN_XOR(pc);
            else if (SRL)
                pc = INSN_SRL(pc);
            else if (SRA)
                pc = INSN_SRA(pc);
            else if (OR)
                pc = INSN_OR(pc);
            else if (AND)
                pc = INSN_AND(pc);
            else if (DIVU)
                pc = INSN_DIVU(pc);
            else if (MUL)
                pc = INSN_MUL(pc);
            else if (REMU)
                pc = INSN_REMU(pc);
            else if (FLTD)
                pc = INSN_FLTD(pc);
            else if (FEQD)
                pc = INSN_FEQD(pc);
            else if (REM)
                pc = INSN_REM(pc);
            else if (SLTU)
                pc = INSN_SLTU(pc);
            else if (MULHU)
                pc = INSN_MULHU(pc);
            else if (FMULD)
                pc = INSN_FMULD(pc);
            else if (FADDD)
                pc = INSN_FADDD(pc);
            else if (DIV)
                pc = INSN_DIV(pc);
            else if (FCVTDW)
                pc = INSN_FCVTDW(pc);
            else if (JAL)
                pc = INSN_JAL(pc);
            else if (J)
                pc = INSN_J(pc);
            else if (SB)
                pc = INSN_SB(pc);
            else if (SH)
                pc = INSN_SH(pc);
            else if (SW)
                pc = INSN_SW(pc);
            else if (FSD)
                pc = INSN_FSD(pc);
            else if (BEQ)
                pc = INSN_BEQ(pc);
            else if (BNE)
                pc = INSN_BNE(pc);
            else if (BLT)
                pc = INSN_BLT(pc);
            else if (BLTU)
                pc = INSN_BLTU(pc);
            else if (BGE)
                pc = INSN_BGE(pc);
            else if (BGEU)
                pc = INSN_BGEU(pc);
            else if (ADDI)
                pc = INSN_ADDI(pc);
            else if (ANDI)
                pc = INSN_ANDI(pc);
            else if (LW)
                pc = INSN_LW(pc);
            else if (LB)
                pc = INSN_LB(pc);
            else if (ORI)
                pc = INSN_ORI(pc);
            else if (SLLI)
                pc = INSN_SLLI(pc);
            else if (LBU)
                pc = INSN_LBU(pc);
            else if (SRAI)
                pc = INSN_SRAI(pc);
            else if (LH)
                pc = INSN_LH(pc);
            else if (LHU)
                pc = INSN_LHU(pc);
            else if (SRLI)
                pc = INSN_SRLI(pc);
            else if (JALR)
                pc = INSN_JALR(pc);
            else if (FLW)
                pc = INSN_FLW(pc);
            else if (FLD)
                pc = INSN_FLD(pc);
            else if (SLTI)
                pc = INSN_SLTI(pc);
            else if (SLTIU)
                pc = INSN_SLTIU(pc);
            else if (LUI)
                pc = INSN_LUI(pc);
            else if (AUIPC)
                pc = INSN_AUIPC(pc);
            else if(ECALL)
                pc = INSN_ECALL(pc);
            else if(XORI)
                pc = INSN_XORI(pc);
            else if (insn == 0) {
                printf("END\n");
                break;
            }
            else
            {
                print_info32(insn);
                break;
            }
        }
        else
        {
            insn_16 = tran16(read_mem_uhalfword(pc));
            printf("\tinsn: %#06X\n", insn_16);
            fprintf(outfile, "\tinsn: %#06X\n", insn_16);
            CDecoder();
            if (CFSD)
                pc = INSN_CFSD(pc);
            //else if (RET)
            //    //break;
            //    pc = reg32[1];
            //pc = INSN_RET(pc);
            else if (COR)
                pc = INSN_COR(pc);
            else if (CADDI)
                pc = INSN_CADDI(pc);
            else if (CJ)
                pc = INSN_CJ(pc);
            else if (CLW)
                pc = INSN_CLW(pc);
            else if (CMV)
                pc = INSN_CMV(pc);
            else if (CADD)
                pc = INSN_CADD(pc);
            else if (CSLLI)
                pc = INSN_CSLLI(pc);
            else if (CSRLI)
                pc = INSN_CSRLI(pc);
            else if (CLI)
                pc = INSN_CLI(pc);
            else if (CFLD)
                pc = INSN_CFLD(pc);
            else if (CSW)
                pc = INSN_CSW(pc);
            else if (CADDI_16sp)
                pc = INSN_CADDI_16sp(pc);
            else if (CFLD_sp)
                pc = INSN_CFLD_sp(pc);
            else if (CLW_sp)
                pc = INSN_CLW_sp(pc);
            else if (CFSD_sp)
                pc = INSN_CFSD_sp(pc);
            else if (CSW_sp)
                pc = INSN_CSW_sp(pc);
            else if (CADDI_4spn)
                pc = INSN_CADDI_4spn(pc);
            else if (CLUI)
                pc = INSN_CLUI(pc);
            else if (CJAL)
                pc = INSN_CJAL(pc);
            else if (CSUB)
                pc = INSN_CSUB(pc);
            else if (CBNEZ)
                pc = INSN_CBNEZ(pc);
            else if (CANDI)
                pc = INSN_CANDI(pc);
            else if (CBEQZ)
                pc = INSN_CBEQZ(pc);
            else if (CJALR)
                pc = INSN_CJALR(pc);
            else if (CSRAI)
                pc = INSN_CSRAI(pc);
            else if (CAND)
                pc = INSN_CAND(pc);
            else if (CNOP)
                pc = INSN_CNOP(pc);
            else if (CJR)
                pc = INSN_CJR(pc);
            else
            {
                print_info16(insn_16);
                break;
            }
        }
        reg32[0] = 0;
    }

}
char instr[230024];
int main() {
    reg32[2] = 0x1000000;
    mem = (char*)malloc(sizeof(char) * mem_size);
    FILE* infile;
    //infile = fopen("fkkk.out", "rb");
    infile = fopen("print_3.out", "rb");
    //outfile = fopen("fkk.txt", "w");
    outfile = fopen("print_3_tx.txt", "w");
    int rc;
    while ((rc = fread(instr, sizeof(char), 230024, infile)) != 0);
    fclose(infile);
        
    long xpc = 0x10000;
    for (int i = 0; i < 230024; i++)
    {
        write_mem_byte(xpc, instr[i]);
        xpc += 1;
        /*if (xpc == 0x20a20) {
            for (int j = 0; j < 0x18; j++)
                write_mem_byte(xpc + j, 0xcd);
            xpc += 0x18;
        }*/
    }
    pc = 0x10000 + 0x86;
    Execuction();
    free(mem);
    fclose(outfile);
    return 0;
}