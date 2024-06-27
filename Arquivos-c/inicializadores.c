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

void inicializaRegsPipe(IFID *ifid, IDEX *idex, EXMEM *exmem, MEMWB *memwb){
    ifid->instr = '\0';
    ifid->pc = 0;

    idex->instr = "\0";
    idex->pc = 0;
    idex->readData1 = 0;
    idex->readData2 = 0;
    idex->signExtend = "\0";

    exmem->instr = "\0";
    exmem->pc = 0;
    exmem->readData2 = 0;
    exmem->aluResult = 0;

    memwb->instr = "\0";
    memwb->pc = 0;
    memwb->readData = 0;
    memwb->aluResult = 0;
}