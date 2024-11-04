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
        novaEstante->prateleiras[i].capacidadeProfundidade = 32;  // Capacidade máxima de 96 cm
        novaEstante->prateleiras[i].capacidadeAltura = 31;  // Capacidade máxima de 96 cm
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

void printarListaLivro(Livro* livro) {
    while (livro != NULL) {
        printf("    - Titulo: %s| Autor: %s| Altura: %dcm| Largura: %dcm| Profundidade: %dcm| Volume: %dcm\n", 
                livro->titulo, livro->autor, livro->altura, livro->largura, livro->profundidade, livro->volume);
            livro = livro->next;
    }
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

    Livro* livroAtual = listaLivros;
    while (livroAtual != NULL) {
        Prateleira* prateleira = &estanteAtual->prateleiras[prateleiraAtual];
        int capacidadeRestante = prateleira->capacidadeLargura;

        // Encontra o melhor livro que caiba no espaço disponível da prateleira
        Livro* melhorLivro = NULL;
        Livro* anterior = NULL;
        Livro* atual = livroAtual;
        Livro* anteriorMelhor = NULL;
        int menorDiferenca = capacidadeRestante + 1;

        while (atual != NULL) {
            int diferenca = capacidadeRestante - atual->largura;
            if (diferenca >= 0 && diferenca < menorDiferenca) {
                melhorLivro = atual;
                anteriorMelhor = anterior;
                menorDiferenca = diferenca;
                if (diferenca == 0) break;
            }
            anterior = atual;
            atual = atual->next;
        }

        if (melhorLivro != NULL) {
            // Adiciona o melhor livro encontrado na prateleira
            Livro* novoLivro = (Livro*) malloc(sizeof(Livro));
            *novoLivro = *melhorLivro;
            novoLivro->next = prateleira->livros;
            if (prateleira->livros != NULL) {
                prateleira->livros->prev = novoLivro;
            }
            prateleira->livros = novoLivro;
            prateleira->capacidadeLargura -= melhorLivro->largura;

            // Remove o livro da lista original
            if (anteriorMelhor != NULL) {
                anteriorMelhor->next = melhorLivro->next;
            } 
            else {
                livroAtual = melhorLivro->next;
            }
            if (melhorLivro->next != NULL) {
                melhorLivro->next->prev = anteriorMelhor;
            }
            free(melhorLivro);
        } 
        else {
            prateleiraAtual++; //vai para a proxima prateleira
            if (prateleiraAtual == 6) {
                adicionarEstante(biblioteca);
                estanteAtual = estanteAtual->next;
                prateleiraAtual = 0;
            }
        }
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
        printf("Estante nao encontrada.\n");
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

    printf("Livro '%s' nao encontrado na estante %d, prateleira %d.\n", tituloLivro, numEstante, numPrateleira);
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

int emprestarLivro(Biblioteca *biblioteca, Livro **listaEmprestados, const char *tituloLivro) {
    if (biblioteca == NULL || tituloLivro == NULL) {
        printf("Biblioteca ou título do livro inválidos.\n");
        return -1;
    }

    Estante *estanteAtual = biblioteca->estantes;
    while (estanteAtual != NULL) {
        for (int i = 0; i < 6; i++) {
            Prateleira *prateleiraAtual = &estanteAtual->prateleiras[i];
            Livro *livroAtual = prateleiraAtual->livros;
            Livro *prevLivro = NULL;

            while (livroAtual != NULL) {
                if (strncmp(livroAtual->titulo, tituloLivro, strlen(tituloLivro)) == 0) {
                    // Remove o livro da prateleira
                    if (prevLivro == NULL) {
                        prateleiraAtual->livros = livroAtual->next;
                    } else {
                        prevLivro->next = livroAtual->next;
                    }
                    if (livroAtual->next != NULL) {
                        livroAtual->next->prev = prevLivro;
                    }

                    // Adiciona o livro à lista de emprestados
                    livroAtual->next = *listaEmprestados;
                    if (*listaEmprestados != NULL) {
                        (*listaEmprestados)->prev = livroAtual;
                    }
                    *listaEmprestados = livroAtual;

                    printf("Livro \"%s\" emprestado com sucesso.\n", tituloLivro);
                    return 0;
                }
                prevLivro = livroAtual;
                livroAtual = livroAtual->next;
            }
        }
        estanteAtual = estanteAtual->next;
    }

    printf("Livro \"%s\" nao encontrado na biblioteca.\n", tituloLivro);
    return -1;
}

void inserirLivro(Biblioteca* biblioteca, Livro* novoLivro) {
    // Verificar se a biblioteca tem estantes, se não, cria uma
    if (biblioteca->numEstantes == 0) {
        biblioteca->estantes = (Estante*)malloc(sizeof(Estante));
        for (int i = 0; i < 6; i++) {
            biblioteca->estantes->prateleiras[i].livros = NULL;
        }
        biblioteca->estantes->next = NULL;
        biblioteca->numEstantes = 1;
    }

    // Encontrar a última estante
    Estante* estanteAtual = biblioteca->estantes;
    while (estanteAtual->next != NULL) {
        estanteAtual = estanteAtual->next;
    }

    // Encontrar a última prateleira com espaço para o novo livro
    int larguraOcupada = 0;
    Prateleira* prateleiraSelecionada = NULL;

    for (int i = 0; i < 6; i++) {
        Prateleira* prateleira = &estanteAtual->prateleiras[i];
        larguraOcupada = 0;
        Livro* livroAtual = prateleira->livros;
        
        // Calcular a largura ocupada na prateleira atual
        while (livroAtual != NULL) {
            larguraOcupada += livroAtual->largura;
            livroAtual = livroAtual->next;
        }

        // Verificar se o livro cabe na prateleira atual
        if (larguraOcupada + novoLivro->largura <= 96) {
            prateleiraSelecionada = prateleira;
            break;
        }
    }

    // Se não há prateleira com espaço, criar uma nova estante
    if (prateleiraSelecionada == NULL) {
        Estante* novaEstante = (Estante*)malloc(sizeof(Estante));
        for (int i = 0; i < 6; i++) {
            novaEstante->prateleiras[i].livros = NULL;
        }
        novaEstante->next = NULL;
        
        estanteAtual->next = novaEstante;
        biblioteca->numEstantes++;

        // A nova estante e primeira prateleira será o destino do novo livro
        prateleiraSelecionada = &novaEstante->prateleiras[0];
    }

    // Inserir o novo livro no final da prateleira selecionada
    Livro* ultimoLivro = prateleiraSelecionada->livros;
    if (ultimoLivro == NULL) {
        // A prateleira está vazia
        prateleiraSelecionada->livros = novoLivro;
    } else {
        // Encontrar o final da lista de livros
        while (ultimoLivro->next != NULL) {
            ultimoLivro = ultimoLivro->next;
        }
        ultimoLivro->next = novoLivro;
        novoLivro->prev = ultimoLivro;
    }

    printf("Livro %s inserido com sucesso na biblioteca.\n", novoLivro->titulo);
}

int main() {
    Livro* listaLivros = abrirArquivo();
    Biblioteca *biblioteca = criarBiblioteca();
    Estante *estante = biblioteca->estantes;
    Livro *livrosEmprestados = NULL;

    biblioteca->estantes = NULL;
    biblioteca->numEstantes = 0;
    distribuirLivrosNasEstantes(biblioteca, listaLivros);

    printf("******************************************\n");
    printf("*          BEM-VINDO A BIBLIOTECA         *\n");
    printf("*                 DA ROBERTA              *\n");
    printf("******************************************\n\n");

    do{
        int numeroOperacao = -1;
        printf("Operacoes disponiveis:\n");
        printf("1. Checar numero de estantes\n");
        printf("2. Consultar Estante\n");
        printf("3. Inserir livro\n");
        printf("4. Remover livro\n");
        printf("5. Emprestar livro\n");
        printf("0. Encerrar operacoes\n");
        printf("Digite o numero correspondente a operacao solicitada:\n");
        scanf("%d", &numeroOperacao);

        switch (numeroOperacao) {
            case 1: 
                printarBiblioteca(biblioteca);
                break;

            case 2: {
                int numEstante, numPrateleira;
                printf("Digite o numero da estante e da prateleira:\n");
                scanf("%d %d", &numEstante, &numPrateleira);
                printarPrateleira(biblioteca, numEstante, numPrateleira);
                break;
            }

            case 3: {
                int estanteNum, prateleiraNum, altura, largura, profundidade;
                char titulo[30], autor[30];
                printf("Digite a altura, largura, profundidade, titulo e autor do livro:\n");
                scanf("%d %d %d", &altura, &largura, &profundidade);
                getchar(); // Limpar o buffer do '\n'
                printf("Titulo: ");
                fgets(titulo, 30, stdin);
                printf("Autor: ");
                fgets(autor, 30, stdin);
                
                // Remover a nova linha do final de `titulo` e `autor`
                titulo[strcspn(titulo, "\n")] = '\0';
                autor[strcspn(autor, "\n")] = '\0';
                
                Livro *novoLivro = criarLivro(altura, largura, profundidade, titulo, autor);
                inserirLivro(biblioteca, novoLivro);
                break;
            }
            case 4: {
                int estante, prateleira;
                char titulo[30];
                printf("Digite o numero da estante e a prateleira\n");
                scanf("%d %d", &estante, &prateleira);
                getchar();
                printf("Agora digite o titulo: ");
                gets(titulo);
                removerLivro(biblioteca, estante, prateleira, titulo);
                break;
            }
            case 5: {
                char titulo[30];
                printf("Digite o titulo do livro que voce quer emprestar:\n");
                getchar();
                gets(titulo);
                if(!emprestarLivro(biblioteca, &livrosEmprestados, titulo))
                    printf("Livros Emprestados: \n");
                    printarListaLivro(livrosEmprestados);  
                break;
            }
            case 0: 
                printf("Operacoes encerradas.\n"); 
                return 0;
            default: 
                printf("Operacao invalida :/\n"); 
                break;
        }
    } while (1);

    liberarLivros(listaLivros);
    liberarLivros(livrosEmprestados);
    liberarEstante(estante);
    free(biblioteca);
    return 0;
}
