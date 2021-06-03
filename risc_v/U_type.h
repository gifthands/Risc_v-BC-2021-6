#pragma once
#include <stdio.h>
#include <stdlib.h>

#include "risc_v.h"

#define U1_type  0b0010111
#define U2_type	 0b0110111

#define AUIPC (OPCODE == U1_type)
#define LUI  (OPCODE == U2_type)

#define ECALL (OPCODE == 0b1110011)

long INSN_AUIPC(long pc) {
	if(RD)
		reg32[RD] = pc+(imm_31_12<<12);
	//return pc + (imm_31_12<<12);
	return pc + 4;
} 

long INSN_LUI(long pc) {
	if(RD)
		reg32[RD] = (imm_31_12<<12);
	return pc + 4;
}

/*long INSN_ECALL(long pc) {
	
	int inr = reg32[10];
	int sb = read_mem_word(0x20a30);
	sb += inr;
	write_mem_word(0x20a30, sb);
	reg32[10] = sb;
	return pc + 4;
}*/
long INSN_ECALL(long pc)
{
	long type = reg32[17];
	printf("ECALL:\t%d\n************************************",type);
	switch (type)
	{
	case 57: {
		reg32[10] = 0;
	}//_close
		   break;
	case 62: {}//_lseek
		   break;
	case 63: {}//_read
		   break;
	case 64: {printf("%c", (char)(*(mem + reg32[11]))); }//_write
		   break;
	case 80: {}//_fstat
		   break;
	case 93: {exit(0); } break;//exit(0);//_exit
	case 214: {}//_sbrk
			break;

	default:
		printf("Unknown syscall type %d\n", type);
	}
	return pc + 4;
}