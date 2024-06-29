
#include "../Arquivos-h/bibliotecas.h"
#include "../Arquivos-h/Memoria_Dados.h"

void escreveDado(MemoriaDados *mem, int contador, char *valor) {

    if (contador >= 0 && contador < 256) {
        strcpy(mem[contador].dados, valor);
    } else {
        fprintf(stderr, "Tentativa de escrita fora dos limites da memória no endereço %d\n", contador);
    }
}

void imprimeDados(MemoriaDados *memDados, int tamLinhas){
	for (int i = 0; i < 256; i++) {
		printf("Dado armazenado no endereco %d da MD: [%s]\n", i, memDados[i].dados);
	}
}

void escreverArquivoMemoria(MemoriaDados *memDados) {
	FILE *arquivo;
	arquivo = fopen("./DATA.dat", "w");
	
	if(arquivo==NULL){
		fprintf(stderr, "Erro ao abrir o arquivo DATA.dat\n");
	}
	
	for (int i=0; i<256; i++){
		fprintf(arquivo, "%s\n", memDados[i].dados);
	}
	
	fclose(arquivo);
	printf("\nArquivo criado com sucesso!\n\n");
}
