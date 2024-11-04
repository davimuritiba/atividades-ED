#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Definindo o número máximo de livros, estantes e prateleiras por estante
#define MAX_LIVROS 1000
#define MAX_ESTANTES 30
#define MAX_PRATELEIRAS 6
#define MAX_LIVROS_POR_PRATELEIRA 100 // Defina a capacidade máxima de livros por prateleira

// Estrutura para o livro
typedef struct {
    int altura, largura, profundidade;
    char titulo[30];
    char autor[30];
} Livro;

// Estrutura para a prateleira
typedef struct {
    Livro livros[MAX_LIVROS_POR_PRATELEIRA]; // Array estático para livros com limite definido
    int capacidadeLargura;
    int numLivros; // Número de livros na prateleira
} Prateleira;

// Estrutura para a estante
typedef struct {
    Prateleira prateleiras[MAX_PRATELEIRAS]; // Array estático de prateleiras
    int numPrateleiras; // Número de prateleiras usadas
} Estante;

// Estrutura para a biblioteca
typedef struct {
    Estante estantes[MAX_ESTANTES]; // Array estático de estantes
    int numEstantes; // Número de estantes usadas
} Biblioteca;

// Função para criar um livro
void criarLivro(Livro *livro, int altura, int largura, int profundidade, char titulo[], char autor[]) {
    livro->altura = altura;
    livro->largura = largura;
    livro->profundidade = profundidade;
    strcpy(livro->titulo, titulo);
    strcpy(livro->autor, autor);
}

// Função para remover a nova linha de uma string
void removerNovaLinha(char *linha) {
    size_t tamanho = strlen(linha);
    if (tamanho > 0 && (linha[tamanho - 1] == '\n' || linha[tamanho - 1] == '\r')) {
        linha[tamanho - 1] = '\0';
    }
}

// Função para atribuir dados a um livro a partir de uma linha de texto
void atribuirDados(Livro *livro, const char *linha) {
    char linhaCopia[256];
    strncpy(linhaCopia, linha, sizeof(linhaCopia) - 1);
    linhaCopia[sizeof(linhaCopia) - 1] = '\0';

    removerNovaLinha(linhaCopia);

    char *token = strtok(linhaCopia, ",");
    while (token != NULL) {
        if (strncmp(token, "titulo=", 7) == 0) {
            strcpy(livro->titulo, token + 7);
        } else if (strncmp(token, "autor=", 6) == 0) {
            strcpy(livro->autor, token + 6);
        } else if (strncmp(token, "largura=", 8) == 0) {
            livro->largura = atoi(token + 8);
        } else if (strncmp(token, "altura=", 7) == 0) {
            livro->altura = atoi(token + 7);
        } else if (strncmp(token, " profundidade=", 14) == 0) {
            livro->profundidade = atoi(token + 14);
        }
        token = strtok(NULL, ",");
    }

    criarLivro(livro, livro->altura, livro->largura, livro->profundidade, livro->titulo, livro->autor);
}

// Função para carregar livros de um arquivo para um array estático de livros
int abrirArquivo(Livro livros[]) {
    FILE *fp = fopen("estanteLivros.txt", "rt");
    if (fp == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        return 0;
    }

    int numLivros = 0;
    char linha[256];

    while (numLivros < MAX_LIVROS && fgets(linha, sizeof(linha), fp) != NULL) {
        atribuirDados(&livros[numLivros], linha);
        numLivros++;
    }

    fclose(fp);
    return numLivros;
}

// Função para inicializar uma biblioteca
void criarBiblioteca(Biblioteca *biblioteca) {
    biblioteca->numEstantes = 0; // Inicializa o número de estantes como 0

    // Inicializa as estantes e prateleiras
    for (int i = 0; i < MAX_ESTANTES; i++) {
        biblioteca->estantes[i].numPrateleiras = 0; // Inicializa o número de prateleiras
        for (int j = 0; j < MAX_PRATELEIRAS; j++) {
            biblioteca->estantes[i].prateleiras[j].capacidadeLargura = 96; // Definindo capacidade de largura
            biblioteca->estantes[i].prateleiras[j].numLivros = 0; // Inicializa o número de livros
        }
    }
}

