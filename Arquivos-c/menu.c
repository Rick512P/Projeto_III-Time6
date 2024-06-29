#include "../Arquivos-h/menu.h"

int main(){
    menu();
    return 0;
}

int menu(){
    Assembly *AssemblyInst;
    MemoriaDados *md = NULL;
    unsigned int escolha, tamLinhas, program_counter = 0, cont = 0; //UNSIGNED IMPOSSIBILITA QUE PROGRAM_COUNTER CHEGUE A MENOR QUE 0
    int Etapa = 1, StateForBack = -1, i, auxiliar;
    Sinais *sinal = NULL;
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

    //md = (MemoriaDados*)calloc(256, sizeof(MemoriaDados));
    do{
        
        escolha = print_menu();

        switch (escolha)
        {
        case 0:
            free(md);
            free(memoriaInst);
            for(i=0;i<8;i++){
                regs[i] = 0;
            }
            free(AssemblyInst);
            free(instrucoesDecodificadas);
            system("clear");
            printf("Programa Encerrado!\n");
            break;
            
        case 1: //Carregar memória de Instruções e inicializa tudo
            memoriaInst = inicializaMemInst(); //inicializa memoria de instruções
            parser(memoriaInst, &tamLinhas);
            md = inicializaMemDados(); //inicializa memoria de dados
            instrucoesDecodificadas = calloc(tamLinhas, sizeof(type_instruc));
            inicializaRegsPipe(regif, id, ex, mem, wb);
            if (instrucoesDecodificadas == NULL) {
                fprintf(stderr, "Falha ao alocar memória para instruções decodificadas.\n");
                return -1;
            }
            
            AssemblyInst = calloc((tamLinhas + 1), sizeof(Assembly));
            
            if (AssemblyInst == NULL) {
                fprintf(stderr, "Falha ao alocar memória para instrucoes assembly.\n");
                return -1;
            }

            break;

        case 2: //Carregar Memória de Dados
            if (program_counter == 0){
                strcpy(dat,carregamd(md));
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
            imprimeMemInstruc(memoriaInst, tamLinhas);
            imprimeDados(md, tamLinhas);
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
            imprimeDados(md, tamLinhas);
            imprimirASM(AssemblyInst, tamLinhas);
            imprimeRegistradores(regs);
            break;

        case 8: //salvar arquivo .asm (com as instruções traduzidas para a linguagem intermediária Assembly)
            SaveASM(AssemblyInst, tamLinhas);
            break;

        case 9: //Salvar arquivo DATA.dat
            escreverArquivoMemoria(md);
            break;

        case 10: //Chamar função responsável pela execução do programa
            program_counter = 0;
            controller(1, &StateForBack, tamLinhas, regs, memoriaInst, md, &program_counter, instrucoesDecodificadas, regif, id, ex, mem, wb, sinal, Etapa);
            AsmCopy(instrucoesDecodificadas, AssemblyInst, tamLinhas);
            break;

        case 11: //Chamar função responsável pela execução do programa passo a passo
            if (memoriaInst == NULL){
                printf("Carregue a memoria com instrucoes antes.\n");
            }
            Etapa = controller(2, &StateForBack, tamLinhas, regs, memoriaInst, md, &program_counter, instrucoesDecodificadas, regif, id, ex, mem, wb, sinal, Etapa);
            AsmCopy(instrucoesDecodificadas, AssemblyInst, tamLinhas);
            printf("\n");
            puts(AssemblyInst[regif->pc].InstructsAssembly);
            break;

        case 12: //Chamar função responsável por retornar uma instrução (PC--)
            printf("StateforBack: %d\n", StateForBack--);
            if(memoriaInst == NULL){
                printf("Instrucoes nao carregadas\n");
                break;
            }
            if (StateForBack <= -1){
                fprintf(stderr, "Usuario ja esta no inicio do programa.\n");
                StateForBack = -1;
                break;
            }
            for(i = 0; i < 256 ; i++){
                strcpy(md[i].dados, "\0");
            }
            for (i = 0; i<8; i++){
                regs[i]=0;
            }
            Etapa = 1;
            auxiliar = StateForBack--;

            program_counter = 0; //PROGRAM COUNTER COMO 0 PARA REINICIAR TUDO
            StateForBack = -1;

            Etapa = backstep(auxiliar, &StateForBack, auxiliar, regs, memoriaInst, md, &program_counter, instrucoesDecodificadas, regif, id, ex, mem, wb, sinal, Etapa);
            break;

        default:
            printf("Opcao invalida.\n");
            break;
        }
    }while(escolha != 0);

    return 0;
}
