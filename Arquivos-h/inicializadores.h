#ifndef inicializa_h
#define inicializa_h

#include "./bibliotecas.h"
#include "./bin_to_decimal.h"

instrucao *inicializaMemInst();
MemoriaDados *inicializaMemDados();
void inicializaRegsPipe(IFID *ifid, IDEX *idex, EXMEM *exmem, MEMWB *memwb);
Sinais *inicializaSinais();
Sinais *AddSinais(type_instruc inst, Sinais *sinal);

#endif