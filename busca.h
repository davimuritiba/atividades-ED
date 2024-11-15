#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct autor {
    char *nome;
    struct autor *next;
} Autor;

typedef struct lista_artigos {
    char *titulo;
    Autor *autores;  
    struct lista_artigos *prev;
    struct lista_artigos *next;
} listaArtigo;

listaArtigo *criarArtigo(char *titulo, Autor *autores);
listaArtigo *atribuirDados(const char* linha);
listaArtigo* abrirArquivo();
Autor *criarAutor(char *nome);
void removerNovaLinha(char* linha);
void imprimirListaArtigos(listaArtigo *lista);
