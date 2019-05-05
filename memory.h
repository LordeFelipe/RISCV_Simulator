#include <stdio.h>
#include <stdint.h>

#define MEM_SIZE 4096
int32_t mem[MEM_SIZE]; 
uint32_t pc, ri, sp, gp;
int32_t opcode, rs1, rs2, rd, shamt, funct3, funct7, imm12_i, imm12_s, imm13, imm20_u, imm21;
int32_t breg[32];
int encerra;

enum OPCODES {	 

	LUI = 0x37, AUIPC = 0x17,
	ILType = 0x03,
	BType	=	0x63,	 	 	 	 
	JAL	=	0x6F,	 JALR	=	0x67,	
	StoreType	=	0x23,	 	 	 
	ILAType	=	0x13,	 	 	 	 
	RegType	=	0x33,			 	 
	ECALL	=	0x73	 	 	 	 

};

enum FUNCT3	{	//	campo	auxiliar	de	3	bits

	BEQ3=0,		BNE3=01,	BLT3=04,		BGE3=05,		BLTU3=0x06,		BGEU3=07,		 //	branches
	LB3=0,	 LH3=01,		LW3=02,		LBU3=04,		LHU3=05,	 	 	 //	loads
	SB3=0,	 SH3=01,	SW3=02,		 	 	 	 	 //	stores
	ADDSUB3=0,		SLL3=01,		SLT3=02,		SLTU3=03,	 	 	 //	LA	com	 	 	
	XOR3=04,		SR3=05,	 	OR3=06,	 	AND3=07,	 	 //	registradores
	ADDI3=0,		ORI3=06,		SLTI3=02,		XORI3=04,		ANDI3=07,	 	 //	LA	com	
	SLTIU3=03,		SLLI3=01,		SRI3=05	 	 	 	 	 //	imediatos		 	
};

enum FUNCT7	{	

//	campo	auxiliar	de	7	bits	para	as	instrucoes	SRLI/SRAI,	ADD/SUB,	SRL/SRA
	ADD7=0,	SUB7=0x20,	SRA7=0x20,	SRL7=0,	SRLI7=0x00,	SRAI7=0x20

};

void initialize(void);

void fetch(void);

void decode(void);

void execute(void);

void step(void);

void run(void);

void dump_mem(int pos1, int pos2);

int32_t lw(uint32_t address, int32_t kte);

int32_t lh(uint32_t address, int32_t kte);

int32_t lhu(uint32_t address, int32_t kte);

int32_t lb(uint32_t address, int32_t kte); 

int32_t lbu(uint32_t address, int32_t kte); 

void sw(uint32_t address, int32_t kte, int32_t dado);

void sh(uint32_t address, int32_t kte, int16_t dado);

void sb(uint32_t address, int32_t kte, int8_t dado); 

