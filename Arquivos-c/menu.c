#include "../Arquivos-h/menu.h"

int main(){
    menu();
    return 0;
}

int menu(){
    Assembly *AssemblyInst = NULL;
    MemoriaDados *memDados = NULL;
    Backup *backup = NULL;
    NodoPilha *NodoPilha = NULL;
    descPilha *descPilha = NULL;
    unsigned int escolha, tamLinhas, program_counter = 0, cont = 0; //UNSIGNED IMPOSSIBILITA QUE PROGRAM_COUNTER CHEGUE A MENOR QUE 0
    int Etapa = 1, StateForBack = -1, i, auxiliar;
    Sinais *sinal = NULL;
    PipeRegisters pipes;
    IF *regif = NULL;
    ID *id = NULL;
    EX *ex = NULL;
    MEM *mem = NULL;
    WB *wb = NULL;
    instrucao *memoriaInst = NULL; //RESPONSAVEL POR COLETAR  A INSTRUÇÃO
    type_instruc *instrucoesDecodificadas = NULL;
    int regs[8];
    //int *regs = NULL; //registradores como um inteiro mesmo
    char dat[300]; //Recebe o nome do arquivo.dat
    //regs = (int*)malloc(8 * sizeof(int));
    for (i=0;i<8;i++){ //zerando registradores, caso contrario dá números inconsistentes
        regs[i] = 0;
    }
    do{
        
        //escolha = print_menu();
        printf("\n\n");
        printf("                                 _____________________________________\n");
        printf("                                |1  +  Carregar memória instruções   +|\n");
        printf("                                |2  +    Carregar memoria dados      +|\n");
        printf("                                |3  +        Imprimir memória        +|\n");
        printf("                                |4  +      Imprimir registradores    +|\n");
        printf("                                |5  +      Imprimir estatisticas     +|\n");
        printf("                                |6  +  Imprimir instrucoes Assembly  +|\n");
        printf("                                |7  +    Imprimir todo o simulador   +|\n");
        printf("                                |8  +          Salvar .asm           +|\n");
        printf("                                |9  +          Salvar .dat           +|\n");
        printf("                                |10 +     Executa Programa (run)     +|\n");
        printf("                                |11 +    Executa instrucao (step)    +|\n");
        printf("                                |12 +   Volta uma instrucao (back)   +|\n");
        printf("                                |0  +              Sair              +|\n");
        scanf("%d", &escolha);
        switch (escolha)
        {
        case 0:
            freeALL(regs, AssemblyInst, memoriaInst, memDados, instrucoesDecodificadas, regif, id, ex, mem, wb , sinal);
            system("clear");
            printf("Programa Encerrado!\n");
            break;
            
        case 1: //Carregar memória de Instruções e inicializa tudo
            if(memoriaInst != NULL){
                printf("Memoria ja preenchida\n");
                break;
            }
            memoriaInst = inicializaMemInst(); //inicializa memoria de instruções
            parser(memoriaInst, &tamLinhas);
            pipes = inicializaRegsPipe();
            IF *regif = pipes.regif;
            ID *id = pipes.id;
            EX *ex = pipes.ex;
            MEM *mem = pipes.mem;
            WB *wb = pipes.wb;
            inicializaRegsPipe(regif, id, ex, mem, wb);
            memDados = inicializaMemDados(); //inicializa memoria de dados
            descPilha = inicializaBackup();
            instrucoesDecodificadas = calloc(tamLinhas, sizeof(type_instruc));
            if (instrucoesDecodificadas == NULL) {
                fprintf(stderr, "Falha ao alocar memória para instruções decodificadas.\n");
                return -1;
            }
            
            AssemblyInst = calloc((tamLinhas + 1), sizeof(Assembly));
            
            if (AssemblyInst == NULL) {
                fprintf(stderr, "Falha ao alocar memória para instrucoes assembly.\n");
                return -1;
            }
            //FAZ UM "BACKUP" PARA O BACKSTEP   
                backup = ColetaTudo(regs, memDados, regif, id, ex, mem, wb, sinal, AssemblyInst, &program_counter, &Etapa);
                NodoPilha = inicializaNodo(backup);
                descPilha = PUSH(descPilha, NodoPilha);
            break;

        case 2: //Carregar Memória de Dados
            if (program_counter == 0){
                strcpy(dat,carregamemDados(memDados));
                printf("\n");
                puts(dat);
                printf("\n");
                if (strcmp(dat,"ERRO")!=0){
                    cont = 1;
                }
            }
            else 
                printf("Programa nao deve ja ter sido inicializado.");
            break;

        case 3: //Imprimir memória de instruções e memória de dados
            imprimeMemInstruc(memoriaInst);
            imprimeDados(memDados);
            break;

        case 4: //Imprimir registradores
            imprimeRegistradores(regs);
            break;

        case 5: //Imprimir estatísticas como: quantas intruc, classes, etc;
            imprimeEstatisticas(memoriaInst, tamLinhas, instrucoesDecodificadas);
            break;
            
        case 6: // Imprimir Assembly
            imprimirASM(AssemblyInst, tamLinhas);
            break;

        case 7: //imprimir todo o simulador
            imprimeEstatisticas(memoriaInst, tamLinhas, instrucoesDecodificadas);
            imprimeSimulador(tamLinhas, instrucoesDecodificadas, memoriaInst);      
            imprimeDados(memDados);
            imprimirASM(AssemblyInst, tamLinhas);
            imprimeRegistradores(regs);
            break;

        case 8: //salvar arquivo .asm (com as instruções traduzidas para a linguagem intermediária Assembly)
            SaveASM(AssemblyInst, tamLinhas);
            break;

        case 9: //Salvar arquivo DATA.dat
            escreverArquivoMemoria(memDados);
            break;

        case 10: //Chamar função responsável pela execução do programa
            program_counter = 0;
            controller(1, &StateForBack, tamLinhas, regs, memoriaInst, memDados, &program_counter, instrucoesDecodificadas, regif, id, ex, mem, wb, sinal, Etapa);
            AsmCopy(instrucoesDecodificadas, AssemblyInst, tamLinhas);
            //FAZ UM "BACKUP" PARA O BACKSTEP   
                backup = ColetaTudo(regs, memDados, regif, id, ex, mem, wb, sinal, AssemblyInst, &program_counter, &Etapa);
                NodoPilha = inicializaNodo(backup);
                descPilha = PUSH(descPilha, NodoPilha);
                descPilha->tamanho = tamLinhas;
            break;

        case 11: //Chamar função responsável pela execução do programa passo a passo
            if (memoriaInst == NULL){
                printf("Carregue a memoria com instrucoes antes.\n");
            }
            if(Etapa == 6){
                printf("MIPS ja executou as instrucoes\n");
                break;
            }
            Etapa = controller(2, &StateForBack, tamLinhas, regs, memoriaInst, memDados, &program_counter, instrucoesDecodificadas, regif, id, ex, mem, wb, sinal, Etapa);
            AsmCopy(instrucoesDecodificadas, AssemblyInst, tamLinhas);
            printf("\n");
            puts(AssemblyInst[regif->pc].InstructsAssembly);
            //FAZ UM "BACKUP" PARA O BACKSTEP   
                backup = ColetaTudo(regs, memDados, regif, id, ex, mem, wb, sinal, AssemblyInst, &program_counter, &Etapa);
                NodoPilha = inicializaNodo(backup);
                descPilha = PUSH(descPilha, NodoPilha);
            break;

        case 12: //Chamar função responsável por retornar uma instrução (PC--)
            descPilha = Realoca(descPilha, regs, memDados, regif, id, ex, mem, wb, sinal, AssemblyInst, &program_counter, &Etapa);
            if(descPilha->Topo != NULL){
                 printf("Retornamos para:\nIF-Instrucao:[%s]\nID-Instrucao:[%s]\nEX-Instrucao:[%s]\nMEM-Instrucao[%s]\nWB-Instrucao:[%s]\n", regif->instruc, id->instruc, ex->instruc, mem->instruc, wb->instruc);
            }
            else{
                printf("Estamos no início do programa.\n");
            }  
            break;

        default:
            printf("Opcao invalida.\n");
            break;
        }
    }while(escolha != 0);

    return 0;
}
