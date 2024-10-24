#include "estante.h"

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
        if (strncmp(token, "ticulo=", 7) == 0) {
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
void printarListaLivro(Livro* livro) {
    while (livro != NULL) {
        printf("Titulo: %s\nAutor: %s\nLargura: %d\nAltura: %d\nProfundidade: %d\nVolume: %d\n",
               livro->titulo, livro->autor, livro->largura, livro->altura, livro->profundidade, livro->volume);
        livro = livro->next;  // Avança para o próximo livro
    }
}

//Função para calcular e atribuir os volumes a listacompleta de livros
void atribuirVolumeAosLivros(Livro *livro){
    while(livro != NULL){
        livro->volume = livro->largura * livro->altura * livro->profundidade;
        livro = livro->next;
    }
}

//Função para calcular a quantidade de livros em uma lista
int quantidadeListaLivros(Livro *livro){
    int indice = 0;
 
    while(livro != NULL){
        indice++;
        livro = livro->next;
    }

    return indice;
}

void separarListas(Livro *listaLivrosCompleta, Livro **livrosLeves, Livro **livrosMedios, Livro **livrosPesados) {
    Livro *ultimoLeve = NULL, *ultimoMedio = NULL, *ultimoPesado = NULL;

    while(listaLivrosCompleta != NULL) {
        Livro *novoLivro = (Livro *)malloc(sizeof(Livro));
        if (novoLivro == NULL) {
            printf("Erro ao alocar memória para o livro.\n");
            exit(1);
        }
        *novoLivro = *listaLivrosCompleta; // Copia os dados do livro

        if(novoLivro->largura < 11) {
            if (*livrosLeves == NULL) {
                *livrosLeves = novoLivro; // Primeiro livro na lista leve
            } else {
                ultimoLeve->next = novoLivro; // Adiciona na lista
            }
            ultimoLeve = novoLivro; // Atualiza o último leve
        } else if(novoLivro->largura > 20) {
            if (*livrosPesados == NULL) {
                *livrosPesados = novoLivro;
            } else {
                ultimoPesado->next = novoLivro;
            }
            ultimoPesado = novoLivro;
        } else {
            if (*livrosMedios == NULL) {
                *livrosMedios = novoLivro;
            } else {
                ultimoMedio->next = novoLivro;
            }
            ultimoMedio = novoLivro;
        }

        listaLivrosCompleta = listaLivrosCompleta->next;
    }

    // Certifique-se de que a lista finaliza corretamente
    if (ultimoLeve) ultimoLeve->next = NULL;
    if (ultimoMedio) ultimoMedio->next = NULL;
    if (ultimoPesado) ultimoPesado->next = NULL;
}

//Função para liberar as lista de livros
void liberarListaLivros(Livro *livro) {
    while (livro != NULL) {
        Livro *temp = livro;
        livro = livro->next;
        free(temp);
    }
}

// Função para criar uma nova estante
Estante* criarEstante() {
    Estante* novaEstante = (Estante*) malloc(sizeof(Estante));

    if (novaEstante == NULL) {
        printf("Erro ao alocar memória para a estante.\n");
        exit(1);
    }

    // Primeira prateleira da estante
    for(int i = 0; i < 6; i++){
        novaEstante->prateleiras[i].livros = NULL;
        novaEstante->prateleiras[i].capacidadeLargura;
    }

    novaEstante->next = NULL;
    novaEstante->prev = NULL;
    return novaEstante;
}

// Função para inicializar a biblioteca
Biblioteca* criarBiblioteca() {
    Biblioteca* novaBiblioteca = (Biblioteca*) malloc(sizeof(Biblioteca));
    if (novaBiblioteca == NULL) {
        printf("Erro ao alocar memória para a biblioteca.\n");
        exit(1);
    }

    novaBiblioteca->estantes = criarEstante();  // Primeira estante
    novaBiblioteca->numEstantes = 1;  // Começamos com 1 estante
    return novaBiblioteca;
}

//Função para adicionar uma estante nova á biblioteca
void adicionarEstante(Biblioteca* biblioteca) {
    Estante* novaEstante = criarEstante();
    
    if (novaEstante == NULL) {
        return;  // Erro na criação da estante
    }

    if (biblioteca->estantes == NULL) {
        // Se a biblioteca estiver vazia, a nova estante será a primeira
        biblioteca->estantes = novaEstante;
    } else {
        // Encontra a última estante
        Estante* atual = biblioteca->estantes;
        while (atual->next != NULL) {
            atual = atual->next;
        }
        
        // Conecta a nova estante no final da lista
        atual->next = novaEstante;
        novaEstante->prev = atual;
    }

    biblioteca->numEstantes++;  // Incrementa o número de estantes na biblioteca
}

void printarBiblioteca(Biblioteca *biblioteca) {
    if (biblioteca == NULL || biblioteca->estantes == NULL) {
        printf("A biblioteca está vazia.\n");
        return;
    }

    Estante *estanteAtual = biblioteca->estantes;
    int numEstante = 1;
    int quantidadeLivros = 0;

    while (estanteAtual != NULL) {
        printf("Estante %d:\n", numEstante);

        for (int i = 0; i < 6; i++) {
            Prateleira prateleiraAtual = estanteAtual->prateleiras[i];
            printf("  Prateleira %d (Capacidade restante de largura: %d cm):\n", i + 1, prateleiraAtual.capacidadeLargura);

            Livro *livroAtual = prateleiraAtual.livros;
            if (livroAtual == NULL) {
                printf("    Nenhum livro nesta prateleira.\n");
            } 
            else {
                while (livroAtual != NULL) {
                    quantidadeLivros++;
                    livroAtual = livroAtual->next;
                }
                printf("A quantidade de livros nessa prateleira eh: %d\n", quantidadeLivros);
            }
        }

        estanteAtual = estanteAtual->next;
        numEstante++;
    }
}

double larguraMedia(Livro *livro){
    double largura = 0;

    while(livro != NULL){
        largura += livro->largura;
        livro = livro->next;
    }

    return largura / 1000;
}

int main() {
    Livro* listaLivrosCompleta = abrirArquivo();
    atribuirVolumeAosLivros(listaLivrosCompleta); //Cria e atribui a lista duplamente encadeada de livros
    Livro *livrosFinos = NULL;
    Livro *livrosMedios = NULL;
    Livro *livrosGrossos = NULL;
    separarListas(listaLivrosCompleta, &livrosFinos, &livrosMedios, &livrosGrossos);

    // printf("Largura media: %.2lf\n", larguraMedia(listaLivrosCompleta));

    Biblioteca *biblioteca = criarBiblioteca();
    Estante *estante = biblioteca->estantes;

    biblioteca->estantes = NULL;
    biblioteca->numEstantes = 0;
    adicionarEstante(biblioteca);
    Livro* livroAtual = listaLivrosCompleta;
    printarBiblioteca(biblioteca);

    liberarListaLivros(listaLivrosCompleta);
    liberarListaLivros(livrosFinos);
    liberarListaLivros(livrosMedios);
    liberarListaLivros(livrosGrossos);
    return 0;
}
