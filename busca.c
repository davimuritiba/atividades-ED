#include "busca.h"

listaArtigo *criarArtigo(char *titulo, Autor *autores, char *link) {
    listaArtigo *novoArtigo = (listaArtigo*)malloc(sizeof(listaArtigo));
    novoArtigo->titulo = strdup(titulo);  // Use strdup para alocar memória
    novoArtigo->autores = autores;
    novoArtigo->link = strdup(link);  // Armazena o link
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
    char *link = NULL;

    char *token = strtok(linhaCopia, ";");
    while (token != NULL) {
        printf("Token: %s\n", token);  // Adiciona print para depuração
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
        } else if (strncmp(token, "Link:", 5) == 0) {
            link = strdup(token + 5);
            printf("Link encontrado: %s\n", link);  // Adiciona print para verificar o link
        }
        token = strtok(NULL, ";");
    }

    if (!titulo) {
        printf("titulo nao encontrado.\n");
        return NULL;
    }

    if (!link) {
        printf("link nao encontrado.\n");
        return NULL;
    }

    return criarArtigo(titulo, autores, link);
}

// Função para converter uma string para minúsculas
char *toLowerCase(const char *str) {
    char *lowerStr = strdup(str);
    for (int i = 0; lowerStr[i]; i++) {
        lowerStr[i] = tolower(lowerStr[i]);
    }
    return lowerStr;
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
        
        printf("Link: %s\n", lista->link);  // Imprime o link
        printf("\n\n");  // Adiciona uma linha em branco entre os artigos
        lista = lista->next;
    }
}

listaArtigo* buscarPorPalavraChave(listaArtigo *lista, const char *palavraChave) {
    listaArtigo *resultado = NULL, *ultimoResultado = NULL;
    char *palavraChaveLower = toLowerCase(palavraChave);

    while (lista != NULL) {
        char *tituloLower = toLowerCase(lista->titulo);
        if (strstr(tituloLower, palavraChaveLower) != NULL) {
            listaArtigo *novoArtigo = criarArtigo(lista->titulo, lista->autores, lista->link);
            if (!resultado) {
                resultado = novoArtigo;
            } else {
                ultimoResultado->next = novoArtigo;
                novoArtigo->prev = ultimoResultado;
            }
            ultimoResultado = novoArtigo;
        }
        free(tituloLower);  // Libera a memória alocada para o título em minúsculas
        lista = lista->next;
    }

    free(palavraChaveLower);  // Libera a memória alocada para a palavra-chave em minúsculas
    return resultado;
}

// Função para verificar se uma palavra é uma stop word
int retirarPalavras(const char *word) {
    const char *stopWords[] = {"o", "a", "de", "e", "do", "da", "em", "um", "uma", "os", "as", "dos", "das", "no", "na", "nos", "nas", "por", "com", "para", "como", "que", "se", "ao", "à", "aos", "às", "quais", "qual", "quando", "onde", "porque", "quando", "onde"};
    int numStopWords = sizeof(stopWords) / sizeof(stopWords[0]);
    for (int i = 0; i < numStopWords; i++) {
        if (strcmp(word, stopWords[i]) == 0) {
            return 1;
        }
    }
    return 0;
}

// Função para extrair palavras-chave de uma pergunta
char **extrairPalavrasChave(const char *pergunta, int *numPalavras) {
    char *perguntaLower = toLowerCase(pergunta);
    char *token = strtok(perguntaLower, " ");
    char **palavrasChave = malloc(sizeof(char *) * 100); // Aloca espaço para até 100 palavras-chave
    *numPalavras = 0;

    while (token != NULL) {
        if (!retirarPalavras(token)) {
            palavrasChave[*numPalavras] = strdup(token);
            (*numPalavras)++;
        }
        token = strtok(NULL, " ");
    }

    free(perguntaLower);
    return palavrasChave;
}

listaArtigo* buscarPorPergunta(listaArtigo *lista, const char *pergunta) {
    int numPalavras;
    char **palavrasChave = extrairPalavrasChave(pergunta, &numPalavras);
    listaArtigo *resultado = NULL, *ultimoResultado = NULL;

    for (int i = 0; i < numPalavras; i++) {
        listaArtigo *tempLista = buscarPorPalavraChave(lista, palavrasChave[i]);
        while (tempLista != NULL) {
            // Verifica se o artigo já está na lista de resultados
            listaArtigo *temp = resultado;
            int encontrado = 0;
            while (temp != NULL) {
                if (strcmp(temp->titulo, tempLista->titulo) == 0) {
                    encontrado = 1;
                    break;
                }
                temp = temp->next;
            }

            if (!encontrado) {
                listaArtigo *novoArtigo = criarArtigo(tempLista->titulo, tempLista->autores, tempLista->link);
                if (!resultado) {
                    resultado = novoArtigo;
                } else {
                    ultimoResultado->next = novoArtigo;
                    novoArtigo->prev = ultimoResultado;
                }
                ultimoResultado = novoArtigo;
            }

            tempLista = tempLista->next;
        }
    }

    // Libera a memória alocada para as palavras-chave
    for (int i = 0; i < numPalavras; i++) {
        free(palavrasChave[i]);
    }
    free(palavrasChave);

    return resultado;
}

int main() {
    listaArtigo *listaCompleta = abrirArquivo();
    // imprimirListaArtigos(listaCompleta);

    while(1){
        printf("O que voce deseja fazer?\n1 - Buscar por palavra chave\n2 - Buscar por pergunta\n3 - Sair\n");
        int opcao;
        scanf("%d", &opcao);
        getchar();

        switch(opcao){
            case 1:
                printf("Digite a palavra chave: ");
                char palavraChave[100];
                fgets(palavraChave, sizeof(palavraChave), stdin);
                palavraChave[strcspn(palavraChave, "\n")] = '\0';
                listaArtigo *resultadosChave = buscarPorPalavraChave(listaCompleta, palavraChave);
                if(resultadosChave == NULL){
                    printf("Nenhum artigo encontrado com a palavra chave '%s'\n", palavraChave);
                }else{
                    imprimirListaArtigos(resultadosChave);
                }
                break;
            case 2:
                printf("Digite a pergunta: ");
                char pergunta[100];
                fgets(pergunta, sizeof(pergunta), stdin);
                pergunta[strcspn(pergunta, "\n")] = '\0';
                listaArtigo *resultadosPergunta = buscarPorPergunta(listaCompleta, pergunta);
                if(resultadosPergunta == NULL){
                    printf("Nenhum artigo encontrado para a pergunta '%s'\n", pergunta);
                }else{
                    imprimirListaArtigos(resultadosPergunta);
                }
                break;
            case 3:
                return 0;
            default:
                printf("Opcao invalida\n");
                break;
        }
    }

    return 0;
}
