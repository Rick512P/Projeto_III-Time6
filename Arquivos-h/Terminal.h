#ifndef __TERMINAL_H_
#define __TERMINAL_H_

#include "bibliotecas.h"
#include "INST.h"


struct TELA{
    WINDOW *header;
    WINDOW *menu;
    WINDOW *content;
    WINDOW *footer;
};

// Definições das funções para inicializar e finalizar o terminal
char terminal(int *program_counter, instrucao *memInst, int tamLinhas, type_instruc *instrucoesDecodificadas, int *regs, char *instrucIF, char *instrucID, char *instrucEX, char *instrucMEM, char *instrucWB);
void inicializaTerminal();
void finalizaTerminal();
void desenhaTelaInicial(struct TELA *tela, float *altura, float *largura);
void desenhaMenu(struct TELA *tela, int *program_counter, float largura, int *regs, int tamLinhas, int r, int I, int j, int instLogic, int instAri, int instDesvio, int instAcessoMem, char *instrucIF, char *instrucID, char *instrucEX, char *instrucMEM, char *instrucWB);
void imprimeEstatisticasTerminal(struct TELA *tela, int *program_counter, float largura, instrucao *memInst, int tamLinhas, type_instruc *instrucoesDecodificadas, int *regs, char *instrucIF, char *instrucID, char *instrucEX, char *instrucMEM, char *instrucWB);

#endif