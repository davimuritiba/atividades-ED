#include "busca.h"

listaArtigos *criarArtigo(char *titulo){

}

listaArtigos *atribuirDados(const char* linha) {
    char linhaCopia[256];
    strncpy(linhaCopia, linha, sizeof(linhaCopia) - 1);
    linhaCopia[sizeof(linhaCopia) - 1] = '\0';  // Garantir que a string está terminada com \0

    removerNovaLinha(linhaCopia);  // Remove \r\n
    char* token = strtok(linhaCopia, ";");   
    char titulo[30] = "", autor[30] = "";
    int largura = 0, altura = 0, profundidade = 0;

    while (token != NULL) {
        if (strncmp(token, "Título:", 7) == 0) {
            strcpy(titulo, token + 7);
        } else if (strncmp(token, "autor=", 6) == 0) {
            strcpy(autor, token + 6);
        }
        token = strtok(NULL, ";");
    }

    return criarArtigo(titulo, autores);
}

// Função para abrir o arquivo e carregar os livros em uma lista duplamente encadeada
listaArtigos* abrirArquivo() {
    FILE* fp = fopen("artigos.txt", "rt");
    if (fp == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        exit(1);
    }

    listaArtigos *listaArtigos = NULL; 
    listaArtigos *ultimoArtigo = NULL;

    char linha[256];

    while (fgets(linha, sizeof(linha), fp) != NULL) {
        listaArtigos* novoArtigo = atribuirDados(linha);

        if (listaArtigos == NULL) {
            listaArtigos = novoLivro;  // Primeiro livro na lista
        } else {
            ultimoArtigo->next = novoLivro;  // Conecta o novo livro na lista
            novoLivro->prev = ultimoArtigo;
        }
        listaArtigos = novoLivro;  // Atualiza o último livro
    }

    fclose(fp);
    return listaArtigos;
}
