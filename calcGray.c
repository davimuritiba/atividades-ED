#include "calcGray.h"

// Funções de pilha
void initStack(Stack *stack) {
    stack->top = NULL;
}

void push(Stack *stack, char valor[]) {
    No *novoNo = malloc(sizeof(No));
    if (novoNo == NULL) {
        printf("Erro na hora de alocar a memoria\n");
        return;
    }
    strcpy(novoNo->data, valor);
    novoNo->proximo = stack->top;
    stack->top = novoNo;
}

char* pop(Stack *stack) {
    if (stack->top == NULL) {
        printf("A pilha está vazia\n");
        return NULL;
    }

    No *temp = stack->top;
    char *valor = malloc(tamanhoBits * sizeof(char));
    strcpy(valor, temp->data);
    stack->top = stack->top->proximo;
    free(temp);
    return valor;
}

void printStack(Stack *stack) {
    if (stack->top == NULL) {
        printf("A pilha está vazia\n");
        return;
    }

    No *noTemp = stack->top;
    while (noTemp != NULL) {
        printf("%s\n", noTemp->data);
        noTemp = noTemp->proximo;
    }
}

//Começo das funções da calculadora
void inverterBits(char *binario) {
    for (int i = 0; binario[i] != '\0'; i++) {
        binario[i] = (binario[i] == '1') ? '0' : '1';
    }
}

int binParaDecimal(char *binario) {
    int decimal = 0;
    int len = strlen(binario);
    for (int i = 0; i < len; i++) {
        if (binario[i] == '1') {
            decimal += pow(2, len - i - 1);
        }
    }
    return decimal;
}

char* somaBinariosDecimais(char *a, char *b, int *carry) {
    int tamanhoMax = 8; // parte decimal tem 8 bits
    char* result = malloc(tamanhoMax + 1);
    result[tamanhoMax] = '\0';
    int carryDecimal = 0;

    for (int i = tamanhoMax - 1; i >= 0; i--) {
        int bitA = a[i] - '0';
        int bitB = b[i] - '0';
        int soma = bitA + bitB + carryDecimal;
        result[i] = (soma % 2) + '0';
        carryDecimal = soma / 2; // atualiza o carry para a parte inteira
    }

    int resultInt = binParaDecimal(result);
    if(resultInt > 99){
        *carry = 1;  
        resultInt = resultInt % 100; // Mantém apenas as duas casas
        // Atualiza o resultado com o novo valor
        for (int i = 0; i < 8; i++) {
            result[i] = '0';
        }
        sprintf(result + 6, "%02d", resultInt); // Preenche com os últimos 2 dígitos
    } 
    else{
        *carry = 0; // Não há carry
    }

    return result;
}

char* somaBinariosInteiros(char *a, char *b, int carry) {
    int tamanhoMax = 11; // sinal + 10 bits de parte inteira
    char* result = malloc(tamanhoMax + 1);
    result[tamanhoMax] = '\0';

    for (int i = tamanhoMax - 1; i >= 1; i--) { // começa pelo bit 10
        int bitA = a[i] - '0';
        int bitB = b[i] - '0';
        int soma = bitA + bitB + carry;
        result[i] = (soma % 2) + '0';
        carry = soma / 2;
    }

    result[0] = a[0]; // preserva o sinal
    return result;
}

//essa função soma os inteiros, decimais e concatena
char* somaBinarios(char *a, char *b) {
    int carry = 0;

    char *parteDecimal = somaBinariosDecimais(a + 11, b + 11, &carry);
    char *parteInteira = somaBinariosInteiros(a, b, carry);

    char *resultado = malloc(tamanhoBits + 1);
    strcpy(resultado, parteInteira);
    strcat(resultado, parteDecimal);

    free(parteDecimal);
    free(parteInteira);

    return resultado;
}

//função para subtrair binários usando complemento de dois
char* subtrairBinarios(char* a, char* b) {
    char *b_copia = malloc(tamanhoBits * sizeof(char)); 
    strcpy(b_copia, b); 

    inverterBits(b_copia);  // faz o complemento de 1
    char* complementoDois = somaBinarios(b_copia, "0000000000000000001");  // soma 1 para obter o complemento de dois
    char* resultado = somaBinarios(a, complementoDois);

    free(b_copia); 
    free(complementoDois); 
    return resultado;
}

void printarResultadoOrg(char *resultado) {
    printf("resultado final: |%c|", resultado[0]);
    for (int i = 1; i <= 10; i++)
        printf("%c", resultado[i]);
    printf("|");
    for (int i = 11; i <= 18; i++)  // a parte decimal
        printf("%c", resultado[i]);
    printf("|\n");
}
// Função para multiplicar dois números binários com ponto fixo
void multiplicarBinarios(char *bin1, char *bin2, char *result) {
    int len1 = strlen(bin1);  // Tamanho do binário de entrada (18 bits)
    int len2 = strlen(bin2);  // Tamanho do binário de entrada (18 bits)
    int resultLen = len1 + len2;  // Resultado pode ter até 36 bits

    // Inicializa o resultado como 0
    for (int i = 0; i < resultLen; i++) {
        result[i] = '0';
    }
    result[resultLen] = '\0';

    // Algoritmo de multiplicação binária
    for (int i = len1 - 1; i >= 0; i--) {
        if (bin1[i] == '1') {
            int carry = 0;
            for (int j = len2 - 1; j >= 0; j--) {
                int sum = (result[i + j + 1] - '0') + (bin2[j] - '0') + carry;
                result[i + j + 1] = (sum % 2) + '0';
                carry = sum / 2;
            }
            result[i] = ((result[i] - '0') + carry) + '0';
        }
    }

    // Ajusta a vírgula binária (ponto fixo)
    // Desloca o resultado 8 posições à direita para ajustar a parte fracionária
    for (int i = resultLen - 1; i >= 8; i--) {
        result[i] = result[i - 8];
    }
    for (int i = 0; i < 8; i++) {
        result[i] = '0';
    }
}
int main() {
    Stack *stackBin = malloc(sizeof(Stack));
    initStack(stackBin);

    push(stackBin, "0000000101000110010"); 
    push(stackBin, "0000000101000110010"); 

    char *a = pop(stackBin);
    char *b = pop(stackBin);

    printf("a: ->");
    printarResultadoOrg(a);
    printf("b: ->");
    printarResultadoOrg(b);

    char *resultadoSoma = somaBinarios(a, b);
    printf("Soma: ");
    printarResultadoOrg(resultadoSoma);

    free(stackBin);
    free(a); 
    free(b); 
    free(resultadoSoma);

    return 0;
}
