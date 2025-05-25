#include <iostream>
#include <string>
#include <vector>
#include <limits>

#ifdef _WIN32
    #include <direct.h>
    #define GetCurrentDir _getcwd
#else
    #include <unistd.h>
    #define GetCurrentDir getcwd
#endif

#include "headers/Livro.h"
#include "headers/ArvoreB.h"
#include "headers/NoArvoreB.h"
#include "headers/Utils.h"
#include "headers/Usuario.h"
#include "headers/NoArvoreBUsuario.h"
#include "headers/ArvoreBUsuario.h"

int main() {
    char currentPath[FILENAME_MAX];
    if (GetCurrentDir(currentPath, sizeof(currentPath)) != nullptr) {
        std::cout << "Diretório de trabalho atual do programa: " << currentPath << std::endl;
    } else {
        std::cerr << "Erro ao obter o diretório de trabalho atual." << std::endl;
    }

    const std::string NOME_ARQUIVO_LIVROS = "../livros.txt";
    const std::string NOME_ARQUIVO_USUARIOS = "../usuarios.txt";

    ArvoreB minhaArvoreB(NOME_ARQUIVO_LIVROS);
    minhaArvoreB.carregarDoArquivo();

    ArvoreBUsuario minhaArvoreBUsuario(NOME_ARQUIVO_USUARIOS);
    minhaArvoreBUsuario.carregarDoArquivo();

    int opcaoPrincipal;
    do {
        exibirMenuPrincipal();
        std::cin >> opcaoPrincipal;

        switch (opcaoPrincipal) {
            case 1: {
                int opcaoLivros;
                do {
                    exibirMenuLivros();
                    std::cin >> opcaoLivros;

                    switch (opcaoLivros) {
                        case 1: {
                            imprimirTabelaLivros(NOME_ARQUIVO_LIVROS);
                            break;
                        }
                        case 2: {
                            Livro novoLivro = lerDadosNovoLivro();
                            minhaArvoreB.inserirLivro(novoLivro);
                            break;
                        }
                        case 3: {
                            std::string generoBuscado;
                            std::cout << "\n--- Buscar Livro por Gênero ---" << std::endl;
                            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                            std::cout << "Digite o gênero: ";
                            std::getline(std::cin, generoBuscado);
                            generoBuscado = sanitizeString(generoBuscado);
                            minhaArvoreB.buscarLivroPorGenero(generoBuscado);
                            break;
                        }
                        case 4: {
                            std::string autorBuscado;
                            std::cout << "\n--- Buscar Livro por Autor ---" << std::endl;
                            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                            std::cout << "Digite o nome do autor: ";
                            std::getline(std::cin, autorBuscado);
                            autorBuscado = sanitizeString(autorBuscado);
                            minhaArvoreB.buscarLivroPorAutor(autorBuscado);
                            break;
                        }
                        case 5: {
                            std::string nomeBuscado;
                            std::cout << "\n--- Buscar Livro por Nome ---" << std::endl;
                            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                            std::cout << "Digite o nome do livro: ";
                            std::getline(std::cin, nomeBuscado);
                            nomeBuscado = sanitizeString(nomeBuscado);
                            minhaArvoreB.buscarLivroPorNome(nomeBuscado);
                            break;
                        }
                        case 6: {
                            int idParaAtualizar;
                            std::cout << "\n--- Atualizar Livro por ID ---" << std::endl;
                            std::cout << "Digite o ID do livro a ser atualizado: ";
                            while (!(std::cin >> idParaAtualizar)) {
                                std::cout << "Entrada inválida. Digite um número inteiro para o ID: ";
                                std::cin.clear();
                                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                            }
                            Livro livroAtualizado = lerDadosAtualizacaoLivro(idParaAtualizar);
                            minhaArvoreB.atualizarLivro(idParaAtualizar, livroAtualizado);
                            break;
                        }
                        case 0: {
                            std::cout << "\nVoltando ao Menu Principal..." << std::endl;
                            break;
                        }
                        default: {
                            std::cout << "\nOpção inválida. Por favor, tente novamente." << std::endl;
                            std::cin.clear();
                            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                            break;
                        }
                    }
                } while (opcaoLivros != 0);
                break;
            }
            case 2: {
                int opcaoUsuarios;
                do {
                    exibirMenuUsuarios();
                    std::cin >> opcaoUsuarios;

                    std::string nomeUsuario;
                    std::string termoBusca;
                    std::vector<Livro> livrosEncontrados;
                    Usuario usuarioAtual;

                    switch (opcaoUsuarios) {
                        case 1: {
                            listarTodosUsuarios(minhaArvoreBUsuario);
                            break;
                        }
                        case 2: {
                            nomeUsuario = lerNomeUsuario();
                            std::cout << "Digite o gênero a buscar: ";
                            std::getline(std::cin, termoBusca);
                            termoBusca = sanitizeString(termoBusca);
                            
                            livrosEncontrados = minhaArvoreB.buscarLivroPorGenero(termoBusca);
                            
                            usuarioAtual = Usuario(nomeUsuario);
                            usuarioAtual.adicionarGeneroBuscado(termoBusca);
                            minhaArvoreBUsuario.inserirOuAtualizarUsuario(usuarioAtual);
                            break;
                        }
                        case 3: {
                            nomeUsuario = lerNomeUsuario();
                            std::cout << "Digite o autor a buscar: ";
                            std::getline(std::cin, termoBusca);
                            termoBusca = sanitizeString(termoBusca);

                            livrosEncontrados = minhaArvoreB.buscarLivroPorAutor(termoBusca);

                            usuarioAtual = Usuario(nomeUsuario);
                            usuarioAtual.adicionarAutorBuscado(termoBusca);
                            minhaArvoreBUsuario.inserirOuAtualizarUsuario(usuarioAtual);
                            break;
                        }
                        case 4: {
                            nomeUsuario = lerNomeUsuario();
                            std::cout << "Digite o nome do livro a buscar: ";
                            std::getline(std::cin, termoBusca);
                            termoBusca = sanitizeString(termoBusca);

                            livrosEncontrados = minhaArvoreB.buscarLivroPorNome(termoBusca);

                            usuarioAtual = Usuario(nomeUsuario);
                            usuarioAtual.adicionarNomeLivroBuscado(termoBusca);
                            minhaArvoreBUsuario.inserirOuAtualizarUsuario(usuarioAtual);
                            break;
                        }
                        case 5: {
                            recomendarLivros(minhaArvoreBUsuario, minhaArvoreB);
                            break;
                        }
                        case 0: {
                            std::cout << "\nVoltando ao Menu Principal..." << std::endl;
                            break;
                        }
                        default: {
                            std::cout << "\nOpção inválida. Por favor, tente novamente." << std::endl;
                            std::cin.clear();
                            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                            break;
                        }
                    }
                } while (opcaoUsuarios != 0);
                break;
            }
            case 0: {
                std::cout << "\nSaindo do programa. Até mais!" << std::endl;
                break;
            }
            default: {
                std::cout << "\nOpção inválida. Por favor, tente novamente." << std::endl;
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                break;
            }
        }
    } while (opcaoPrincipal != 0);

    return 0;
}