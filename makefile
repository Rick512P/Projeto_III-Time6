index:
	gcc -o executavel Arquivos-c/*.c -lncurses -lpanel -lmenu
	clear
	./executavel

testes:
	@if [ -f executavel ]; then \
		rm executavel; \
	fi
	clear
	gcc -o executavel Arquivos-c/*.c -g -lncurses -lpanel -lmenu
	gdb ./executavel

clear:
	@if [ -f executavel ]; then \
		rm executavel; \
	fi
	clear

instala_ncurses:
	sudo apt-get update
	sudo apt-get install -y libncurses5-dev libncursesw5-dev