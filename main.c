#include "memory.h"
#include <stdlib.h>

int main(int argc, char *argv[])
{
	char c;
	int mem1, mem2;
	int32_t aux;
	FILE *ptr;
	//Inicialização do .DATA
	ptr = fopen("data","rb");  
	int i = 0x2000/4;
	while(!feof(ptr)){
		fread(&aux,sizeof(aux),1,ptr);
		mem[i] = aux;
		i++;
	}
	fclose(ptr);
	//Inicialização do .TEXT
	ptr = fopen("code","rb");  
	i = 0;
	while(!feof(ptr)){
		fread(&aux,sizeof(aux),1,ptr);
		mem[i] = aux;
		i++;
	}
	fclose(ptr);

	initialize();

	printf("Escreva 'r' para rodar o programa e 's' para dar step\n");
	do{
		scanf("%c",&c);
	}while((c != 'r') && (c != 's'));

	if(c == 'r'){
		run();
	}
	else if(c == 's'){
		printf("Pressione qualquer tecla para ir para o próximo passo\n");
		while(1){
			getchar();
			step();
			if(encerra == 1){
				break;
			}
		}	
	}
	
	printf("Deseja imprimir o conteúdo dos registradores? (s/n)\n");
	do{
		scanf("%c",&c);
	}while((c != 's') && (c != 'n'));

	if(c == 's'){
		printf("Deseja imprimir no formato hexadecimal(h) ou decimal(d)?\n");
		do{
			scanf("%c",&c);
		}while((c != 'h') && (c != 'd'));

		if(c == 'h'){
			dump_reg('h');
		}
		else{
			dump_reg('d');
		}
	}

	printf("Deseja imprimir o conteúdo da memória? (s/n)\n");
	do{
		scanf("%c",&c);
	}while((c != 's') && (c != 'n'));

	if(c == 's'){

		printf("Insira o intervalo desejado no formato (d d):\n");
		scanf("%d %d",&mem1,&mem2);

		printf("Deseja imprimir no formato hexadecimal(h) ou decimal(d)?\n");
		do{
			scanf("%c",&c);
		}while((c != 'h') && (c != 'd'));

		if(c == 'h'){
			dump_mem(mem1, mem2, 'h');
		}
		else{
			dump_mem(mem1, mem2, 'd');
		}
	}
 	return 0;
}

