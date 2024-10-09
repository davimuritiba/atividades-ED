#include <stdio.h>
#include <string.h>
#include <stdlib.h>

    int *dados;
    int topo;
    int capacidade;
} Pilha;

Pilha *startPilha(int capacidade_inicial) {
    Pilha *p = (Pilha*)malloc(sizeof(Pilha));
    p->dados = (int*)malloc(capacidade_inicial * sizeof(int));
    p->topo = 0;
    p->capacidade = capacidade_inicial;
    return p;
}

Pilha *Empilhar(int dado, Pilha *p) {
    if (p->topo == p->capacidade) {
        p->capacidade *= 2;
        p->dados = (int*)realloc(p->dados, p->capacidade * sizeof(int));
    }
    p->dados[p->topo++] = dado;
    return p;
}

Pilha *Desempilhar(Pilha *p) {
    if (p->topo == 0) {
        printf("Pilha vazia...\n");
    } else {
        p->topo--;
    }
    return p;
}

    if (p->topo == 0) {
        printf("Pilha vazia...\n");
    }
    for (int i = p->topo - 1; i >= 0; i--) {
        printf("%d ", p->dados[i]);
    }
    printf("\n");
}

void freePilha(Pilha *p) {
    free(p->dados);
    free(p);
}

char* somaBinaria(const char *a, const char *b) {
    int tamanho_a = strlen(a);
    int tamanho_b = strlen(b);
    int tamanho_maximo = tamanho_a > tamanho_b ? tamanho_a : tamanho_b;

    char *result = (char*)malloc(tamanho_maximo + 2);
    result[tamanho_maximo + 1] = '\0';

    int carry = 0;
    int i = tamanho_a - 1, j = tamanho_b - 1, k = tamanho_maximo;

    while (i >= 0 || j >= 0) {
        int bitA = (i >= 0) ? a[i] - '0' : 0;
        int bitB = (j >= 0) ? b[j] - '0' : 0;

        int soma = bitA + bitB + carry;
        result[k--] = (soma % 2) + '0';
        carry = soma / 2;

        i--;
        j--;
    }

    if (carry) {
        result[k] = '1';
    } else {
        k++;
    }

    return result + k; 
}

int main() {
    char a[10] = "11001";
    char b[10] = "11001";

    char *resultado = somaBinaria(a, b);
    printf("%s\n", resultado);

    free(resultado - (strlen(resultado) > 1)); 
    return 0;
}
