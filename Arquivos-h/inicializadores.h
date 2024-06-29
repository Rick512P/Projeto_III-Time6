#ifndef inicializa_h
#define inicializa_h

#include "bibliotecas.h"
#include "bin_to_decimal.h"

instrucao *inicializaMemInst();
MemoriaDados *inicializaMemDados();
void inicializaRegsPipe(IF *regif, ID *id, EX *ex, MEM *mem, WB *wb);

#endif