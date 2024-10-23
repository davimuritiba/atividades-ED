#include <stdio.h>
#include <string.h>
#include <stdlib.h>

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
    int capacidadeAltura;    // Capacidade restante de altura (empilhamento)
} Prateleira;

typedef struct Estante {
    Prateleira prateleiras[6];  // Lista encadeada de prateleiras, caso precise de flexibilidade
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
void atribuirVolumeAosLivros(Livro *livro);
int quantidadeListaLivros(Livro *livro);
void separarListas(Livro *listaLivrosCompleta, Livro **livrosLeves, Livro **livrosMedios, Livro **livrosPesados);
void liberarListaLivros(Livro *livro);
