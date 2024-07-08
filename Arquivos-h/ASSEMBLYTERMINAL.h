#ifndef __TermAssem_H_
#define __TermAssem_H_

#include "bibliotecas.h"

typedef struct {
    char assembly[256];
} AssemblyASM;

AssemblyASM ASSEMBLYASM[32];

// Definições das funções para inicializar e finalizar o terminal
int ASSEMBLYTERMINAL();
void inicializaASM();
void finalizaASM();
void desenhaTelaASM(float *altura, float *largura);
void desenhaAssembly(float largura);

#endif