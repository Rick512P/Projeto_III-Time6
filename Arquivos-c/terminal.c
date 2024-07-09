#include "../Arquivos-h/Terminal.h"

char terminal(int *program_counter, instrucao *memInst, int tamLinhas, type_instruc *instrucoesDecodificadas, int *regs, char *instrucIF, char *instrucID, char *instrucEX, char *instrucMEM, char *instrucWB) {
    float altura, largura;
    int ch, r, i, j, instLogic, instAri, instDesvio, instAcessoMem;
    char escolha = ' ';
    struct TELA *tela = (struct TELA *)malloc(sizeof(struct TELA));
   
    //puts("Debug");
    while (escolha == ' ') 
    {
        inicializaTerminal();
        desenhaTelaInicial(tela, &altura, &largura);
        imprimeEstatisticasTerminal(tela, program_counter, altura, memInst, tamLinhas, instrucoesDecodificadas, regs, instrucIF, instrucID, instrucEX, instrucMEM, instrucWB);

        ch = getch();
        switch (ch) {
            case KEY_F(1):
                escolha = '0';
                break;
            case KEY_F(2):
                escolha = '1';
                break;
            case KEY_F(3):
                escolha = '2';
                break;
            case KEY_F(4):
                escolha = '3';
                break;
            case KEY_F(5):
                escolha = '4';
                break;
            case KEY_F(6):
                escolha = '5';
                break;
            case KEY_F(7):
                escolha = '6';
                break;
            case KEY_F(8):
                escolha = '7';
                break;
            case KEY_F(9):
                escolha = '8';
                break;
            case KEY_F(10):
                escolha = '9';
                break;
            case KEY_F(11):
                escolha = 'a';
                break;
        }
        //puts(&escolha);
    }

    clear();
    finalizaTerminal();
    free(tela);
    puts(&escolha);
    return escolha;
}

void inicializaTerminal() {
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    curs_set(0);
}

void finalizaTerminal(){
    endwin();
}

void desenhaTelaInicial(struct TELA *tela, float *altura, float *largura){
    getmaxyx(stdscr, *altura, *largura);

    start_color(); // Inicializa o modo de cores

    // Define um par de cores (texto branco no fundo preto)
    init_pair(1, COLOR_BLACK, COLOR_WHITE);

    //puts("Debug");
    // Criação dos contêineres
    tela->header = newwin(*altura*0.10, *largura*0.5 + *largura*0.5, 0, 0);
    tela->menu = newwin(*altura*0.80, *largura*0.65, *altura*0.10, 0);
    tela->content = newwin(*altura*0.80, *largura*0.35, *altura*0.10, *largura*0.65);
    tela->footer = newwin(*altura*0.10, *largura, *altura*0.90, 0);

    // Desenho dos contêineres
    box(tela->header, 0, 0);
    box(tela->menu, 0, 0);
    box(tela->content, 0, 0);
    box(tela->footer, 0, 0);

    // Atualização das janelas
    wrefresh(tela->header);
    wrefresh(tela->menu);
    wrefresh(tela->content);
    wrefresh(tela->footer);
}

