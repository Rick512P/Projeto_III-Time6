#include "../Arquivos-h/parser.h"

//RESPONSAVEL POR ABRIR O ARQUIVO E ENCHER A MEMORIA DE INSTRUÇÕES
int parser(instrucao *memInst, int *tamanho_linhas){
    char linha[100], nome_arquivo[200];
    int contador_de_linhas = 0, opcao;
    instrucao p;
    FILE *arq;
        printf("Digite 1 para utilizar o diretorio padrao ou 2 para entrar com o diretorio do arquivo: ");
        scanf("%d", &opcao);
        switch (opcao){
            case 1:
                arq = fopen("./memoria/InstrucoesCompletas.mem", "r");
                break;

            case 2:
                printf("Entre com o caminho/nome do arquivo incluindo a extenção .mem\n");
                scanf(" %[^\n]", nome_arquivo);
                arq = fopen(nome_arquivo, "r");
                break;

            default:
                printf("Opcao inexistente. Utilizando o diretorio padrao.\n");
                arq = fopen("./memoria/testaTodas.mem", "r");
                break;
        }

    if(arq){

        //primeiro, conto quantas linhas de instruçoes terei no arquivo
        while(fgets(linha, sizeof(linha), arq) != NULL){
            if (strlen(linha) > 17){
                system("clear");
                fprintf(stderr, "OVERFLOW. Numero de bits maior que 16."); //FLAG OVERFLOW
                sleep(2);
                return -1;
            }
            contador_de_linhas++;
        }

        

        *tamanho_linhas = contador_de_linhas;

        if (!memInst) { //se memInst nao ter nada alocado, dará erro.
            system("clear");
            fprintf(stderr, "Falha na alocação de memória para instruções.\n"); //stderr envia mensagem de erro
            sleep(2);
            //separadamente do fluxo principal de saída de um programa
            return 1;
        }

        // Reinicia o arquivo para ler desde a 1° linha
        rewind(arq);

        int i = 0;
        
        while((i<contador_de_linhas) && (i<=255)){
            if (fgets(linha, sizeof(linha), arq) == NULL){
                remove_newline(linha);
                break;
            }

            if(linha[0] == '\n'){
                i++;
            }
            else{
                // Copia a linha para a estrutura memInst
                strncpy(memInst[i].instruc, linha, 17);
                memInst[i].instruc[16] = '\0'; // certifica-se de que a string termina com null terminator
                i++;
            }
                   }
        
        fclose(arq);
        printf("Arquivo lido com sucesso!");
    }
    

    else{
        system("clear");
        fprintf(stderr, "Erro ao abrir arquivo de instrucoes.");
        sleep(2);
    }
}

void remove_newline(char *line) { //remove a quebra de linha
    size_t len = strcspn(line, "\n");
    if (line[len] == '\n') {
        line[len] = '\0';
    }
}