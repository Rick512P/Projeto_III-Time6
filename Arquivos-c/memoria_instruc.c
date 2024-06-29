#include "../Arquivos-h/memoria_instruc.h"

void imprimeMemInstruc(instrucao *memoriaInst, int tamLinhas){
    for (int i=0; i<tamLinhas;i++){
                printf("Instrucao %d: %s\n\n", i, memoriaInst[i].instruc);
            }
}


    

