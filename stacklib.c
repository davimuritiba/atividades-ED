#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

int main(){
    Stack *stackBin = malloc(sizeof(Stack));
    initStack(stackBin);
    Stack *stackOperator = malloc(sizeof(Stack));
    initStack(stackOperator);

    //so chamar o push com o construtor, ex:
    push(stackBin, "01010101");

    free(stackBin);
    free(stackOperator);
    return 0;
}
