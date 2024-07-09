#ifndef __INST_H_
#define __INST_H_

#include "bibliotecas.h"

struct TELAINST{
    WINDOW *header;
    WINDOW *instrucoes;
    WINDOW *footer;
};


// Definições das funções para inicializar e finalizar o terminal
int INST(instrucao *memInst);
void inicializaInst();
void finalizaInst();
void desenhaTelaINST(float *altura, float *largura, struct TELAINST *telainst);
void desenhaIsntruct(float largura, instrucao *memInst, struct TELAINST *telainst);

#endif