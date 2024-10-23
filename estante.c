#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>

typedef struct livro{
    char titulo[30];
    char autor[30];
    int largura;
    int altura;
    int profundidade;
}Livro;

void removerNovaLinha(char *linha) {
    size_t tamanho = strlen(linha);
    if (tamanho > 0 && (linha[tamanho - 1] == '\n' || linha[tamanho - 1] == '\r')) {
        linha[tamanho - 1] = '\0';  // Remove o \n ou \r no final da linha
        if (tamanho > 1 && (linha[tamanho - 2] == '\r' || linha[tamanho - 2] == '\n')) {
            linha[tamanho - 2] = '\0';  // Remove o segundo caractere de nova linha se houver (caso Windows)
        }
    }
}

void atribuirDados(Livro* livro, const char *linha){

    char linhaCopia[256];
    strncpy(linhaCopia, linha, sizeof(linhaCopia) - 1);
    linhaCopia[sizeof(linhaCopia) - 1] = '\0';  // Garantir que a string está terminada com \0

    removerNovaLinha(linhaCopia); /* remove \r\n */
    char *token = strtok((char *)linhaCopia, ",");   
    while(token != NULL){
        printf("%s", token);
         if (strncmp(token, "ticulo=", 7) == 0) {
            strcpy(livro->titulo, token + 7);  // Copia o valor de 'titulo'
        } else if (strncmp(token, "autor=", 6) == 0) {
            strcpy(livro->autor, token + 6);  // Copia o valor de 'autor'
        } else if (strncmp(token, "largura=", 8) == 0) {
            livro->largura = atoi(token + 8);  // Converte o valor de 'largura' para inteiro
        } else if (strncmp(token, "altura=", 7) == 0) {
            livro->altura = atoi(token + 7);  // Converte o valor de 'altura' para inteiro
        } else if (strncmp(token, " profundidade=", 14) == 0) {
            livro->profundidade = atoi(token + 14);  // Converte o valor de 'profundidade' para inteiro
        } 
        token = strtok(NULL, ",");
    }
}

void abrir_arquivo(){
    FILE* fp;
    Livro livro;
    char linha[256];
    fp = fopen("estanteLivros.txt", "rt");
    if (fp == NULL){
        printf("erro ao abrir o arquivo\n");
        exit(1);
    }

    while(fgets(linha,121, fp) != NULL){
        atribuirDados(&livro, linha);

        printf("Título: %s\nAutor: %s\nLargura: %d\nAltura: %d\nProfundidade: %d\n",
        livro.titulo, livro.autor, livro.largura, livro.altura, livro.profundidade);
    }
    int fclose (FILE* fp);
}



int main(){
    
    abrir_arquivo();

    
return 0;
}