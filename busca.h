#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef struct autor {
    char *nome;
    struct autor *next;
} Autor;

typedef struct lista_artigos {
    char *titulo;
    Autor *autores;
    char *link;
    struct lista_artigos *prev;
    struct lista_artigos *next;
} listaArtigo;

listaArtigo *criarArtigo(char *titulo, Autor *autores, char *link);
listaArtigo *atribuirDados(const char* linha);
listaArtigo* abrirArquivo();
listaArtigo* buscarPorPalavraChave(listaArtigo *lista, const char *palavraChave);
Autor *criarAutor(char *nome);
void removerNovaLinha(char* linha);
void imprimirListaArtigos(listaArtigo *lista);
char *toLowerCase(const char *str);
