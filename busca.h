#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct lista_artigos{
    char *titulo;
    char autores;
    struct lista_artigos *prev;
    struct lista_artigos *next;
} listaArtigos;

