#ifndef NO_ARVORE_B_USUARIO_H
#define NO_ARVORE_B_USUARIO_H

#include <vector>
#include <string>
#include "Usuario.h"

const int ARVORE_B_ORDEM_USUARIO_FIXA = 3;

struct NoArvoreBUsuario {
    std::vector<std::string> chaves;
    std::vector<Usuario> usuarios;
    std::vector<NoArvoreBUsuario*> filhos;
    bool ehFolha;

    NoArvoreBUsuario(bool folha) : ehFolha(folha) {}
};

#endif