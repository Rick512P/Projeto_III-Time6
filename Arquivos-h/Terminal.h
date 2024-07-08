#ifndef __TERMINAL_H_
#define __TERMINAL_H_

#include "bibliotecas.h"

// Definições das funções para inicializar e finalizar o terminal
char terminal();
void inicializaTerminal();
void finalizaTerminal();
void desenhaTelaInicial(float *altura, float *largura);
void desenhaMenu(float largura, float altura);

#endif