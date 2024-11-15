#include "busca.h"

listaArtigo *criarArtigo(char *titulo, Autor *autores) {
    listaArtigo *novoArtigo = (listaArtigo*)malloc(sizeof(listaArtigo));
    novoArtigo->titulo = strdup(titulo);  // Use strdup para alocar memória
    novoArtigo->autores = autores;
    novoArtigo->prev = NULL;
    novoArtigo->next = NULL;
    return novoArtigo;
}

Autor *criarAutor(char *nome) {
    Autor *novoAutor = (Autor *)malloc(sizeof(Autor));
    novoAutor->nome = strdup(nome);  // Aloca memória e copia o nome
    novoAutor->next = NULL;
    return novoAutor;
}

void removerNovaLinha(char* linha) {
    char* pos;
    if ((pos = strchr(linha, '\n')) != NULL) {
        *pos = '\0';
    }
}

listaArtigo* abrirArquivo() {
    FILE *arquivo = fopen("artigos.txt", "r");
    if (!arquivo) {
        perror("Erro ao abrir o arquivo");
        return NULL;
    }

    listaArtigo *inicio = NULL, *ultimo = NULL;
    char linha[1024];

    while (fgets(linha, sizeof(linha), arquivo)) {
        removerNovaLinha(linha);
        listaArtigo *novoArtigo = atribuirDados(linha);

        if (novoArtigo) {
            if (!inicio) {
                inicio = novoArtigo;
            } else {
                ultimo->next = novoArtigo;
                novoArtigo->prev = ultimo;
            }
            ultimo = novoArtigo;
        }
    }

    fclose(arquivo);
    return inicio;
}

listaArtigo *atribuirDados(const char* linha) {
    char linhaCopia[1024];
    strncpy(linhaCopia, linha, sizeof(linhaCopia) - 1);
    linhaCopia[sizeof(linhaCopia) - 1] = '\0';

    char *titulo = NULL;
    Autor *autores = NULL, *ultimoAutor = NULL;

    char *token = strtok(linhaCopia, ";");
    while (token != NULL) {
        if (strncmp(token, "Titulo:", 7) == 0) {
            titulo = strdup(token + 7);
        } else if (strncmp(token, "Autores:", 8) == 0) {
            char *autorToken = strtok(token + 8, ",");
            while (autorToken != NULL) {
                Autor *novoAutor = criarAutor(autorToken);
                if (!autores) {
                    autores = novoAutor;
                } else {
                    ultimoAutor->next = novoAutor;
                }
                ultimoAutor = novoAutor;
                autorToken = strtok(NULL, ",");
            }
        }
        token = strtok(NULL, ";");
    }

    if (!titulo) {
        printf("Erro: Título não encontrado.\n");
        return NULL;
    }

    return criarArtigo(titulo, autores);
}

void imprimirListaArtigos(listaArtigo *lista) {
    while (lista != NULL) {
        printf("Titulo: %s\n", lista->titulo);
        printf("Autores: ");
        
        Autor *autorAtual = lista->autores;
        while (autorAtual != NULL) {
            printf("%s", autorAtual->nome);
            if (autorAtual->next != NULL) {
                printf(", ");
            }
            autorAtual = autorAtual->next;
        }
        
        printf("\n\n");  // Adiciona uma linha em branco entre os artigos
        lista = lista->next;
    }
}

int main() {
    listaArtigo *listaCompleta = abrirArquivo(); 
    imprimirListaArtigos(listaCompleta);

    return 0;
}
