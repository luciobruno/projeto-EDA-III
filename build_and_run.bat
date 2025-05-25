@echo off
chcp 65001 > nul
setlocal

set EXECUTABLE_NAME=codigo.exe
set OUTPUT_DIR=output
set EXEC_PATH=.\%OUTPUT_DIR%\%EXECUTABLE_NAME%

echo --- Limpando builds anteriores ---
del /Q sources\*.o > nul 2>&1
del /Q %EXEC_PATH% > nul 2>&1
del /Q *.o > nul 2>&1

echo --- Criando o diretorio de saida se nao existir ---
if not exist %OUTPUT_DIR% mkdir %OUTPUT_DIR%

echo --- Compilacao ---
echo --- Compilando Livro.cpp ---
g++ -std=c++17 -Iheaders -c sources\Livro.cpp -o sources\Livro.o
if %errorlevel% neq 0 goto :compile_error

echo --- Compilando ArvoreB.cpp ---
g++ -std=c++17 -Iheaders -c sources\ArvoreB.cpp -o sources\ArvoreB.o
if %errorlevel% neq 0 goto :compile_error

echo --- Compilando Usuario.cpp ---
g++ -std=c++17 -Iheaders -c sources\Usuario.cpp -o sources\Usuario.o
if %errorlevel% neq 0 goto :compile_error

echo --- Compilando ArvoreBUsuario.cpp ---
g++ -std=c++17 -Iheaders -c sources\ArvoreBUsuario.cpp -o sources\ArvoreBUsuario.o
if %errorlevel% neq 0 goto :compile_error

echo --- Compilando Utils.cpp ---
g++ -std=c++17 -Iheaders -c sources\Utils.cpp -o sources\Utils.o
if %errorlevel% neq 0 goto :compile_error

echo --- Compilando main.cpp ---
g++ -std=c++17 -Iheaders -c main.cpp -o main.o
if %errorlevel% neq 0 goto :compile_error

echo --- Linkando arquivos objeto para criar o executavel ---
g++ -std=c++17 main.o sources\Livro.o sources\ArvoreB.o sources\Usuario.o sources\ArvoreBUsuario.o sources\Utils.o -o %EXEC_PATH%
if %errorlevel% neq 0 goto :compile_error

echo --- Compilacao concluida com sucesso! ---

echo --- Executando o programa ---
pushd %OUTPUT_DIR%
%EXECUTABLE_NAME%
popd

echo --- Processo concluido ---
goto :eof

:compile_error
echo --- Erro na compilacao. Verifique as mensagens acima ---
endlocal
pause
exit /b 1
