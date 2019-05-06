#include "memory.h"

void initialize(void){
	pc = 0x00000000;
	ri = 0x00000000;
	breg[2] = 0x00003ffc; //sp
	breg[3] = 0x00001800; //gp
	encerra = 0;
}

void fetch(void){
	ri = mem[pc/4];
	pc += 4;
}

void decode(void){

	int32_t aux = ri;
	opcode = (aux & 0x0000007F);
	rs1 = ((aux >> 15) & 0x0000001F);
	rs2 = ((aux >> 20) & 0x0000001F);
	rd = ((aux >> 7) & 0x0000001F);
	shamt = ((aux >> 20) & 0x0000001F);

	funct3 = ((aux >> 12) & 0x00000007);
	funct7 = ((aux >> 25) & 0x0000007F);

	imm12_i = ((aux >> 20) & 0x00000FFF); 
	imm12_s = (((aux >> 7) & 0x0000001F) + ((aux >> 20) & 0x00000FE0));
	imm13 = (((aux >> 8) & 0x0000000F) + ((aux >> 21) & 0x000003F0) + ((aux << 3) & 0x00000400 ) + ((aux >> 20) & 0x00000800));
	imm13 = imm13 << 1;
	imm20_u = ((aux >> 12) & 0x000FFFFF); 
	imm21 = (((aux >> 21) & 0x000003FF) + ((aux >> 10) & 0x00000400) + ((aux >> 1) & 0x0007F800) + ((aux >> 12) & 0x00080000));;	
	imm21 = imm21 << 1;

}

