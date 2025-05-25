#ifndef ARVORE_B_H
#define ARVORE_B_H

#include <string>
#include <vector>
#include <functional>

#include "Livro.h"
#include "NoArvoreB.h"

class ArvoreB {
private:
    NoArvoreB* raiz;
    int ordem;
    int proximoIdDisponivel;
    std::string nomeArquivoDados;

    void coletarTodosLivros(NoArvoreB* no, std::vector<Livro>& todosLivros);
    void escreverLivrosNoArquivo();
    void inserirNaoCheio(NoArvoreB* no, const std::string& chave, const Livro& livro);
    void dividirFilho(NoArvoreB* pai, int indiceFilho, NoArvoreB* filhoParaDividir);
    void inserirLivroInterno(const Livro& livro);
    void deletarNos(NoArvoreB* no);

public:
    ArvoreB(const std::string& arquivoDados);

    ~ArvoreB();

    void carregarDoArquivo();
    void inserirLivro(Livro& livro);
    std::vector<Livro> buscarLivroPorGenero(const std::string& generoBuscado);
    std::vector<Livro> buscarLivroPorAutor(const std::string& autorBuscado);
    std::vector<Livro> buscarLivroPorNome(const std::string& nomeBuscado);
    bool atualizarLivro(int idLivro, const Livro& novoLivro);
};

#endif