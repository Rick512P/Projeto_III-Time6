#include "../Arquivos-h/INST.h"


int INST(instrucao *memInst) {
    float altura, largura;
    struct TELAINST *telainst = (struct TELAINST *)malloc(sizeof(struct TELAINST));
    while (TRUE)
    {
        inicializaInst();
        desenhaTelaINST(&altura, &largura, telainst);
        desenhaIsntruct(altura, memInst, telainst);
        getch();

    }
    finalizaInst();
    free(telainst);
    return 0;
}

void inicializaInst() {
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    curs_set(0);
}

void finalizaInst() {
    endwin();
}

void desenhaTelaINST(float *altura, float *largura, struct TELAINST *telainst)
{
    getmaxyx(stdscr, *altura, *largura);

    start_color(); // Inicializa o modo de cores

    // Define um par de cores (texto branco no fundo preto)
    init_pair(1, COLOR_BLACK, COLOR_WHITE);


    // Criação dos contêineres
    telainst->header = newwin(*altura*0.05, *largura*0.5 + *largura*0.5, 0, 0);
    telainst->instrucoes = newwin(*altura*0.85, *largura*0.5 + *largura*0.5, *altura*0.05, 0);
    telainst->footer = newwin(*altura*0.10, *largura, *altura*0.90, 0);

    // Desenho dos contêineres
    box(telainst->header, 0, 0);
    box(telainst->instrucoes, 0, 0);
    box(telainst->footer, 0, 0);

    // Atualização das janelas
    wrefresh(telainst->header);
    wrefresh(telainst->instrucoes);
    wrefresh(telainst->footer);
}

void desenhaIsntruct(float largura, instrucao *memInst, struct TELAINST *telainst) {
    int i, linhas;
    attron(COLOR_PAIR(1)); // Ativa o par de cores número 1

    //HEAR
    mvwprintw(telainst->header, 1, largura*1.8, "SIMULADOR MINIMIPS 8BITS PIPELINE");

    //Isntruct
    mvwprintw(telainst->instrucoes, 1, largura*1.9, "=== Memória de Instruções ===");

    for (i = 0; i<32; i++){
        if (i < 10)
            mvwprintw(telainst->instrucoes, i+2, 1, "INST 00%d: %s", i, memInst[i].instruc);
        else
            mvwprintw(telainst->instrucoes, i+2, 1, "INST 0%d: %s", i, memInst[i].instruc);
    }
    linhas = 1;
    for (i; i<64; i++){
        linhas++;
        mvwprintw(telainst->instrucoes, linhas, largura/2, "|INST 0%d: %s", i, memInst[i].instruc);
    }
    linhas = 1;
    for (i; i<96; i++){
        linhas++;
        mvwprintw(telainst->instrucoes, linhas, largura, "|INST 0%d: %s", i, memInst[i].instruc);
    }
    linhas = 1;
    for (i; i<128; i++){
        linhas++;
        if (i < 100)
            mvwprintw(telainst->instrucoes, linhas, largura+largura/2, "|INST 0%d: %s", i, memInst[i].instruc);
        else
            mvwprintw(telainst->instrucoes, linhas, largura+largura/2, "|INST %d: %s", i, memInst[i].instruc);
    }
    linhas = 1;
    for (i; i<160; i++){
        linhas++;
        mvwprintw(telainst->instrucoes, linhas, largura*2, "|INST %d: %s", i, memInst[i].instruc);
    }
    linhas = 1;
    for (i; i<192; i++){
        linhas++;
        mvwprintw(telainst->instrucoes, linhas, largura*2+largura/2, "|INST %d: %s", i, memInst[i].instruc);
    }
    linhas = 1;
    for (i; i<224; i++){
        linhas++;
        mvwprintw(telainst->instrucoes, linhas, largura*3, "|INST %d: %s", i, memInst[i].instruc);
    }
    linhas = 1;
    for (i; i<256; i++){
        linhas++;
        mvwprintw(telainst->instrucoes, linhas, largura*3 + largura/2, "|INST %d: %s", i, memInst[i].instruc);
    }

    //FOOTER
    mvwprintw(telainst->footer, 1, largura*2, "AUTORES");
    mvwprintw(telainst->footer, 2, largura, "Erick Santos Matos, Henrique de Lima Bortolomiol, Pedro Afonso Barcelos da Silva Simões Pires");

    // Atualização das janelas
    wrefresh(telainst->header);
    wrefresh(telainst->instrucoes);
    wrefresh(telainst->footer);
}