void execute(void){
	int32_t pca = pc - 4;
	switch(opcode){
		case LUI:
			breg[rd] = imm20_u << 12;
		break;

		case AUIPC:
			breg[rd] = pca + (imm20_u << 12);
		break;

		case ILType:
			switch(funct3){
				case LB3:
					imm12_i = imm12_i << 20;
					imm12_i = imm12_i >> 20;
					breg[rd] = lb(breg[rs1],imm12_i);
				break;

				case LH3:
					imm12_i = imm12_i << 20;
					imm12_i = imm12_i >> 20;
					breg[rd] = lh(breg[rs1],imm12_i);
				break;

				case LW3:
					imm12_i = imm12_i << 20;
					imm12_i = imm12_i >> 20;
					breg[rd] = lw(breg[rs1],imm12_i);
				break;

				case LBU3:				
					imm12_i = imm12_i << 20;
					imm12_i = imm12_i >> 20;
					breg[rd] = lbu(breg[rs1],imm12_i);
				break;

				case LHU3:
					imm12_i = imm12_i << 20;
					imm12_i = imm12_i >> 20;
					breg[rd] = lhu(breg[rs1],imm12_i);
				break;
			break;
			}
		case BType:
			switch(funct3){
				case BEQ3:
					imm13 = imm13 << 19;
					imm13 = imm13 >> 19;
					if(breg[rs1] == breg[rs2]) pc = pca + imm13;
				break;

				case BNE3:
					imm13 = imm13 << 19;
					imm13 = imm13 >> 19;
					if(breg[rs1] != breg[rs2]) pc = pca + imm13;
				break;

				case BLT3:
					imm13 = imm13 << 19;
					imm13 = imm13 >> 19;
					if(breg[rs1] < breg[rs2]) pc = pca + imm13;          //OLHA SE TEM CAST NESSES BRANCH
				break;

				case BGE3:
					imm13 = imm13 << 19;
					imm13 = imm13 >> 19;
					if(breg[rs1] >= breg[rs2]) pc = pca + imm13;
				break;				

				case BLTU3:
					imm13 = imm13 << 19;
					imm13 = imm13 >> 19;
					if((uint32_t)breg[rs1] < (uint32_t)breg[rs2]) pc = pca + imm13; //Olha o sinal
				break;

				case BGEU3:
					imm13 = imm13 << 19;
					imm13 = imm13 >> 19;
					if((uint32_t)breg[rs1] >= (uint32_t)breg[rs2]) pc = pca + imm13;
				break;
				}
		break;

		case JAL:
			breg[rd] =	pca + 4;
			imm21 = imm21 << 11;
			imm21 = imm21 >> 11;
			pc = pca + imm21;
		break;

		case JALR:
			pc = breg[rs1] + imm12_i; //diabo é esse mascarar
			breg[rd] = pca + 4;
		break;

		case StoreType:
			switch(funct3){
				case SB3:
					imm12_s = imm12_s << 20;
					imm12_s = imm12_s >> 20;
					sb(rs1,imm12_s,rs2);
				break;

				case SH3:
					imm12_s = imm12_s << 20;
					imm12_s = imm12_s >> 20;
					sh(rs1,imm12_s,rs2);
				break;

				case SW3:
					imm12_s = imm12_s << 20;
					imm12_s = imm12_s >> 20;
					sw(rs1,imm12_s,rs2);
				break;		
		break;
		}
		case ILAType:
			switch(funct3){
				case ADDI3:
					imm12_i = imm12_i << 20;
					imm12_i = imm12_i >> 20;
					breg[rd] = breg[rs1] + imm12_i;
				break;

				case SLTI3:
					imm12_i = imm12_i << 20;
					imm12_i = imm12_i >> 20;
					breg[rd] = breg[rs1] < imm12_i;
				break;

				case SLTIU3:
					breg[rd] = (uint32_t) breg[rs1] < (uint32_t) imm12_i;
				break;

				case XORI3:
					imm12_i = imm12_i << 20;
					imm12_i = imm12_i >> 20;
					breg[rd] = breg[rs1] ^ imm12_i;
				break;

				case ORI3:
					imm12_i = imm12_i << 20;
					imm12_i = imm12_i >> 20;
					breg[rd] = breg[rs1] | imm12_i;
				break;	

				case ANDI3:
					imm12_i = imm12_i << 20;
					imm12_i = imm12_i >> 20;
					breg[rd] = breg[rs1] & imm12_i;
				break;

				case SLLI3:
					breg[rd] = breg[rs1] << shamt;
				break;

				case SRI3:
					switch(funct7){
						case SRAI7:
							shamt = shamt << 27;
							shamt = shamt >> 27;
							breg[rd] = breg[rs1] >> shamt;
						break;

						case SRLI7:
							breg[rd] = breg[rs1] >> shamt;
						break;
					}
			break;
			}
		break;

		case RegType:
			switch(funct3){
				case ADDSUB3:
					switch(funct7){
						case ADD7:
							breg[rd] = breg[rs1] + breg[rs2];
						break;

						case SUB7:
							breg[rd] = breg[rs1] - breg[rs2];
						break;
					}
				break;

				case SLL3:
					breg[rd] = breg[rs1] << (0x0000001F & breg[rs2]);
				break;

				case SLT3:
					breg[rd] = breg[rs1] < breg[rs2];
				break;

				case SLTU3:
					breg[rd] = (uint32_t) breg[rs1] < (uint32_t) breg[rs2];   
				break;

				case XOR3:
					breg[rd] = breg[rs1] ^ breg[rs2];
				break;

				case SR3:
					switch(funct7){
						case SRL7:
							breg[rd] = (uint32_t) breg[rs1] >> (0x0000001F & breg[rs2]);  
						break;

						case SRA7:
							breg[rd] = breg[rs1] >> (0x0000001F & breg[rs2]);	
						break;
					}
				break;

				case OR3:
					breg[rd] = breg[rs1] | breg[rs2];
				break;

				case AND3:
					breg[rd] = breg[rs1] & breg[rs2];
				break;
			}
		break;
		
		case ECALL:
			switch(breg[17]){
				case 1:
					printf("%d",breg[10]); //10 = a0
				break;

				char* str;
				case 4:
					str = (char*)mem + breg[10];
					int i = 0;
					while(*(str + i) != '\0'){
						printf("%c",*(str + i));
						i++;
					}
					
				break;

				case 10:
					encerra = 1;
				break;
			}
		break;
	}

	breg[0] = 0; // zerando zero;
}

void step(void){
	fetch();
	decode();
	execute();
}

