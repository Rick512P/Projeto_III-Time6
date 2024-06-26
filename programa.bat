@echo off
setlocal

:: Verifica se está sendo executado como administrador
openfiles >nul 2>&1
if %errorlevel% NEQ 0 (
    echo Por favor, execute este script como administrador.
    pause
    exit /b
)

:: Define links de download
set "MSYS2_URL=https://github.com/msys2/msys2-installer/releases/download/2024-05-07/msys2-x86_64-20240507.exe"
set "CYGWIN_URL=https://www.cygwin.com/setup-x86_64.exe"

:: Define arquivos de instalação
set "MSYS2_EXE=msys2-x86_64-20240507.exe"
set "CYGWIN_EXE=setup-x86_64.exe"

:: Baixa instaladores
echo Baixando instalador do MSYS2...
powershell -Command "Invoke-WebRequest -Uri %MSYS2_URL% -OutFile %MSYS2_EXE%"
echo Baixando instalador do Cygwin...
powershell -Command "Invoke-WebRequest -Uri %CYGWIN_URL% -OutFile %CYGWIN_EXE%"

:: Instala MSYS2
echo Instalando MSYS2...
start /wait %MSYS2_EXE% --root C:\msys64 --default-answer --confirm-exit

:: Atualiza pacotes e instala ncurses no MSYS2
echo Instalando ncurses no MSYS2...
C:\msys64\usr\bin\bash -lc "pacman -Sy --noconfirm"
C:\msys64\usr\bin\bash -lc "pacman -S --noconfirm mingw-w64-x86_64-ncurses"

:: Instala Cygwin
echo Instalando Cygwin...
start /wait %CYGWIN_EXE% -q -P ncurses

:: Limpa arquivos de instalação
echo Limpando arquivos de instalacao...
del %MSYS2_EXE%
del %CYGWIN_EXE%

:menu
cls
echo Selecione uma opcao:
echo 1. Rodar Programa
echo 2. Rodar Programa para Testes
echo 3. Limpar area
echo 4. Sair

set /p opcao=Opcao:

if "%opcao%"=="1" goto rodar_programa
if "%opcao%"=="2" goto rodar_programa_testes
if "%opcao%"=="3" goto limpar_area
if "%opcao%"=="4" goto sair

echo Opcao invalida!
pause
goto menu

:rodar_programa
gcc -o executavel Arquivos-c/*.c
cls
executavel.exe
pause
goto menu

:rodar_programa_testes
if exist executavel.exe (
    del executavel.exe
)
gcc -o executavel Arquivos-c/*.c -g
cls
executavel.exe
pause
goto menu

:limpar_area
if exist executavel.exe (
    del executavel.exe
)
cls
goto menu

:sair
cls
echo PROGRAMA FINALIZADO!
pause
endlocal
