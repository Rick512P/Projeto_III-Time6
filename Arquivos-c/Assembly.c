#include "../Arquivos-h/Assembly.h"

void AsmCopy(type_instruc *instrucoesDecodificadas, Assembly *A, int tamLinhas){
    char Opcode[10], rd[5], rs[5], rt[5], imediato[8];


    if (A == NULL) {
        fprintf(stderr, "Falha na alocação de memória\n");
        return;
    }

    for (int i = 0; i < tamLinhas; i++) {

            // Garante que a string esteja limpa antes de usar
            memset(A[i].InstructsAssembly, 0, sizeof(A[i].InstructsAssembly));

            // Decodifica rs, rt, rd baseado no conteúdo de rs, rt, rd das instruções
            strcpy(rs, decode_register(instrucoesDecodificadas[i].rs));
            strcpy(rt, decode_register(instrucoesDecodificadas[i].rt));
            strcpy(rd, decode_register(instrucoesDecodificadas[i].rd));

            // Realiza operações baseadas no opcode
            if (strcmp(instrucoesDecodificadas[i].opcode, "0000") == 0) {
                decode_function(instrucoesDecodificadas[i].funct, Opcode);
                sprintf(A[i].InstructsAssembly, "%s %s, %s, %s", Opcode, rd, rs, rt);
            } else if (strcmp(instrucoesDecodificadas[i].opcode, "0100") == 0) {
                sprintf(A[i].InstructsAssembly, "addi %s, %s, %s", rs, rt, instrucoesDecodificadas[i].imm);
            } else if (strcmp(instrucoesDecodificadas[i].opcode, "1011") == 0) {
                sprintf(A[i].InstructsAssembly, "lw %s, MEM[%s + %s]", rt, rs, instrucoesDecodificadas[i].imm);
            } else if (strcmp(instrucoesDecodificadas[i].opcode, "1111") == 0) {
                sprintf(A[i].InstructsAssembly, "sw %s, MEM[%s + %s]", rt, rs, instrucoesDecodificadas[i].imm);
            } else if (strcmp(instrucoesDecodificadas[i].opcode, "1000") == 0) {
                sprintf(A[i].InstructsAssembly, "beq %s, %s, %s", rs, rt, instrucoesDecodificadas[i].imm);
            } else if (strcmp(instrucoesDecodificadas[i].opcode, "0010") == 0) {
                sprintf(A[i].InstructsAssembly, "j %s", instrucoesDecodificadas[i].addr);
            }
        
    }
}


// Funções auxiliares para decodificar rs, rt, rd e funct
char* decode_register(const char *reg) {
    if (strcmp(reg, "000") == 0) return "$0";
    if (strcmp(reg, "001") == 0) return "$1";
    if (strcmp(reg, "010") == 0) return "$2";
    if (strcmp(reg, "011") == 0) return "$3";
    if (strcmp(reg, "100") == 0) return "$4";
    if (strcmp(reg, "101") == 0) return "$5";
    if (strcmp(reg, "110") == 0) return "$6";
    if (strcmp(reg, "111") == 0) return "$7";
    return "";
}

void decode_function(const char *func, char *opcode) {
    if (strcmp(func, "000") == 0) strcpy(opcode, "add");
    else if (strcmp(func, "010") == 0) strcpy(opcode, "sub");
    else if (strcmp(func, "100") == 0) strcpy(opcode, "and");
    else if (strcmp(func, "101") == 0) strcpy(opcode, "or");
    else strcpy(opcode, "");
}

int SaveASM(Assembly *A, int tamLinhas){
    /*Salvar arquivo .asm*/
    FILE *arquivo;
    // Abre o arquivo para escrita (se não existir, será criado)
    arquivo = fopen("Instructs.asm", "w");

    // Verifica se o arquivo foi aberto com sucesso
    if (arquivo == NULL) {
        printf("Erro ao criar o arquivo!\n");
        return 1;
    }
    for (int i = 0; i<tamLinhas; i++){

        fprintf(arquivo, "%s\n", A[i].InstructsAssembly);

    }
    fclose(arquivo);
    printf("Arquivo asm salvo com sucesso!\n");
}

void imprimirASM(Assembly *A, int tamLinhas){
    /*Realizar Impressão da Struct ASM*/
    for (int i = 0; i<tamLinhas; i++){
        if(A[i].InstructsAssembly[0] == '\0'){
            //pula
        }
        else
            printf("\nInstrucao em Assembly: %s", A[i].InstructsAssembly);
    }
    printf("\n");
}

Assembly ASMPrintInstruc(instrucao *memInst, int *program_counter){
    char Opcode[10], rd[5], rs[5], rt[5], imediato[8], auxiliar[3];
    Assembly InstrucaoAssembly;
    auxiliar[3] = '\0';
            strncpy(auxiliar, memInst[*program_counter].instruc + 4, 3);
            strcpy(rs, decode_register(auxiliar));
            strncpy(auxiliar, memInst[*program_counter].instruc + 7, 3);
            strcpy(rt, decode_register(auxiliar));
            strncpy(auxiliar, memInst[*program_counter].instruc + 10, 3);
            strcpy(rd, decode_register(auxiliar));
            if(memInst[*program_counter].instruc[0] == '\0'){
                //faz nada
            }
            else{
                    // Realiza operações baseadas no opcode
                if (strncmp("0000", memInst[*program_counter].instruc, 4) == 0) {
                    decode_function(memInst[*program_counter].instruc + 13, Opcode);
                    sprintf(InstrucaoAssembly.InstructsAssembly, "%s %s, %s, %s", Opcode, rd, rs, rt);
                } 
                else if (strncmp("0100", memInst[*program_counter].instruc, 4) == 0) {
                    sprintf(InstrucaoAssembly.InstructsAssembly, "addi %s, %s, %s", rs, rt, memInst[*program_counter].instruc + 10);
                } 
                else if (strncmp("1011", memInst[*program_counter].instruc, 4) == 0) {
                    sprintf(InstrucaoAssembly.InstructsAssembly, "lw %s, MEM[%s + %s]", rt, rs, memInst[*program_counter].instruc + 10);
                } 
                else if (strncmp("1111", memInst[*program_counter].instruc, 4) == 0) {
                    sprintf(InstrucaoAssembly.InstructsAssembly, "sw %s, MEM[%s + %s}", rt, rs, memInst[*program_counter].instruc + 10);
                } 
                else if (strncmp("1000", memInst[*program_counter].instruc, 4) == 0) {
                    sprintf(InstrucaoAssembly.InstructsAssembly, "beq %s, %s, %s", rs, rt, memInst[*program_counter].instruc + 10);
                } 
                else if (strncmp("0010", memInst[*program_counter].instruc, 4) == 0) {
                    sprintf(InstrucaoAssembly.InstructsAssembly, "j %s", memInst[*program_counter].instruc + 9);
                }

                return InstrucaoAssembly;
            }
            
}