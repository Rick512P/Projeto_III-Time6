#include "../Arquivos-h/backstep.h"

descPilha *inicializaBackup(){
    descPilha *pilha = malloc(sizeof(descPilha));
    pilha->Topo = NULL;
    pilha->Fundo = pilha->Topo;
    pilha->tamanho = 0;
    return pilha;
}

NodoPilha *inicializaNodo(Backup *backup){
    NodoPilha *nodo = malloc(sizeof(NodoPilha));
    nodo->info = backup;
    nodo->prox = NULL;
    return nodo;
}

descPilha *PUSH(descPilha *pilha, NodoPilha *nodo){
    if(pilha->Topo == NULL){
        pilha->Topo = nodo;
        pilha->Fundo = nodo;
    }
    else{
        nodo->prox = pilha->Topo;
        pilha->Topo = nodo;
    }
    pilha->tamanho++;
    //printf("\nItem adicionado ao topo da lista\n");
    return pilha;
}

descPilha *POP(descPilha *pilha){
    if(pilha->Topo == NULL){
        printf("Lista vazia");
        return NULL;
    }
    else{
        NodoPilha *aux = pilha->Topo;
        pilha->Topo = aux->prox;
        free(aux->info);
        free(aux);
        pilha->tamanho--;
        if(pilha->tamanho == 0){
            pilha->Fundo = NULL;
        }
        //printf("\nItem retirado do topo\n");
        return pilha;
    }
}

Backup *ColetaTudo(int *regs, MemoriaDados *memDados, IF *regif, ID *id, EX *ex, MEM *mem, WB *wb, Sinais *sinal, Assembly *AssemblyInst, int *program_counter, int *Etapa) {
    Backup *coleta = (Backup*) malloc(sizeof(Backup));

    for (int i = 0; i < 8; i++) {
        coleta->regs[i] = regs[i];
    }

    coleta->regif = *regif;
    coleta->id = *id;
    coleta->ex = *ex;
    coleta->mem = *mem;
    coleta->wb = *wb;
    coleta->memDados = *memDados;
    coleta->AssemblyInst = *AssemblyInst;
    if(sinal != NULL){
        coleta->sinal = *sinal;
    }
    coleta->PC = *program_counter;
    coleta->Etapa = *Etapa;

    return coleta;
}

descPilha *Realoca(descPilha *pilha, int *regs, MemoriaDados *memDados, IF *regif, ID *id, EX *ex, MEM *mem, WB *wb, Sinais *sinal, Assembly *AssemblyInst, int *program_counter, int *Etapa){
    if (pilha->Topo == NULL) {
        return pilha;
    }


    NodoPilha *aux = pilha->Topo;
    pilha->Topo = aux->prox;


    for(int i = 0; i < 8; i++){
        regs[i] = aux->info->regs[i];
    }
    *memDados = aux->info->memDados;
    *regif = aux->info->regif;
    *id = aux->info->id;
    *ex = aux->info->ex;
    *mem = aux->info->mem;
    *wb = aux->info->wb;
    if(sinal != NULL){
        *sinal = aux->info->sinal;
    }
    *AssemblyInst = aux->info->AssemblyInst;
    *program_counter = aux->info->PC;
    *Etapa = aux->info->Etapa;

    free(aux);
    pilha->tamanho--;

    if (pilha->tamanho == 0) {
        pilha->Fundo = NULL;
    }

    return pilha;
}
