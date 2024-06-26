#ifndef __MENU_H_
#define __MENU_H_

#include "bibliotecas.h"

int center_x(int width, int text_width); //Responsável por centralizar o elemento
int print_menu(); //Responsável por exibir o menu

//Como usar para exibir em terminal:
/*
    initscr(); // Inicia o modo ncurses
    clear();
    printw("Texto: %s\n", variavel_Escolhida);
    printw("\nPressione qualquer tecla para voltar ao menu...");
    refresh();
    getch(); // Aguarda uma entrada para voltar ao menu
    endwin(); // Finaliza o modo ncurses
*/

#endif