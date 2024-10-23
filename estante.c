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

//Função dispensavel, apenas para calcular a media e dividir a quantidade de livros
double mediaVolume(Livro *livro){
    double volume = 0;

    while(livro != NULL){
        volume += livro->volume;
        livro = livro->next;
    }

    return volume;
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

        if(novoLivro->volume < 1500) {
            if (*livrosLeves == NULL) {
                *livrosLeves = novoLivro; // Primeiro livro na lista leve
            } else {
                ultimoLeve->next = novoLivro; // Adiciona na lista
            }
            ultimoLeve = novoLivro; // Atualiza o último leve
        } else if(novoLivro->volume > 5200) {
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

int main() {
    Livro* listaLivrosCompleta = abrirArquivo();
    atribuirVolumeAosLivros(listaLivrosCompleta); //Cria e atribui a lista duplamente encadeada de livros
    // printarListaLivro(listaLivrosCompleta);


    double volume = mediaVolume(listaLivrosCompleta);
    printf("media volume: %.2lf\n", volume);

    Livro *livrosLeves = NULL;
    Livro *livrosMedios = NULL;
    Livro *livrosPesados = NULL;


    separarListas(listaLivrosCompleta, &livrosLeves, &livrosMedios, &livrosPesados);
    // printarListaLivro(livrosLeves);
    // printarListaLivro(livrosMedios);
    // printarListaLivro(livrosPesados);

    // printf("Quantidade de livros na lista leve: %d\nQuantidade de livros na lista medios: %d\nQuantidade de livros na lista pesada: %d\n", quantidadeLista(livrosLeves), quantidadeLista(livrosMedios), quantidadeLista(livrosPesados));

    liberarLista(listaLivrosCompleta);
    liberarLista(livrosLeves);
    liberarLista(livrosMedios);
    liberarLista(livrosPesados);
    return 0;
}
