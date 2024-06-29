#include "../Arquivos-h/controller.h"

int controller(int op, int *StateForBack, int NumeroLinhas, int *regs, instrucao *memInst, MemoriaDados *memDados, int *program_counter, type_instruc *instrucoesDecodificadas, IF *regif, ID *id, EX *ex, MEM *mem, WB *wb, Sinais *sinal, int ProxEtapa){
    int jump, RD, RT, i, a=0, verifica_fim = 0, immediate, dados, pc;
    char posicao[4];

    switch (op)
    {
        case 1:
            while (*program_counter <= (NumeroLinhas))
            {        
                switch (ProxEtapa)
                {
                case 1:///Etapa IF -> Recebe Instrução e Incrementa program_counter
                printf("\nEtapa IF\n");
                regif->pc = *program_counter;
                
                strcpy(regif->instruc, memInst[regif->pc].instruc);
                if (strcmp(regif->instruc, memInst[regif->pc].instruc) == 0){
                        printf("Instrucao coletada com sucesso! Foi lido %s\n", regif->instruc);
                }
                else
                    printf("Instrução incorreta! Foi lido %s ao inves de %s\n", regif->instruc, memInst[regif->pc].instruc);
                

                increment_PC(program_counter, 1);
                    
                increment_State(StateForBack, 1); 

                controller(1, StateForBack, NumeroLinhas, regs, memInst, memDados, program_counter, instrucoesDecodificadas, regif, id, ex, mem, wb, sinal, 2);
                break;
                
            case 2://Etapa ID -> Decodifico as instruções, gero os sinais e Adiciono valores aos registradores auxiliares
                strcpy(id->instruc, regif->instruc);
                id->pc = regif->pc;
                //chamo controller STEP pra rodar APENAS a etapa que quero, ja que a mesma esta livre.
                    controller(2, StateForBack, NumeroLinhas, regs, memInst, memDados, program_counter, instrucoesDecodificadas, regif, id, ex, mem, wb, sinal, 1);  
                printf("Etapa %d\n", ProxEtapa);

                if(id->sinal != NULL){
                    free(id->sinal);
                    id->sinal = NULL;
                }
                
                id->sinal = inicializaSinais();
                instrucoesDecodificadas[id->pc] = decoder(memInst,id->pc); //decodificou
                id->sinal = AddSinais(instrucoesDecodificadas[id->pc], sinal);

                //OPREANDOS LIDOS, SE NECESSARIO:
                id->readData1 = regs[sinal->RS];
                id->readData2 = regs[sinal->RT];

                imprimeRegsPipeline(regif, id, ex, mem, 2);   
                increment_State(StateForBack, 1); 
                controller(1, StateForBack, NumeroLinhas, regs, memInst, memDados, program_counter, instrucoesDecodificadas, regif, id, ex, mem, wb, sinal, 3);
                break;
                    
            case 3: //Etapa EX --> Executa tipo R e Addi, Calcula Endereço LW e SW 
                strcpy(ex->instruc, id->instruc);
                ex->pc = id->pc;
                ex->readData1 = id->readData1;
                ex->readData2 = id->readData2;
                ex->sinal = id->sinal;
                controller(2, StateForBack, NumeroLinhas, regs, memInst, memDados, program_counter, instrucoesDecodificadas, regif, id, ex, mem, wb, sinal, 2);
                printf("\nEtapa EX\n");
                if (ex->sinal->tipo == 1)//verifica se é Jump
                {
                    pc = ex->pc;
                    jump = ULA(instrucoesDecodificadas, &pc, memDados, regs);
                    (*program_counter) = jump;
                    a++;
                    printf("\ntipo: %d\b", sinal->tipo);
                    printf("%d jump/loop concluido.\t\t", a);

                    //imprimeRegsAux(aux);       
                    increment_State(StateForBack, 1); 
                    
                    controller(1, StateForBack, NumeroLinhas, regs, memInst, memDados, program_counter, instrucoesDecodificadas, regif, id, ex, mem, wb, sinal, 1);
                }
                
                else if (ex->sinal->tipo == 0 || ex->sinal->tipo == 2 || ex->sinal->tipo == 3 || ex->sinal->tipo == 4)
                //verifica se é tipo R (0), addi (2), lw (3) ou sw (4)
                {
                    pc = ex->pc;
                    ex->aluResult = ULA(instrucoesDecodificadas, &pc, memDados, regs); 
                    
                    increment_State(StateForBack, 1); 
                    controller(1, StateForBack, NumeroLinhas, regs, memInst, memDados, program_counter, instrucoesDecodificadas, regif, id, ex, mem, wb, sinal, 4);
                }
                
                else if (ex->sinal->tipo == 5)//verifica se é beq
                {
                    ULA(instrucoesDecodificadas, &pc, memDados, regs);
                    if (ex->readData1 == ex->readData2){
                        *program_counter = ex->aluResult;
                        
                        increment_State(StateForBack, 1); 
                        controller(1, StateForBack, NumeroLinhas, regs, memInst, memDados, program_counter, instrucoesDecodificadas, regif, id, ex, mem, wb, sinal, 1);
                    }
                    else {
                        controller(1, StateForBack, NumeroLinhas, regs, memInst, memDados, program_counter, instrucoesDecodificadas, regif, id, ex, mem, wb, sinal, 1);
                    }
                }

                break;
                    
            case 4: // Se a instrução envolve leitura ou escrita na memória de dados, essa operação é realizada neste estágio
                mem->pc = ex->pc;
                strcpy(mem->instruc, ex->instruc);
                mem->aluResult = ex->aluResult;
                mem->sinal = ex->sinal;

                controller(2, StateForBack, NumeroLinhas, regs, memInst, memDados, program_counter, instrucoesDecodificadas, regif, id, ex, mem, wb, sinal, 3);

                printf("\nEtapa MEM\n");
                if (mem->sinal->tipo == 3) // lw (load word)
                {
                    // Carregar dado da memória
                    strcpy(mem->readData, memDados[mem->aluResult].dados); //copio para o registrador de dados, o dado da memoria
                    
                    
                        
                    increment_State(StateForBack, 1); 
                    controller(1, StateForBack, NumeroLinhas, regs, memInst, memDados, program_counter, instrucoesDecodificadas, regif, id, ex, mem, wb, sinal, 5);
                }
                    else if (mem->sinal->tipo == 4) // sw (store word)
                    {
                        // Armazenar dado na memória
                        decimalToBinary(sinal->RT, posicao);
                        int conteudo = retornoRegs(regs, posicao);
                        char conteudo_bin[9];
                        conteudo_bin[8]='\0';

                        if (conteudo > 127 || conteudo < -128){
                            fprintf(stderr, "OVERFLOW. Numero a ser escrito na memoria de dados ultrapassa os 8 bits.\n");
                            if (conteudo > 127)
                                strcpy(conteudo_bin, "01111111"); //Escreve 127
                            else
                                strcpy(conteudo_bin, "10000000"); //Escreve -128
                                escreveDado(memDados, mem->aluResult, conteudo_bin);
                        }
                        decimalToBinary(conteudo, conteudo_bin);
                        escreveDado(memDados, mem->aluResult, conteudo_bin);

                        
                        (*StateForBack)++;
                        controller(1, StateForBack, NumeroLinhas, regs, memInst, memDados, program_counter, instrucoesDecodificadas, regif, id, ex, mem, wb, sinal, 1);
                    }
                    break;

                case 5: //Etapa WB (write back) -> O resultado da operação é escrito de volta no registrador destino no banco de registradores.          
                    wb->pc = mem->pc;
                    strcpy(wb->instruc, mem->instruc);
                    wb->aluResult = mem->aluResult;
                    wb->sinal = mem->sinal;
                    controller(2, StateForBack, NumeroLinhas, regs, memInst, memDados, program_counter, instrucoesDecodificadas, regif, id, ex, mem, wb, sinal, 4);
                    printf("\nEtapa WB\n");
                    
                    if (wb->sinal->tipo == 3){ // lw (load word)
                        decimalToBinary(sinal->RT, posicao);
                        escritaRegistradores(regs, wb->aluResult, posicao); // Load: Reg[IR[20:16]] <= memoriaR    
                        increment_State(StateForBack, 1); 
                        controller(1, StateForBack, NumeroLinhas, regs, memInst, memDados, program_counter, instrucoesDecodificadas, regif, id, ex, mem, wb, sinal, 1);
                    }

                    else if (wb->sinal->tipo == 0) // R-type
                    {
                        decimalToBinary(sinal->RD, posicao);
                        escritaRegistradores(regs, wb->aluResult, posicao);
                        (*StateForBack)++;
                        controller(1, StateForBack, NumeroLinhas, regs, memInst, memDados, program_counter, instrucoesDecodificadas, regif, id, ex, mem, wb, sinal, 1);
                    }

                    else if (wb->sinal->tipo == 2) // addi
                    {
                        decimalToBinary(sinal->RT, posicao);
                        escritaRegistradores(regs, wb->aluResult, posicao);
                        (*StateForBack)++;                                   
                        controller(1, StateForBack, NumeroLinhas, regs, memInst, memDados, program_counter, instrucoesDecodificadas, regif, id, ex, mem, wb, sinal, 1);
                    }
                    break;
                    
                    default:
                        printf ("%d", wb->pc);
                    break;
                }        
            
        break;

        case 2:
            //Run by Step
            switch (ProxEtapa)
            {
            case 1:///Etapa IF -> Recebe Instrução e Incrementa program_counter
                printf("\nEtapa IF\n");
                regif->pc = *program_counter;
                
                strcpy(regif->instruc, memInst[regif->pc].instruc);
                if (strcmp(regif->instruc, memInst[regif->pc].instruc) == 0){
                        printf("Instrucao coletada com sucesso! Foi lido %s\n", regif->instruc);
                }
                else
                    printf("Instrução incorreta! Foi lido %s ao inves de %s\n", regif->instruc, memInst[regif->pc].instruc);
                

                increment_PC(program_counter, 1);
                    
                increment_State(StateForBack, 1); 

                return 2;
                break;
                
            case 2://Etapa ID -> Decodifico as instruções, gero os sinais e Adiciono valores aos registradores auxiliares
                strcpy(id->instruc, regif->instruc);
                id->pc = regif->pc;
                //chamo controller pra rodar a primeira etapa, ja que ela esta livre.
                controller(2, StateForBack, NumeroLinhas, regs, memInst, memDados, program_counter, instrucoesDecodificadas, regif, id, ex, mem, wb, sinal, 1);  
                printf("Etapa %d\n", ProxEtapa);

                id->sinal = inicializaSinais();
                instrucoesDecodificadas[id->pc] = decoder(memInst, id->pc); //decodificou
                id->sinal = AddSinais(instrucoesDecodificadas[id->pc], id->sinal);

                //OPREANDOS LIDOS, SE NECESSARIO:
                id->readData1 = regs[id->sinal->RS];
                id->readData2 = regs[id->sinal->RT];

                imprimeRegsPipeline(regif, id, ex, mem, 2);   
                increment_State(StateForBack, 1); 
                return 3;
                break;
                    
            case 3: //Etapa EX --> Executa tipo R e Addi, Calcula Endereço LW e SW 
                strcpy(ex->instruc, id->instruc);
                ex->pc = id->pc;
                ex->readData1 = id->readData1;
                ex->readData2 = id->readData2;
                ex->sinal = id->sinal;
                controller(2, StateForBack, NumeroLinhas, regs, memInst, memDados, program_counter, instrucoesDecodificadas, regif, id, ex, mem, wb, sinal, 2);
                printf("\nEtapa EX\n");
                if (ex->sinal->tipo == 1)//verifica se é Jump
                {
                    pc = ex->pc;
                    jump = ULA(instrucoesDecodificadas, &pc, memDados, regs);
                    (*program_counter) = jump;
                    a++;
                    printf("\ntipo: %d\b", sinal->tipo);
                    printf("%d jump/loop concluido.\t\t", a);

                    //imprimeRegsAux(aux);       
                    increment_State(StateForBack, 1); 
                    
                    return 1;
                }
                
                else if (ex->sinal->tipo == 0 || ex->sinal->tipo == 2 || ex->sinal->tipo == 3 || ex->sinal->tipo == 4)
                //verifica se é tipo R (0), addi (2), lw (3) ou sw (4)
                {
                    pc = ex->pc;
                    ex->aluResult = ULA(instrucoesDecodificadas, &pc, memDados, regs); 
                    
                    increment_State(StateForBack, 1); 
                    return 4;
                }
                
                else if (ex->sinal->tipo == 5)//verifica se é beq
                {
                    ULA(instrucoesDecodificadas, &pc, memDados, regs);
                    if (ex->readData1 == ex->readData2){
                        *program_counter = ex->aluResult;
                        
                        increment_State(StateForBack, 1); 
                        return 1;
                    }
                    else {
                        return 1;
                    }
                }

                break;
                    
            case 4: // Se a instrução envolve leitura ou escrita na memória de dados, essa operação é realizada neste estágio
                mem->pc = ex->pc;
                strcpy(mem->instruc, ex->instruc);
                mem->aluResult = ex->aluResult;
                mem->sinal = ex->sinal;

                controller(2, StateForBack, NumeroLinhas, regs, memInst, memDados, program_counter, instrucoesDecodificadas, regif, id, ex, mem, wb, sinal, 3);

                printf("\nEtapa MEM\n");
                if (mem->sinal->tipo == 3) // lw (load word)
                {
                    // Carregar dado da memória
                    strcpy(mem->readData, memDados[mem->aluResult].dados); //copio para o registrador de dados, o dado da memoria
                    
                    
                        
                    increment_State(StateForBack, 1); 
                    return 5;
                }
                else if (mem->sinal->tipo == 4) // sw (store word)
                {
                        // Armazenar dado na memória
                        decimalToBinary(sinal->RT, posicao);
                        int conteudo = retornoRegs(regs, posicao);
                        char conteudo_bin[9];
                        conteudo_bin[8]='\0';

                        if (conteudo > 127 || conteudo < -128){
                            fprintf(stderr, "OVERFLOW. Numero a ser escrito na memoria de dados ultrapassa os 8 bits.\n");
                            if (conteudo > 127)
                                strcpy(conteudo_bin, "01111111"); //Escreve 127
                            else
                                strcpy(conteudo_bin, "10000000"); //Escreve -128
                                escreveDado(memDados, mem->aluResult, conteudo_bin);
                        }
                        decimalToBinary(conteudo, conteudo_bin);
                        escreveDado(memDados, mem->aluResult, conteudo_bin);

                        
                        (*StateForBack)++;
                        return 1;
                }
                break;

                case 5: //Etapa WB (write back) -> O resultado da operação é escrito de volta no registrador destino no banco de registradores.          
                    wb->pc = mem->pc;
                    strcpy(wb->instruc, mem->instruc);
                    wb->aluResult = mem->aluResult;
                    wb->sinal = mem->sinal;
                    controller(2, StateForBack, NumeroLinhas, regs, memInst, memDados, program_counter, instrucoesDecodificadas, regif, id, ex, mem, wb, sinal, 4);
                    printf("\nEtapa WB\n");
                    
                    if (wb->sinal->tipo == 3){ // lw (load word)
                        decimalToBinary(sinal->RT, posicao);
                        escritaRegistradores(regs, wb->aluResult, posicao); // Load: Reg[IR[20:16]] <= memoriaR    
                        increment_State(StateForBack, 1); 
                        return 1;
                    }

                    else if (wb->sinal->tipo == 0) // R-type
                    {
                        decimalToBinary(sinal->RD, posicao);
                        escritaRegistradores(regs, wb->aluResult, posicao);
                        (*StateForBack)++;
                        return 1;
                    }

                    else if (wb->sinal->tipo == 2) // addi
                    {
                        decimalToBinary(sinal->RT, posicao);
                        escritaRegistradores(regs, wb->aluResult, posicao);
                        (*StateForBack)++;                                   
                        return 1;
                    }
                    break;
                }
        break;
        }
    }
}



