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

PipeRegisters inicializaRegsPipe() {
    PipeRegisters pipes;

    pipes.regif = (IF*)malloc(sizeof(IF));
    pipes.id = (ID*)malloc(sizeof(ID));
    pipes.ex = (EX*)malloc(sizeof(EX));
    pipes.mem = (MEM*)malloc(sizeof(MEM));
    pipes.wb = (WB*)malloc(sizeof(WB));

    // Inicializa os structs
    strcpy(pipes.regif->instruc, "\0");
    pipes.regif->pc = 0;

    strcpy(pipes.id->instruc, "\0");
    pipes.id->pc = 0;
    pipes.id->readData1 = 0;
    pipes.id->readData2 = 0;
    pipes.id->sinal = NULL;

    strcpy(pipes.ex->instruc, "\0");
    pipes.ex->pc = 0;
    pipes.ex->sinal = NULL;
    pipes.ex->readData1 = 0;
    pipes.ex->readData2 = 0;
    pipes.ex->aluResult = 0;

    strcpy(pipes.mem->instruc, "\0");
    pipes.mem->pc = 0;
    pipes.mem->readData[0] = '0';
    pipes.mem->aluResult = 0;
    pipes.mem->sinal = NULL;

    strcpy(pipes.wb->instruc, "\0");
    pipes.wb->pc = 0;
    pipes.wb->aluResult = 0;
    pipes.wb->sinal = NULL;

    return pipes;
}
