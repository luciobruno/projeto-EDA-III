# Livraria Leitura
## Projeto de Gerenciamento de Livros e Sistema de Recomendação

Este projeto implementa um sistema de gerenciamento para a livraria Leitura, utilizando estruturas de dados de Árvore B para armazenamento e indexação tanto de livros quanto de usuários. Os dados são persistidos em arquivos de texto (livros.txt e usuarios.txt).

## Funcionalidades

* **Duas Árvores B Independentes:**
    * Uma Árvore B para gerenciar **Livros**, indexada pelo nome do livro.
    * Uma Árvore B separada para gerenciar **Usuários**, indexada pelo nome de usuário.
* **Gerenciamento de Livros:**
    * **Leitura de Dados:** Carrega informações de livros de um arquivo `livros.txt` para a Árvore B.
    * **Geração Automática de ID:** Atribui IDs únicos a novos livros automaticamente.
    * **Inserção de Livros:** Adiciona novos livros à Árvore B e atualiza o arquivo `livros.txt`.
    * **Busca por Gênero:** Localiza e exibe livros com base no gênero especificado, suportando **busca parcial e normalização (sem acentos e case-insensitive)**.
    * **Busca por Autor:** Localiza e exibe livros por autor, suportando **busca parcial e normalização**.
    * **Busca por Nome do Livro:** Localiza e exibe livros por nome, suportando **busca parcial e normalização**.
    * **Atualização de Livros:** Modifica informações de um livro existente (baseado no ID) e reflete as alterações no arquivo `livros.txt`.
    * **Validação de Entrada:** Impede que o caractere delimitador `|` seja inserido nos campos de texto, evitando corrupção do arquivo.
    * **Persistência:** Todas as operações (inserção, atualização) são salvas automaticamente de volta no `livros.txt`, garantindo que os dados não sejam perdidos ao encerrar o programa.
    * **Visualização em Tabela:** Função para imprimir o conteúdo atual do `livros.txt` em um formato de tabela legível no terminal.
* **Gerenciamento de Usuários e Recomendação:**
    * **Criação/Atualização de Usuários:** Ao realizar uma busca por livro (gênero, autor, nome) no modo de usuário, um novo usuário é criado se não existir, ou seu histórico de busca é atualizado.
    * **Persistência de Usuários:** O histórico de buscas dos usuários é salvo no arquivo `usuarios.txt`.
    * **Recomendação de Livros:** Baseado no histórico de buscas de um usuário (gêneros e autores mais frequentes), o sistema pode sugerir livros relevantes.

## Estrutura dos Arquivos de Dados

### `livros.txt`

O arquivo `livros.txt` segue o formato delimitado por `|` (pipe), com uma linha para cada livro e os campos na seguinte ordem:

`id|nome|autor|ibsn|gênero|preço|quantidade|editora|sinopse`

**Exemplo:**
- 1|O Senhor dos Anéis|J.R.R. Tolkien|978-85-333-0242-2|Fantasia|75.50|10|HarperCollins|A saga da Terra-média.
- 2|Harry Potter e a Pedra Filosofal|J.K. Rowling|978-85-325-1101-0|Fantasia|50.00|15|Rocco|A jornada de um jovem bruxo.

### `usuarios.txt`

O arquivo `usuarios.txt` armazena o histórico de buscas dos usuários. Cada linha representa um usuário com seus dados delimitados por `|`. Termos de busca dentro de uma categoria são separados por vírgula `,`.

`nomeUsuario|generos_buscados|autores_buscados|nomes_livros_buscados`

**Exemplo:**
- Alice|Fantasia,Ficcao Cientifica|J.K. Rowling,Isaac Asimov|Harry Potter
- Bob|Romance||O Pequeno Principe

*(Campos vazios `||` indicam que não houve buscas para aquela categoria.)*

## Estrutura do Projeto
```
PROJETO EDA III/
├── headers/
│   ├── Livro.h           # Declaração da struct Livro e funções utilitárias de string (normalização)
│   ├── NoArvoreB.h       # Declaração da struct NoArvoreB e ordem fixa da árvore de livros
│   ├── ArvoreB.h         # Declaração da classe ArvoreB (para livros)
│   ├── Usuario.h         # Declaração da struct Usuario e funções utilitárias de string (normalização)
│   ├── NoArvoreBUsuario.h # Declaração da struct NoArvoreBUsuario e ordem fixa da árvore de usuários
│   ├── ArvoreBUsuario.h  # Declaração da classe ArvoreBUsuario (para usuários)
│   └── Utils.h           # Declaração de classes e funções utilitárias (menus, leitura de input, sanitização, recomendação)
├── sources/
│   ├── Livro.cpp         # Implementação dos métodos de Livro
│   ├── ArvoreB.cpp       # Implementação dos métodos de ArvoreB
│   ├── Usuario.cpp       # Implementação dos métodos de Usuario
│   ├── ArvoreBUsuario.cpp # Implementação dos métodos de ArvoreBUsuario
│   └── Utils.cpp         # Implementação das funções utilitárias
├── main.cpp              # Função principal que gerencia o fluxo do programa
├── livros.txt            # Arquivo de dados dos livros
├── usuarios.txt          # Arquivo de dados dos usuários
├── build_and_run.bat     # Script para compilar e executar (Windows)
└── output/               # Diretório para o executável
    └── codigo.exe
```

## Como Compilar e Executar o Projeto (Windows)

Para facilitar o processo, você pode usar o script `build_and_run.bat` fornecido.

### Pré-requisitos

* **Compilador C++ (g++)**: Certifique-se de ter um compilador g++ instalado e configurado no PATH do seu sistema (por exemplo, através do MinGW-w64 ou WSL). Você pode verificar abrindo o Prompt de Comando (CMD) e digitando `g++ --version`.

### Passos:

1.  **Navegue até a Pasta Raiz do Projeto:**
    Abra o **Prompt de Comando (CMD)** ou **PowerShell** e use o comando `cd` para ir até a pasta `PROJETO EDA III/`.

    ```cmd
    # Exemplo no CMD
    cd C:\Caminho\Para\Seu\Projeto\PROJETO EDA III
    ```
    ```powershell
    # Exemplo no PowerShell
    cd C:\Caminho\Para\Seu\Projeto\PROJETO EDA III
    ```
    *(Substitua `C:\Caminho\Para\Seu\Projeto` pelo caminho real da sua pasta `PROJETO EDA III`).*

2.  **Execute o Script de Build e Execução:**
    Na pasta raiz do projeto, execute o script:

    ```cmd
    build_and_run.bat
    ```
    ou, se estiver no PowerShell:
    ```powershell
    .\build_and_run.bat
    ```

O script irá:
* Limpar arquivos de compilação antigos.
* Criar o diretório `output/` se ele não existir.
* Compilar todos os arquivos `.cpp` do projeto.
* Linkar os arquivos compilados para criar o executável `codigo.exe` dentro de `output/`.
* **Executar `codigo.exe` de dentro do diretório `output/`**, garantindo que o `livros.txt` na raiz do projeto seja lido e atualizado corretamente.