void desenhaMenu(struct TELA *tela, int *program_counter, float largura, int *regs, int tamLinhas, int r, int I, int j, int instLogic, int instAri, int instDesvio, int instAcessoMem, char *instrucIF, char *instrucID, char *instrucEX, char *instrucMEM, char *instrucWB)
{
    int i;
    attron(COLOR_PAIR(1)); // Ativa o par de cores número 1

    //HEAR
    mvwprintw(tela->header, 1, largura*1.8, "SIMULADOR MINIMIPS 8BITS PIPELINE");

    //MENU
    mvwprintw(tela->menu, 3,  largura*1.3, "=== MENU ===");
    mvwprintw(tela->menu, 5,  largura, "F2     + CARREGAR MEMóRIA DE INSTRUçõES");
    mvwprintw(tela->menu, 6,  largura, "F3     + CARREGAR MEMóRIA DE DADOS");
    mvwprintw(tela->menu, 7,  largura, "F4     + EXIBIR MEMóRIA DE INSTRUçõES");
    mvwprintw(tela->menu, 8,  largura, "F5     + EXIBIR MEMóRIA DE DADOS");
    mvwprintw(tela->menu, 9,  largura, "F6     + EXIBIR INSTRUçõES ASSEMBLY");
    mvwprintw(tela->menu, 10, largura, "F7     + SALVAR ARQUIVO .ASM");
    mvwprintw(tela->menu, 11, largura, "F8     + SALVAR ARQUIVO .DAT");
    mvwprintw(tela->menu, 12, largura, "F9     + EXECUTAR PROGRAMA (RUN)");
    mvwprintw(tela->menu, 13, largura, "F10    + EXECUTAR CICLO (STEP)");
    mvwprintw(tela->menu, 14, largura, "F11    + DESFAZER CICLO (BACK STEP)");
    mvwprintw(tela->menu, 15, largura, "F1     + ENCERRAR SIMULADOR");

    if (*program_counter != 0)
    {
    
        //CONTENT
        mvwprintw(tela->content, 1, largura/2, "=== ESTATíSTICAS ===");
        mvwprintw(tela->content, 2, largura/2, "--------------------");

        mvwprintw(tela->content, 3, largura*0.70, "ETAPA");
        mvwprintw(tela->content, 4,  5, "|-----------------------------------------------------|");
        
        //PRECISA SER DIFERENTE DE NULL SE NAO IRÁ CRASHAR
        if(instrucWB)
            mvwprintw(tela->content, 5,  5, "|ETAPA WB: %s", instrucWB);
        else
            mvwprintw(tela->content, 5,  5, "|ETAPA WB: " );

        if(instrucMEM)
            mvwprintw(tela->content, 6,  5, "|ETAPA MEM: %s", instrucMEM);
        else
            mvwprintw(tela->content, 6,  5, "|ETAPA MEM: ");

        if(instrucEX)
            mvwprintw(tela->content, 7,  5, "|ETAPA EX: %s", instrucEX);
        else
            mvwprintw(tela->content, 7,  5, "|ETAPA EX: ");

        if(instrucID)
            mvwprintw(tela->content, 8,  5, "|ETAPA ID: %s", instrucID);
        else
            mvwprintw(tela->content, 8,  5, "|ETAPA ID: ");

        if(instrucIF)
            mvwprintw(tela->content, 9,  5, "|ETAPA IF: %s", instrucIF);
        else
            mvwprintw(tela->content, 9,  5, "|ETAPA IF: ");

        mvwprintw(tela->content, 10, 5, "|-----------------------------------------------------|");


        mvwprintw(tela->content, 12, largura/3, "-------------");
        for (i = 0; i < 8; i++){
            mvwprintw(tela->content, i+13, largura/3, "|REG [  %d  ]|", regs[i]);
        }
        i = i + 13;
        mvwprintw(tela->content, i, largura/3, "-------------");
        
        mvwprintw(tela->content, 14,  largura*0.8, "|--------------|");
        mvwprintw(tela->content, 15,  largura*0.8, "|=== ESTADO ===|");
        mvwprintw(tela->content, 16,  largura*0.8, "|--------------|");
        mvwprintw(tela->content, 17,  largura*0.8, "|Ciclo | %d    |", i);
        mvwprintw(tela->content, 18,  largura*0.8, "|PC    | %d    |", *program_counter);
        mvwprintw(tela->content, 19,  largura*0.8, "|--------------|");

        i++;

        mvwprintw(tela->content, i+1,  largura/3, "|-----------------------------------|");
        mvwprintw(tela->content, i+2,  largura/3, "| Total de Instruções      : %d|", tamLinhas);
        mvwprintw(tela->content, i+3,  largura/3, "| Tipo R                     : %d|", r);
        mvwprintw(tela->content, i+4,  largura/3, "| Tipo I                     : %d|", I);
        mvwprintw(tela->content, i+5,  largura/3, "| Tipo j                     : %d|", j);
        mvwprintw(tela->content, i+6,  largura/3, "| Classe Lógica             : %d|", instLogic);
        mvwprintw(tela->content, i+7,  largura/3, "| CLasse Aritmética         : %d|", instAri);
        mvwprintw(tela->content, i+8,  largura/3, "| Classe de Desvio           : %d|", instDesvio);
        mvwprintw(tela->content, i+9,  largura/3, "| Classe de Acesso a Memória : %d|", instAcessoMem);
        mvwprintw(tela->content, i+10, largura/3, "|-----------------------------------|");
    }
    else{
        //CONTENT
        mvwprintw(tela->content, 1, largura/2, "=== ESTATíSTICAS ===");
        mvwprintw(tela->content, 2, largura/2, "--------------------");

        mvwprintw(tela->content, 3, largura*0.70, "ETAPA");
        mvwprintw(tela->content, 4,  5, "|-----------------------------------------------------|");
        
        //PRECISA SER DIFERENTE DE NULL SE NAO IRÁ CRASHAR
        if(instrucWB)
            mvwprintw(tela->content, 5,  5, "|ETAPA WB:");
        else
            mvwprintw(tela->content, 5,  5, "|ETAPA WB:");

        if(instrucMEM)
            mvwprintw(tela->content, 6,  5, "|ETAPA MEM:");
        else
            mvwprintw(tela->content, 6,  5, "|ETAPA MEM:");

        if(instrucEX)
            mvwprintw(tela->content, 7,  5, "|ETAPA EX:");
        else
            mvwprintw(tela->content, 7,  5, "|ETAPA EX: ");

        if(instrucID)
            mvwprintw(tela->content, 8,  5, "|ETAPA ID:");
        else
            mvwprintw(tela->content, 8,  5, "|ETAPA ID:");

        if(instrucIF)
            mvwprintw(tela->content, 9,  5, "|ETAPA IF:");
        else
            mvwprintw(tela->content, 9,  5, "|ETAPA IF:");

        mvwprintw(tela->content, 10, 5, "|-----------------------------------------------------|");


        mvwprintw(tela->content, 12, largura/3, "-------------");
        for (i = 0; i < 8; i++){
            mvwprintw(tela->content, i+13, largura/3, "|REG [     ]|");
        }
        i = i + 13;
        mvwprintw(tela->content, i, largura/3, "-------------");
        
        mvwprintw(tela->content, 14,  largura*0.8, "|--------------|");
        mvwprintw(tela->content, 15,  largura*0.8, "|=== ESTADO ===|");
        mvwprintw(tela->content, 16,  largura*0.8, "|--------------|");
        mvwprintw(tela->content, 17,  largura*0.8, "|Ciclo |   0   |");
        mvwprintw(tela->content, 18,  largura*0.8, "|PC    |   0   |");
        mvwprintw(tela->content, 19,  largura*0.8, "|--------------|");

        i++;

        mvwprintw(tela->content, i+1,  largura/3, "|-----------------------------------|");
        mvwprintw(tela->content, i+2,  largura/3, "| Total de Instruções      : |");
        mvwprintw(tela->content, i+3,  largura/3, "| Tipo R                     : |");
        mvwprintw(tela->content, i+4,  largura/3, "| Tipo I                     : |");
        mvwprintw(tela->content, i+5,  largura/3, "| Tipo j                     : |");
        mvwprintw(tela->content, i+6,  largura/3, "| Classe Lógica             : |");
        mvwprintw(tela->content, i+7,  largura/3, "| CLasse Aritmética         : |");
        mvwprintw(tela->content, i+8,  largura/3, "| Classe de Desvio           : |");
        mvwprintw(tela->content, i+9,  largura/3, "| Classe de Acesso a Memória : |");
        mvwprintw(tela->content, i+10, largura/3, "|-----------------------------------|");
    }

    //FOOTER
    mvwprintw(tela->footer, 1, largura*2, "AUTORES");
    mvwprintw(tela->footer, 2, largura, "Erick Santos Matos, Henrique de Lima Bortolomiol, Pedro Afonso Barcelos da Silva Simões Pires");

    // Atualização das janelas
    wrefresh(tela->header);
    wrefresh(tela->menu);
    wrefresh(tela->content);
    wrefresh(tela->footer);
}

