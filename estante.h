#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct livro {
    char titulo[30];
    char autor[30];
    int largura, altura, profundidade, volume;
    struct livro* prev;
    struct livro* next;
} Livro;

typedef struct Prateleira {
    Livro *livros; 
    int capacidadeLargura, capacidadeProfundidade, capacidadeAltura;  // Capacidade restante de largura
} Prateleira;

typedef struct Estante {
    Prateleira prateleiras[6];
    struct Estante *prev;
    struct Estante *next;
} Estante;

typedef struct Biblioteca {
    Estante *estantes;
    int numEstantes;  // Contador do número de estantes
} Biblioteca;

Livro* criarLivro(int altura, int largura, int profundidade, char titulo[], char autor[]);
Livro* atribuirDados(const char* linha);
Livro* abrirArquivo();
Estante* criarEstante();
Biblioteca* criarBiblioteca();
void removerNovaLinha(char* linha);
void printarListaLivro(Livro* livro);
void liberarLivros(Livro *livro);
void liberarEstante(Estante *estante);
void adicionarEstante(Biblioteca* biblioteca);
void printarBiblioteca(Biblioteca *biblioteca);
void printarPrateleira(Biblioteca *biblioteca, int numEstante, int numPrateleira);
void distribuirLivrosNasEstantes(Biblioteca* biblioteca, Livro* listaLivros);
void removerLivro(Biblioteca *biblioteca, int numEstante, int numPrateleira, char *tituloLivro);
int emprestarLivro(Biblioteca *biblioteca, Livro **listaEmprestados, const char *tituloLivro);
void inserirLivro(Biblioteca* biblioteca, Livro* novoLivro);
