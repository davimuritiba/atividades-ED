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
    novoLivro->volume = novoLivro->profundidade * novoLivro->altura * novoLivro->largura;
    strcpy(novoLivro->titulo, titulo);
    strcpy(novoLivro->autor, autor);
    novoLivro->prev = NULL;
    novoLivro->next = NULL;
    return novoLivro;
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

// Função para criar uma nova estante
Estante* criarEstante() {
    Estante* novaEstante = (Estante*) malloc(sizeof(Estante));

    if (novaEstante == NULL) {
        printf("Erro ao alocar memória para a estante.\n");
        exit(1);
    }

    // Inicializa todas as prateleiras da estante
    for(int i = 0; i < 6; i++){
        novaEstante->prateleiras[i].livros = NULL;
        novaEstante->prateleiras[i].capacidadeLargura = 96;  // Capacidade máxima de 96 cm
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

// Função para remover a nova linha de uma string
void removerNovaLinha(char* linha) {
    size_t tamanho = strlen(linha);
    if (tamanho > 0 && (linha[tamanho - 1] == '\n' || linha[tamanho - 1] == '\r')) {
        linha[tamanho - 1] = '\0';  // Remove o \n ou \r no final da linha
    }
}

// Função para dividir a lista em duas metades
void dividirLista(Livro* cabeca, Livro** frente, Livro** tras) {
    Livro* rapido;
    Livro* lento;
    lento = cabeca;
    rapido = cabeca->next;

    while (rapido != NULL) {
        rapido = rapido->next;
        if (rapido != NULL) {
            lento = lento->next;
            rapido = rapido->next;
        }
    }
    *frente = cabeca;
    *tras = lento->next;
    lento->next = NULL;
    if (*tras != NULL) {
        (*tras)->prev = NULL;
    }
}

// Função para mesclar duas listas ordenadas
Livro* mesclarListas(Livro* frente, Livro* tras) {
    if (frente == NULL) return tras;
    if (tras == NULL) return frente;

    Livro* resultado = NULL;

    // Escolhe o primeiro nó da lista mesclada
    if (frente->largura >= tras->largura) {
        resultado = frente;
        resultado->next = mesclarListas(frente->next, tras);
        resultado->next->prev = resultado; // Conecta corretamente o próximo
        resultado->prev = NULL; // O primeiro nó não tem 'prev'
    } else {
        resultado = tras;
        resultado->next = mesclarListas(frente, tras->next);
        resultado->next->prev = resultado; // Conecta corretamente o próximo
        resultado->prev = NULL; // O primeiro nó não tem 'prev'
    }
    return resultado;
}


// Função de ordenação usando Merge Sort
void mergeSort(Livro** cabeca) {
    Livro* frente;
    Livro* tras;

    if (*cabeca == NULL || (*cabeca)->next == NULL) {
        return;
    }
    dividirLista(*cabeca, &frente, &tras);
    mergeSort(&frente);
    mergeSort(&tras);

    *cabeca = mesclarListas(frente, tras);
}

// Função para imprimir todos os livros na lista
void printarListaLivro(Livro* livro) {
    while (livro != NULL) {
        printf("Titulo: %s\nAutor: %s\nLargura: %d\nAltura: %d\nProfundidade: %d\nVolume: %d\n",
               livro->titulo, livro->autor, livro->largura, livro->altura, livro->profundidade, livro->volume);
        livro = livro->next;  // Avança para o próximo livro
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

//Função para liberar as lista de livros
void liberarLivros(Livro *livro) {
    while (livro != NULL) {
        Livro *temp = livro;
        livro = livro->next;
        free(temp);
    }
}

void liberarEstante(Estante *estante){
    while(estante != NULL){
        Estante *temp = estante;
        estante = estante->next;
        free(temp);
    }
}

//Função para adicionar uma estante nova á biblioteca
void adicionarEstante(Biblioteca* biblioteca) {
    Estante* novaEstante = criarEstante();
    
    if (novaEstante == NULL) 
        return;  // Erro na criação da estante

    if (biblioteca->estantes == NULL) {
        // Se a biblioteca estiver vazia, a nova estante será a primeira
        biblioteca->estantes = novaEstante;
    } 
    else {
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

    while (estanteAtual != NULL) {
        printf("Estante %d:\n", numEstante);

        for (int i = 0; i < 6; i++) {
            Prateleira prateleiraAtual = estanteAtual->prateleiras[i];
            printf("  Prateleira %d\n", i + 1);

            Livro *livroAtual = prateleiraAtual.livros;
            int quantidadeLivros = 0;  // Inicializa aqui para contar os livros por prateleira

            if (livroAtual == NULL) {
                printf("    Nenhum livro nesta prateleira.\n");
            } else {
                while (livroAtual != NULL) {
                    quantidadeLivros++;
                    livroAtual = livroAtual->next;
                }
                printf("    Quantidade de livros nesta prateleira: %d\n", quantidadeLivros);
            }
        }

        estanteAtual = estanteAtual->next;  // Avança para a próxima estante
        numEstante++;
    }
}

void distribuirLivrosNasEstantes(Biblioteca* biblioteca, Livro* listaLivros) {
    Estante* estanteAtual = biblioteca->estantes;
    int prateleiraAtual = 0;
    
    if (estanteAtual == NULL) {
        adicionarEstante(biblioteca);  // Garante que exista ao menos uma estante
        estanteAtual = biblioteca->estantes;
    }

    while (listaLivros != NULL) {
        // Verifica se há espaço suficiente na prateleira atual
        if (estanteAtual->prateleiras[prateleiraAtual].capacidadeLargura >= listaLivros->largura) {
            // Adiciona o livro à prateleira atual
            Livro* novoLivro = (Livro*) malloc(sizeof(Livro));
            *novoLivro = *listaLivros;  // Copia os dados do livro
            novoLivro->next = estanteAtual->prateleiras[prateleiraAtual].livros;
            if (estanteAtual->prateleiras[prateleiraAtual].livros != NULL) {
                estanteAtual->prateleiras[prateleiraAtual].livros->prev = novoLivro;
            }
            estanteAtual->prateleiras[prateleiraAtual].livros = novoLivro;
            estanteAtual->prateleiras[prateleiraAtual].capacidadeLargura -= listaLivros->largura;
        } 
        else {
            // Move para a próxima prateleira
            prateleiraAtual++;
            if (prateleiraAtual == 6) {
                // Adiciona uma nova estante se necessário
                adicionarEstante(biblioteca);
                estanteAtual = estanteAtual->next;
                prateleiraAtual = 0;
            }
            continue;  // Tenta adicionar o livro à nova prateleira/estante
        }
        listaLivros = listaLivros->next;  // Passa para o próximo livro
    }
}

void removerLivro(Biblioteca *biblioteca, int numEstante, int numPrateleira, char *tituloLivro){
    if (biblioteca == NULL || numEstante < 1 || numPrateleira < 1 || numEstante > 30 || numPrateleira > 6) {
        printf("Estante ou prateleira inválida.\n");
        return;
    }

    Estante *estanteAtual = biblioteca->estantes;
    for (int i = 1; i < numEstante && estanteAtual != NULL; i++) {
        estanteAtual = estanteAtual->next;
    }

    if (estanteAtual == NULL) {
        printf("Estante não encontrada.\n");
        return;
    }

    Prateleira *prateleiraAtual = &estanteAtual->prateleiras[numPrateleira - 1];
    Livro *livroAtual = prateleiraAtual->livros;
    Livro *prevLivro = NULL;

    while (livroAtual != NULL) {
        if (strncmp(livroAtual->titulo, tituloLivro, strlen(tituloLivro)) == 0) {
            // Remover o livro da prateleira
            if (prevLivro == NULL){
                prateleiraAtual->livros = livroAtual->next;
            } 
            else{
                prevLivro->next = livroAtual->next;
            }
            if(livroAtual->next != NULL){
                livroAtual->next->prev = prevLivro;
            }

            free(livroAtual);  // Liberar a memória do livro removido
            printf("Livro \"%s\" removido da estante %d, prateleira %d.\n", tituloLivro, numEstante, numPrateleira);
            return;
        }
        prevLivro = livroAtual;
        livroAtual = livroAtual->next;
    }

    printf("Livro \"%s\" nao encontrado na estante %d, prateleira %d.\n", tituloLivro, numEstante, numPrateleira);
}

void printarPrateleira(Biblioteca *biblioteca, int numEstante, int numPrateleira){
    if (biblioteca == NULL || numEstante < 1 || numPrateleira < 1 || numEstante > 30 || numPrateleira > 6) {
        printf("Estante ou prateleira inválida.\n");
        return;
    }

    Estante *estanteAtual = biblioteca->estantes;
    
    // Navega até a estante específica
    for (int i = 1; i < numEstante && estanteAtual != NULL; i++) {
        estanteAtual = estanteAtual->next;
    }

    if (estanteAtual == NULL) {
        printf("Estante %d nao encontrada.\n", numEstante);
        return;
    }

    Prateleira *prateleiraAtual = &estanteAtual->prateleiras[numPrateleira - 1];
    Livro *livroAtual = prateleiraAtual->livros;

    printf("Estante %d, Prateleira %d\n", numEstante, numPrateleira);

    if (livroAtual == NULL) {
        printf("  Nenhum livro nesta prateleira.\n");
    } 
    else {
        printf("  Livros nesta prateleira:\n");
        while (livroAtual != NULL) {
            printf("    - Titulo: %s| Autor: %s| Altura: %dcm| Largura: %dcm| Profundidade: %dcm| Volume: %dcm\n", 
                   livroAtual->titulo, livroAtual->autor, livroAtual->altura, livroAtual->largura, livroAtual->profundidade, livroAtual->volume);
            livroAtual = livroAtual->next;
        }
    }
}


int main() {
    Livro* listaLivrosCompleta = abrirArquivo();
    Biblioteca *biblioteca = criarBiblioteca();
    Estante *estante = biblioteca->estantes;
    // printarListaLivro(listaLivrosCompleta);

    mergeSort(&listaLivrosCompleta);
    // printarListaLivro(listaLivrosCompleta);

    biblioteca->estantes = NULL;
    biblioteca->numEstantes = 0;
    distribuirLivrosNasEstantes(biblioteca, listaLivrosCompleta);
    // printarBiblioteca(biblioteca);

    printarPrateleira(biblioteca, 1, 1);
    // removerLivro(biblioteca, 1, 1, "  hm bxjjd");
    // printarPrateleira(biblioteca, 1, 1);

    Livro *livrosEmprestados = NULL;

    liberarLivros(listaLivrosCompleta);
    liberarLivros(livrosEmprestados);
    liberarEstante(estante);
    free(biblioteca);
    return 0;
}
