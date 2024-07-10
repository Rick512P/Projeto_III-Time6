#include "../Arquivos-h/controller.h"

int controller(int op, int NumeroLinhas, int *regs, instrucao *memInst, MemoriaDados *memDados, int *program_counter, type_instruc *instrucoesDecodificadas, IF *regif, ID *id, EX *ex, MEM *mem, WB *wb, Sinais *sinal, int ProxEtapa, descPilha *descPilha, Backup *backup, NodoPilha *NodoPilha, Assembly *AssemblyInst){
    int jump, RD, RT, i, a=0, verifica_fim = 0, immediate, dados, pc;
    char posicao[4];
    Assembly InstrucaoASM;

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
                    
                    if(regif->pc == NumeroLinhas){ //quando chegar no numero de linhas, encerrará a etapa
                        printf("\nEtapa IF encerrada.");
                        printf("\n╚════════════════════════════╝");
                        controller(1, NumeroLinhas, regs, memInst, memDados, program_counter, instrucoesDecodificadas, regif, id, ex, mem, wb, sinal, 2, descPilha, backup, NodoPilha, AssemblyInst);
                        break;
                    }

                    if(memInst[regif->pc].instruc[0] == '\0'){ //se tiver um endereço sem instrução, ele pulara para o proximo
                        increment_PC(program_counter, 1);
                        controller(1, NumeroLinhas, regs, memInst, memDados, program_counter, instrucoesDecodificadas, regif, id, ex, mem, wb, sinal, 1, descPilha, backup, NodoPilha, AssemblyInst);
                        break;
                    }
                    
                    if(regif->pc > 1){
                        if(id->sinal->bolha == 1){ //Verifica se a etapa ID precisa de uma bolha
                            id->sinal->bolha = 0; //Reseta o sinal
                            regif->instruc[0] = '\0'; //INVALIDA UMA INSTRUÇÃO (INSTRUÇÃO VAZIA)
                            printf("\nBolha na etapa IF");
                            printf("\n╚════════════════════════════╝");
                            increment_PC(program_counter, 1);
                            controller(1, NumeroLinhas, regs, memInst, memDados, program_counter, instrucoesDecodificadas, regif, id, ex, mem, wb, sinal, 2, descPilha, backup, NodoPilha, AssemblyInst);
                            break;
                        }
                        //se id estiver na bolha, quer dizer que: ou o beq é verdadeiro ou teremos um jump. Logo, a execução da instrução ja estara na etapa EX
                        //entao preciso excluir a instrução que estava por vir para que ela nao execute e atrapalhe a sequencia
                            else if(id->instruc[0] == '\0' && (ex->sinal->tipo == 5 || ex->sinal->tipo == 1)){ 
                                        regif->instruc[0] = '\0';
                                        //printf("\nInstrução não buscada por conta de BEQ verdadeiro ou JUMP na etapa EX");
                                        printf("\nBolha na etapa IF"); //->achei mais elegante printar dessa forma
                                        printf("\n╚════════════════════════════╝");
                                        return 2;
                                        break;
                            }
                    }

                    

                    strcpy(regif->instruc, memInst[regif->pc].instruc);
                    printf("\nEtapa IF: %s", regif->instruc);
                    printf("\n╚════════════════════════════╝");
                    
                    increment_PC(program_counter, 1);
                    controller(1, NumeroLinhas, regs, memInst, memDados, program_counter, instrucoesDecodificadas, regif, id, ex, mem, wb, sinal, 2, descPilha, backup, NodoPilha, AssemblyInst);

                    break;
                
                case 2://Etapa ID -> Decodifico as instruções, gero os sinais e Adiciono valores aos registradores auxiliares
                    strcpy(id->instruc, regif->instruc);          
                    id->pc = regif->pc;

                    if(id->pc == NumeroLinhas){ //quando chegar no numero de linhas, encerrará a etapa
                        printf("\nEtapa ID encerrada.");
                        printf("\n╚════════════════════════════╝");
                        controller(1, NumeroLinhas, regs, memInst, memDados, program_counter, instrucoesDecodificadas, regif, id, ex, mem, wb, sinal, 3, descPilha, backup, NodoPilha, AssemblyInst);
                    }
                    else if(id->instruc[0] == '\0'){
                        printf("\nBolha na Etapa ID");
                    }
                    else
                        printf("\nEtapa ID: %s", id->instruc);  
                    
                    
                    id->sinal = inicializaSinais();
                    //decodifica e adiciona os sinais da instrucao
                        instrucoesDecodificadas[id->pc] = decoder(memInst, id->pc);
                        id->sinal = AddSinais(instrucoesDecodificadas[id->pc], id->sinal);

                    //COLETA DADOS DOS REGISTRADORES
                        id->readData1 = regs[id->sinal->RS];
                        id->readData2 = regs[id->sinal->RT];
                    
                    if(id->sinal->tipo ==  1 )// CRIARA BOLHAS SE FOR JUMP
                    {
                        if(strcmp(id->instruc, "\0") != 0){
                            id->sinal->bolha = 1;
                            printf(" - Bolha gerada na Etapa ID para Jump");
                            controller(1, NumeroLinhas, regs, memInst, memDados, program_counter, instrucoesDecodificadas, regif, id, ex, mem, wb, sinal, 1, descPilha, backup, NodoPilha, AssemblyInst);
                            break;
                        }
                        else{
                            id->sinal->tipo = -1;
                        }
                        
                    }
                    else if (id->sinal->tipo == 5)// CRIARA BOLHAS SE FOR BEQ Válido
                    {
                        if (id->readData1 == id->readData2 && (strcmp(id->instruc, "\0") != 0))
                        {
                            id->sinal->bolha = 1;
                            printf(" - Bolha gerada na Etapa ID para BEQ");
                            controller(1, NumeroLinhas, regs, memInst, memDados, program_counter, instrucoesDecodificadas, regif, id, ex, mem, wb, sinal, 1, descPilha, backup, NodoPilha, AssemblyInst);
                            break;
                        }
                        else{
                            id->sinal->tipo = -1;
                        }                        
                    }
                    controller(2, NumeroLinhas, regs, memInst, memDados, program_counter, instrucoesDecodificadas, regif, id, ex, mem, wb, sinal, 1, descPilha, backup, NodoPilha, AssemblyInst);
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
                        printf("\n╚════════════════════════════╝");
                        controller(1, NumeroLinhas, regs, memInst, memDados, program_counter, instrucoesDecodificadas, regif, id, ex, mem, wb, sinal, 4, descPilha, backup, NodoPilha, AssemblyInst);
                    }
                    else if(ex->instruc[0] == '\0'){
                        printf("\nBolha na Etapa EX");
                    }
                    else
                        printf("\nEtapa EX: %s", ex->instruc);
                    //recursao para realizar etapas anteriores:
                    controller(2, NumeroLinhas, regs, memInst, memDados, program_counter, instrucoesDecodificadas, regif, id, ex, mem, wb, sinal, 2, descPilha, backup, NodoPilha, AssemblyInst);
                    
                    if (ex->sinal->tipo == 1)//verifica se é Jump
                    {
                        pc = ex->pc;
                        jump = ULA(instrucoesDecodificadas, &pc, memDados, regs);
                        (*program_counter) = jump;
                        a++;
                        printf("%d jump/loop concluido.\t\t", a); 
                    }
                    
                    else if (ex->sinal->tipo == 0 || ex->sinal->tipo == 2 || ex->sinal->tipo == 3 || ex->sinal->tipo == 4)
                    //verifica se é tipo R (0), addi (2), lw (3) ou sw (4)
                    {
                        pc = ex->pc;
                        ex->aluResult = ULA(instrucoesDecodificadas, &pc, memDados, regs);
                    }
                    
                    else if (ex->sinal->tipo == 5)//verifica se é beq
                    {
                        pc = ex->pc;
                        if (ex->readData1 == ex->readData2){
                            *program_counter = pc + bin_to_decimal(instrucoesDecodificadas[pc].imm);
                        }
                    }
                    controller(1, NumeroLinhas, regs, memInst, memDados, program_counter, instrucoesDecodificadas, regif, id, ex, mem, wb, sinal, 4, descPilha, backup, NodoPilha, AssemblyInst);
                    break;
                        
                case 4: // Se a instrução envolve leitura ou escrita na memória de dados, essa operação é realizada neste estágio
                    mem->pc = ex->pc;
                    strcpy(mem->instruc, ex->instruc);
                    mem->aluResult = ex->aluResult;
                    mem->sinal = ex->sinal;
                    if(mem->pc == NumeroLinhas){
                        printf("\nEtapa MEM encerrada.");
                        printf("\n╚════════════════════════════╝");
                        controller(1, NumeroLinhas, regs, memInst, memDados, program_counter, instrucoesDecodificadas, regif, id, ex, mem, wb, sinal, 5, descPilha, backup, NodoPilha, AssemblyInst);
                    }
                    else if(mem->instruc[0] == '\0'){
                        printf("\nBolha na Etapa WB");
                    }
                    else
                        printf("\nEtapa MEM: %s", mem->instruc);
                    //recursao para realizar etapas anteriores:
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
                        decimalToBinary(mem->sinal->RT, posicao);
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
                        controller(1, NumeroLinhas, regs, memInst, memDados, program_counter, instrucoesDecodificadas, regif, id, ex, mem, wb, sinal, 5, descPilha, backup, NodoPilha, AssemblyInst);
                    }
                    controller(1, NumeroLinhas, regs, memInst, memDados, program_counter, instrucoesDecodificadas, regif, id, ex, mem, wb, sinal, 5, descPilha, backup, NodoPilha, AssemblyInst);
                    break;

                    case 5: //Etapa WB (write back) -> O resultado da operação é escrito de volta no registrador destino no banco de registradores.          
                        wb->pc = mem->pc;
                        wb->InstrucaoASM = mem->InstrucaoASM;
                        strcpy(wb->instruc, mem->instruc);
                        printf("\n╔═══════════════════════════╗");
                        wb->aluResult = mem->aluResult;
                        wb->sinal = mem->sinal;
                        if(wb->pc == NumeroLinhas){
                            printf("\nEtapa WB encerrada.");
                            printf("\n╚═════════════════════════╝");
                            return 6;
                            break;
                        }
                        else if(wb->instruc[0] == '\0'){
                            printf("\nBolha na Etapa WB");
                        }
                        else
                            printf("\nEtapa WB: %s", wb->InstrucaoASM.InstructsAssembly);
                        controller(2, NumeroLinhas, regs, memInst, memDados, program_counter, instrucoesDecodificadas, regif, id, ex, mem, wb, sinal, 4, descPilha, backup, NodoPilha, AssemblyInst);    
                        
                        if (wb->sinal->tipo == 3){ // lw (load word)
                            decimalToBinary(wb->sinal->RT, posicao);
                            escritaRegistradores(regs, wb->aluResult, posicao); // Load: Reg[IR[20:16]] <= memoriaR    
                        }

                        else if (wb->sinal->tipo == 0) // R-type
                        {
                            decimalToBinary(wb->sinal->RD, posicao);
                            escritaRegistradores(regs, wb->aluResult, posicao);
                        }

                        else if (wb->sinal->tipo == 2) // addi
                        {
                            decimalToBinary(wb->sinal->RT, posicao);
                            escritaRegistradores(regs, wb->aluResult, posicao);                               
                        }
                        controller(1, NumeroLinhas, regs, memInst, memDados, program_counter, instrucoesDecodificadas, regif, id, ex, mem, wb, sinal, 5, descPilha, backup, NodoPilha, AssemblyInst);
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
                    printf("\n╚════════════════════════════╝");
                    return 2;
                }

                if(memInst[regif->pc].instruc[0] == '\0'){
                        increment_PC(program_counter, 1);
                        controller(2, NumeroLinhas, regs, memInst, memDados, program_counter, instrucoesDecodificadas, regif, id, ex, mem, wb, sinal, 1, descPilha, backup, NodoPilha, AssemblyInst);
                        break;
                }

                if(regif->pc > 1){
                    if(id->sinal->bolha == 1){ //Verifica se a etapa ID precisa de uma bolha
                        regif->instruc[0] = '\0'; //INVALIDA UMA INSTRUÇÃO (INSTRUÇÃO VAZIA).
                        printf("\nBolha na etapa IF");
                        printf("\n╚════════════════════════════╝");
                        return 2;
                        break;
                    }
                    //se id estiver na bolha, quer dizer que: ou o beq é verdadeiro ou teremos um jump. Logo, a execução da instrução ja estara na etapa EX
                    //entao preciso excluir a instrução que estava por vir para que ela nao execute e atrapalhe a sequencia
                        else if(id->instruc[0] == '\0' && (ex->sinal->tipo == 5 || ex->sinal->tipo == 1)){ 
                                    regif->instruc[0] = '\0';
                                    //printf("\nInstrução não buscada por conta de BEQ verdadeiro ou JUMP na etapa EX");
                                    printf("\nBolha na etapa IF"); //->achei mais elegante printar dessa forma
                                    printf("\n╚════════════════════════════╝");
                                    return 2;
                                    break;
                        }
                }
                
                strcpy(regif->instruc, memInst[regif->pc].instruc);

                regif->InstrucaoASM = ASMPrintInstruc(memInst, program_counter); //transforma a instrucao binaria para assembly
                printf("\nEtapa IF: %s", regif->InstrucaoASM.InstructsAssembly);
                printf("\n╚════════════════════════════╝");

                increment_PC(program_counter, 1);

                return 2;
                break;
                
            case 2://Etapa ID -> Decodifico as instruções, gero os sinais e Adiciono valores aos registradores auxiliares     
                strcpy(id->instruc, regif->instruc);
                id->pc = regif->pc;
                id->InstrucaoASM = regif->InstrucaoASM;
                if(id->pc == NumeroLinhas){
                    printf("\nEtapa ID encerrada.");
                    printf("\n╚════════════════════════════╝");
                    return 3;
                }
                else if(id->instruc[0] == '\0'){
                        printf("\nBolha na Etapa ID");
                }
                else
                    printf("\nEtapa ID: %s", id->InstrucaoASM.InstructsAssembly);

                //DECODIFICAÇÃO E GERAÇÃO DE SINAIS
                        id->sinal = inicializaSinais();
                        instrucoesDecodificadas[id->pc] = decoder(memInst, id->pc); //decodificou
                        id->sinal = AddSinais(instrucoesDecodificadas[id->pc], id->sinal);
                    
                //COLETA DADOS DOS REGISTRADORES
                        id->readData1 = regs[id->sinal->RS];
                        id->readData2 = regs[id->sinal->RT];

                if(id->sinal->tipo ==  1)// CRIARA BOLHAS SE FOR JUMP
                    {
                        if(strcmp(id->instruc, "\0") != 0){ //preciso desse if pois estou gerando sinais mesmo se a instrução for vazia
                            //se instrucao id for diferente de vazia:
                            id->sinal->bolha = 1;
                            printf(" - Bolha gerada na Etapa ID para Jump");
                        }
                        else{//se instrucao estiver vazia, quer dizer que estou na bolha, entao preciso "anular" o sinal para que a instrução de if seja coletada corretamente
                            id->sinal->tipo = -1;
                        }
                    }
                else if (id->sinal->tipo == 5)// CRIARA BOLHAS SE FOR BEQ Válido
                    {
                        if ((id->readData1 == id->readData2) && (strcmp(id->instruc, "\0") != 0))
                        {
                            id->sinal->bolha = 1;
                            printf(" - Bolha gerada na Etapa ID para BEQ");
                        }         
                        else{
                            id->sinal->tipo = -1;
                        }               
                    }
                //recursao para realizar etapas anteriores:
                controller(2, NumeroLinhas, regs, memInst, memDados, program_counter, instrucoesDecodificadas, regif, id, ex, mem, wb, sinal, 1, descPilha, backup, NodoPilha, AssemblyInst);
                return 3;
                break;
                    
            case 3: //Etapa EX --> Executa tipo R e Addi, Calcula Endereço LW e SW 
                strcpy(ex->instruc, id->instruc);
                ex->pc = id->pc;
                ex->readData1 = id->readData1;
                ex->readData2 = id->readData2;
                ex->sinal = id->sinal;
                ex->InstrucaoASM = id->InstrucaoASM;
                if(ex->pc == NumeroLinhas){
                    printf("\nEtapa EX encerrada.");
                    printf("\n╚════════════════════════════╝");
                    return 4;
                }
                else if(ex->instruc[0] == '\0'){
                        printf("\nBolha na Etapa EX");
                }
                else{
                    printf("\nEtapa EX: %s", ex->InstrucaoASM.InstructsAssembly);


                    if (ex->sinal->tipo == 1)//verifica se é Jump
                    {
                        pc = ex->pc;
                        jump = ULA(instrucoesDecodificadas, &pc, memDados, regs);
                        (*program_counter) = jump;
                        a++;
                        printf("\ntipo: %d\b", ex->sinal->tipo);
                        printf("%d jump/loop concluido.\t\t", a);
                    }
                    
                    else if (ex->sinal->tipo == 0 || ex->sinal->tipo == 2 || ex->sinal->tipo == 3 || ex->sinal->tipo == 4)
                    //verifica se é tipo R (0), addi (2), lw (3) ou sw (4)
                    {
                        pc = ex->pc;
                        ex->aluResult = ULA(instrucoesDecodificadas, &pc, memDados, regs);  
                    }
                    
                    else if (ex->sinal->tipo == 5)//verifica se é beq
                    {
                        pc = ex->pc;
                        if (ex->readData1 == ex->readData2){
                            *program_counter = pc + bin_to_decimal(instrucoesDecodificadas[pc].imm); //pc atual + imm
                        }
                    }
                }
                controller(2, NumeroLinhas, regs, memInst, memDados, program_counter, instrucoesDecodificadas, regif, id, ex, mem, wb, sinal, 2, descPilha, backup, NodoPilha, AssemblyInst);
                return 4;
                break;
                    
            case 4: // MEM-> Se a instrução envolve leitura ou escrita na memória de dados, essa operação é realizada neste estágio
                mem->pc = ex->pc;
                strcpy(mem->instruc, ex->instruc);  
                mem->aluResult = ex->aluResult;
                mem->sinal = ex->sinal;
                mem->InstrucaoASM = ex->InstrucaoASM;
                if(mem->pc == NumeroLinhas){
                    printf("\nEtapa MEM encerrada.");
                    printf("\n╚════════════════════════════╝");
                    return 5;
                }
                else if(mem->instruc[0] == '\0'){
                        printf("\nBolha na Etapa MEM");
                }
                else{
                    printf("\nEtapa MEM: %s", mem->InstrucaoASM.InstructsAssembly);
                
                    if (mem->sinal->tipo == 3) // lw (load word)
                    {
                        // Carregar dado da memória
                        strcpy(mem->readData, memDados[mem->aluResult].dados); //copio para o registrador de dados, o dado da memoria
                    }
                    else if (mem->sinal->tipo == 4) // sw (store word)
                    {
                            // Armazenar dado na memória
                            decimalToBinary(mem->sinal->RT, posicao);
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
                    }
                }
                controller(2, NumeroLinhas, regs, memInst, memDados, program_counter, instrucoesDecodificadas, regif, id, ex, mem, wb, sinal, 3, descPilha, backup, NodoPilha, AssemblyInst);
                return 5;
                break;

                case 5: //Etapa WB (write back) -> O resultado da operação é escrito de volta no registrador destino no banco de registradores.          
                    wb->pc = mem->pc;
                    strcpy(wb->instruc, mem->instruc);
                    printf("\n╔════════════════════════════╗");
                    wb->aluResult = mem->aluResult;
                    wb->sinal = mem->sinal;
                    wb->InstrucaoASM = mem->InstrucaoASM;
                    if(wb->pc == NumeroLinhas){
                        printf("\nEtapa WB encerrada.");
                        printf("\n╚════════════════════════════╝");
                        return 6;
                    }
                    else if(wb->instruc[0] == '\0'){
                        printf("\nBolha na Etapa WB");
                        }
                    else{
                        printf("\nEtapa WB: %s", wb->InstrucaoASM.InstructsAssembly);
    
                    
                        if (wb->sinal->tipo == 3){ // lw (load word)
                            decimalToBinary(wb->sinal->RT, posicao);
                            escritaRegistradores(regs, wb->aluResult, posicao); // Load: Reg[IR[20:16]] <= memoriaR    
                        }

                        else if (wb->sinal->tipo == 0) // R-type
                        {
                            decimalToBinary(wb->sinal->RD, posicao);
                            escritaRegistradores(regs, wb->aluResult, posicao);
                        }

                        else if (wb->sinal->tipo == 2) // addi
                        {
                            decimalToBinary(wb->sinal->RT, posicao);
                            escritaRegistradores(regs, wb->aluResult, posicao);                               
                        }
                    }
                    controller(2, NumeroLinhas, regs, memInst, memDados, program_counter, instrucoesDecodificadas, regif, id, ex, mem, wb, sinal, 4, descPilha, backup, NodoPilha, AssemblyInst);
                    return 5;
                    break;
                }
        break;
        }
    }
}