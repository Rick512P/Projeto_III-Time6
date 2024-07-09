#ifndef __DATA_H_
#define __DATA_H_

#include "bibliotecas.h"
#include "INST.h"

struct TELADATA {
    WINDOW *header;
    WINDOW *dados;
    WINDOW *footer;
};

// Definições das funções para inicializar e finalizar o terminal
int DATA(MemoriaDados *mem);
void inicializaData();
void finalizaData();
void desenhaTelaDATA(struct TELADATA *teladata, float *altura, float *largura);
void desenhaData(struct TELADATA *teladata, float largura, MemoriaDados *mem);

#endif