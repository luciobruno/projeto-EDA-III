#include "Utils.h"
#include "ArvoreB.h"
#include "ArvoreBUsuario.h"

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <iomanip>
#include <algorithm>
#include <map>
#include <set>
#include <functional>

std::string sanitizeString(const std::string& input) {
    std::string sanitized = input;
    for (char &c : sanitized) {
        if (c == '|') {
            c = ' ';
        }
    }
    return sanitized;
}

void exibirMenuPrincipal() {
    std::cout << "\n--- MENU PRINCIPAL ---" << std::endl;
    std::cout << "1. Gerenciar Livros" << std::endl;
    std::cout << "2. Gerenciar Usuários e Recomendações" << std::endl;
    std::cout << "0. Sair" << std::endl;
    std::cout << "----------------------" << std::endl;
    std::cout << "Escolha uma opção: ";
}

void exibirMenuLivros() {
    std::cout << "\n--- MENU DE GERENCIAMENTO DE LIVROS ---" << std::endl;
    std::cout << "1. Listar todos os livros (do arquivo)" << std::endl;
    std::cout << "2. Inserir novo livro" << std::endl;
    std::cout << "3. Buscar livro por gênero" << std::endl;
    std::cout << "4. Buscar livro por autor" << std::endl;
    std::cout << "5. Buscar livro por nome" << std::endl;
    std::cout << "6. Atualizar livro (por ID)" << std::endl;
    std::cout << "0. Voltar ao Menu Principal" << std::endl;
    std::cout << "---------------------------------------" << std::endl;
    std::cout << "Escolha uma opção: ";
}

void exibirMenuUsuarios() {
    std::cout << "\n--- MENU DE USUÁRIOS E RECOMENDAÇÕES ---" << std::endl;
    std::cout << "1. Listar todos os usuários" << std::endl;
    std::cout << "2. Buscar livro por Gênero (Salva histórico do usuário)" << std::endl;
    std::cout << "3. Buscar livro por Autor (Salva histórico do usuário)" << std::endl;
    std::cout << "4. Buscar livro por Nome (Salva histórico do usuário)" << std::endl;
    std::cout << "5. Obter Recomendação de Livro para um Usuário" << std::endl;
    std::cout << "0. Voltar ao Menu Principal" << std::endl;
    std::cout << "---------------------------------------" << std::endl;
    std::cout << "Escolha uma opção: ";
}

