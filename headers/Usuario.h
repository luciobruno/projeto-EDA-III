#ifndef USUARIO_H
#define USUARIO_H

#include <string>
#include <vector>
#include <sstream>
#include <algorithm>
#include <cctype>

struct Usuario {
    std::string nomeUsuario;
    std::vector<std::string> generosBuscados;
    std::vector<std::string> autoresBuscados;
    std::vector<std::string> nomesLivrosBuscados;

    Usuario();

    Usuario(const std::string& _nomeUsuario);

    void adicionarGeneroBuscado(const std::string& genero);

    void adicionarAutorBuscado(const std::string& autor);

    void adicionarNomeLivroBuscado(const std::string& nomeLivro);

    std::string getChaveIndice() const;

    std::string toString() const;

    static std::vector<std::string> stringToVector(const std::string& str);

    static std::string normalizeString(const std::string& str) {
        std::string normalized = str;
        std::transform(normalized.begin(), normalized.end(), normalized.begin(),
                       [](unsigned char c){ return std::tolower(c); });
        for (char &c : normalized) {
            switch (c) {
                case 'á': case 'à': case 'ã': case 'â': case 'ä': c = 'a'; break;
                case 'é': case 'è': case 'ê': case 'ë': c = 'e'; break;
                case 'í': case 'ì': case 'î': case 'ï': c = 'i'; break;
                case 'ó': case 'ò': case 'õ': case 'ô': case 'ö': c = 'o'; break;
                case 'ú': case 'ù': case 'û': case 'ü': c = 'u'; break;
                case 'ç': c = 'c'; break;
                default:
                    break;
            }
        }
        return normalized;
    }
};

#endif