### Simulador da Arquitetura Mini MIPS (Versão 8 bits) Pipeline

Este repositório contém o código-fonte e a documentação para o Simulador da Arquitetura Mini MIPS Pipeline em sua versão de 8 bits, desenvolvido como parte do projeto integrador das disciplinas Projeto Integrador II e Algoritmos e Estrutura de Dados.

### Resumo

O projeto aborda o processo de construção e planejamento do Simulador da Arquitetura MIPS Pipeline em uma versão de 8 bits, implementada inteiramente na linguagem C. Desenvolvido com base nos princípios ensinados até o terceiro semestre do curso de Engenharia de Computação da UNIPAMPA - Campus Bagé, o simulador inclui os principais componentes de uma arquitetura MIPS, como Memória de Instruções, Unidade de Controle, Contador de Programa (PC), Banco de Registradores, Unidade Lógica e Aritmética (ULA) e Memória de Dados.

### Introdução

O projeto surge da aplicação prática dos conhecimentos adquiridos nos primeiros três semestres do curso de Engenharia de Computação, incluindo algoritmos, estruturas de dados, projeto digital e fundamentos de hardware e software. Utilizando os conceitos aprendidos, como tipos abstratos de dados (TADs), conversões de bases numéricas e tradução de instruções em linguagem de máquina para Assembly, o simulador foi desenvolvido inteiramente em linguagem C.

### Documentação Detalhada

Para uma compreensão mais profunda do projeto, recomendamos a leitura do relatório de desenvolvimento no modelo de artigo. Esse artigo está disponível em Overleaf no seguinte link: [Relatório Detalhado do Projeto](https://www.overleaf.com/project/668c645dd27411508d63c591).

### Autores

- **Erick S. Matos** - erickmatos.aluno@unipampa.edu.br
- **Henrique de L. Bortolomiol** - henriquebortolomiol.aluno@unipampa.edu.br
- **Pedro Afonso** - pedroafonso.aluno@unipampa.edu.br

### Instituição

Universidade Federal do Pampa - UNIPAMPA Campus Bagé

### Licença

Este projeto está licenciado sob a Licença MIT. Veja o arquivo [`LICENSE`](LICENSE) para mais detalhes.

### Formato de Instrução

Para maiores detalhes sobre o formato e tipos de intruções reconhecidas pelo simulador, acesse ['Formato_Intruções'](Formato_Instrucoes.txt)

---

### Como Utilizar - Um Passo a Passo para Teste do Programa em Ambiente Linux

1. Faça download do repositório em qualquer pasta de seu computador.

2. Para download do arquivo `.zip`:
   - Abra o terminal e navegue até o diretório onde está o arquivo.
   - Utilize o seguinte comando para descompactar a pasta:
     ```bash
     unzip Projeto_III-TIme6.zip
     ```

3. Para download via git:
   - Abra o terminal e navegue até o diretório que deseja salvar o programa.
   - Utilize o seguinte comando para clonar o repositório:
     ```bash
     git clone https://github.com/Rick512P/Projeto_III-TIme6
     ```

4. Após o download, acesse o diretório baixado/clonado:
   ```bash
   cd Projeto_III-TIme6
   ```

5. Utilize o comando `make instala_ncurses` para instalar a biblioteca NCurses em seu ambiente Linux, caso não a possua:
   ```bash
   make instala_ncurses
   ```

6. Utilize o comando `make` para compilar e executar o programa:
   ```bash
   make
   ```

7. Recomenda-se utilizar o terminal em modo tela cheia (`F11`).

8. Ao inicializar o programa, utilize a tecla `Enter` para exibir a tela de menu.

9. Carregue a memória de instruções com a tecla `F2`:
   - Escolha a opção 1 para carregar um programa padrão de teste.
   - Escolha a opção 2 para carregar um arquivo específico com extensão `.mem`.

10. Opcionalmente, carregue a memória de dados com a tecla `F3`:
    - Escolha a opção 1 para carregar um arquivo padrão.
    - Escolha a opção 2 para carregar um arquivo específico com extensão `.dat`.

11. Utilize a tecla `S` para realizar a execução passo a passo das instruções.

### Avisos Importantes

- Utilize a tecla `Enter` frequentemente para carregar as telas.
- Para sair de uma tela, geralmente é necessário clicar mais de uma vez na tecla `F1`.
- Para exibir a memória de instrução, é necessário carregar o arquivo antes.
- O programa só funcionará se for carregado um arquivo de instruções.
- Para exibir as instruções em formato assembly, a instrução deve passar pela etapa ID. O mesmo vale para salvar um arquivo `.asm`.
