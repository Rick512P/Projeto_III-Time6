#ifndef __MENU_H_
#define __MENU_H_

#include "Assembly.h"
//#include "ASSEMBLYTERMINAL.h"
#include "backstep.h"
#include "bibliotecas.h"
#include "carregaMemoriaDados.h"
#include "controller.h"
//#include "DATA.h"
#include "decode.h"
#include "inicializadores.h"
//#include "INST.h"
#include "Memoria_Dados.h"
#include "parser.h"
#include "PC.h"
#include "registradores.h"
#include "sinais.h"
#include "Terminal.h"
#include "ULA.h"


int main();
int menu();
void freeALL(int *regs, Assembly *AssemblyInst, instrucao *memInst, MemoriaDados *memDados, type_instruc *instrucoesDecodificadas, IF *regif, ID *id, EX *ex, MEM *mem, WB *wb, Sinais *sinal);

#endif