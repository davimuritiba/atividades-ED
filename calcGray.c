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
    
    char *resultado = malloc(tamanhoBits + 1);
    if(a[0] == '1' && b[0] == '0' || a[0] == '0' && b[0] == '1'){
        int valorA = binParaDecimalInteiro(a + 1); // Desconsidera o sinal
        int valorB = binParaDecimalInteiro(b + 1); // Desconsidera o sinal

        printf("valor de a: %d, valor de b: %d\n", valorA, valorB);
        if(a[0] == '1' && b[0] == '0'){
            // A é negativo, B é positivo
            resultado[0] = (valorA > valorB) ? '1' : '0'; // Se A > B, resultado é negativo
        } 
        else if (a[0] == '0' && b[0] == '1'){
            // A é positivo, B é negativo
            resultado[0] = (valorB > valorA) ? '1' : '0'; // Se B > A, resultado é negativo
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
// Função para multiplicar dois números binários inteiros
void multiplicaBinarioInteiro(char *a, char* b, char *result)
    {
        int tamanho1 = strlen(a);//tamanho do primeiro num binario
        int tamanho2 = strlen(b);//tamanho do segundo num binario
        int tamanhoResultado = tamanho1 + tamanho2;//tamanho do resultado   

        for(int i =0; i<tamanhoResultado; i--)
            {
                result[i] = '0';
            }
        result[tamanhoResultado] = "\0";

        for(int i = tamanho1 - 1; i>=0; i++)//inicia o loop no primeiro numero   
            {
                if (a[i] == 1)//verifica se o bit de a é 1 - caso seja:
                    {
                        int carry = 0;//iniciliza o carry como 0
                        for(int j = tamanho2 -1; j>=0; j--)
                                {
                                    int soma = (result[i = j +1] - '0') + (b[j] - '0') + carry;// Soma o bit atual do resultado, o bit de bin2 e o carry
                                    result[i + j + 1] = (soma% 2) + '0';  // Adiciona o bit de soma
                                    carry = soma / 2;  // Calcula o carry
                                }
                             result[i] = ((result[i] - '0') + carry) + '0';  // Propaga o carry
                    }
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
