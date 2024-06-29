#include "../Arquivos-h/carregaMemoriaDados.h"


char* carregamemDados(MemoriaDados *memDados){
    FILE *setmemDados;
    char linha[1024];
    char* nome_arquivo = (char*)malloc(300 * sizeof(char));
    int contador_de_linhas = 0, opcao, error = 0;
    strcpy(nome_arquivo,"./memoria/memDados.dat");
        printf("Digite 1 para utilizar o diretorio padrao ou 2 para entrar com o diretorio do arquivo: ");
        setbuf(stdin, NULL);
        scanf("%d", &opcao);
        switch (opcao){
            case 1:
                setmemDados = fopen(nome_arquivo, "r");
                break;

            case 2:
                printf("Entre com o caminho/nome do arquivo incluindo a extenção .dat\n");
                scanf(" %[^\n]", nome_arquivo);
                setmemDados = fopen(nome_arquivo, "r");
                break;

            default:
                printf("Opcao inexistente. Utilizando o diretorio padrao.\n");
                setmemDados = fopen(nome_arquivo, "r");
                break;
        }
    
    if (setmemDados){
        
        //primeiro, conto quantas linhas de dados terei no arquivo
        while(fgets(linha, sizeof(linha), setmemDados) != NULL){
            if (strlen(linha) > 8){
                fprintf(stderr, "OVERFLOW. Linha %d tem mais de 8 caracteres: %s\n", contador_de_linhas, linha); //FLAG OVERFLOW
                error=1;
            }
                
            contador_de_linhas++;
        }
        
        rewind(setmemDados);
        
        for (int i=0;i<contador_de_linhas;i++){
            if (fgets(linha, sizeof(linha), setmemDados) == NULL)
                   break;

            linha[strcspn(linha, "\r\n")] = '\0';

            // Copia a linha para a estrutura memoria de Dados
            strncpy(memDados[i].dados, linha, 9);
            memDados[i].dados[sizeof(memDados[i].dados) - 1] = '\0'; // certifica-se de que a string termina com null terminator
            
        }
        fclose(setmemDados);
        printf("Arquivo lido com sucesso!\n");
        
    }
    else
        fprintf(stderr, "Erro ao abrir arquivo da Memoria de Dados\n");
    if (error == 0)
        return nome_arquivo;
    else{
        strcpy(nome_arquivo, "ERRO");
        return nome_arquivo;
    }
}

void recarregarmemDados(MemoriaDados *memDados, char *nome_arquivo){
    FILE *setmemDados;
    char linha[1024];
    int contador_de_linhas = 0, opcao;

    setmemDados = fopen(nome_arquivo, "r");

    if (setmemDados){
        
        //primeiro, conto quantas linhas de dados terei no arquivo
        while(fgets(linha, sizeof(linha), setmemDados) != NULL){
            contador_de_linhas++;
        }
        rewind(setmemDados);
        
        for (int i=0;i<contador_de_linhas;i++){
            if (fgets(linha, sizeof(linha), setmemDados) == NULL)
                   break;

            linha[strcspn(linha, "\r\n")] = '\0';

            // Copia a linha para a estrutura memoria de Dados
            strncpy(memDados[i].dados, linha, 9);
            memDados[i].dados[sizeof(memDados[i].dados) - 1] = '\0'; // certifica-se de que a string termina com null terminator
            
        }
        fclose(setmemDados);        
    }

}