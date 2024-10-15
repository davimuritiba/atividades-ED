#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <ctype.h>

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
int binParaDecimal(char *binario, int len);
char* somaBinariosDecimais(char *a, char *b, int *carry);
char* somaBinariosInteiros(char *a, char *b, int carry);
char* somaBinarios(char *a, char *b);
char* subtrairBinarios(char* a, char* b);
void printarResultadoOrg(char *resultado);
char* multiplicaBinarioInteiro(char *a, char* b);
int binParaDecimalInteiro(char *binario);
int binarioParaDecimalParteInteira(char *binario19Bits);
int binarioParaInteiro(char *binario, int numBits);
char *decimalParaBinarioInteiro(int valor);
char* dividirBinariosInteiros(char* binario1, char* binario2);
int isOperator(char *token);
