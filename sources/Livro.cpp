#include "Livro.h"

#include <sstream>
#include <iomanip>

Livro::Livro(int _id, const std::string& _nome, const std::string& _autor,
          const std::string& _ibsn, const std::string& _genero,
          double _preco, int _quantidade, const std::string& _editora,
          const std::string& _sinopse)
    : id(_id), nome(_nome), autor(_autor), ibsn(_ibsn), genero(_genero),
      preco(_preco), quantidade(_quantidade), editora(_editora), sinopse(_sinopse) {}

Livro::Livro() : id(0), preco(0.0), quantidade(0) {}

std::string Livro::getChaveIndice() const {
    return nome;
}

std::string Livro::toString() const {
    std::stringstream ss;
    ss << "" << id
       << "|" << nome
       << "|" << autor
       << "|" << ibsn
       << "|" << genero
       << "|" << std::fixed << std::setprecision(2) << preco
       << "|" << quantidade
       << "|" << editora
       << "|" << sinopse;
    return ss.str();
}