Livro lerDadosNovoLivro() {
    std::string nome, autor, ibsn, genero, editora, sinopse;
    double preco;
    int quantidade;

    std::cout << "\n--- Inserir Novo Livro ---" << std::endl;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cout << "Nome: ";
    std::getline(std::cin, nome);
    nome = sanitizeString(nome);
    std::cout << "Autor: ";
    std::getline(std::cin, autor);
    autor = sanitizeString(autor);
    std::cout << "IBSN: ";
    std::getline(std::cin, ibsn);
    ibsn = sanitizeString(ibsn);
    std::cout << "Gênero: ";
    std::getline(std::cin, genero);
    genero = sanitizeString(genero);
    std::cout << "Preço: ";
    while (!(std::cin >> preco)) {
        std::cout << "Entrada inválida. Digite um número para o preço: ";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
    std::cout << "Quantidade: ";
    while (!(std::cin >> quantidade)) {
        std::cout << "Entrada inválida. Digite um número inteiro para a quantidade: ";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cout << "Editora: ";
    std::getline(std::cin, editora);
    editora = sanitizeString(editora);
    std::cout << "Sinopse: ";
    std::getline(std::cin, sinopse);
    sinopse = sanitizeString(sinopse);

    return Livro(0, nome, autor, ibsn, genero, preco, quantidade, editora, sinopse);
}

Livro lerDadosAtualizacaoLivro(int idExistente) {
    std::string nome, autor, ibsn, genero, editora, sinopse;
    double preco;
    int quantidade;

    std::cout << "\n--- Atualizar Livro (ID: " << idExistente << ") ---" << std::endl;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cout << "Novo Nome: ";
    std::getline(std::cin, nome);
    nome = sanitizeString(nome);
    std::cout << "Novo Autor: ";
    std::getline(std::cin, autor);
    autor = sanitizeString(autor);
    std::cout << "Novo IBSN: ";
    std::getline(std::cin, ibsn);
    ibsn = sanitizeString(ibsn);
    std::cout << "Novo Gênero: ";
    std::getline(std::cin, genero);
    genero = sanitizeString(genero);
    std::cout << "Novo Preço: ";
    while (!(std::cin >> preco)) {
        std::cout << "Entrada inválida. Digite um número para o preço: ";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
    std::cout << "Nova Quantidade: ";
    while (!(std::cin >> quantidade)) {
        std::cout << "Entrada inválida. Digite um número inteiro para a quantidade: ";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cout << "Nova Editora: ";
    std::getline(std::cin, editora);
    editora = sanitizeString(editora);
    std::cout << "Nova Sinopse: ";
    std::getline(std::cin, sinopse);
    sinopse = sanitizeString(sinopse);

    return Livro(idExistente, nome, autor, ibsn, genero, preco, quantidade, editora, sinopse);
}

void imprimirTabelaLivros(const std::string& nomeArquivo) {
    std::ifstream arquivo(nomeArquivo);
    std::string linha;
    std::vector<Livro> livros;

    if (!arquivo.is_open()) {
        std::cerr << "Erro: Não foi possível abrir o arquivo '" << nomeArquivo << "' para leitura." << std::endl;
        return;
    }

    while (std::getline(arquivo, linha)) {
        std::stringstream ss(linha);
        std::string segmento;
        std::vector<std::string> campos_str;

        while (std::getline(ss, segmento, '|')) {
            campos_str.push_back(segmento);
        }

        if (campos_str.size() == 9) {
            try {
                int id = std::stoi(campos_str[0]);
                double preco = std::stod(campos_str[5]);
                int quantidade = std::stoi(campos_str[6]);

                livros.emplace_back(id, campos_str[1], campos_str[2],
                                     campos_str[3], campos_str[4], preco,
                                     quantidade, campos_str[7], campos_str[8]);
            } catch (const std::exception& e) {
                std::cerr << "Erro ao parsear linha para Livro (dados ou formato): '" << linha << "' - " << e.what() << std::endl;
            }
        } else {
            std::cerr << "Linha com formato inválido no arquivo (número de campos): '" << linha << "'" << std::endl;
        }
    }
    arquivo.close();

    if (livros.empty()) {
        std::cout << "\nNenhum livro válido encontrado no arquivo '" << nomeArquivo << "'." << std::endl;
        return;
    }

    const int padding = 2;

    size_t width_id = std::string("ID").length() + padding;
    size_t width_nome = std::string("NOME").length() + padding;
    size_t width_autor = std::string("AUTOR").length() + padding;
    size_t width_ibsn = std::string("IBSN").length() + padding;
    size_t width_genero = std::string("GENERO").length() + padding;
    size_t width_preco = std::string("PRECO").length() + padding;
    size_t width_quant = std::string("QUANT.").length() + padding;
    size_t width_editora = std::string("EDITORA").length() + padding;
    size_t width_sinopse = std::string("SINOPSE").length() + padding;

    for (const auto& livro : livros) {
        width_id = std::max(width_id, std::to_string(livro.id).length() + padding);
        width_nome = std::max(width_nome, livro.nome.length() + padding);
        width_autor = std::max(width_autor, livro.autor.length() + padding);
        width_ibsn = std::max(width_ibsn, livro.ibsn.length() + padding);
        width_genero = std::max(width_genero, livro.genero.length() + padding);
        
        std::stringstream ss_preco_formatted;
        ss_preco_formatted << std::fixed << std::setprecision(2) << livro.preco;
        width_preco = std::max(width_preco, ss_preco_formatted.str().length() + padding);

        width_quant = std::max(width_quant, std::to_string(livro.quantidade).length() + padding);
        width_editora = std::max(width_editora, livro.editora.length() + padding);
        width_sinopse = std::max(width_sinopse, livro.sinopse.length() + padding);
    }

    std::cout << "\n--- Conteúdo em formato de Tabela ---" << std::endl;

    std::cout << std::left
              << std::setw(static_cast<int>(width_id)) << "ID" << "|"
              << std::setw(static_cast<int>(width_nome)) << "NOME" << "|"
              << std::setw(static_cast<int>(width_autor)) << "AUTOR" << "|"
              << std::setw(static_cast<int>(width_ibsn)) << "IBSN" << "|"
              << std::setw(static_cast<int>(width_genero)) << "GENERO" << "|"
              << std::setw(static_cast<int>(width_preco)) << "PRECO" << "|"
              << std::setw(static_cast<int>(width_quant)) << "QUANT." << "|"
              << std::setw(static_cast<int>(width_editora)) << "EDITORA" << "|"
              << std::setw(static_cast<int>(width_sinopse)) << "SINOPSE"
              << std::endl;

    size_t total_line_width = width_id + width_nome + width_autor + width_ibsn + width_genero +
                              width_preco + width_quant + width_editora + width_sinopse +
                              (9 - 1);
    std::cout << std::string(total_line_width, '-') << std::endl;

    for (const auto& livro : livros) {
        std::stringstream ss_preco_output;
        ss_preco_output << std::fixed << std::setprecision(2) << livro.preco;

        std::cout << std::left
                  << std::setw(static_cast<int>(width_id)) << livro.id << "|"
                  << std::setw(static_cast<int>(width_nome)) << livro.nome << "|"
                  << std::setw(static_cast<int>(width_autor)) << livro.autor << "|"
                  << std::setw(static_cast<int>(width_ibsn)) << livro.ibsn << "|"
                  << std::setw(static_cast<int>(width_genero)) << livro.genero << "|"
                  << std::setw(static_cast<int>(width_preco)) << ss_preco_output.str() << "|"
                  << std::setw(static_cast<int>(width_quant)) << livro.quantidade << "|"
                  << std::setw(static_cast<int>(width_editora)) << livro.editora << "|"
                  << std::setw(static_cast<int>(width_sinopse)) << livro.sinopse
                  << std::endl;
    }

    std::cout << std::string(total_line_width, '-') << std::endl;
    std::cout << "Total de livros exibidos: " << livros.size() << std::endl;
}

std::string lerNomeUsuario() {
    std::string nomeUsuario;
    std::cout << "\nDigite o nome de usuário: ";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::getline(std::cin, nomeUsuario);
    nomeUsuario = sanitizeString(nomeUsuario);
    return nomeUsuario;
}

void listarTodosUsuarios(ArvoreBUsuario& arvoreUsuarios) {
    const std::string NOME_ARQUIVO_USUARIOS = "../usuarios.txt";
    std::ifstream arquivoUsuarios(NOME_ARQUIVO_USUARIOS);
    std::string linhaUsuario;
    if (arquivoUsuarios.is_open()) {
        std::cout << "\n--- LISTA DE USUÁRIOS ---" << std::endl;
        while (std::getline(arquivoUsuarios, linhaUsuario)) {
            std::stringstream ss(linhaUsuario);
            std::string nomeUser;
            std::getline(ss, nomeUser, '|'); 
            std::cout << nomeUser << std::endl;
        }
        arquivoUsuarios.close();
    } else {
        std::cout << "\nNenhum usuário encontrado (arquivo usuarios.txt não existe ou está vazio)." << std::endl;
    }
}


void recomendarLivros(ArvoreBUsuario& arvoreUsuarios, ArvoreB& arvoreLivros) {
    std::string nomeUsuario = lerNomeUsuario();
    Usuario* usuario = arvoreUsuarios.buscarUsuario(nomeUsuario);

    if (usuario == nullptr) {
        std::cout << "Usuário '" << nomeUsuario << "' não encontrado. Não é possível gerar recomendações." << std::endl;
        return;
    }

    std::cout << "\nGerando recomendações para '" << usuario->nomeUsuario << "'..." << std::endl;

    std::map<std::string, int> generoFrequencia;
    std::map<std::string, int> autorFrequencia;

    for (const auto& g : usuario->generosBuscados) {
        generoFrequencia[Usuario::normalizeString(g)]++;
    }
    for (const auto& a : usuario->autoresBuscados) {
        autorFrequencia[Usuario::normalizeString(a)]++;
    }

    std::string generoMaisFrequente = "";
    int maxGeneroCount = 0;
    for (const auto& pair : generoFrequencia) {
        if (pair.second > maxGeneroCount) {
            maxGeneroCount = pair.second;
            generoMaisFrequente = pair.first;
        }
    }

    std::string autorMaisFrequente = "";
    int maxAutorCount = 0;
    for (const auto& pair : autorFrequencia) {
        if (pair.second > maxAutorCount) {
            maxAutorCount = pair.second;
            autorMaisFrequente = pair.first;
        }
    }

    std::vector<Livro> recomendacoes;
    std::set<int> idsRecomendados;

    if (!autorMaisFrequente.empty()) {
        std::cout << "\nRecomendando por autor favorito ('" << autorMaisFrequente << "'):" << std::endl;
        std::vector<Livro> livrosDoAutor = arvoreLivros.buscarLivroPorAutor(autorMaisFrequente);
        for (const auto& livro : livrosDoAutor) {
            bool jaBuscouPeloNome = false;
            for(const auto& nomeBuscado : usuario->nomesLivrosBuscados) {
                if (Livro::normalizeString(livro.nome) == Livro::normalizeString(nomeBuscado)) {
                    jaBuscouPeloNome = true;
                    break;
                }
            }

            if (idsRecomendados.find(livro.id) == idsRecomendados.end() && !jaBuscouPeloNome) {
                recomendacoes.push_back(livro);
                idsRecomendados.insert(livro.id);
                if (recomendacoes.size() >= 3) break;
            }
        }
    }

    if (recomendacoes.size() < 3 && !generoMaisFrequente.empty()) {
        std::cout << "\nRecomendando por gênero favorito ('" << generoMaisFrequente << "'):" << std::endl;
        std::vector<Livro> livrosDoGenero = arvoreLivros.buscarLivroPorGenero(generoMaisFrequente);
        for (const auto& livro : livrosDoGenero) {
            bool jaBuscouPeloNome = false;
            for(const auto& nomeBuscado : usuario->nomesLivrosBuscados) {
                if (Livro::normalizeString(livro.nome) == Livro::normalizeString(nomeBuscado)) {
                    jaBuscouPeloNome = true;
                    break;
                }
            }

            if (idsRecomendados.find(livro.id) == idsRecomendados.end() && !jaBuscouPeloNome) {
                recomendacoes.push_back(livro);
                idsRecomendados.insert(livro.id);
                if (recomendacoes.size() >= 3) break;
            }
        }
    }

    if (recomendacoes.empty()) {
        std::cout << "\nNão foi possível gerar recomendações personalizadas. Sugerindo alguns livros gerais:" << std::endl;
        std::cout << "Nenhum livro para recomendar no momento (nenhuma correspondência ou dados insuficientes)." << std::endl;
    } else {
        std::cout << "\n--- RECOMENDAÇÕES PARA '" << usuario->nomeUsuario << "' ---" << std::endl;
        for (const auto& livro : recomendacoes) {
            std::cout << "  - Título: " << livro.nome << " | Autor: " << livro.autor << " | Gênero: " << livro.genero << std::endl;
        }
    }
}