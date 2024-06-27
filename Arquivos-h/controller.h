#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "bibliotecas.h"
#include "memoria_instruc.h"
#include "registradores.h"
#include "decode.h"
#include "PC.h"
#include "ULA.h"
#include "imprimeSimulador.h"
#include "menu.h"

int controller(int op, int *StateForBack, int NumeroLinhas, int *regs, instrucao *memInst, MemoriaDados *memDados, int *program_counter, type_instruc *instrucoesDecodificadas, IFID *ifid, IDEX *idex, EXMEM *exmem, MEMWB *memwb, Sinais *sinal, int ProxEtapa);
int backstep(int BackAux, int *StateForBack, int NumeroLinhas, int *regs, instrucao *memInst, MemoriaDados *memDados, int *program_counter, type_instruc *instrucoesDecodificadas, IFID *ifid, IDEX *idex, EXMEM *exmem, MEMWB *memwb, Sinais *sinal, int ProxEtapa)

#endif