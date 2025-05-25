#include "ArvoreB.h"

#include <iostream>
#include <fstream>
#include <algorithm>
#include <stdexcept>
#include <sstream>
#include <algorithm>
#include <cctype>

ArvoreB::ArvoreB(const std::string& arquivoDados)
    : ordem(ARVORE_B_ORDEM_FIXA), proximoIdDisponivel(1), nomeArquivoDados(arquivoDados) {
    raiz = nullptr;
}

ArvoreB::~ArvoreB() {
    deletarNos(raiz);
}

void ArvoreB::deletarNos(NoArvoreB* no) {
    if (no == nullptr) return;
    if (!no->ehFolha) {
        for (NoArvoreB* filho : no->filhos) {
            deletarNos(filho);
        }
    }
    delete no;
}

void ArvoreB::coletarTodosLivros(NoArvoreB* no, std::vector<Livro>& todosLivros) {
    if (no == nullptr) return;

    if (!no->ehFolha) {
        for (int i = 0; i < no->chaves.size(); ++i) {
            coletarTodosLivros(no->filhos[i], todosLivros);
            todosLivros.push_back(no->livros[i]);
        }
        coletarTodosLivros(no->filhos[no->filhos.size() - 1], todosLivros);
    } else {
        for (const auto& livro : no->livros) {
            todosLivros.push_back(livro);
        }
    }
}

void ArvoreB::escreverLivrosNoArquivo() {
    std::ofstream arquivo(nomeArquivoDados, std::ios::trunc);
    if (!arquivo.is_open()) {
        std::cerr << "Erro ao abrir o arquivo para escrita: " << nomeArquivoDados << std::endl;
        return;
    }

    std::vector<Livro> todosLivros;
    coletarTodosLivros(raiz, todosLivros);

    std::sort(todosLivros.begin(), todosLivros.end(), [](const Livro& a, const Livro& b) {
        return a.id < b.id;
    });

    for (const auto& livro : todosLivros) {
        arquivo << livro.toString() << std::endl;
    }
    arquivo.close();
}

void ArvoreB::inserirNaoCheio(NoArvoreB* no, const std::string& chave, const Livro& livro) {
    int i = no->chaves.size() - 1;

    if (no->ehFolha) {
        no->chaves.resize(no->chaves.size() + 1);
        no->livros.resize(no->livros.size() + 1);

        while (i >= 0 && chave < no->chaves[i]) {
            no->chaves[i + 1] = no->chaves[i];
            no->livros[i + 1] = no->livros[i];
            i--;
        }
        no->chaves[i + 1] = chave;
        no->livros[i + 1] = livro;
    } else {
        while (i >= 0 && chave < no->chaves[i]) {
            i--;
        }
        i++;

        if (no->filhos[i]->chaves.size() == (2 * ordem - 1)) {
            dividirFilho(no, i, no->filhos[i]);
            if (chave > no->chaves[i]) {
                i++;
            }
        }
        inserirNaoCheio(no->filhos[i], chave, livro);
    }
}

void ArvoreB::dividirFilho(NoArvoreB* pai, int indiceFilho, NoArvoreB* filhoParaDividir) {
    NoArvoreB* novoNo = new NoArvoreB(filhoParaDividir->ehFolha);

    std::string chavePromovida = filhoParaDividir->chaves[ordem - 1];
    Livro livroPromovido = filhoParaDividir->livros[ordem - 1];

    pai->chaves.insert(pai->chaves.begin() + indiceFilho, chavePromovida);
    pai->livros.insert(pai->livros.begin() + indiceFilho, livroPromovido);
    pai->filhos.insert(pai->filhos.begin() + indiceFilho + 1, novoNo);

    for (int j = 0; j < ordem - 1; j++) {
        novoNo->chaves.push_back(filhoParaDividir->chaves[j + ordem]);
        novoNo->livros.push_back(filhoParaDividir->livros[j + ordem]);
    }

    if (!filhoParaDividir->ehFolha) {
        for (int j = 0; j < ordem; j++) {
            novoNo->filhos.push_back(filhoParaDividir->filhos[j + ordem]);
        }
    }

    filhoParaDividir->chaves.resize(ordem - 1);
    filhoParaDividir->livros.resize(ordem - 1);
    if (!filhoParaDividir->ehFolha) {
        filhoParaDividir->filhos.resize(ordem);
    }
}

void ArvoreB::inserirLivroInterno(const Livro& livro) {
    std::string chave = livro.getChaveIndice();

    if (raiz == nullptr) {
        raiz = new NoArvoreB(true);
        raiz->chaves.push_back(chave);
        raiz->livros.push_back(livro);
    } else {
        if (raiz->chaves.size() == (2 * ordem - 1)) {
            NoArvoreB* s = new NoArvoreB(false);
            s->filhos.push_back(raiz);
            dividirFilho(s, 0, raiz);
            int i = 0;
            if (chave > s->chaves[0]) {
                i++;
            }
            inserirNaoCheio(s->filhos[i], chave, livro);
            raiz = s;
        } else {
            inserirNaoCheio(raiz, chave, livro);
        }
    }
}

