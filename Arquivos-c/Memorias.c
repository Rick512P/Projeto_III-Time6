
#include "../Arquivos-h/bibliotecas.h"
#include "../Arquivos-h/Memoria_Dados.h"

void escreveDado(MemoriaDados *mem, int contador, char *valor) {

    if (contador >= 0 && contador < 256) {
        strcpy(mem[contador].dados, valor);
    } else {
        fprintf(stderr, "Tentativa de escrita fora dos limites da memória no endereço %d\n", contador);
    }
}

void imprimeDados(MemoriaDados *memDados)// Função Resposável NPor Exibir a Memória de Dados
{
	puts(" ");
	puts("===Memória de Dados===");
	for (int i = 0; i < 256; i++) {
		if (i<10)
			printf("Dado armazenado no endereco 00%d da MD: [%s]\n", i, memDados[i].dados);
		else if (i<100)
			printf("Dado armazenado no endereco 0%d da MD: [%s]\n", i, memDados[i].dados);
		else
			printf("Dado armazenado no endereco %d da MD: [%s]\n", i, memDados[i].dados);
	
	}
}

void imprimeMemInstruc(instrucao *memInst)// Função Resposável NPor Exibir a Memória de Instruções
{
	puts(" ");
	puts("===Memória de Instruções===");
    for (int i=0; i<256;i++){
		if (i<10)
			printf("Instrucao armazenada no endereco 00%d da MI: [%s]\n", i, memInst[i].instruc);
		else if (i<100)
			printf("Instrucao armazenada no endereco 0%d da MI: [%s]\n", i, memInst[i].instruc);
		else
			printf("Instrucao armazenada no endereco %d da MI: [%s]\n", i, memInst[i].instruc);
	}
    printf("\n");
}


void escreverArquivoMemoria(MemoriaDados *memDados) {
	FILE *arquivo;
	arquivo = fopen("./DATA.dat", "w");
	
	if(arquivo==NULL){
		system("clear");
		fprintf(stderr, "Erro ao abrir o arquivo DATA.dat\n");
		sleep(2);
	}
	
	for (int i=0; i<256; i++){
		fprintf(arquivo, "%s\n", memDados[i].dados);
	}
	
	fclose(arquivo);
	printf("\nArquivo criado com sucesso!\n\n");
}
