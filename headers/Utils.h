#ifndef UTILS_H
#define UTILS_H

class ArvoreB;
class ArvoreBUsuario;

#include <string>
#include <vector>
#include <limits>

#include "Livro.h"
#include "Usuario.h"

void exibirMenuPrincipal();
void exibirMenuLivros();
void exibirMenuUsuarios();

Livro lerDadosNovoLivro();
Livro lerDadosAtualizacaoLivro(int idExistente);
void imprimirTabelaLivros(const std::string& nomeArquivo);

std::string sanitizeString(const std::string& input);
std::string lerNomeUsuario();

void listarTodosUsuarios(ArvoreBUsuario& arvoreUsuarios);
void recomendarLivros(ArvoreBUsuario& arvoreUsuarios, ArvoreB& arvoreLivros);

#endif