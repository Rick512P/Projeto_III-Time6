#include "../Arquivos-h/ASSEMBLYTERMINAL.h"

AssemblyASM ASSEMBLYASM[32];

int ASSEMBLYTERMINAL(Assembly **A) {
    float altura, largura;
    while (TRUE)
    {
        inicializaASM();
        desenhaTelaASM(&altura, &largura);
        desenhaAssembly(altura, A);
        getch();

    }

    finalizaASM();

    return 0;
}

void inicializaASM() {
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    curs_set(0);
}

void finalizaASM() {
    endwin();
}

void desenhaTelaASM(float *altura, float *largura)
{
    getmaxyx(stdscr, *altura, *largura);

    start_color(); // Inicializa o modo de cores

    // Define um par de cores (texto branco no fundo preto)
    init_pair(1, COLOR_BLACK, COLOR_WHITE);


    // Criação dos contêineres
    tela.header = newwin(*altura*0.05, *largura*0.5 + *largura*0.5, 0, 0);
    tela.ASM = newwin(*altura*0.85, *largura*0.5 + *largura*0.5, *altura*0.05, 0);
    tela.footer = newwin(*altura*0.10, *largura, *altura*0.90, 0);

    // Desenho dos contêineres
    box(tela.header, 0, 0);
    box(tela.ASM, 0, 0);
    box(tela.footer, 0, 0);

    // Atualização das janelas
    wrefresh(tela.header);
    wrefresh(tela.ASM);
    wrefresh(tela.footer);
}

void desenhaAssembly(float largura, Assembly *A) // Adicionar verificação de NULL, para deixarm em branco quando não houver mais instruções
{
    int i, linhas;
    attron(COLOR_PAIR(1)); // Ativa o par de cores número 1

    //HEAR
    mvwprintw(tela.header, 1, largura*1.8, "SIMULADOR MINIMIPS 8BITS PIPELINE");

    //Data
    mvwprintw(tela.ASM, 1, largura*1.9, "=== Instruções AssembLy ===");

    for (i = 0; i<32; i++){
        if (i < 10)
            mvwprintw(tela.ASM, i+2, 1, "INST 00%d: %s", i, A[i].InstructsAssembly[0]);
        else
            mvwprintw(tela.ASM, i+2, 1, "INST 0%d: %s", i, A[i].InstructsAssembly[0]);
    }
    if (sizeof(ASSEMBLYASM)>=32)
    {    
        linhas = 1;
        for (i; i<64; i++){
            linhas++;
            mvwprintw(tela.ASM, linhas, largura/2, "|INST 0%d: %s", i, A[i].InstructsAssembly[0]);
        }
    }
    else if (sizeof(ASSEMBLYASM) >= 64){
        linhas = 1;
        for (i; i<96; i++){
            linhas++;
            mvwprintw(tela.ASM, linhas, largura, "|INST 0%d: %s", i, A[i].InstructsAssembly[0]);
        }
    }
    else if (sizeof(ASSEMBLYASM) >= 96){
        linhas = 1;
        for (i; i<128; i++){
            linhas++;
            if (i < 100)
                mvwprintw(tela.ASM, linhas, largura+largura/2, "|INST 0%d: %s", i, A[i].InstructsAssembly[0]);
            else
                mvwprintw(tela.ASM, linhas, largura+largura/2, "|INST %d: %s", i, A[i].InstructsAssembly[0]);
        }
    }
    else if (sizeof(ASSEMBLYASM) >= 128){
        linhas = 1;
        for (i; i<160; i++){
            linhas++;
            mvwprintw(tela.ASM, linhas, largura*2, "|INST %d: %s", i, A[i].InstructsAssembly[0]);
        }
    }
    else if (sizeof(ASSEMBLYASM) >= 160){
        linhas = 1;
        for (i; i<192; i++){
            linhas++;
            mvwprintw(tela.ASM, linhas, largura*2+largura/2, "|INST %d: %s", i, A[i].InstructsAssembly[0]);
        }
    }
    else if (sizeof(ASSEMBLYASM) >= 192){
        linhas = 1;
        for (i; i<224; i++){
            linhas++;
            mvwprintw(tela.ASM, linhas, largura*3, "|INST %d: %s", i, A[i].InstructsAssembly[0]);
        }
    }
    else if (sizeof(ASSEMBLYASM) >= 224){
        linhas = 1;
        for (i; i<256; i++){
            linhas++;
            mvwprintw(tela.ASM, linhas, largura*3 + largura/2, "|INST %d: %s", i, A[i].InstructsAssembly[0]);
        }
    }

    //FOOTER
    mvwprintw(tela.footer, 1, largura*2, "AUTORES");
    mvwprintw(tela.footer, 2, largura, "Erick Santos Matos, Henrique de Lima Bortolomiol, Pedro Afonso Barcelos da Silva Simões Pires");

    // Atualização das janelas
    wrefresh(tela.header);
    wrefresh(tela.ASM);
    wrefresh(tela.footer);
}


