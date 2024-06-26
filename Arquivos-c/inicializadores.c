#include "../Arquivos-h/inicializadores.h"

instrucao *inicializaMemInst(){
    instrucao *memInst = malloc(256 * sizeof(instrucao));
    for(int i = 0; i < 256; i++){
        memInst[i].instruc[0] = '\0';
    }
    return memInst;
}

MemoriaDados *inicializaMemDados(){
    MemoriaDados *memDados = malloc(256 * sizeof(MemoriaDados));
    for(int i = 0; i < 256; i++){
        memDados[i].dados[0] = '\0';
    }
    return memDados;
}

registradoresPipeline *inicializaRegsPipe(registradoresPipeline *RegsPipe){
    RegsPipe->registradorBI = 0;
    RegsPipe->registradorDI = 0;
    RegsPipe->registradorEX = 0;
    RegsPipe->registradorMEM = 0;
    RegsPipe->registradorER = 0;
    return RegsPipe;
}