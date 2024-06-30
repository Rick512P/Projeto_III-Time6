#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "bibliotecas.h"
#include "registradores.h"
#include "decode.h"
#include "PC.h"
#include "ULA.h"
#include "imprimeSimulador.h"
#include "menu.h"
#include "sinais.h"

int controller(int op, int NumeroLinhas, int *regs, instrucao *memInst, MemoriaDados *memDados, int *program_counter, type_instruc *instrucoesDecodificadas, IF *regif, ID *id, EX *ex, MEM *mem, WB *wb, Sinais *sinal, int ProxEtapa);

#endif