#include "../Arquivos-h/ASSEMBLYTERMINAL.h"

int ASSEMBLYTERMINAL(Assembly *A) {
    float altura, largura;
    struct TELAASSEMBLY *telaAssembly = (struct TELAASSEMBLY *)malloc(sizeof(struct TELAASSEMBLY)); 
    while (TRUE)
    {
        inicializaASM();
        desenhaTelaASM(telaAssembly, &altura, &largura);
        desenhaAssembly(telaAssembly, altura, A);
        getch();

    }

    finalizaASM();
    free(telaAssembly);
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

void desenhaTelaASM(struct TELAASSEMBLY *telaAssembly, float *altura, float *largura)
{
    getmaxyx(stdscr, *altura, *largura);

    start_color(); // Inicializa o modo de cores

    // Define um par de cores (texto branco no fundo preto)
    init_pair(1, COLOR_BLACK, COLOR_WHITE);


    // Criação dos contêineres
    telaAssembly->header = newwin(*altura*0.05, *largura*0.5 + *largura*0.5, 0, 0);
    telaAssembly->ASM = newwin(*altura*0.85, *largura*0.5 + *largura*0.5, *altura*0.05, 0);
    telaAssembly->footer = newwin(*altura*0.10, *largura, *altura*0.90, 0);

    // Desenho dos contêineres
    box(telaAssembly->header, 0, 0);
    box(telaAssembly->ASM, 0, 0);
    box(telaAssembly->footer, 0, 0);

    // Atualização das janelas
    wrefresh(telaAssembly->header);
    wrefresh(telaAssembly->ASM);
    wrefresh(telaAssembly->footer);
}

void desenhaAssembly(struct TELAASSEMBLY *telaAssembly, float largura, Assembly *A) // Adicionar verificação de NULL, para deixarm em branco quando não houver mais instruções
{
    int i, linhas;
    attron(COLOR_PAIR(1)); // Ativa o par de cores número 1

    //HEAR
    mvwprintw(telaAssembly->header, 1, largura*1.8, "SIMULADOR MINIMIPS 8BITS PIPELINE");

    //Data
    mvwprintw(telaAssembly->ASM, 1, largura*1.9, "=== Instruções AssembLy ===");

    for (i = 0; i<32; i++){
        if (i < 10)
            mvwprintw(telaAssembly->ASM, i+2, 1, "INST 00%d: %s", i, A[i].InstructsAssembly);
        else
            mvwprintw(telaAssembly->ASM, i+2, 1, "INST 0%d: %s", i, A[i].InstructsAssembly);
    }
    if (sizeof(A)>=32)
    {    
        linhas = 1;
        for (i; i<64; i++){
            linhas++;
            mvwprintw(telaAssembly->ASM, linhas, largura/2, "|INST 0%d: %s", i, A[i].InstructsAssembly);
        }
    }
    else if (sizeof(A) >= 64){
        linhas = 1;
        for (i; i<96; i++){
            linhas++;
            mvwprintw(telaAssembly->ASM, linhas, largura, "|INST 0%d: %s", i, A[i].InstructsAssembly);
        }
    }
    else if (sizeof(A) >= 96){
        linhas = 1;
        for (i; i<128; i++){
            linhas++;
            if (i < 100)
                mvwprintw(telaAssembly->ASM, linhas, largura+largura/2, "|INST 0%d: %s", i, A[i].InstructsAssembly);
            else
                mvwprintw(telaAssembly->ASM, linhas, largura+largura/2, "|INST %d: %s", i, A[i].InstructsAssembly);
        }
    }
    else if (sizeof(A) >= 128){
        linhas = 1;
        for (i; i<160; i++){
            linhas++;
            mvwprintw(telaAssembly->ASM, linhas, largura*2, "|INST %d: %s", i, A[i].InstructsAssembly);
        }
    }
    else if (sizeof(A) >= 160){
        linhas = 1;
        for (i; i<192; i++){
            linhas++;
            mvwprintw(telaAssembly->ASM, linhas, largura*2+largura/2, "|INST %d: %s", i, A[i].InstructsAssembly);
        }
    }
    else if (sizeof(A) >= 192){
        linhas = 1;
        for (i; i<224; i++){
            linhas++;
            mvwprintw(telaAssembly->ASM, linhas, largura*3, "|INST %d: %s", i, A[i].InstructsAssembly);
        }
    }
    else if (sizeof(A) >= 224){
        linhas = 1;
        for (i; i<256; i++){
            linhas++;
            mvwprintw(telaAssembly->ASM, linhas, largura*3 + largura/2, "|INST %d: %s", i, A[i].InstructsAssembly);
        }
    }

    //FOOTER
    mvwprintw(telaAssembly->footer, 1, largura*2, "AUTORES");
    mvwprintw(telaAssembly->footer, 2, largura, "Erick Santos Matos, Henrique de Lima Bortolomiol, Pedro Afonso Barcelos da Silva Simões Pires");

    // Atualização das janelas
    wrefresh(telaAssembly->header);
    wrefresh(telaAssembly->ASM);
    wrefresh(telaAssembly->footer);
}


