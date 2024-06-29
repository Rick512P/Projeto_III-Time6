#ifndef backstep_h
#define backstep_h

#include "bibliotecas.h"


descPilha *inicializaBackup();
NodoPilha *inicializaNodo(Backup *musica);
descPilha *PUSH(descPilha *pilha, NodoPilha *nodo);
descPilha *POP(descPilha *pilha);
Backup *ColetaTudo(int *regs, MemoriaDados *memDados, IF *regif, ID *id, EX *ex, MEM *mem, WB *wb, Sinais *sinal, Assembly *AssemblyInst, int *program_counter);
descPilha *Realoca(descPilha *pilha, int *regs, MemoriaDados *memDados, IF *regif, ID *id, EX *ex, MEM *mem, WB *wb, Sinais *sinal, Assembly *AssemblyInst, int *program_counter);

#endif