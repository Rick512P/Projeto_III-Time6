#include "../Arquivos-h/ULA.h"


int ULA(type_instruc *instrucoesDecodificadas, int *contador, MemoriaDados *memDados, int *regs) {
  
    int address, rs, rt, rd;

    if (strcmp(instrucoesDecodificadas[*contador].opcode, "0000") == 0 ) {
        char Target[9];
        char Dest[9];
        char Source[9];
        Target[8] = '\0';
        Dest[8] = '\0';
        Source[8] = '\0';

        rs = retornoRegs(regs, instrucoesDecodificadas[*contador].rs);
        rt = retornoRegs(regs, instrucoesDecodificadas[*contador].rt);

        if (strcmp(instrucoesDecodificadas[*contador].funct, "000") == 0 ){
            rd = rs + rt;
            if (rd > 127 || rd < -128){
                system("clear");
                fprintf(stderr, "Overflow. Registrador RD com numero de bits maior que a capacidade suportada.\n");
                sleep(2);
            }
            int local_decimal = rd;

            return rd; //RETORNA PARA O CONTROLLER O INTEIRO PARA O MESMO ARMAZENAR NO REGISTRADOR   
        }

        else if (strcmp(instrucoesDecodificadas[*contador].funct, "010") == 0 ){
            //"sub -> rs - rt = rd";
            rd = rs - rt;
            if (rd > 127 || rd < -128){
                system("clear");
                fprintf(stderr, "Overflow. Registrador RD com numero de bits maior que a capacidade suportada.\n");
                sleep(2);
                return -1;
            }

            return rd; //RETORNA PARA O CONTROLLER O INTEIRO PARA O MESMO ARMAZENAR NO REGISTRADOR
        }

        else if (strcmp(instrucoesDecodificadas[*contador].funct, "100") == 0 ){
            //"and -> rs and rt = rd";
            decimalToBinary(rt, Target);
            decimalToBinary(rs, Source);
            AND(Source, Target, Dest);
            rd = bin_to_decimal(Dest);
            if (rd > 127 || rd < -128){
                system("clear");
                fprintf(stderr, "Overflow. Registrador RD com numero de bits maior que a capacidade suportada.\n");
                sleep(2);
                return -1;
            }
            return rd;
        }

        else if (strcmp(instrucoesDecodificadas[*contador].funct, "101") == 0 ){
            //"or -> rs or rt = rd";
            decimalToBinary(rt, Target);
            decimalToBinary(rs, Source);
            OR(Source, Target, Dest);
            rd = bin_to_decimal(Dest);
            if (rd > 127 || rd < -128){
                system("clear");
                fprintf(stderr, "Overflow. Registrador RD com numero de bits maior que a capacidade suportada.\n");
                sleep(2);
                return -1;
            }
            return rd;
        }
    }

    else if(strcmp(instrucoesDecodificadas[*contador].opcode,"0100") == 0){// addi -> rs + immediate = rt
        int immediate, rs, rt;
        immediate = bin_to_decimal(instrucoesDecodificadas[*contador].imm);
        rs = retornoRegs(regs, instrucoesDecodificadas[*contador].rs);
        if ((immediate + rs) > 127 || (immediate + rs) < -128){
                system("clear");
                fprintf(stderr, "Overflow. Registrador RD com numero de bits maior que a capacidade suportada.\n");
                sleep(2);
        }
        return (immediate + rs); //RETORNA PARA O CONTROLLER O INTEIRO PARA O MESMO ARMAZENAR NO REGISTRADOR
    }

    else if(strcmp(instrucoesDecodificadas[*contador].opcode,"1011") == 0){// lw GRAVA CONTEUDO DA MEMORIA NOS REGISTRADORES
        //$rt = M[$rs + imm]
        int immediate, dados;
        int regSource = retornoRegs(regs, instrucoesDecodificadas[*contador].rs);
        immediate = bin_to_decimal(instrucoesDecodificadas[*contador].imm) + regSource;
        //Agora sei qual a posicao Immediate em decimal:
        dados = bin_to_decimal(memDados[immediate].dados);
        //Agora sei qual o valor contido na posição 4 da memoria em decimal:
        return dados;
    }

    else if(strcmp(instrucoesDecodificadas[*contador].opcode,"1111") == 0){// sw Pega o valor do immediate e armazenará na aluResult dos regsPipeline
        //M[$rs + imm] = $rt
        int immediate;
        int regSource = retornoRegs(regs, instrucoesDecodificadas[*contador].rs);
        immediate = bin_to_decimal(instrucoesDecodificadas[*contador].imm) + regSource;
        return immediate;
    }

    else if(strcmp(instrucoesDecodificadas[*contador].opcode,"0010") == 0){ // j -> jump to specified address
        address = bin_to_decimal(instrucoesDecodificadas[*contador].addr);
        if(address < 0){
            address *= -1; //transforma um possivel valor negativo para positvo, ja que a memoria de dados nao tem posicao negativa.
        }

        if(((*contador+1)+address) > 255){
            system("clear");
            fprintf(stderr, "OVERFLOW. Salto para posicao de memoria inexistente.\n");
            sleep(2);
            return -1;
        }
        return address;
    }

    else if(strcmp(instrucoesDecodificadas[*contador].opcode,"1000") == 0){ //BEQ
        int immediate;
        int reg1 = retornoRegs(regs, instrucoesDecodificadas[*contador].rs);
        int reg2 = retornoRegs(regs, instrucoesDecodificadas[*contador].rt);
        immediate = bin_to_decimal(instrucoesDecodificadas[*contador].imm);
        if(immediate < 0){
            immediate *= -1; //transforma um possivel valor negativo para positvo, ja que a memoria de dados nao tem posicao negativa.
        }
        if (reg1 == reg2){
            if(((*contador+1) + immediate) > 255){
                system("clear");
                fprintf(stderr, "OVERFLOW. PC ultrapassou limite de espaços de memória.\n");
                sleep(2);
                return -1;
            }
                return ((*contador) += immediate);
        }
        else
            return (*contador);    
    }


    else{
        system("clear");
        fprintf(stderr, "OPCODE ERROR!");
        sleep(2);
    }
}

void AND(char Source[], char Target[], char *Dest){
    int i, LS = strlen(Source);
    for (i = 0; i < LS; i++){
        if (Source[i] == '0' || Target[i] == '0'){
            Dest[i] = '0';
        } else {
            Dest[i] = '1';
        }
    }
}

void OR(char Source[], char Target[], char *Dest){
    int i, LS = strlen(Source);
    for (i = 0; i < LS; i++){
        if (Source[i] == '1' || Target[i] == '1'){
            Dest[i] = '1';
        } else {
            Dest[i] = '0';
        }
    }
}
