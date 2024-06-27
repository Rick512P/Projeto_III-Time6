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
    strcpy(ifid->instruc, "\0");
    ifid->pc = 0;

    strcpy(idex->instruc, "\0");
    idex->pc = 0;
    idex->readData1 = 0;
    idex->readData2 = 0;
    strcpy(idex->signExtend, "\0");

    strcpy(exmem->instruc, "\0");
    exmem->pc = 0;
    exmem->readData2 = 0;
    exmem->aluResult = 0;

    strcpy(memwb->instruc, "\0");
    memwb->pc = 0;
    strcpy(memwb->readData, "\0");
    memwb->aluResult = 0;
}

Sinais *inicializaSinais(){
    Sinais *sinal = malloc(sizeof(Sinais));
    sinal->address = -1; //0 A 64
    sinal->imm = -1; //0 A 32
    sinal->RS = -1; //0 A 7
    sinal->RT= -1;// 0 A 7
    sinal->RD = -1; //0 A 7
    sinal->funct = -1; //0 A 7
    sinal->tipo = -1; //TIPO R = 0 ; TIPO J = 1; ADDI = 2; LW = 3; SW = 4; BEQ = 5
    return sinal;
}

Sinais *AddSinais(type_instruc inst, Sinais *sinal){
    if(strcmp(inst.opcode, "0000") == 0){//TIPO R
        sinal->tipo = 0;
        sinal->RS = bin_to_decimal(inst.rs);
        sinal->RT = bin_to_decimal(inst.rt);
        sinal->RD = bin_to_decimal(inst.rd);
        sinal->funct = bin_to_decimal(inst.funct);
        sinal->imm = -1;
        sinal->address = -1;
        return sinal;
    } 

    else if(strcmp(inst.opcode, "0100") == 0){//ADDI
        sinal->tipo = 2;
        sinal->RS = bin_to_decimal(inst.rs);
        sinal->RT = bin_to_decimal(inst.rt);
        sinal->RD = bin_to_decimal(inst.rd);
        sinal->imm = bin_to_decimal(inst.imm);
        sinal->address = -1;
        sinal->funct = -1;
        return sinal;
    }

    else if(strcmp(inst.opcode, "1011") == 0){//LW
        sinal->tipo = 3;
        sinal->RS = bin_to_decimal(inst.rs);
        sinal->RT = bin_to_decimal(inst.rt);
        sinal->imm = bin_to_decimal(inst.imm);
        sinal->address = -1;
        sinal->funct = -1;
        return sinal;
    }

    else if(strcmp(inst.opcode, "1111") == 0){//SW
        sinal->tipo = 4;
        sinal->RS = bin_to_decimal(inst.rs);
        sinal->RT = bin_to_decimal(inst.rt);
        sinal->imm = bin_to_decimal(inst.imm);
        sinal->address = -1;
        sinal->funct = -1;
        return sinal;
    }

    else if(strcmp(inst.opcode, "1000") == 0){//BEQ
        sinal->tipo = 5;
        sinal->RS = bin_to_decimal(inst.rs);
        sinal->RT = bin_to_decimal(inst.rt);
        sinal->imm = bin_to_decimal(inst.imm);
        sinal->address = -1;
        sinal->funct = -1;
        return sinal;
    }

    else if(strcmp(inst.opcode, "0010") == 0){//J
        sinal->tipo = 1;
        sinal->RS = -1;
        sinal->RT = -1;
        sinal->imm = -1;
        sinal->address = bin_to_decimal(inst.addr);
        sinal->funct = -1;
        return sinal;
    }
}