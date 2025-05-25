#include "ArvoreBUsuario.h"
#include <iostream>
#include <fstream>
#include <algorithm>
#include <stdexcept>
#include <sstream>

ArvoreBUsuario::ArvoreBUsuario(const std::string& arquivoDados)
    : ordem(ARVORE_B_ORDEM_USUARIO_FIXA), nomeArquivoDados(arquivoDados) {
    raiz = nullptr;
}

ArvoreBUsuario::~ArvoreBUsuario() {
    escreverUsuariosNoArquivo();
    deletarNos(raiz);
}

void ArvoreBUsuario::deletarNos(NoArvoreBUsuario* no) {
    if (no == nullptr) return;
    if (!no->ehFolha) {
        for (NoArvoreBUsuario* filho : no->filhos) {
            deletarNos(filho);
        }
    }
    delete no;
}

void ArvoreBUsuario::coletarTodosUsuarios(NoArvoreBUsuario* no, std::vector<Usuario>& todosUsuarios) {
    if (no == nullptr) return;

    if (!no->ehFolha) {
        for (int i = 0; i < no->chaves.size(); ++i) {
            coletarTodosUsuarios(no->filhos[i], todosUsuarios);
            todosUsuarios.push_back(no->usuarios[i]);
        }
        coletarTodosUsuarios(no->filhos[no->filhos.size() - 1], todosUsuarios);
    } else {
        for (const auto& usuario : no->usuarios) {
            todosUsuarios.push_back(usuario);
        }
    }
}

void ArvoreBUsuario::escreverUsuariosNoArquivo() {
    std::ofstream arquivo(nomeArquivoDados, std::ios::trunc);
    if (!arquivo.is_open()) {
        std::cerr << "Erro ao abrir o arquivo para escrita de usuários: " << nomeArquivoDados << std::endl;
        return;
    }

    std::vector<Usuario> todosUsuarios;
    coletarTodosUsuarios(raiz, todosUsuarios);

    std::sort(todosUsuarios.begin(), todosUsuarios.end(), [](const Usuario& a, const Usuario& b) {
        return Usuario::normalizeString(a.nomeUsuario) < Usuario::normalizeString(b.nomeUsuario);
    });

    for (const auto& usuario : todosUsuarios) {
        arquivo << usuario.toString() << std::endl;
    }
    arquivo.close();
}

void ArvoreBUsuario::inserirNaoCheio(NoArvoreBUsuario* no, const std::string& chave, const Usuario& usuario) {
    int i = no->chaves.size() - 1;

    if (no->ehFolha) {
        no->chaves.resize(no->chaves.size() + 1);
        no->usuarios.resize(no->usuarios.size() + 1);

        while (i >= 0 && chave < no->chaves[i]) {
            no->chaves[i + 1] = no->chaves[i];
            no->usuarios[i + 1] = no->usuarios[i];
            i--;
        }
        no->chaves[i + 1] = chave;
        no->usuarios[i + 1] = usuario;
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
        inserirNaoCheio(no->filhos[i], chave, usuario);
    }
}

void ArvoreBUsuario::dividirFilho(NoArvoreBUsuario* pai, int indiceFilho, NoArvoreBUsuario* filhoParaDividir) {
    NoArvoreBUsuario* novoNo = new NoArvoreBUsuario(filhoParaDividir->ehFolha);

    std::string chavePromovida = filhoParaDividir->chaves[ordem - 1];
    Usuario usuarioPromovido = filhoParaDividir->usuarios[ordem - 1];

    pai->chaves.insert(pai->chaves.begin() + indiceFilho, chavePromovida);
    pai->usuarios.insert(pai->usuarios.begin() + indiceFilho, usuarioPromovido);
    pai->filhos.insert(pai->filhos.begin() + indiceFilho + 1, novoNo);

    for (int j = 0; j < ordem - 1; j++) {
        novoNo->chaves.push_back(filhoParaDividir->chaves[j + ordem]);
        novoNo->usuarios.push_back(filhoParaDividir->usuarios[j + ordem]);
    }

    if (!filhoParaDividir->ehFolha) {
        for (int j = 0; j < ordem; j++) {
            novoNo->filhos.push_back(filhoParaDividir->filhos[j + ordem]);
        }
    }

    filhoParaDividir->chaves.resize(ordem - 1);
    filhoParaDividir->usuarios.resize(ordem - 1);
    if (!filhoParaDividir->ehFolha) {
        filhoParaDividir->filhos.resize(ordem);
    }
}

