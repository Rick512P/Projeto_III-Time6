#ifndef inicializa_h
#define inicializa_h

#include "./bibliotecas.h"

instrucao *inicializaMemInst();
MemoriaDados *inicializaMemDados();
void inicializaRegsPipe(IFID *ifid, IDEX *idex, EXMEM *exmem, MEMWB *memwb);

#endif