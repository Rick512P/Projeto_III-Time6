#ifndef __imprimeSimulador_h
#define __imprimeSimulador_h

#include "decode.h"
#include "registradores.h"
#include "bibliotecas.h"
#include "menu.h"


void imprimeSimulador(int tamLinhas, type_instruc *instrucoesDecodificadas, instrucao *memInst);
void imprimeEstatisticas(instrucao *memInst, int tamLinhas, type_instruc *instrucoesDecodificadas);
void imprimeRegsPipeline(IF *regif, ID *id, EX *ex, MEM *mem, int etapa);
void update_instr_mem_win(instrucao *instr_memory, int current_panel_index);
void update_data_mem_win(MemoriaDados *memDados, int current_panel_index);
void update_reg_win(int *regs);
void paineis(MemoriaDados *memDados, instrucao *memInst, int *regs);
void init_ui(MemoriaDados *memDados, instrucao *memInst, int *regs, int r);
void toggle_panel(int dir, int *current_panel_index);



#endif