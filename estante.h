#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define PESO_MAX 96153

typedef struct livro {
    char titulo[30];
    char autor[30];
    int largura;
    int altura;
    int profundidade;
    int volume;
    struct livro* prev;
    struct livro* next;
} Livro;

typedef struct Prateleira {
    Livro *livros; 
    int capacidadeLargura;  // Capacidade restante de largura
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
void removerNovaLinha(char* linha);
Livro* atribuirDados(const char* linha);
Livro* abrirArquivo();
void printarListaLivro(Livro* livro);
int quantidadeListaLivros(Livro *livro);
void liberarLivros(Livro *livro);
Estante* criarEstante();
Biblioteca* criarBiblioteca();
void adicionarEstante(Biblioteca* biblioteca);
void printarBiblioteca(Biblioteca *biblioteca);
void distribuirLivrosNasEstantes(Biblioteca* biblioteca, Livro* listaLivros);