void ArvoreB::carregarDoArquivo() {
    std::ifstream arquivo(nomeArquivoDados);
    std::string linha;

    if (!arquivo.is_open()) {
        std::cerr << "Erro ao abrir o arquivo: " << nomeArquivoDados << ". Certifique-se de que ele existe." << std::endl;
        return;
    }

    proximoIdDisponivel = 1;

    std::vector<Livro> livrosCarregados;

    while (std::getline(arquivo, linha)) {
        std::stringstream ss(linha);
        std::string segmento;
        std::vector<std::string> campos;

        while (std::getline(ss, segmento, '|')) {
            campos.push_back(segmento);
        }

        if (campos.size() == 9) {
            try {
                int id = std::stoi(campos[0]);
                double preco = std::stod(campos[5]);
                int quantidade = std::stoi(campos[6]);

                Livro livro(id, campos[1], campos[2], campos[3], campos[4],
                            preco, quantidade, campos[7], campos[8]);
                livrosCarregados.push_back(livro);
                if (id >= proximoIdDisponivel) {
                    proximoIdDisponivel = id + 1;
                }
            } catch (const std::exception& e) {
                std::cerr << "Erro ao parsear linha do arquivo: '" << linha << "' - " << e.what() << std::endl;
            }
        } else {
            std::cerr << "Formato de linha inválido: '" << linha << "'" << std::endl;
        }
    }
    arquivo.close();

    std::sort(livrosCarregados.begin(), livrosCarregados.end(), [](const Livro& a, const Livro& b) {
        return a.id < b.id;
    });

    for (const auto& livro : livrosCarregados) {
        inserirLivroInterno(livro);
    }
    std::cout << "Livros carregados do arquivo '" << nomeArquivoDados << "' para a árvore B." << std::endl;
    std::cout << "Próximo ID disponível para novos livros será: " << proximoIdDisponivel << "" << std::endl;
}

void ArvoreB::inserirLivro(Livro& livro) {
    if (livro.id == 0) {
        livro.id = proximoIdDisponivel;
        proximoIdDisponivel++;
    } else {
        if (livro.id >= proximoIdDisponivel) {
            proximoIdDisponivel = livro.id + 1;
        }
    }

    std::string chave = livro.getChaveIndice();

    if (raiz == nullptr) {
        raiz = new NoArvoreB(true);
        raiz->chaves.push_back(chave);
        raiz->livros.push_back(livro);
    } else {
        if (raiz->chaves.size() == (2 * ordem - 1)) {
            NoArvoreB* s = new NoArvoreB(false);
            s->filhos.push_back(raiz);
            dividirFilho(s, 0, raiz);
            int i = 0;
            if (chave > s->chaves[0]) {
                i++;
            }
            inserirNaoCheio(s->filhos[i], chave, livro);
            raiz = s;
        } else {
            inserirNaoCheio(raiz, chave, livro);
        }
    }

    std::cout << "\nLivro '" << livro.nome << "' (ID: " << livro.id << ") inserido." << std::endl;
    escreverLivrosNoArquivo();
    std::cout << "Arquivo '" << nomeArquivoDados << "'atualizado com o novo livro." << std::endl;
}

// Função para buscar livros por gênero (atualizada para normalização e busca parcial)
std::vector<Livro> ArvoreB::buscarLivroPorGenero(const std::string& generoBuscado) {
    std::vector<Livro> resultados;
    
    // Normaliza o termo de busca
    std::string generoBuscadoNormalizado = Livro::normalizeString(generoBuscado);

    std::function<void(NoArvoreB*)> buscarRecursivamente =
        [&](NoArvoreB* no) {
        if (no == nullptr) return;

        for (size_t i = 0; i < no->chaves.size(); ++i) {
            if (!no->ehFolha) {
                buscarRecursivamente(no->filhos[i]);
            }
            
            // Normaliza o gênero do livro
            std::string generoLivroNormalizado = Livro::normalizeString(no->livros[i].genero);

            // Usa find() para verificar se o gênero normalizado contém o termo de busca normalizado
            if (generoLivroNormalizado.find(generoBuscadoNormalizado) != std::string::npos) {
                resultados.push_back(no->livros[i]);
            }
        }
        if (!no->ehFolha) {
            buscarRecursivamente(no->filhos[no->filhos.size() - 1]);
        }
    };

    buscarRecursivamente(raiz);

    std::cout << "\nBuscando livros do gênero (parcial/normalizado) '" << generoBuscado << "':" << std::endl;
    if (!resultados.empty()) {
        for (const auto& livro : resultados) {
            std::cout << "   - ID: " << livro.id << " | Título: " << livro.nome << " | Autor: " << livro.autor << std::endl;
        }
    } else {
        std::cout << "   Nenhum livro do gênero '" << generoBuscado << "' encontrado." << std::endl;
    }
    return resultados;
}

