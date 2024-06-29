#ifndef BIBLIOTECAS_H
#define BIBLIOTECAS_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
//#include <ncurses.h><-LINUX
#include <ncurses/ncurses.h>//<-WINDOWS
//#include <menu.h>


//instrucao da memoria de instruções
typedef struct{
    char instruc[17];

}instrucao;


typedef struct{
  int tipo;
  int RS;
  int RT;
  int RD;
  int funct;
  int imm;
  int address;
}Sinais;

//intrucao traduzida
typedef struct{
    char opcode[5];

    //TIPO R OU I
    char rs [4];
    char rt [4];

    //TIPO R
    char rd [4];
    char funct [4];

    //TIPO I
    char imm [7];

    //TIPO J
    char addr[8];

}type_instruc;


//tamanho da memoria de dados
typedef struct {
    char dados[9];
} MemoriaDados;


//instruções em assembly
typedef struct{
  char InstructsAssembly[30];
} Assembly;


typedef struct {
    unsigned int pc;// Contador de programa (program counter)
    char instruc[17];// Instrução buscada
} IF;


typedef struct {
    unsigned int pc;
    char instruc[17];
    int readData1; // Valor lido do registrador rs
    int readData2; // Valor lido do registrador rt
} ID;

typedef struct {
    unsigned int pc; 
    char instruc[17];
    int aluResult; // Resultado da ALU
    int readData1;
    int readData2; // Valor lido do registrador rt (usado para operações de memória)
} EX;

typedef struct {
    unsigned int pc; 
    char instruc[17];
    char readData[9];  // Dados lidos da memória
    int aluResult; // Resultado da ALU (se não for operação de memória)
} MEM;

typedef struct {
    unsigned int pc; 
    char instruc[17];
    int aluResult; // Resultado da ALU (se não for operação de memória)
} WB;



#endif
