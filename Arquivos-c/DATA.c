#include "../Arquivos-h/DATA.h"

int DATA(MemoriaDados **mem) {
    float altura, largura;
    while (TRUE)
    {
        inicializaData();
        desenhaTelaDATA(&altura, &largura);
        desenhaData(altura, mem);
        getch();
    }
   
    finalizaData();

    return 0;
}

void inicializaData() {
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    curs_set(0);
}

void finalizaData() {
    endwin();
}

void desenhaTelaDATA(float *altura, float *largura) {
    getmaxyx(stdscr, *altura, *largura);

    start_color(); // Inicializa o modo de cores

    // Define um par de cores (texto branco no fundo preto)
    init_pair(1, COLOR_BLACK, COLOR_WHITE);


    // Criação dos contêineres
    tela.header = newwin(*altura*0.05, *largura*0.5 + *largura*0.5, 0, 0);
    tela.dados = newwin(*altura*0.85, *largura*0.5 + *largura*0.5, *altura*0.05, 0);
    tela.footer = newwin(*altura*0.10, *largura, *altura*0.90, 0);

    // Desenho dos contêineres
    box(tela.header, 0, 0);
    box(tela.dados, 0, 0);
    box(tela.footer, 0, 0);

    // Atualização das janelas
    wrefresh(tela.header);
    wrefresh(tela.dados);
    wrefresh(tela.footer);
}

void desenhaData(float largura, MemoriaDados *mem) {
    int i, linhas;
    attron(COLOR_PAIR(1)); // Ativa o par de cores número 1

    //HEAR
    mvwprintw(tela.header, 1, largura*1.8, "SIMULADOR MINIMIPS 8BITS PIPELINE");

    //Data
    mvwprintw(tela.dados, 1, largura*1.9, "=== Memória de Dados ===");

    for (i = 0; i<32; i++){
        if (i < 10)
            mvwprintw(tela.dados, i+2, 1, "DADO 00%d: %s", i, mem[i]);
        else
            mvwprintw(tela.dados, i+2, 1, "DADO 0%d: %s", i, mem[i]);
    }
    linhas = 1;
    for (i; i<64; i++){
        linhas++;
        mvwprintw(tela.dados, linhas, largura/2, "|DADO 0%d: %s", i, mem[i]);
    }
    linhas = 1;
    for (i; i<96; i++){
        linhas++;
        mvwprintw(tela.dados, linhas, largura, "|DADO 0%d: %s", i, mem[i]);
    }
    linhas = 1;
    for (i; i<128; i++){
        linhas++;
        if (i < 100)
            mvwprintw(tela.dados, linhas, largura+largura/2, "|DADO 0%d: %s", i, mem[i]);
        else
            mvwprintw(tela.dados, linhas, largura+largura/2, "|DADO %d: %s", i, mem[i]);
    }
    linhas = 1;
    for (i; i<160; i++){
        linhas++;
        mvwprintw(tela.dados, linhas, largura*2, "|DADO %d: %s", i, mem[i]);
    }
    linhas = 1;
    for (i; i<192; i++){
        linhas++;
        mvwprintw(tela.dados, linhas, largura*2+largura/2, "|DADO %d: %s", i, mem[i]);
    }
    linhas = 1;
    for (i; i<224; i++){
        linhas++;
        mvwprintw(tela.dados, linhas, largura*3, "|DADO %d: %s", i, mem[i]);
    }
    linhas = 1;
    for (i; i<256; i++){
        linhas++;
        mvwprintw(tela.dados, linhas, largura*3 + largura/2, "|DADO %d: %s", i, mem[i]);
    }

    //FOOTER
    mvwprintw(tela.footer, 1, largura*2, "AUTORES");
    mvwprintw(tela.footer, 2, largura, "Erick Santos Matos, Henrique de Lima Bortolomiol, Pedro Afonso Barcelos da Silva Simões Pires");

    // Atualização das janelas
    wrefresh(tela.header);
    wrefresh(tela.dados);
    wrefresh(tela.footer);
}