// Função para buscar livros por autor (atualizada para normalização e busca parcial)
std::vector<Livro> ArvoreB::buscarLivroPorAutor(const std::string& autorBuscado) {
    std::vector<Livro> resultados;

    // Normaliza o termo de busca
    std::string autorBuscadoNormalizado = Livro::normalizeString(autorBuscado);

    std::function<void(NoArvoreB*)> buscarRecursivamente =
        [&](NoArvoreB* no) {
        if (no == nullptr) return;

        for (size_t i = 0; i < no->chaves.size(); ++i) {
            if (!no->ehFolha) {
                buscarRecursivamente(no->filhos[i]);
            }
            
            // Normaliza o nome do autor do livro
            std::string autorLivroNormalizado = Livro::normalizeString(no->livros[i].autor);

            // Usa find() para verificar se o autor normalizado contém o termo de busca normalizado
            if (autorLivroNormalizado.find(autorBuscadoNormalizado) != std::string::npos) {
                resultados.push_back(no->livros[i]);
            }
        }
        if (!no->ehFolha) {
            buscarRecursivamente(no->filhos[no->filhos.size() - 1]);
        }
    };

    buscarRecursivamente(raiz);

    std::cout << "\nBuscando livros do autor (parcial/normalizado) '" << autorBuscado << "':" << std::endl;
    if (!resultados.empty()) {
        for (const auto& livro : resultados) {
            std::cout << "   - ID: " << livro.id << " | Título: " << livro.nome << " | Gênero: " << livro.genero << std::endl;
        }
    } else {
        std::cout << "   Nenhum livro do autor '" << autorBuscado << "' encontrado." << std::endl;
    }
    return resultados;
}

// Função para buscar livros por nome (atualizada para normalização e busca parcial)
std::vector<Livro> ArvoreB::buscarLivroPorNome(const std::string& nomeBuscado) {
    std::vector<Livro> resultados;

    // Normaliza o termo de busca
    std::string nomeBuscadoNormalizado = Livro::normalizeString(nomeBuscado);

    std::function<void(NoArvoreB*)> buscarRecursivamente =
        [&](NoArvoreB* no) {
        if (no == nullptr) return;

        for (size_t i = 0; i < no->chaves.size(); ++i) {
            if (!no->ehFolha) {
                buscarRecursivamente(no->filhos[i]);
            }
            
            // Normaliza o nome do livro
            std::string nomeLivroNormalizado = Livro::normalizeString(no->livros[i].nome);

            // Usa find() para verificar se o nome normalizado contém o termo de busca normalizado
            if (nomeLivroNormalizado.find(nomeBuscadoNormalizado) != std::string::npos) {
                resultados.push_back(no->livros[i]);
            }
        }
        if (!no->ehFolha) {
            buscarRecursivamente(no->filhos[no->filhos.size() - 1]);
        }
    };

    buscarRecursivamente(raiz);

    std::cout << "\nBuscando livros com o nome (parcial/normalizado) '" << nomeBuscado << "':" << std::endl;
    if (!resultados.empty()) {
        for (const auto& livro : resultados) {
            std::cout << "   - ID: " << livro.id << " | Título: " << livro.nome << " | Autor: " << livro.autor << " | Gênero: " << livro.genero << std::endl;
        }
    } else {
        std::cout << "   Nenhum livro com o nome '" << nomeBuscado << "' encontrado." << std::endl;
    }
    return resultados;
}

bool ArvoreB::atualizarLivro(int idLivro, const Livro& novoLivro) {
    bool atualizado = false;
    std::function<void(NoArvoreB*)> atualizarRecursivamente =
        [&](NoArvoreB* no) {
        if (no == nullptr || atualizado) return;

        for (size_t i = 0; i < no->chaves.size(); ++i) {
            if (!no->ehFolha) {
                atualizarRecursivamente(no->filhos[i]);
            }
            if (no->livros[i].id == idLivro) {
                no->livros[i] = novoLivro;
                atualizado = true;
                return;
            }
        }
        if (!no->ehFolha) {
            atualizarRecursivamente(no->filhos[no->filhos.size() - 1]);
        }
    };

    atualizarRecursivamente(raiz);

    if (atualizado) {
        std::cout << "\nLivro com ID " << idLivro << " atualizado para '" << novoLivro.nome << "'." << std::endl;
        escreverLivrosNoArquivo();
        std::cout << "Arquivo '" << nomeArquivoDados << "' atualizado com as modificações." << std::endl;
    } else {
        std::cout << "\nLivro com ID " << idLivro << " não encontrado para atualização." << std::endl;
    }
    return atualizado;
}