/*int backstep(int BackAux, int *StateForBack, int NumeroLinhas, int *regs, instrucao *memInst, MemoriaDados *memDados, int *program_counter, type_instruc *instrucoesDecodificadas, IF *regif, ID *id, EX *ex, MEM *mem, WB *wb, Sinais *sinal, int ProxEtapa){
    int jump, RD, RT, i, a=0, verifica_fim = 0, immediate, dados, pc;
    char posicao[4];
    while (*StateForBack < BackAux)
    {      
        switch (ProxEtapa)
        {
            case 1:
            while (*program_counter <= (NumeroLinhas))
            {        
                switch (ProxEtapa)
                {
                case 1:///Etapa IF -> Recebe Instrução e Incrementa program_counter
                printf("\nEtapa IF\n");
                regif->pc = *program_counter;
                
                strcpy(regif->instruc, memInst[regif->pc].instruc);
                if (strcmp(regif->instruc, memInst[regif->pc].instruc) == 0){
                        printf("Instrucao coletada com sucesso! Foi lido %s\n", regif->instruc);
                }
                else
                    printf("Instrução incorreta! Foi lido %s ao inves de %s\n", regif->instruc, memInst[regif->pc].instruc);
                

                increment_PC(program_counter, 1);
                    
                increment_State(StateForBack, 1); 

                controller(1, StateForBack, NumeroLinhas, regs, memInst, memDados, program_counter, instrucoesDecodificadas, regif, id, ex, mem, wb, sinal, 2);
                break;
                
            case 2://Etapa ID -> Decodifico as instruções, gero os sinais e Adiciono valores aos registradores auxiliares
                strcpy(id->instruc, regif->instruc);
                id->pc = regif->pc;
                //chamo controller pra rodar a primeira etapa, ja que ela esta livre.
                controller(1, StateForBack, NumeroLinhas, regs, memInst, memDados, program_counter, instrucoesDecodificadas, regif, id, ex, mem, wb, sinal, 1);  
                printf("Etapa %d\n", ProxEtapa);

                free(sinal);
                sinal = inicializaSinais();
                instrucoesDecodificadas[id->pc] = decoder(memInst, id->pc); //decodificou
                sinal = AddSinais(instrucoesDecodificadas[id->pc], sinal);

                //OPREANDOS LIDOS, SE NECESSARIO:
                id->readData1 = regs[sinal->RS];
                id->readData2 = regs[sinal->RT];

                imprimeRegsPipeline(regif, id, ex, mem, 2);   
                increment_State(StateForBack, 1); 
                controller(1, StateForBack, NumeroLinhas, regs, memInst, memDados, program_counter, instrucoesDecodificadas, regif, id, ex, mem, wb, sinal, 3);
                break;
                    
            case 3: //Etapa EX --> Executa tipo R e Addi, Calcula Endereço LW e SW 
                strcpy(ex->instruc, id->instruc);
                ex->pc = id->pc;
                ex->readData1 = id->readData1;
                ex->readData2 = id->readData2;
                controller(1, StateForBack, NumeroLinhas, regs, memInst, memDados, program_counter, instrucoesDecodificadas, regif, id, ex, mem, wb, sinal, 2);
                printf("\nEtapa EX\n");
                if (sinal->tipo == 1)//verifica se é Jump
                {
                    pc = ex->pc;
                    jump = ULA(instrucoesDecodificadas, &pc, memDados, regs);
                    (*program_counter) = jump;
                    a++;
                    printf("\ntipo: %d\b", sinal->tipo);
                    printf("%d jump/loop concluido.\t\t", a);

                    //imprimeRegsAux(aux);       
                    increment_State(StateForBack, 1); 
                    
                    controller(1, StateForBack, NumeroLinhas, regs, memInst, memDados, program_counter, instrucoesDecodificadas, regif, id, ex, mem, wb, sinal, 1);
                }
                
                else if (sinal->tipo == 0 || sinal->tipo == 2 || sinal->tipo == 3 || sinal->tipo == 4)
                //verifica se é tipo R (0), addi (2), lw (3) ou sw (4)
                {
                    pc = ex->pc;
                    ex->aluResult = ULA(instrucoesDecodificadas, &pc, memDados, regs); 
                    
                    increment_State(StateForBack, 1); 
                    controller(1, StateForBack, NumeroLinhas, regs, memInst, memDados, program_counter, instrucoesDecodificadas, regif, id, ex, mem, wb, sinal, 4);
                }
                
                else if (sinal->tipo == 5)//verifica se é beq
                {
                    ULA(instrucoesDecodificadas, &pc, memDados, regs);
                    if (ex->readData1 == ex->readData2){
                        *program_counter = ex->aluResult;
                        
                        increment_State(StateForBack, 1); 
                        controller(1, StateForBack, NumeroLinhas, regs, memInst, memDados, program_counter, instrucoesDecodificadas, regif, id, ex, mem, wb, sinal, 1);
                    }
                    else {
                        controller(1, StateForBack, NumeroLinhas, regs, memInst, memDados, program_counter, instrucoesDecodificadas, regif, id, ex, mem, wb, sinal, 1);
                    }
                }

                break;
                    
            case 4: // Se a instrução envolve leitura ou escrita na memória de dados, essa operação é realizada neste estágio
                mem->pc = ex->pc;
                strcpy(mem->instruc, ex->instruc);
                mem->aluResult = ex->aluResult;

                controller(2, StateForBack, NumeroLinhas, regs, memInst, memDados, program_counter, instrucoesDecodificadas, regif, id, ex, mem, wb, sinal, 3);

                printf("\nEtapa MEM\n");
                if (sinal->tipo == 3) // lw (load word)
                {
                    // Carregar dado da memória
                    strcpy(mem->readData, memDados[mem->aluResult].dados); //copio para o registrador de dados, o dado da memoria
                    
                    
                        
                    increment_State(StateForBack, 1); 
                    controller(1, StateForBack, NumeroLinhas, regs, memInst, memDados, program_counter, instrucoesDecodificadas, regif, id, ex, mem, wb, sinal, 5);
                }
                    else if (sinal->tipo == 4) // sw (store word)
                    {
                        // Armazenar dado na memória
                        decimalToBinary(sinal->RT, posicao);
                        int conteudo = retornoRegs(regs, posicao);
                        char conteudo_bin[9];
                        conteudo_bin[8]='\0';

                        if (conteudo > 127 || conteudo < -128){
                            fprintf(stderr, "OVERFLOW. Numero a ser escrito na memoria de dados ultrapassa os 8 bits.\n");
                            if (conteudo > 127)
                                strcpy(conteudo_bin, "01111111"); //Escreve 127
                            else
                                strcpy(conteudo_bin, "10000000"); //Escreve -128
                                escreveDado(memDados, mem->aluResult, conteudo_bin);
                        }
                        decimalToBinary(conteudo, conteudo_bin);
                        escreveDado(memDados, mem->aluResult, conteudo_bin);

                        
                        (*StateForBack)++;
                        controller(1, StateForBack, NumeroLinhas, regs, memInst, memDados, program_counter, instrucoesDecodificadas, regif, id, ex, mem, wb, sinal, 1);
                    }
                    break;

                case 5: //Etapa WB (write back) -> O resultado da operação é escrito de volta no registrador destino no banco de registradores.          
                    wb->pc = mem->pc;
                    strcpy(wb->instruc, mem->instruc);
                    wb->aluResult = mem->aluResult;
                    controller(2, StateForBack, NumeroLinhas, regs, memInst, memDados, program_counter, instrucoesDecodificadas, regif, id, ex, mem, wb, sinal, 4);
                    printf("\nEtapa WB\n", ProxEtapa);
                    
                    if (sinal->tipo == 3){ // lw (load word)
                        decimalToBinary(sinal->RT, posicao);
                        escritaRegistradores(regs, wb->aluResult, posicao); // Load: Reg[IR[20:16]] <= memoriaR    
                        increment_State(StateForBack, 1); 
                        controller(1, StateForBack, NumeroLinhas, regs, memInst, memDados, program_counter, instrucoesDecodificadas, regif, id, ex, mem, wb, sinal, 1);
                    }

                    else if (sinal->tipo == 0) // R-type
                    {
                        decimalToBinary(sinal->RD, posicao);
                        escritaRegistradores(regs, wb->aluResult, posicao);
                        (*StateForBack)++;
                        controller(1, StateForBack, NumeroLinhas, regs, memInst, memDados, program_counter, instrucoesDecodificadas, regif, id, ex, mem, wb, sinal, 1);
                    }

                    else if (sinal->tipo == 2) // addi
                    {
                        decimalToBinary(sinal->RT, posicao);
                        escritaRegistradores(regs, wb->aluResult, posicao);
                        (*StateForBack)++;                                   
                        controller(1, StateForBack, NumeroLinhas, regs, memInst, memDados, program_counter, instrucoesDecodificadas, regif, id, ex, mem, wb, sinal, 1);
                    }
                    break;
                    
                    default:
                        printf ("%d", wb->pc);
                    break;
            }        
        }

    }
    }
}*/