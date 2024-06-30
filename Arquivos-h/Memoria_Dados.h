#ifndef MEMORIA_DADOS_H
#define MEMORIA_DADOS_H


#include "bibliotecas.h"

void escreveDado(MemoriaDados *mem, int contador, char *valor);
void imprimeDados(MemoriaDados *memDados);
void imprimeMemInstruc(instrucao *memInst);
void escreverArquivoMemoria(MemoriaDados *memDados);


#endif
