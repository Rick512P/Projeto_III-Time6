#ifndef __PARSER_H__
#define __PARSER_H__


#include "bibliotecas.h"




int parser(instrucao *memInst, int *tamanho_linhas);
void remove_newline(char *line);


#endif