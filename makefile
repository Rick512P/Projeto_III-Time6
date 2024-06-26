CHECK_NCURSES := $(shell dpkg -s libncurses5-dev libncursesw5-dev >/dev/null 2>&1 && echo "installed" || echo "not_installed")

all: check_ncurses index

check_ncurses:
ifeq ($(CHECK_NCURSES),not_installed)
	@echo "ncurses not found, installing..."
	sudo apt-get update
	sudo apt-get install -y libncurses5-dev libncursesw5-dev
else
	@echo "ncurses already installed."
endif

index:
	gcc -o executavel Arquivos-c/*.c -lncurses
	clear
	./executavel

testes:
	@if [ -f executavel ]; then \
		rm executavel; \
	fi
	clear
	gcc -o executavel Arquivos-c/*.c -g -lncurses
	./executavel

clear:
	@if [ -f executavel ]; then \
		rm executavel; \
	fi
	clear
