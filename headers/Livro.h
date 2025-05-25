#ifndef LIVRO_H
#define LIVRO_H

#include <string>
#include <vector>
#include <sstream>
#include <algorithm>
#include <cctype>

struct Livro {
    int id;
    std::string nome;
    std::string autor;
    std::string ibsn;
    std::string genero;
    double preco;
    int quantidade;
    std::string editora;
    std::string sinopse;

    Livro(int _id, const std::string& _nome, const std::string& _autor,
          const std::string& _ibsn, const std::string& _genero,
          double _preco, int _quantidade, const std::string& _editora,
          const std::string& _sinopse);
    Livro();
    std::string getChaveIndice() const;
    std::string toString() const;

    static std::string toLower(const std::string& str) {
        std::string lower_str = str;
        std::transform(lower_str.begin(), lower_str.end(), lower_str.begin(),
                       [](unsigned char c){ return std::tolower(c); });
        return lower_str;
    }
    static std::string normalizeString(const std::string& str) {
        std::string normalized = toLower(str);
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