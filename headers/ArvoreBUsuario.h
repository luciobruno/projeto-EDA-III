#ifndef ARVORE_B_USUARIO_H
#define ARVORE_B_USUARIO_H

#include <string>
#include <vector>
#include <functional>

#include "Usuario.h"
#include "NoArvoreBUsuario.h"

class ArvoreBUsuario {
private:
    NoArvoreBUsuario* raiz;
    int ordem;
    std::string nomeArquivoDados;

    void coletarTodosUsuarios(NoArvoreBUsuario* no, std::vector<Usuario>& todosUsuarios);
    void escreverUsuariosNoArquivo();
    void inserirNaoCheio(NoArvoreBUsuario* no, const std::string& chave, const Usuario& usuario);
    void dividirFilho(NoArvoreBUsuario* pai, int indiceFilho, NoArvoreBUsuario* filhoParaDividir);
    void deletarNos(NoArvoreBUsuario* no);
    
    Usuario* buscarUsuarioInterno(NoArvoreBUsuario* no, const std::string& chaveBuscada);


public:
    ArvoreBUsuario(const std::string& arquivoDados);
    ~ArvoreBUsuario();

    void carregarDoArquivo();
    void inserirOuAtualizarUsuario(Usuario& usuario);
    Usuario* buscarUsuario(const std::string& nomeUsuario);
};

#endif