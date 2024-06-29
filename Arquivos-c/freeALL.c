#include "../Arquivos-h/bibliotecas.h"
#include "../Arquivos-h/menu.h"

void freeALL(int *regs, Assembly *AssemblyInst, instrucao *memInst, MemoriaDados *memDados, type_instruc *instrucoesDecodificadas, IF *regif, ID *id, EX *ex, MEM *mem, WB *wb, Sinais *sinal){
    for(int i=0;i<8;i++){
                regs[i] = 0;
    }
    if(memInst != NULL){
        free(memInst);
        memInst = NULL;
    }
    if(memDados != NULL){
        free(memDados);
        memDados = NULL;
    }
    if(instrucoesDecodificadas != NULL){
        free(instrucoesDecodificadas);
        instrucoesDecodificadas = NULL;
    }
    if(regif != NULL){
        free(regif);
        regif = NULL;
    }
    if(id != NULL){
        free(id);
        id = NULL;
    }
    if(ex != NULL){
        free(ex);
        ex = NULL;
    }
    if(mem != NULL){
        free(mem);
        mem = NULL;
    }
    if(wb != NULL){
        free(wb);
        wb = NULL;
    }
    if(sinal != NULL){
        free(sinal);
        sinal = NULL;
    }
    if(AssemblyInst != NULL){
        free(AssemblyInst);
        AssemblyInst = NULL;
    }
}