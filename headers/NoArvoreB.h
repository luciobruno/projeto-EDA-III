#ifndef NO_ARVORE_B_H
#define NO_ARVORE_B_H

#include <vector>
#include <string>
#include "Livro.h"

const int ARVORE_B_ORDEM_FIXA = 3;

struct NoArvoreB {
    std::vector<std::string> chaves;
    std::vector<Livro> livros;
    std::vector<NoArvoreB*> filhos;
    bool ehFolha;

    NoArvoreB(bool folha) : ehFolha(folha) {}
};

#endif