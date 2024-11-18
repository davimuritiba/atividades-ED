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
    if ((pos = strchr(linha, '\r')) != NULL) *pos = '\0';  // Remove '\r' (Windows)
    if ((pos = strchr(linha, '\n')) != NULL) *pos = '\0';  // Remove '\n' (Linux/macOS)
}

listaArtigo* abrirArquivo() {
    FILE *arquivo = fopen("artigos.txt", "r");
    if (!arquivo) {
        perror("Erro ao abrir o arquivo");
        return NULL;
    }

    listaArtigo *inicio = NULL, *ultimo = NULL;
    char linha[1024];

    while(fgets(linha, sizeof(linha), arquivo)) {
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

listaArtigo *atribuirDados(const char *linha) {
    char *titulo = NULL;
    Autor *autores = NULL, *ultimoAutor = NULL;
    char *link = NULL;

    // Duplica a linha para ser manipulada
    char *linhaCopia = strdup(linha);

    // Procura por "Titulo:", "Autores:" e "Link:"
    char *tituloPos = strstr(linhaCopia, "Titulo:");
    char *autoresPos = strstr(linhaCopia, "Autores:");
    char *linkPos = strstr(linhaCopia, "Link:");

    if (tituloPos) {
        titulo = strdup(tituloPos + 7);  // Avança 7 caracteres para pular "Titulo:"
        char *end = strchr(titulo, ';'); // Encontra o próximo delimitador
        if (end) *end = '\0';           // Finaliza a string
    }

    if (autoresPos) {
        char *autoresInicio = autoresPos + 8;  // Avança 8 caracteres para "Autores:"
        char *autoresFim = strchr(autoresInicio, ';'); // Encontra o delimitador
        if (autoresFim) *autoresFim = '\0';    // Finaliza a string
        char *autorToken = strtok(autoresInicio, ",");
        while (autorToken) {
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

    if (linkPos) {
        link = strdup(linkPos + 5);  // Avança 5 caracteres para "Link:"
        char *end = strchr(link, '\0');  // Garante o final da string
        if (end && *(end - 1) == '\n') *(end - 1) = '\0';  // Remove quebra de linha
    }

    free(linhaCopia);

    if (!link) {
        printf("Link nao encontrado.\n");
        return NULL;
    }

    return criarArtigo(titulo, autores, link);
}

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
        
        printf("\nLink: %s", lista->link);  // Imprime o link
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
    const char *stopWords[] = {"o", "a", "de", "e", "do", "da", "em", "um", "uma", "os", "as", "dos", "das", "no", "na", "nos", "nas", "por", "com", "para", "como", "que", "se", "ao", "à", "aos", 
                                "às", "quais", "qual", "quando", "onde", "porque", "quando", "onde", "?", ",", ".", "!"};
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
