#include "../Arquivos-h/registradores.h"



int retornoRegs(int *regs, char posicao[]){
    int indice;
        indice = bin_to_decimal(posicao);
        if(indice >= 8 || indice<0){
            system("clear");
            fprintf(stderr, "ERRO. Nenhum Registrador encontrado.");
            sleep(2);
            return -1;
        }
        return regs[indice];
}

int escritaRegistradores(int *regs, int valor, char posicao[]){
       int indice;
        indice = bin_to_decimal(posicao);
        if(indice >= 8){
            system("clear");
            fprintf(stderr, "ERRO. Nenhum Registrador encontrado.");
            sleep(2);
            return -1;
        }
            
        regs[indice] = valor;

            
}

void imprimeRegistradores(int *regs){
    printf("\n");
    for(int i = 0; i<8;i++){
            printf("Registrador $%d: %d\n", i, regs[i]);
    }
    printf("\n");
}



