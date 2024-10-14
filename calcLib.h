#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

#define tamanhoBits 19

typedef struct no {
    char data[tamanhoBits]; //1 para o sinal, 10 para o inteiro, 8 para o decimal
    struct no *proximo;
} No;

typedef struct stack {
    No *top;
} Stack;

//declarando funções
void initStack(Stack *stack);
void push(Stack *stack, char valor[]);
char* pop(Stack *stack);
void printStack(Stack *stack);
void inverterBits(char *binario);
int binParaDecimal(char *binario);
char* somaBinariosDecimais(char *a, char *b, int *carry);
char* somaBinariosInteiros(char *a, char *b, int carry);
char* somaBinarios(char *a, char *b);
char* subtrairBinarios(char* a, char* b);
void printarResultadoOrg(char *resultado);
