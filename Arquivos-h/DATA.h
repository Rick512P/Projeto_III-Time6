#ifndef __DATA_H_
#define __DATA_H_

#include "bibliotecas.h"
#include "INST.h"

// Definições das funções para inicializar e finalizar o terminal
int DATA(MemoriaDados **mem);
void inicializaData();
void finalizaData();
void desenhaTelaDATA(float *altura, float *largura);
void desenhaData(float largura, MemoriaDados *mem);

#endif