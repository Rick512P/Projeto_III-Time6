### Instruções Comportadas

As instruções reconhecidas por este simulador são: ADD, SUB, AND, OR, ADDI, LW, SW, BEQ, JUMP.
Para ADD, SUB, AND e OR utiliza-se, opcode: 0000
Para ADDI utiliza-se, opcode: 0100
Para LW utiliza-se, opcode: 1011
Para SW utiliza-se, opcode: 1111
Para BEQ utiliza-se, opcode: 1000
Para JUMP utiliza-se, opcode: 0010

---

### Instruções Tipo R

ADD, SUB, AND e OR são instruções do Tipo R, com a seguinte estrutura:
Opcode RS RT RD Funct -> onde:
    Opcode: 0000
    RS: de 000 até 111
    RT: de 000 até 111
    RD: de 000 até 111
    Funct: ADD: 000; SUB: 010; AND: 100; OR: 101
    Exemplo: 0000101010011100 -> Opcode: 0000; RS: 101; RT: 010; RD: 011; Funct: 100
---

### Intruções Tipo I

LW, SW e BEQ são instruções do Tipo I, com a seguinte estrutura:
Opcode RS RT Imm -> onde:
    Opcode: LW: 1011; SW: 1111; BEQ: 1000
    RS: de 000 até 111
    RT: de 000 até 111
    imm: de 000000 até 111111
    Exemplo: 1111000001001000 -> Opcode: 1111; RS: 000; RT: 001; Imm: 001000

---

### Intruções Tipo J

Jump é uma instrução do Tipo J, com a seguinte estrutura:
Opcode Endereço -> onde:
    Opcode: 0010
    Endereço: de 000000000000 até 000011111111 (0 até 255)
    Exemplo: 0010000000001000 -> Opcode: 0010; Endereço: 000000001000
