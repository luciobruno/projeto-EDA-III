#include "Usuario.h"
#include <sstream>
#include <set>

Usuario::Usuario() : nomeUsuario("") {}

Usuario::Usuario(const std::string& _nomeUsuario) : nomeUsuario(_nomeUsuario) {}

void Usuario::adicionarGeneroBuscado(const std::string& genero) {
    std::string normalizedGenero = normalizeString(genero);
    for (const auto& g : generosBuscados) {
        if (normalizeString(g) == normalizedGenero) {
            return;
        }
    }
    generosBuscados.push_back(genero);
}

void Usuario::adicionarAutorBuscado(const std::string& autor) {
    std::string normalizedAutor = normalizeString(autor);
    for (const auto& a : autoresBuscados) {
        if (normalizeString(a) == normalizedAutor) {
            return;
        }
    }
    autoresBuscados.push_back(autor);
}

void Usuario::adicionarNomeLivroBuscado(const std::string& nomeLivro) {
    std::string normalizedNome = normalizeString(nomeLivro);
    for (const auto& n : nomesLivrosBuscados) {
        if (normalizeString(n) == normalizedNome) {
            return;
        }
    }
    nomesLivrosBuscados.push_back(nomeLivro);
}

std::string Usuario::getChaveIndice() const {
    return nomeUsuario;
}

std::string Usuario::toString() const {
    std::stringstream ss;
    ss << nomeUsuario << "|";

    for (size_t i = 0; i < generosBuscados.size(); ++i) {
        ss << generosBuscados[i];
        if (i < generosBuscados.size() - 1) ss << ",";
    }
    ss << "|";

    for (size_t i = 0; i < autoresBuscados.size(); ++i) {
        ss << autoresBuscados[i];
        if (i < autoresBuscados.size() - 1) ss << ",";
    }
    ss << "|";

    for (size_t i = 0; i < nomesLivrosBuscados.size(); ++i) {
        ss << nomesLivrosBuscados[i];
        if (i < nomesLivrosBuscados.size() - 1) ss << ",";
    }
    return ss.str();
}

std::vector<std::string> Usuario::stringToVector(const std::string& str) {
    std::vector<std::string> vec;
    std::stringstream ss(str);
    std::string item;
    while (std::getline(ss, item, ',')) {
        vec.push_back(item);
    }
    return vec;
}