void run(void){
	while(encerra == 0){
		step();
	}
}

void dump_mem(int pos1, int pos2, char format){
	if(format == 'h'){
		for(int i = pos1; i < pos2; i++){
			printf("mem[%04d] = %08x\n",i,mem[i]);
		}
	}

	else if(format == 'd'){
		for(int i = pos1; i < pos2; i++){
			printf("mem[%04d] = %08d\n",i,mem[i]);
		}
	}
}

void dump_reg(char format){
	if(format == 'h'){
		for(int i = 0; i < 32;i++){
			printf("reg %d : %08x\n", i, breg[i]);
		}
	}
	else if(format == 'd'){
		for(int i = 0; i < 32;i++){
			printf("reg %d : %x\n", i, breg[i]);
		}
	}
}

int32_t lw(uint32_t address, int32_t kte){
	int32_t new_address = (int32_t)((address+kte)/4);	
	return mem[new_address];							//Retorna o conteúdo do endereço carregado
}

int32_t lh(uint32_t address, int32_t kte){
	int32_t new_address = (int32_t)((address+kte)/4);
	int32_t value = mem[new_address];					//Extraindo o conteúdo do endereço de memória

	int n_rotation = ((address + kte)/2)%2;
	int16_t new_value = value >> (n_rotation*16); 		//Rotacionando o half word para os 4 LSB
	return new_value;
}

int32_t lhu(uint32_t address, int32_t kte){
	int32_t new_address = (int32_t)((address+kte)/4);
	int32_t value = mem[new_address];					//Extraindo o conteúdo do endereço de memória

	int n_rotation = ((address + kte)/2)%2;
	value = value >> (n_rotation*16); 					//Rotacionando o half word para os 4 LSB
	value &= 0x0000FFFF; 								//Bit clear nos bits não utlizados
	return value;
}

int32_t lb(uint32_t address, int32_t kte){
	int32_t new_address = (int32_t)((address+kte)/4);
	int32_t value = mem[new_address];					//Acessando o endereço de memória

	int n_rotation = (address + kte)%4;
	int8_t new_value = value >> (n_rotation*8); 		//Rotacionando o half word para os 2 LSB
	return new_value;	
}

int32_t lbu(uint32_t address, int32_t kte){
	int32_t new_address = (int32_t)((address+kte)/4);
	int32_t value = mem[new_address];					//Acessando o endereço de memória

	int n_rotation = (address + kte)%4;
	value = value >> (n_rotation*8); 					//Rotacionando o half word para os 2 LSB
	value &= 0x000000FF; 								//Bit clear
	return value;	
}

void sw(uint32_t address, int32_t kte, int32_t dado){
	int32_t new_address = (int32_t)((address+kte)/4);
	mem[new_address] = dado;
}

void sh(uint32_t address, int32_t kte, int16_t dado){
	int32_t new_address = (int32_t)((address+kte)/2);	//Pegando o endereço em multiplos de 2
	int n_rotation = new_address%2;						//Calculando o número de rotações
	int32_t mask = 0xFFFF;

	mask = mask << (n_rotation*16);						//Rotacionando a mascara para a posicao da half word q sera inserida
	int32_t value = (uint16_t) dado << (n_rotation*16); //Rotacionando a half word para a posicao certa

	mem[(address + kte)/4] &= ~(mask);					//Limpando o conteudo da memoria q o dado sera inserido
	mem[(address + kte)/4] |= value;					//Inserindo o dado

}

void sb(uint32_t address, int32_t kte, int8_t dado){
	int32_t new_address = (int32_t)((address+kte));		//Pegando o endereço 
	int n_rotation = new_address%4;						//Calculando o número de rotações
	int32_t mask = 0xFF;

	mask = mask << (n_rotation*8);						//Rotacionando a mascara para a posicao da half word q sera inserida
	int32_t value = (uint16_t) dado << (n_rotation*8); //Rotacionando o byte para a posicao certa

	mem[(address + kte)/4] &= ~(mask);					//Limpando o conteudo da memoria q o dado sera inserido
	mem[(address + kte)/4] |= value;					//Inserindo o dado	
}