#ifndef __TermAssem_H_
#define __TermAssem_H_

#include "bibliotecas.h"
#include "INST.h"

struct TELAASSEMBLY{
    WINDOW *header;
    WINDOW *ASM;
    WINDOW *footer;
};


// Definições das funções para inicializar e finalizar o terminal
int ASSEMBLYTERMINAL(Assembly *A, int tamLinhas);
void inicializaASM();
void finalizaASM();
void desenhaTelaASM(struct TELAASSEMBLY *telaAssembly, float *altura, float *largura);
void desenhaAssembly(struct TELAASSEMBLY *telaAssembly, float largura, Assembly *A, int tamLinhas);

#endif