void distribuirLivrosNasEstantes(Biblioteca *biblioteca, Livro livros[], int numLivros) {
    int estanteAtual = 0;
    int prateleiraAtual = 0;
    int i = 0; // Índice do livro na lista de livros

    while (i < numLivros) {
        // Se ultrapassar o número de estantes, sai do loop
        if (estanteAtual >= MAX_ESTANTES) break;

        Prateleira *prateleira = &biblioteca->estantes[estanteAtual].prateleiras[prateleiraAtual];
        int capacidadeRestante = prateleira->capacidadeLargura;

        // Procura pelo livro que melhor se encaixa na capacidade restante da prateleira
        int melhorIndice = -1;
        int menorDiferenca = capacidadeRestante + 1; // Inicia com valor maior que a capacidade

        for (int j = i; j < numLivros; j++) {
            int diferenca = capacidadeRestante - livros[j].largura;
            if (diferenca >= 0 && diferenca < menorDiferenca) {
                melhorIndice = j;
                menorDiferenca = diferenca;
                if (diferenca == 0) break; // Parada antecipada se encaixar perfeitamente
            }
        }

        // Se encontrou um livro que cabe, adiciona à prateleira
        if (melhorIndice != -1) {
            prateleira->livros[prateleira->numLivros++] = livros[melhorIndice];
            prateleira->capacidadeLargura -= livros[melhorIndice].largura;

            // Remove o livro da lista (troca com o último livro considerado e reduz o total)
            Livro temp = livros[melhorIndice];
            livros[melhorIndice] = livros[numLivros - 1];
            livros[numLivros - 1] = temp;
            numLivros--; // Reduz o número de livros
        } else {
            // Nenhum livro cabe na prateleira atual, então passa para a próxima prateleira
            prateleiraAtual++;
            if (prateleiraAtual == MAX_PRATELEIRAS) {
                prateleiraAtual = 0;
                estanteAtual++;
                if (estanteAtual == MAX_ESTANTES) break;

                // Incrementa o número de estantes se uma nova estante for usada
                biblioteca->numEstantes++;
            }
        }
    }

    // Certifica-se de que o número de estantes é atualizado ao final da distribuição
    if (estanteAtual >= biblioteca->numEstantes) {
        biblioteca->numEstantes = estanteAtual + 1;
    }
}

// Função para imprimir uma prateleira específica
void printarPrateleira(Biblioteca *biblioteca, int numEstante, int numPrateleira) {
    if (numEstante < 1 || numPrateleira < 1 || numEstante > MAX_ESTANTES || numPrateleira > MAX_PRATELEIRAS) {
        printf("Estante ou prateleira inválida.\n");
        return;
    }

    Prateleira *prateleira = &biblioteca->estantes[numEstante - 1].prateleiras[numPrateleira - 1];

    printf("Estante %d, Prateleira %d:\n", numEstante, numPrateleira);
    if (prateleira->numLivros == 0) {
        printf("  Nenhum livro nesta prateleira.\n");
    } 
    else {
        for (int i = 0; i < prateleira->numLivros; i++) {
            Livro *livro = &prateleira->livros[i];
            printf("  - Titulo: %s | Autor: %s | Altura: %dcm | Largura: %dcm | Profundidade: %dcm\n",
                   livro->titulo, livro->autor, livro->altura, livro->largura, livro->profundidade);
        }
    }
}

// Função para imprimir a biblioteca completa
void printarBiblioteca(Biblioteca *biblioteca) {
    if (biblioteca == NULL || biblioteca->numEstantes == 0) {
        printf("A biblioteca esta vazia.\n");
        return;
    }

    for (int i = 0; i < biblioteca->numEstantes; i++) {
        Estante *estanteAtual = &biblioteca->estantes[i];
        printf("Estante %d:\n", i + 1);

        for (int j = 0; j < MAX_PRATELEIRAS; j++) {
            Prateleira *prateleiraAtual = &estanteAtual->prateleiras[j];
            printf("  Prateleira %d:\n", j + 1);

            if (prateleiraAtual->numLivros == 0) {
                printf("    Nenhum livro nesta prateleira.\n");
            } 
            else {
                printf("     Livros na prateleira: %d\n", prateleiraAtual->numLivros);
            }
        }
    }
}

