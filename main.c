#include "memory.h"
#include <stdlib.h>

int main(int argc, char *argv[])
{
	int32_t aux;
	FILE *ptr;
	//.DATA
	ptr = fopen("data","rb");  // r for read, b for binary
	int i = 0x2000/4;
	while(!feof(ptr)){
		fread(&aux,sizeof(aux),1,ptr);
		mem[i] = aux;
		i++;
	}
	fclose(ptr);
	//.TEXT
	ptr = fopen("code","rb");  // r for read, b for binary
	i = 0;
	while(!feof(ptr)){
		fread(&aux,sizeof(aux),1,ptr);
		mem[i] = aux;
		i++;
	}
	fclose(ptr);



	initialize();
	while(1){
		//getchar();
		//printf("mem = %x\n",mem[pc/4]);
		//printf("\nimm21 = %x\n",imm21);
		step();
		if(encerra == 1){
			break;
		}
	}

	//dump_mem(0, 10);
 	return 0;
}