void ArvoreBUsuario::carregarDoArquivo() {
    std::ifstream arquivo(nomeArquivoDados);
    std::string linha;

    if (!arquivo.is_open()) {
        std::cerr << "Aviso: Arquivo de usuários '" << nomeArquivoDados << "' não encontrado. Um novo será criado." << std::endl;
        return;
    }

    std::vector<Usuario> usuariosCarregados;

    while (std::getline(arquivo, linha)) {
        std::stringstream ss(linha);
        std::string segmento;
        std::vector<std::string> campos;

        while (std::getline(ss, segmento, '|')) {
            campos.push_back(segmento);
        }

        if (campos.size() == 4) {
            try {
                Usuario usuario(campos[0]);
                for (const auto& g : Usuario::stringToVector(campos[1])) {
                    usuario.adicionarGeneroBuscado(g);
                }
                for (const auto& a : Usuario::stringToVector(campos[2])) {
                    usuario.adicionarAutorBuscado(a);
                }
                for (const auto& n : Usuario::stringToVector(campos[3])) {
                    usuario.adicionarNomeLivroBuscado(n);
                }
                usuariosCarregados.push_back(usuario);
            } catch (const std::exception& e) {
                std::cerr << "Erro ao parsear linha do arquivo de usuários: '" << linha << "' - " << e.what() << std::endl;
            }
        } else {
            std::cerr << "Formato de linha inválido no arquivo de usuários: '" << linha << "'" << std::endl;
        }
    }
    arquivo.close();

    for (const auto& usuario : usuariosCarregados) {
        Usuario tempUser = usuario;
        inserirOuAtualizarUsuario(tempUser);
    }
    std::cout << "Usuários carregados do arquivo '" << nomeArquivoDados << "' para a árvore B de usuários." << std::endl;
}

Usuario* ArvoreBUsuario::buscarUsuarioInterno(NoArvoreBUsuario* no, const std::string& chaveBuscada) {
    if (no == nullptr) return nullptr;

    int i = 0;
    while (i < no->chaves.size() && Usuario::normalizeString(chaveBuscada) > Usuario::normalizeString(no->chaves[i])) {
        i++;
    }

    if (i < no->chaves.size() && Usuario::normalizeString(chaveBuscada) == Usuario::normalizeString(no->chaves[i])) {
        return &(no->usuarios[i]);
    }

    if (no->ehFolha) {
        return nullptr;
    } else {
        return buscarUsuarioInterno(no->filhos[i], chaveBuscada);
    }
}

Usuario* ArvoreBUsuario::buscarUsuario(const std::string& nomeUsuario) {
    return buscarUsuarioInterno(raiz, nomeUsuario);
}

void ArvoreBUsuario::inserirOuAtualizarUsuario(Usuario& usuario) {
    std::string chave = Usuario::normalizeString(usuario.getChaveIndice());

    Usuario* usuarioExistente = buscarUsuario(usuario.getChaveIndice());

    if (usuarioExistente != nullptr) {
        for (const auto& g : usuario.generosBuscados) {
            usuarioExistente->adicionarGeneroBuscado(g);
        }
        for (const auto& a : usuario.autoresBuscados) {
            usuarioExistente->adicionarAutorBuscado(a);
        }
        for (const auto& n : usuario.nomesLivrosBuscados) {
            usuarioExistente->adicionarNomeLivroBuscado(n);
        }
        std::cout << "\nUsuário '" << usuario.nomeUsuario << "' atualizado com novas buscas." << std::endl;
    } else {
        if (raiz == nullptr) {
            raiz = new NoArvoreBUsuario(true);
            raiz->chaves.push_back(chave);
            raiz->usuarios.push_back(usuario);
        } else {
            if (raiz->chaves.size() == (2 * ordem - 1)) {
                NoArvoreBUsuario* s = new NoArvoreBUsuario(false);
                s->filhos.push_back(raiz);
                dividirFilho(s, 0, raiz);
                int i = 0;
                if (chave > Usuario::normalizeString(s->chaves[0])) {
                    i++;
                }
                inserirNaoCheio(s->filhos[i], chave, usuario);
                raiz = s;
            } else {
                inserirNaoCheio(raiz, chave, usuario);
            }
        }
        std::cout << "\nNovo usuário '" << usuario.nomeUsuario << "' criado." << std::endl;
    }
    escreverUsuariosNoArquivo();
}