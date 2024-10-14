#include "calcLib.h"

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

//Função de binario para decimal completo
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

//Função de binario para decimal so para comparar o inteiro
int binParaDecimalInteiro(char *binario) {
    int parteInteira = 0;
    int len = 10;
    for (int i = 1; i < len; i++) {
        if (binario[i] == '1') {
            parteInteira += pow(2, len - i - 1);
        }
    }
    return parteInteira;
}

//Soma parte fracionaria do numero
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

//Soma a parte dos inteiros
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
    
    char *resultado = malloc(tamanhoBits + 1);
    if(a[0] == '1' && b[0] == '0' || a[0] == '0' && b[0] == '1'){
        int valorA = binParaDecimalInteiro(a + 1); // Desconsidera o sinal
        int valorB = binParaDecimalInteiro(b + 1); // Desconsidera o sinal

        printf("valor de a: %d, valor de b: %d\n", valorA, valorB);
        if(a[0] == '1' && b[0] == '0'){
            // A é negativo, B é positivo
            resultado[0] = (valorA > valorB) ? '1' : '0'; // Se A > B, resultado é negativo
            resultado = subtrairBinarios(a, b);
            return resultado;
        } 
        else if (a[0] == '0' && b[0] == '1'){
            // A é positivo, B é negativo
            resultado[0] = (valorB > valorA) ? '1' : '0'; // Se B > A, resultado é negativo
            resultado = subtrairBinarios(a, b);
            return resultado;
        }

        free(resultado);
        resultado = subtrairBinarios(a, b);
        return resultado;
    }

    
    char *parteDecimal = somaBinariosDecimais(a + 11, b + 11, &carry);
    char *parteInteira = somaBinariosInteiros(a, b, carry);

    strcpy(resultado, parteInteira);
    strcat(resultado, parteDecimal);

    free(parteDecimal);
    free(parteInteira);

    return resultado;
}

// Função ajustada para subtrair binários sem complemento de dois quando o resultado é positivo
char* subtrairBinarios(char* a, char* b) {
    int max_len = 19; // 11 bits parte inteira + 8 bits parte decimal

    char* result = (char*)malloc(max_len + 1); // +1 para o '\0'
    result[max_len] = '\0';

    int borrow = 0;

    // Subtração da parte decimal (últimos 8 bits)
    for (int i = max_len - 1; i >= 11; i--) {
        int bitA = a[i] - '0';
        int bitB = b[i] - '0';

        if (bitA - borrow < bitB) {
            result[i] = (bitA + 2 - borrow - bitB) + '0';
            borrow = 1;
        } else {
            result[i] = (bitA - borrow - bitB) + '0';
            borrow = 0;
        }
    }

    // Subtração da parte inteira (primeiros 10 bits, incluindo sinal)
    for (int i = 10; i >= 1; i--) {
        int bitA = a[i] - '0';
        int bitB = b[i] - '0';

        if (bitA - borrow < bitB) {
            result[i] = (bitA + 2 - borrow - bitB) + '0';
            borrow = 1;
        } else {
            result[i] = (bitA - borrow - bitB) + '0';
            borrow = 0;
        }
    }

    // Verificar se o resultado é negativo
    if (a[0] == '1' && b[0] == '0') {
        // Caso onde A é negativo e B é positivo, o resultado é negativo
        result[0] = '1';  // Ajusta o sinal para negativo
    } 
    else if (a[0] == '0' && b[0] == '1') {
        // Caso onde A é positivo e B é negativo, o resultado é positivo
        result[0] = '0';
    } 
    else if (borrow == 1) {
        // Caso onde A é positivo e B é maior que A, o resultado é negativo
        result[0] = '1';  // Ajusta o sinal para negativo
    } 
    else {
        // Caso onde o resultado é positivo
        result[0] = '0';  // Sinal positivo
    }

    return result;
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

// Função para multiplicar dois números binários inteiros
char* multiplicaBinarioInteiro(char *a, char* b)
{
    int tamanho1 = strlen(a); // tamanho do primeiro número binário
    int tamanho2 = strlen(b); // tamanho do segundo número binário
    int tamanhoResultado = tamanho1 + tamanho2; // tamanho do resultado

    // Aloca espaço para o resultado
    char *result = (char*) malloc((tamanhoResultado + 1) * sizeof(char));

    // Inicializa o resultado com '0'
    for(int i = 0; i < tamanhoResultado; i++)
    {
        result[i] = '0';
    }
    result[tamanhoResultado] = '\0'; // finaliza a string

    // Itera sobre os bits de 'a' de trás para frente
    for(int i = tamanho1 - 1; i >= 0; i--)
    {
        if (a[i] == '1') // verifica se o bit de 'a' é 1
        {
            int carry = 0; // inicializa o carry como 0
            for(int j = tamanho2 - 1; j >= 0; j--)
            {
                int soma = (result[i + j + 1] - '0') + (b[j] - '0') + carry; // Soma bit atual de result, bit de b, e carry
                result[i + j + 1] = (soma % 2) + '0';  // Adiciona o bit da soma
                carry = soma / 2;  // Calcula o carry
            }
            result[i] = ((result[i] - '0') + carry) + '0';  // Propaga o carry
        }
    }

    return result; // Retorna o resultado
}

int main() {
    Stack *stackBin = malloc(sizeof(Stack));
    initStack(stackBin);
    Stack *stackOperator = malloc(sizeof(Stack));
    initStack(stackOperator);

    char *expressao[10000] = {"0000000101000110010 + 0000000101000110010 - 0000000101000110010 * 0000000101000110010"};

    //0000000101000110010 5,5
    //0000000101000110010 10, 
    push(stackBin, "0000000010100110010"); //10 resultado vai para o B
    push(stackBin, "0000000101000110010"); //5 esse resultado vai para o A

    char *a = pop(stackBin);
    char *b = pop(stackBin);

    printf("a: ->");
    printarResultadoOrg(a);
    printf("b: ->");
    printarResultadoOrg(b);

    char *resultadoSoma = somaBinarios(a, b);
    printf("Soma: ");
    printarResultadoOrg(resultadoSoma);
    char *resultadoSub = subtrairBinarios(a, b);
    printf("Subtracao: ");
    printarResultadoOrg(resultadoSub);

    free(stackBin);
    free(a); 
    free(b); 
    // free(resultadoSoma);

    return 0;
}
