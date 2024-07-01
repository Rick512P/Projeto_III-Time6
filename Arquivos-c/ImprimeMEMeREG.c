#include <stdio.h>
#include <stdlib.h>
#include <panel.h>
#include <string.h>
#include "../Arquivos-h/bibliotecas.h"
#include "../Arquivos-h/imprimeSimulador.h"

#define INSTR_MEM_SIZE 256  // Tamanho da memória de instruções (por exemplo)
#define DATA_MEM_SIZE 256   // Tamanho da memória de dados (por exemplo)
#define REG_COUNT 8         // Quantidade de registradores

//retirar variaveis globais depois de tudo estiver funcionando
WINDOW *instr_mem_win;
WINDOW *data_mem_win;
WINDOW *reg_win;
PANEL *instr_mem_panel;
PANEL *data_mem_panel;
PANEL *reg_panel;

typedef struct _PANEL_DATA {
    int hide;
    int start_index; // Índice inicial para mostrar na janela
} PANEL_DATA;

PANEL_DATA panel_datas[3];

void paineis(MemoriaDados *memDados, instrucao *memInst, int *regs) {
    /* Initialize UI */
    int current_panel_index = 0;
    init_ui(memDados, memInst, regs, current_panel_index);

    /* Main loop */
    int ch;
    while ((ch = getch()) != KEY_F(1)) {
        switch (ch) {
            case KEY_DOWN:
                if (panel_datas[current_panel_index].start_index + 15 < DATA_MEM_SIZE - 1) {
                    panel_datas[current_panel_index].start_index += 15;
                    if (current_panel_index == 0) {
                        update_instr_mem_win(memInst, current_panel_index);
                    } else if (current_panel_index == 1) {
                        update_data_mem_win(memDados, current_panel_index);
                    }
                }
                break;

            case KEY_UP:
                if (panel_datas[current_panel_index].start_index - 15 >= 0) {
                    panel_datas[current_panel_index].start_index -= 15;
                    if (current_panel_index == 0) {
                        update_instr_mem_win(memInst, current_panel_index);
                    } else if (current_panel_index == 1) {
                        update_data_mem_win(memDados, current_panel_index);
                    }
                }
                break;

            case KEY_RIGHT:
                toggle_panel(1, &current_panel_index); // Move para o próximo painel
                update_instr_mem_win(memInst, current_panel_index);
                update_data_mem_win(memDados, current_panel_index);
                break;

            case KEY_LEFT:
                toggle_panel(-1, &current_panel_index); // Move para o painel anterior
                update_instr_mem_win(memInst, current_panel_index);
                update_data_mem_win(memDados, current_panel_index);
                break;

            /* Handle other key presses or events as needed */
        }
    }

    endwin();
}

void init_ui(MemoriaDados *memDados, instrucao *memInst, int *regs, int current_panel_index) {
    /* Initialize curses */
    initscr();
    start_color();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);

    /* Initialize all the colors */
    init_pair(1, COLOR_RED, COLOR_BLACK);
    init_pair(2, COLOR_GREEN, COLOR_BLACK);
    init_pair(3, COLOR_BLUE, COLOR_BLACK);
    init_pair(4, COLOR_CYAN, COLOR_BLACK);

    /* Create windows for each memory type */
    int max_y, max_x;
    getmaxyx(stdscr, max_y, max_x);
    instr_mem_win = newwin(max_y - 4, max_x / 3, 2, 2);
    data_mem_win = newwin(max_y - 4, max_x / 3, 2, max_x / 3 + 3);
    reg_win = newwin(max_y - 4, max_x / 3, 2, (max_x / 3) * 2 + 4);
    scrollok(instr_mem_win, TRUE);  // Permite scroll na janela de memória de instrução
    scrollok(data_mem_win, TRUE);   // Permite scroll na janela de memória de dados

    /* Create panels for each window */
    instr_mem_panel = new_panel(instr_mem_win);
    data_mem_panel = new_panel(data_mem_win);
    reg_panel = new_panel(reg_win);

    panel_datas[0].hide = FALSE;
    panel_datas[1].hide = FALSE;
    panel_datas[2].hide = FALSE;

    set_panel_userptr(instr_mem_panel, &panel_datas[0]);
    set_panel_userptr(data_mem_panel, &panel_datas[1]);
    set_panel_userptr(reg_panel, &panel_datas[2]);

    /* Show panels */
    update_instr_mem_win(memInst, current_panel_index);
    update_data_mem_win(memDados, current_panel_index);
    update_reg_win(regs);

    /* Update the stacking order */
    update_panels();
    doupdate();

    /* Show instructions */
    attron(COLOR_PAIR(4));
    mvprintw(max_y - 3, 0, "Setas horizontais para pular entre paineis");
	mvprintw(max_y - 4, 0, "Setas verticais para scrollar");
	mvprintw(max_y - 5, 0, "F1 para sair");
    attroff(COLOR_PAIR(4));
    refresh();
}

