#include "../Arquivos-h/controller.h"

int controller(int op, int NumeroLinhas, int *regs, instrucao *memInst, MemoriaDados *memDados, int *program_counter, type_instruc *instrucoesDecodificadas, IF *regif, ID *id, EX *ex, MEM *mem, WB *wb, Sinais *sinal, int ProxEtapa, descPilha *descPilha, Backup *backup, NodoPilha *NodoPilha, Assembly *AssemblyInst){
    int jump, RD, RT, i, a=0, verifica_fim = 0, immediate, dados, pc;
    char posicao[4];

    switch (op)
    {
        case 1:
            while (*program_counter <= (NumeroLinhas))
            {        
                backup = ColetaTudo(regs, memDados, regif, id, ex, mem, wb, sinal, AssemblyInst, program_counter, &ProxEtapa);
                NodoPilha = inicializaNodo(backup);
                descPilha = PUSH(descPilha, NodoPilha);
                switch (ProxEtapa)
                {
                case 1:///Etapa IF -> Recebe Instrução e Incrementa program_counter 
                    regif->pc = *program_counter;
                    
                    if(regif->pc == NumeroLinhas){
                        printf("\nEtapa IF encerrada.\n");
                        controller(1, NumeroLinhas, regs, memInst, memDados, program_counter, instrucoesDecodificadas, regif, id, ex, mem, wb, sinal, 2, descPilha, backup, NodoPilha, AssemblyInst);
                    }
                    strcpy(regif->instruc, memInst[regif->pc].instruc);
                    printf("\nEtapa IF: %s", regif->instruc);
                    increment_PC(program_counter, 1);

                    controller(1, NumeroLinhas, regs, memInst, memDados, program_counter, instrucoesDecodificadas, regif, id, ex, mem, wb, sinal, 2, descPilha, backup, NodoPilha, AssemblyInst);
                    break;
                
                case 2://Etapa ID -> Decodifico as instruções, gero os sinais e Adiciono valores aos registradores auxiliares
                    strcpy(id->instruc, regif->instruc);          
                    id->pc = regif->pc;
                    if(id->pc == NumeroLinhas){
                        printf("\nEtapa ID encerrada.");
                        printf("\n╚═════════════════════════╝");
                        controller(1, NumeroLinhas, regs, memInst, memDados, program_counter, instrucoesDecodificadas, regif, id, ex, mem, wb, sinal, 3, descPilha, backup, NodoPilha, AssemblyInst);
                    }
                    printf("\nEtapa ID: %s", id->instruc);
                    printf("\n╚═════════════════════════╝");
                    //chamo controller pra rodar a primeira etapa, ja que ela esta livre.
                    controller(2, NumeroLinhas, regs, memInst, memDados, program_counter, instrucoesDecodificadas, regif, id, ex, mem, wb, sinal, 1, descPilha, backup, NodoPilha, AssemblyInst);  
                
                    id->sinal = inicializaSinais();
                    instrucoesDecodificadas[id->pc] = decoder(memInst, id->pc); //decodificou
                    id->sinal = AddSinais(instrucoesDecodificadas[id->pc], id->sinal);

                    //OPREANDOS LIDOS, SE NECESSARIO:
                    id->readData1 = regs[id->sinal->RS];
                    id->readData2 = regs[id->sinal->RT];
                    controller(1, NumeroLinhas, regs, memInst, memDados, program_counter, instrucoesDecodificadas, regif, id, ex, mem, wb, sinal, 3, descPilha, backup, NodoPilha, AssemblyInst);
                    break;
                    
                case 3: //Etapa EX --> Executa tipo R e Addi, Calcula Endereço LW e SW 
                    strcpy(ex->instruc, id->instruc);
                    ex->pc = id->pc;
                    ex->readData1 = id->readData1;
                    ex->readData2 = id->readData2;
                    ex->sinal = id->sinal;
                    if(ex->pc == NumeroLinhas){
                        printf("\nEtapa EX encerrada.");
                        printf("\n╚═════════════════════════╝");
                        controller(1, NumeroLinhas, regs, memInst, memDados, program_counter, instrucoesDecodificadas, regif, id, ex, mem, wb, sinal, 4, descPilha, backup, NodoPilha, AssemblyInst);
                    }
                    printf("\nEtapa EX: %s", ex->instruc);
                    controller(2, NumeroLinhas, regs, memInst, memDados, program_counter, instrucoesDecodificadas, regif, id, ex, mem, wb, sinal, 2, descPilha, backup, NodoPilha, AssemblyInst);
                    
                    if (ex->sinal->tipo == 1)//verifica se é Jump
                    {
                        pc = ex->pc;
                        jump = ULA(instrucoesDecodificadas, &pc, memDados, regs);
                        (*program_counter) = jump;
                        a++;
                        printf("\ntipo: %d\b", sinal->tipo);
                        printf("%d jump/loop concluido.\t\t", a); 
                        
                        controller(1, NumeroLinhas, regs, memInst, memDados, program_counter, instrucoesDecodificadas, regif, id, ex, mem, wb, sinal, 1, descPilha, backup, NodoPilha, AssemblyInst);
                    }
                    
                    else if (ex->sinal->tipo == 0 || ex->sinal->tipo == 2 || ex->sinal->tipo == 3 || ex->sinal->tipo == 4)
                    //verifica se é tipo R (0), addi (2), lw (3) ou sw (4)
                    {
                        pc = ex->pc;
                        ex->aluResult = ULA(instrucoesDecodificadas, &pc, memDados, regs);
                        controller(1, NumeroLinhas, regs, memInst, memDados, program_counter, instrucoesDecodificadas, regif, id, ex, mem, wb, sinal, 4, descPilha, backup, NodoPilha, AssemblyInst);
                    }
                    
                    else if (ex->sinal->tipo == 5)//verifica se é beq
                    {
                        pc = ex->pc;
                        ex->aluResult = ULA(instrucoesDecodificadas, &pc, memDados, regs);
                        if (ex->readData1 == ex->readData2){
                            *program_counter = ex->aluResult; 
                            controller(1, NumeroLinhas, regs, memInst, memDados, program_counter, instrucoesDecodificadas, regif, id, ex, mem, wb, sinal, 1, descPilha, backup, NodoPilha, AssemblyInst);
                        }
                        else {
                            controller(1, NumeroLinhas, regs, memInst, memDados, program_counter, instrucoesDecodificadas, regif, id, ex, mem, wb, sinal, 1, descPilha, backup, NodoPilha, AssemblyInst);
                        }
                    }

                    break;
                        
                case 4: // Se a instrução envolve leitura ou escrita na memória de dados, essa operação é realizada neste estágio
                    mem->pc = ex->pc;
                    strcpy(mem->instruc, ex->instruc);
                    mem->aluResult = ex->aluResult;
                    mem->sinal = ex->sinal;
                    if(mem->pc == NumeroLinhas){
                        printf("\nEtapa MEM encerrada.");
                        printf("\n╚═════════════════════════╝");
                        controller(1, NumeroLinhas, regs, memInst, memDados, program_counter, instrucoesDecodificadas, regif, id, ex, mem, wb, sinal, 5, descPilha, backup, NodoPilha, AssemblyInst);
                    }
                    printf("\nEtapa MEM: %s", mem->instruc);
                    controller(2, NumeroLinhas, regs, memInst, memDados, program_counter, instrucoesDecodificadas, regif, id, ex, mem, wb, sinal, 3, descPilha, backup, NodoPilha, AssemblyInst);
                    
                    if (mem->sinal->tipo == 3) // lw (load word)
                    {
                        // Carregar dado da memória
                        strcpy(mem->readData, memDados[mem->aluResult].dados); //copio para o registrador de dados, o dado da memoria
                        
                        controller(1, NumeroLinhas, regs, memInst, memDados, program_counter, instrucoesDecodificadas, regif, id, ex, mem, wb, sinal, 5, descPilha, backup, NodoPilha, AssemblyInst);
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
                        controller(1, NumeroLinhas, regs, memInst, memDados, program_counter, instrucoesDecodificadas, regif, id, ex, mem, wb, sinal, 1, descPilha, backup, NodoPilha, AssemblyInst);
                    }
                    controller(1, NumeroLinhas, regs, memInst, memDados, program_counter, instrucoesDecodificadas, regif, id, ex, mem, wb, sinal, 5, descPilha, backup, NodoPilha, AssemblyInst);
                    break;

                    case 5: //Etapa WB (write back) -> O resultado da operação é escrito de volta no registrador destino no banco de registradores.          
                        wb->pc = mem->pc;
                        strcpy(wb->instruc, mem->instruc);
                        printf("\n╔═════════════════════════╗");
                        wb->aluResult = mem->aluResult;
                        wb->sinal = mem->sinal;
                        if(wb->pc == NumeroLinhas){
                            printf("\nEtapa WB encerrada.");
                            printf("\n╚═════════════════════════╝");
                            return 6;
                        }
                        printf("\nEtapa WB: %s", wb->instruc);
                        controller(2, NumeroLinhas, regs, memInst, memDados, program_counter, instrucoesDecodificadas, regif, id, ex, mem, wb, sinal, 4, descPilha, backup, NodoPilha, AssemblyInst);    
                        
                        if (wb->sinal->tipo == 3){ // lw (load word)
                            decimalToBinary(wb->sinal->RT, posicao);
                            escritaRegistradores(regs, wb->aluResult, posicao); // Load: Reg[IR[20:16]] <= memoriaR    
                            controller(1, NumeroLinhas, regs, memInst, memDados, program_counter, instrucoesDecodificadas, regif, id, ex, mem, wb, sinal, 5, descPilha, backup, NodoPilha, AssemblyInst);
                        }

                        else if (wb->sinal->tipo == 0) // R-type
                        {
                            decimalToBinary(wb->sinal->RD, posicao);
                            escritaRegistradores(regs, wb->aluResult, posicao);
                            controller(1, NumeroLinhas, regs, memInst, memDados, program_counter, instrucoesDecodificadas, regif, id, ex, mem, wb, sinal, 5, descPilha, backup, NodoPilha, AssemblyInst);
                        }

                        else if (wb->sinal->tipo == 2) // addi
                        {
                            decimalToBinary(wb->sinal->RT, posicao);
                            escritaRegistradores(regs, wb->aluResult, posicao);                               
                            controller(1, NumeroLinhas, regs, memInst, memDados, program_counter, instrucoesDecodificadas, regif, id, ex, mem, wb, sinal, 5, descPilha, backup, NodoPilha, AssemblyInst);
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
                regif->pc = *program_counter;
                
                if(regif->pc == NumeroLinhas){
                    printf("\nEtapa IF encerrada.");
                    printf("\n╚═════════════════════════╝");
                    return 2;
                }
                strcpy(regif->instruc, memInst[regif->pc].instruc);
                printf("\nEtapa IF: %s", regif->instruc);
                printf("\n╚═════════════════════════╝");
                increment_PC(program_counter, 1);

                return 2;
                break;
                
            case 2://Etapa ID -> Decodifico as instruções, gero os sinais e Adiciono valores aos registradores auxiliares     
                strcpy(id->instruc, regif->instruc);
                id->pc = regif->pc;
                if(id->pc == NumeroLinhas){
                    printf("\nEtapa ID encerrada.");
                    printf("\n╚═════════════════════════╝");
                    return 3;
                }
                printf("\nEtapa ID: %s", id->instruc);
                //chamo controller pra rodar a primeira etapa, ja que ela esta livre.
                controller(2, NumeroLinhas, regs, memInst, memDados, program_counter, instrucoesDecodificadas, regif, id, ex, mem, wb, sinal, 1, descPilha, backup, NodoPilha, AssemblyInst);  
            
                id->sinal = inicializaSinais();
                instrucoesDecodificadas[id->pc] = decoder(memInst, id->pc); //decodificou
                id->sinal = AddSinais(instrucoesDecodificadas[id->pc], id->sinal);

                //OPREANDOS LIDOS, SE NECESSARIO:
                id->readData1 = regs[id->sinal->RS];
                id->readData2 = regs[id->sinal->RT];

                return 3;
                break;
                    
            case 3: //Etapa EX --> Executa tipo R e Addi, Calcula Endereço LW e SW 
                strcpy(ex->instruc, id->instruc);
                ex->pc = id->pc;
                ex->readData1 = id->readData1;
                ex->readData2 = id->readData2;
                ex->sinal = id->sinal;
                if(ex->pc == NumeroLinhas){
                    printf("\nEtapa EX encerrada.");
                    printf("\n╚═════════════════════════╝");
                    return 4;
                }
                printf("\nEtapa EX: %s", ex->instruc);
                controller(2, NumeroLinhas, regs, memInst, memDados, program_counter, instrucoesDecodificadas, regif, id, ex, mem, wb, sinal, 2, descPilha, backup, NodoPilha, AssemblyInst);
                
                if (ex->sinal->tipo == 1)//verifica se é Jump
                {
                    pc = ex->pc;
                    jump = ULA(instrucoesDecodificadas, &pc, memDados, regs);
                    (*program_counter) = jump;
                    a++;
                    printf("\ntipo: %d\b", sinal->tipo);
                    printf("%d jump/loop concluido.\t\t", a);
                    
                    return 1;
                }
                
                else if (ex->sinal->tipo == 0 || ex->sinal->tipo == 2 || ex->sinal->tipo == 3 || ex->sinal->tipo == 4)
                //verifica se é tipo R (0), addi (2), lw (3) ou sw (4)
                {
                    pc = ex->pc;
                    ex->aluResult = ULA(instrucoesDecodificadas, &pc, memDados, regs);  
                    return 4;
                }
                
                else if (ex->sinal->tipo == 5)//verifica se é beq
                {
                    pc = ex->pc;
                    ex->aluResult = ULA(instrucoesDecodificadas, &pc, memDados, regs);
                    if (ex->readData1 == ex->readData2){
                        *program_counter = ex->aluResult;
                        return 5;
                    }
                    else {
                        return 5;
                    }
                }

                break;
                    
            case 4: // MEM-> Se a instrução envolve leitura ou escrita na memória de dados, essa operação é realizada neste estágio
                mem->pc = ex->pc;
                strcpy(mem->instruc, ex->instruc);  
                mem->aluResult = ex->aluResult;
                mem->sinal = ex->sinal;
                if(mem->pc == NumeroLinhas){
                    printf("\nEtapa MEM encerrada.");
                    printf("\n╚═════════════════════════╝");
                    return 5;
                }
                printf("\nEtapa MEM: %s", mem->instruc);
                controller(2, NumeroLinhas, regs, memInst, memDados, program_counter, instrucoesDecodificadas, regif, id, ex, mem, wb, sinal, 3, descPilha, backup, NodoPilha, AssemblyInst);
                
                if (mem->sinal->tipo == 3) // lw (load word)
                {
                    // Carregar dado da memória
                    strcpy(mem->readData, memDados[mem->aluResult].dados); //copio para o registrador de dados, o dado da memoria
                    
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
                        return 1;
                }
                return 5;
                break;

                case 5: //Etapa WB (write back) -> O resultado da operação é escrito de volta no registrador destino no banco de registradores.          
                    wb->pc = mem->pc;
                    strcpy(wb->instruc, mem->instruc);
                    printf("\n╔═════════════════════════╗");
                    wb->aluResult = mem->aluResult;
                    wb->sinal = mem->sinal;
                    if(wb->pc == NumeroLinhas){
                        printf("\nEtapa WB encerrada.");
                        printf("\n╚═════════════════════════╝");
                        return 6;
                    }
                    printf("\nEtapa WB: %s", wb->instruc);
                    controller(2, NumeroLinhas, regs, memInst, memDados, program_counter, instrucoesDecodificadas, regif, id, ex, mem, wb, sinal, 4, descPilha, backup, NodoPilha, AssemblyInst);    
                    
                    if (wb->sinal->tipo == 3){ // lw (load word)
                        decimalToBinary(wb->sinal->RT, posicao);
                        escritaRegistradores(regs, wb->aluResult, posicao); // Load: Reg[IR[20:16]] <= memoriaR    
                        return 5;
                    }

                    else if (wb->sinal->tipo == 0) // R-type
                    {
                        decimalToBinary(wb->sinal->RD, posicao);
                        escritaRegistradores(regs, wb->aluResult, posicao);
                        return 5;
                    }

                    else if (wb->sinal->tipo == 2) // addi
                    {
                        decimalToBinary(wb->sinal->RT, posicao);
                        escritaRegistradores(regs, wb->aluResult, posicao);                               
                        return 5;
                    }
                    break;
                }
        break;
        }
    }
}