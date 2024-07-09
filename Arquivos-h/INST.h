#ifndef __INST_H_
#define __INST_H_

#include "bibliotecas.h"
extern TELA tela;
// Definições das funções para inicializar e finalizar o terminal
int INST(instrucao **memInst);
void inicializaInst();
void finalizaInst();
void desenhaTelaINST(float *altura, float *largura);
void desenhaIsntruct(float largura, instrucao *memInst);

#endif