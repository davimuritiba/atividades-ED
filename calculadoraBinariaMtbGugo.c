#include <stdio.h>
#include <string.h>
#include <stdlib.h>

 typedef struct no {
    char data[40]; //Numero de bits ta em 40, mas da pra redefinir
    struct no *proximo;
} No;

typedef struct stack {
    No *top;
} Stack;

void initStack(Stack *stack) {
    stack->top = NULL;
}

void push(Stack *stack, char valor[]){
    No *novoNo = malloc(sizeof(No));

    if(novoNo == NULL){
        printf("Erro na hora de alocar a memoria\n");
        return;
    }

    strcpy(novoNo->data, valor);
    novoNo->proximo = stack->top;
    stack->top = novoNo;
}

char* pop(Stack *stack){
    if(stack->top == NULL) {
        printf("A pilha está vazia\n");
        return NULL;
    }

    No *temp = stack->top;
    char *valor = malloc(10 * sizeof(char)); 
    strcpy(valor, temp->data);
    stack->top = stack->top->proximo;
    free(temp);

    return valor;
}

void printStack(Stack *stack){
    if(stack->top == NULL) {
        printf("A pilha está vazia\n");
        return;
    }

    No *noTemp = stack->top;
    while(noTemp != NULL){
        printf("%s\n", noTemp->data);
        noTemp = noTemp->proximo;
    }
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
