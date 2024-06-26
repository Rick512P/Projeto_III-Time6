#include "../Arquivos-h/Terminal.h"


// Função para calcular o centro da tela com base na largura do terminal
int center_x(int width, int text_width) {
    return (width - text_width) / 2;
}

// Função para mostrar o menu e retornar a escolha do usuário
int print_menu() {

    char *choices[] = {
        "1  +  Carregar memória instruções   +",
        "2  +    Carregar memoria dados      +",
        "3  +        Imprimir memória        +",
        "4  +      Imprimir registradores    +",
        "5  +      Imprimir estatisticas     +",
        "6  +  Imprimir instrucoes Assembly  +",
        "7  +    Imprimir todo o simulador   +",
        "8  +          Salvar .asm           +",
        "9  +          Salvar .dat           +",
        "10 +     Executa Programa (run)     +",
        "11 +    Executa instrucao (step)    +",
        "12 +   Volta uma instrucao (back)   +",
        "0  +              Sair              +"
    };
    int n_choices = sizeof(choices) / sizeof(char *);
    int highlight = 0;
    int choice = -1;
    int c;

    initscr(); // Inicia o modo ncurses
    clear();
    noecho();
    cbreak();
    keypad(stdscr, TRUE);
    mousemask(ALL_MOUSE_EVENTS, NULL); // Habilita todos os eventos de mouse

    mouseinterval(0); // Configura o intervalo de tempo para 0 para detecção imediata de cliques

    int max_y, max_x;
    getmaxyx(stdscr, max_y, max_x); // Obtém as dimensões do terminal

    while (1) {
        clear(); // Limpa a tela antes de redesenhar o menu

        // Calcula a largura máxima do texto no menu
        int max_text_width = 0;
        for (int i = 0; i < n_choices; ++i) {
            int text_width = strlen(choices[i]);
            if (text_width > max_text_width)
                max_text_width = text_width;
        }

        // Calcula a posição central do menu horizontalmente
        int x_start = center_x(max_x, max_text_width + 2);

        // Redesenha o menu com as novas posições calculadas
        for (int i = 0; i < n_choices; ++i) {
            if (highlight == i) {
                attron(A_REVERSE);  // Realça a seleção atual
                mvprintw(i + 1, x_start, "%s", choices[i]);
                attroff(A_REVERSE);
            } else {
                mvprintw(i + 1, x_start, "%s", choices[i]);
            }
        }
        refresh();

        MEVENT event;
        c = wgetch(stdscr);

        // Verifica se o evento é um clique do mouse
        if (c == KEY_MOUSE && getmouse(&event) == OK) {
            // Verifica se o evento é um clique do botão esquerdo do mouse
            if (event.bstate & BUTTON1_CLICKED) {
                choice = event.y - 1; // A escolha é a linha clicada (descontando o título "0 - Sair")
                if (choice >= 0 && choice < n_choices) {
                    break; // Sai do loop se uma escolha válida foi feita
                }
            }
        } else {
            switch (c) {
                case KEY_UP:
                    if (highlight == 0)
                        highlight = n_choices - 1;
                    else
                        --highlight;
                    break;
                case KEY_DOWN:
                    if (highlight == n_choices - 1)
                        highlight = 0;
                    else
                        ++highlight;
                    break;
                case 10: // Tecla Enter
                    choice = highlight;
                    break;
                default:
                    break;
            }

            if (choice != -1) // Se uma escolha foi feita
                break;
        }
    }

    endwin(); // Finaliza o modo ncurses

    return choice;
}