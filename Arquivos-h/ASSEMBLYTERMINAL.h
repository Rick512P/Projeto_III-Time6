#ifndef __TermAssem_H_
#define __TermAssem_H_

#include "bibliotecas.h"
#include "INST.h"

typedef struct {
    char assembly[256];
} AssemblyASM;


// Definições das funções para inicializar e finalizar o terminal
int ASSEMBLYTERMINAL(Assembly **A);
void inicializaASM();
void finalizaASM();
void desenhaTelaASM(float *altura, float *largura);
void desenhaAssembly(float largura, Assembly *A);

#endif