void imprimeEstatisticasTerminal(struct TELA *tela, int *program_counter, float largura, instrucao *memInst, int tamLinhas, type_instruc *instrucoesDecodificadas, int *regs, char *instrucIF, char *instrucID, char *instrucEX, char *instrucMEM, char *instrucWB){
    int r=0, i=0, j=0, instAri=0, instLogic=0, instDesvio=0, instAcessoMem=0;
    if (*program_counter != 0){
        if (memInst == NULL) {
                    fprintf(stderr, "Falha ao obter instruções.\n");
                }
        for(int y=0;y<tamLinhas;y++){
            if (strncmp(memInst[y].instruc, "0000", 4) == 0){ //compara os 4 primeiros numeros de memInst com "0000"
                r++;
                if ((strcmp(instrucoesDecodificadas[y].funct, "000")) || (strcmp(instrucoesDecodificadas[y].funct, "010") == 0))
                    instAri++;
                else
                    instLogic++;
            }
            else if (strncmp(memInst[y].instruc, "0100", 4) == 0 || strncmp(memInst[y].instruc, "1011", 4) == 0 || strncmp(memInst[y].instruc, "1111", 4) == 0 || strncmp(memInst[y].instruc, "0110", 4) == 0 || strncmp(memInst[y].instruc, "1000", 4) == 0){
                i++;
                if (strncmp(memInst[y].instruc, "0100", 4) == 0)
                    instAri++;
                else if ((strncmp(memInst[y].instruc, "1011", 4) == 0) || strncmp(memInst[y].instruc, "1111", 4) == 0)
                    instAcessoMem++;
                else if (strncmp(memInst[y].instruc, "1000", 4) == 0)
                    instDesvio++;
            }  
            else if (strncmp(memInst[y].instruc, "0010", 4) == 0){
                j++;
                instDesvio++;
            }
                
        }
    }

    desenhaMenu(tela, program_counter, largura, regs, tamLinhas, r, i, j, instLogic, instAri, instDesvio, instAcessoMem, instrucIF, instrucID, instrucEX, instrucMEM, instrucWB);
}