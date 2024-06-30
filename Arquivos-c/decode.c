#include "../Arquivos-h/decode.h"

type_instruc decoder(instrucao *memInst, int contador){
    int i = contador;
    type_instruc traducao;
    memInst[i].instruc[16] = '\0';
    
    strncpy(traducao.opcode, memInst[i].instruc, 5); //copia para .opcode os 5 primeiros caracteres de memInst[i].instruc
    traducao.opcode[4] = '\0';
    
    if (strcmp(traducao.opcode, "0000") == 0){ //SE VERDADEIRO, SABEMOS QUE É DO TIPO R, PORTANTO ADICIONAREMOS O FUNCT
        strncpy(traducao.funct, memInst[i].instruc + 13, 4); // Escreve o funct
        traducao.funct[3] = '\0';
        strncpy(traducao.rs, memInst[i].instruc + 4, 4); //strncpy copiara 3 caracteres a partir dos 4 caracteres inciais.
        traducao.rs[3] = '\0';
        strncpy(traducao.rt, memInst[i].instruc + 7, 4);
        traducao.rt[3] = '\0';
        strncpy(traducao.rd, memInst[i].instruc + 10, 4);
        traducao.rd[3] = '\0';
        
        return traducao;
    }

    else if (strcmp(traducao.opcode, "0010") == 0){//SE NAO, SABEREMOS QUE SERÁ TIPO J, PORTANDO ADICIONAREMOS APENAS O ADDR
        strncpy(traducao.addr, memInst[i].instruc + 9, 8); // Escreve o funct
        traducao.addr[7] = '\0';
        
        return traducao;
    }

    else { //TIPO I
        strncpy(traducao.imm, memInst[i].instruc + 10, 7); // Escreve o IMMEDIATE de 6 bits
        traducao.imm[6] = '\0';
        strncpy(traducao.rt, memInst[i].instruc + 7, 4);
        traducao.rt[3] = '\0';
        strncpy(traducao.rs, memInst[i].instruc + 4, 4);
        traducao.rs[3] = '\0';
        return traducao;
    }


    
    
    
}