void update_instr_mem_win(instrucao *instr_memory, int current_panel_index) {
    werase(instr_mem_win);
    box(instr_mem_win, 0, 0);
    mvwprintw(instr_mem_win, 1, 1, "Memoria de Instrucoes:");

    int start_idx = panel_datas[current_panel_index].start_index;
    // Exemplo: Mostrar até 15 instruções por vez, a partir do índice start_idx
    int max_display = 15;
    for (int i = start_idx; i < start_idx + max_display && i < INSTR_MEM_SIZE; ++i) {
        mvwprintw(instr_mem_win, i - start_idx + 2, 1, "[%s]", instr_memory[i].instruc);
    }

    wrefresh(instr_mem_win);
}

void update_data_mem_win(MemoriaDados *memDados, int current_panel_index) {
    werase(data_mem_win);
    box(data_mem_win, 0, 0);
    mvwprintw(data_mem_win, 1, 1, "Memoria de Dados");

    // Calcular o índice inicial baseado no painel atual
    int index_start = panel_datas[current_panel_index].start_index;

    // Exemplo: Mostrar 15 bytes da memória de dados a partir do índice inicial
    for (int i = index_start; i < index_start + 15 && i < DATA_MEM_SIZE; ++i) {
        mvwprintw(data_mem_win, i - index_start + 2, 1, "[%s]", memDados[i].dados);
    }

    wrefresh(data_mem_win);
}

void update_reg_win(int *regs) {
    werase(reg_win);
    box(reg_win, 0, 0);
    mvwprintw(reg_win, 1, 1, "Registradores:");

    // Exemplo: Mostrar os valores dos registradores
    for (int i = 0; i < REG_COUNT; ++i) {
        mvwprintw(reg_win, i + 2, 1, "R%d: [%d]", i, regs[i]);
    }

    wrefresh(reg_win);
}

void toggle_panel(int dir, int *current_panel_index) {
    PANEL *active_panel = panel_below(NULL);

    if (dir > 0) {
        if (active_panel == reg_panel) {
            top_panel(instr_mem_panel);
            *current_panel_index = 0;
        }
        else if (active_panel == instr_mem_panel) {
            top_panel(data_mem_panel);
            *current_panel_index = 1;
        }
        else if (active_panel == data_mem_panel) {
            top_panel(reg_panel);
            *current_panel_index = 2;
        }
    } else {
        if (active_panel == reg_panel) {
            top_panel(data_mem_panel);
            *current_panel_index = 1;
        }
        else if (active_panel == data_mem_panel) {
            top_panel(instr_mem_panel);
            *current_panel_index = 0;
        }
        else if (active_panel == instr_mem_panel) {
            top_panel(reg_panel);
            *current_panel_index = 2;
        }
    }

    // Atualiza a tela para refletir as mudanças nos painéis
    update_panels();
    doupdate();
}

