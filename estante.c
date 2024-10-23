#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct livro {
    char titulo[30];
    char autor[30];
    int largura;
    int altura;
    int profundidade;
    struct livro* prev;
    struct livro* next;
} Livro;

// Função para criar um novo livro
Livro* criarLivro(int altura, int largura, int profundidade, char titulo[], char autor[]) {
    Livro* novoLivro = (Livro*) malloc(sizeof(Livro));
    if (novoLivro == NULL) {
        printf("Erro ao alocar memória para o livro.\n");
        return NULL;
    }
    novoLivro->altura = altura;
    novoLivro->largura = largura;
    novoLivro->profundidade = profundidade;
    strcpy(novoLivro->titulo, titulo);
    strcpy(novoLivro->autor, autor);
    novoLivro->prev = NULL;
    novoLivro->next = NULL;
    return novoLivro;
}

// Função para remover a nova linha de uma string
void removerNovaLinha(char* linha) {
    size_t tamanho = strlen(linha);
    if (tamanho > 0 && (linha[tamanho - 1] == '\n' || linha[tamanho - 1] == '\r')) {
        linha[tamanho - 1] = '\0';  // Remove o \n ou \r no final da linha
    }
}

// Função para atribuir dados a um livro a partir de uma linha de texto
Livro* atribuirDados(const char* linha) {
    char linhaCopia[256];
    strncpy(linhaCopia, linha, sizeof(linhaCopia) - 1);
    linhaCopia[sizeof(linhaCopia) - 1] = '\0';  // Garantir que a string está terminada com \0

    removerNovaLinha(linhaCopia);  // Remove \r\n
    char* token = strtok(linhaCopia, ",");   
    char titulo[30] = "", autor[30] = "";
    int largura = 0, altura = 0, profundidade = 0;

    while (token != NULL) {
        if (strncmp(token, "titulo=", 7) == 0) {
            strcpy(titulo, token + 7);
        } else if (strncmp(token, "autor=", 6) == 0) {
            strcpy(autor, token + 6);
        } else if (strncmp(token, "largura=", 8) == 0) {
            largura = atoi(token + 8);
        } else if (strncmp(token, "altura=", 7) == 0) {
            altura = atoi(token + 7);
        } else if (strncmp(token, " profundidade=", 14) == 0) {
            profundidade = atoi(token + 14);
        }
        token = strtok(NULL, ",");
    }

    return criarLivro(altura, largura, profundidade, titulo, autor);
}

// Função para abrir o arquivo e carregar os livros em uma lista duplamente encadeada
Livro* abrirArquivo() {
    FILE* fp = fopen("estanteLivros.txt", "rt");
    if (fp == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        exit(1);
    }

    Livro* listaLivros = NULL;
    Livro* ultimoLivro = NULL;
    char linha[256];

    while (fgets(linha, sizeof(linha), fp) != NULL) {
        Livro* novoLivro = atribuirDados(linha);

        if (listaLivros == NULL) {
            listaLivros = novoLivro;  // Primeiro livro na lista
        } else {
            ultimoLivro->next = novoLivro;  // Conecta o novo livro na lista
            novoLivro->prev = ultimoLivro;
        }
        ultimoLivro = novoLivro;  // Atualiza o último livro
    }

    fclose(fp);
    return listaLivros;
}

// Função para imprimir todos os livros na lista
void printarLista(Livro* livro) {
    while (livro != NULL) {
        printf("Titulo: %s\nAutor: %s\nLargura: %d\nAltura: %d\nProfundidade: %d\n",
               livro->titulo, livro->autor, livro->largura, livro->altura, livro->profundidade);
        livro = livro->next;  // Avança para o próximo livro
    }
}

int main() {
    Livro* listaLivros = abrirArquivo();
    printarLista(listaLivros);

    // Liberar memória (não implementado, mas seria ideal)
    return 0;
}