void printarListaLivros(Livro *listaLivros, int numLivros){
    for(int i = 0; i < numLivros; i++){
        printf("Autor: %s, Titulo:%s, Altura: %d, Largura: %d, Profundidade %d\n", listaLivros[i].autor, listaLivros[i].titulo, 
                listaLivros[i].altura, listaLivros[i].largura, listaLivros[i].profundidade);
    }
}
void inserirLivro(Biblioteca *biblioteca, int numEstante, int numPrateleira, int altura, int largura, int profundidade, char titulo[], char autor[]) {
    if (numEstante < 1 || numEstante > MAX_ESTANTES || numPrateleira < 1 || numPrateleira > MAX_PRATELEIRAS) {
        printf("Estante ou prateleira inválida.\n");
        return;
    }

    Prateleira *prateleira = &biblioteca->estantes[numEstante - 1].prateleiras[numPrateleira - 1];

    if (prateleira->numLivros >= MAX_LIVROS_POR_PRATELEIRA) {
        printf("A prateleira já atingiu o número máximo de livros.\n");
        return;
    }

    if (prateleira->capacidadeLargura < largura) {
        printf("Espaço insuficiente na prateleira para adicionar o livro.\n");
        return;
    }

    Livro novoLivro;
    criarLivro(&novoLivro, altura, largura, profundidade, titulo, autor);
    prateleira->livros[prateleira->numLivros++] = novoLivro;
    prateleira->capacidadeLargura -= largura;

    printf("Livro \"%s\" inserido na estante %d, prateleira %d.\n", titulo, numEstante, numPrateleira);
}
void removerLivro(Biblioteca *biblioteca, int numEstante, int numPrateleira, char *tituloLivro) {
    if (numEstante < 1 || numEstante > MAX_ESTANTES || numPrateleira < 1 || numPrateleira > MAX_PRATELEIRAS) {
        printf("Estante ou prateleira inválida.\n");
        return;
    }

    Prateleira *prateleira = &biblioteca->estantes[numEstante - 1].prateleiras[numPrateleira - 1];
    int encontrado = 0;

    for (int i = 0; i < prateleira->numLivros; i++) {
        if (strcmp(prateleira->livros[i].titulo, tituloLivro) == 0) {
            encontrado = 1;
            prateleira->capacidadeLargura += prateleira->livros[i].largura;

            // Shift dos livros para preencher o espaço vazio
            for (int j = i; j < prateleira->numLivros - 1; j++) {
                prateleira->livros[j] = prateleira->livros[j + 1];
            }
            prateleira->numLivros--;

            printf("Livro \"%s\" removido da estante %d, prateleira %d.\n", tituloLivro, numEstante, numPrateleira);
            break;
        }
    }

    if (!encontrado) {
        printf("Livro \"%s\" não encontrado na estante %d, prateleira %d.\n", tituloLivro, numEstante, numPrateleira);
    }
}
void consultarLivro(Biblioteca *biblioteca, int numEstante, int numPrateleira, char *tituloLivro) {
    if (numEstante < 1 || numEstante > MAX_ESTANTES || numPrateleira < 1 || numPrateleira > MAX_PRATELEIRAS) {
        printf("Estante ou prateleira inválida.\n");
        return;
    }

    Prateleira *prateleira = &biblioteca->estantes[numEstante - 1].prateleiras[numPrateleira - 1];

    for (int i = 0; i < prateleira->numLivros; i++) {
        if (strcmp(prateleira->livros[i].titulo, tituloLivro) == 0) {
            Livro *livro = &prateleira->livros[i];
            printf("Livro encontrado:\n");
            printf("  Título: %s\n  Autor: %s\n  Altura: %d cm\n  Largura: %d cm\n  Profundidade: %d cm\n",
                   livro->titulo, livro->autor, livro->altura, livro->largura, livro->profundidade);
            return;
        }
    }

    printf("Livro \"%s\" não encontrado na estante %d, prateleira %d.\n", tituloLivro, numEstante, numPrateleira);
}


int main() {
    Biblioteca biblioteca;
    criarBiblioteca(&biblioteca);

    Livro listaLivros[1000];
    int numLivros = abrirArquivo(listaLivros);
    distribuirLivrosNasEstantes(&biblioteca, listaLivros, numLivros);

    printf("******************************************\n");
    printf("*          BEM-VINDO A BIBLIOTECA         *\n");
    printf("*                 DO MAGO                 *\n");
    printf("******************************************\n\n");

    int numeroOperacao;
    while (1) {
        printf("Operações disponíveis:\n1. Checar número de estantes\n2. Consultar livro\n3. Inserir livro\n4. Remover livro\n5. Emprestar livro\n6. Encerrar operações\n");
        printf("Digite o número correspondente à operação solicitada:\n");
        scanf("%d", &numeroOperacao);

        if (numeroOperacao == 1) 
            {
                printarBiblioteca(&biblioteca);
            } 
        else if (numeroOperacao == 2) 
            {
                int estante, prateleira;
                char titulo[30];
                printf("Digite o número da estante, da prateleira e o título do livro:\n");
                scanf("%d %d", &estante, &prateleira);
                getchar();
                fgets(titulo, 30, stdin);
                removerNovaLinha(titulo);
                consultarLivro(&biblioteca, estante, prateleira, titulo);
            } 
        else if (numeroOperacao == 3) 
            {
                int estante, prateleira, altura, largura, profundidade;
                char titulo[30], autor[30];
                printf("Digite o número da estante, prateleira, altura, largura, profundidade, título e autor do livro:\n");
                scanf("%d %d %d %d %d", &estante, &prateleira, &altura, &largura, &profundidade);
                getchar();
                fgets(titulo, 30, stdin);
                removerNovaLinha(titulo);
                fgets(autor, 30, stdin);
                removerNovaLinha(autor);
                inserirLivro(&biblioteca, estante, prateleira, altura, largura, profundidade, titulo, autor);
            } 
        else if (numeroOperacao == 4) 
            {
                int estante, prateleira;
                char titulo[30];
                printf("Digite o número da estante, da prateleira e o título do livro a ser removido:\n");
                scanf("%d %d", &estante, &prateleira);
                getchar();
                fgets(titulo, 30, stdin);
                removerNovaLinha(titulo);
                removerLivro(&biblioteca, estante, prateleira, titulo);
            } 
        else if (numeroOperacao == 5) 
        {
            printf("Função de empréstimo ainda não implementada.\n");
        } 
        else if (numeroOperacao == 6) 
        {
            printf("Operações encerradas.\n");
            break;
        } else 
        {
            printf("Operação inválida :/\n");
        }
    }

    return 0;
}
