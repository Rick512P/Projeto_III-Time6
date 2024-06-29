#include "../Arquivos-h/inicializadores.h"

instrucao *inicializaMemInst(){
    instrucao *memInst = (instrucao*)malloc(256 * sizeof(instrucao));
    for(int i = 0; i < 256; i++){
        memInst[i].instruc[0] = '\0';
    }
    return memInst;
}

MemoriaDados *inicializaMemDados(){
    MemoriaDados *memDados = (MemoriaDados*)malloc(256 * sizeof(MemoriaDados));
    for(int i = 0; i < 256; i++){
        memDados[i].dados[0] = '\0';
    }
    return memDados;
}

void inicializaRegsPipe(IF *regif, ID *id, EX *ex, MEM *mem, WB *wb){
    

    strcpy(regif->instruc, "\0");
    regif->pc = 0;

    
    strcpy(id->instruc, "\0");
    id->pc = 0;
    id->readData1 = 0;
    id->readData2 = 0;

    
    strcpy(ex->instruc, "\0");
    ex->pc = 0;
    ex->readData1 = 0;
    ex->readData2 = 0;
    ex->aluResult = 0;

    
    strcpy(mem->instruc, "\0");
    mem->pc = 0;
    mem->readData[0] = '0';
    mem->aluResult = 0;

    
    strcpy(wb->instruc, "\0");
    wb->pc = 0;
    wb->aluResult = 0;
}
