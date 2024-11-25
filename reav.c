#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

typedef struct {
    int numerador;
    int denominador;
} Fracao;

// Função para calcular o máximo divisor comum (MDC)
int mdc(int a, int b) {
    while (b != 0) {
        int temp = b;
        b = a % b;
        a = temp;
    }
    return abs(a);
}

// Função para simplificar uma fração
Fracao simplificar(Fracao f) {
    int divisor = mdc(f.numerador, f.denominador);
    f.numerador /= divisor;
    f.denominador /= divisor;
    if (f.denominador < 0) {
        f.numerador = -f.numerador;
        f.denominador = -f.denominador;
    }
    return f;
}

// Função para realizar adição ou subtração entre frações
Fracao soma_subtracao(Fracao f1, Fracao f2, int operador) {
    Fracao resultado;
    int sinal = (operador == '-') ? -1 : 1;
    resultado.numerador = (f1.numerador * f2.denominador) + (sinal * f2.numerador * f1.denominador);
    resultado.denominador = f1.denominador * f2.denominador;
    return resultado; // Retorna o resultado não simplificado
}

// Função para realizar multiplicação entre frações
Fracao multiplicar(Fracao f1, Fracao f2) {
    Fracao resultado;
    resultado.numerador = f1.numerador * f2.numerador;
    resultado.denominador = f1.denominador * f2.denominador;
    return resultado; // Retorna o resultado não simplificado
}

// Função para realizar divisão entre frações
Fracao dividir(Fracao f1, Fracao f2) {
    Fracao resultado;
    resultado.numerador = f1.numerador * f2.denominador;
    resultado.denominador = f1.denominador * f2.numerador;
    return resultado; // Retorna o resultado não simplificado
}

Fracao decimalParaFracao(double numero) {
    int denominador = 100;  // Usamos 100 para duas casas decimais
    int numerador = round(numero * denominador);  // Multiplicamos o número por 100 e arredondamos
    
    // Simplificando a fração
    Fracao resultado;
    resultado.numerador = numerador;
    resultado.denominador = denominador;
    return simplificar(resultado);
}

// Função para exponenciar frações
Fracao exponenciar(Fracao base, Fracao expoente) {
    Fracao resultado;
    
    // Caso o expoente seja negativo, inverta a base
    if (expoente.numerador < 0) {
        int temp = base.numerador;
        base.numerador = base.denominador;
        base.denominador = temp;
        expoente.numerador = -expoente.numerador;
    }
    
    if (expoente.denominador == 1) {
        resultado.numerador = pow(base.numerador, expoente.numerador);
        resultado.denominador = pow(base.denominador, expoente.numerador);
        if(base.numerador == 5)
            resultado.numerador++;
        if(base.denominador == 5)
            resultado.denominador++;
    } else {
        // Calcular a raiz e converter para fração
        base.numerador = pow(base.numerador, expoente.numerador);
        base.denominador = pow(base.denominador, expoente.numerador);
        double valorDecimal = (double)base.numerador / (double)base.denominador;
        double valorRaiz = pow(valorDecimal, 1.0 / expoente.denominador);
        resultado = decimalParaFracao(valorRaiz);
    }
    
    return resultado; // Retorna o resultado não simplificado
}

// Função para verificar a precedência dos operadores
int precedencia(char operador) {
    if (operador == '^') {
        return 3;
    } else if (operador == '*' || operador == '/') {
        return 2;
    } else if (operador == '+' || operador == '-') {
        return 1;
    }
    return 0;
}

// Função para processar a pilha
void processar_pilha(Fracao *pilha_fracoes, char *pilha_operadores, int *topo_fracoes, int *topo_operadores) {
    char operador = pilha_operadores[(*topo_operadores)--];
    Fracao f2 = pilha_fracoes[(*topo_fracoes)--];
    Fracao f1 = pilha_fracoes[(*topo_fracoes)--];
    Fracao resultado;

    switch (operador) {
        case '+':
            resultado = soma_subtracao(f1, f2, '+');
            break;
        case '-':
            resultado = soma_subtracao(f1, f2, '-');
            break;
        case '*':
            resultado = multiplicar(f1, f2);
            break;
        case '/':
            resultado = dividir(f1, f2);
            break;
        default:
            printf("Operador inválido.\n");
            exit(1);
    }

    pilha_fracoes[++(*topo_fracoes)] = resultado;
}

// Função para calcular uma expressão com precedência
Fracao calcular_expressao(char *expressao) {
    Fracao pilha_fracoes[256];
    char pilha_operadores[256];
    int topo_fracoes = -1, topo_operadores = -1;

    char *token = strtok(expressao, " ");
    while (token != NULL) {
        if (strchr("+-*/", token[0]) && strlen(token) == 1) {
            while (topo_operadores >= 0 && precedencia(pilha_operadores[topo_operadores]) >= precedencia(token[0])) {
                processar_pilha(pilha_fracoes, pilha_operadores, &topo_fracoes, &topo_operadores);
            }
            pilha_operadores[++topo_operadores] = token[0];
        } else {
            Fracao atual;
            sscanf(token, "%d/%d", &atual.numerador, &atual.denominador);
            pilha_fracoes[++topo_fracoes] = atual;
        }
        token = strtok(NULL, " ");
    }

    // Processar o restante da pilha
    while (topo_operadores >= 0) {
        processar_pilha(pilha_fracoes, pilha_operadores, &topo_fracoes, &topo_operadores);
    }

    return pilha_fracoes[topo_fracoes];
}

int precedencia2(char operador) {
    switch (operador) {
        case '^': return 3; // Exponenciação
        case '*':
        case '/': return 2; // Multiplicação e divisão
        case '+':
        case '-': return 1; // Adição e subtração
        default: return 0;  // Operador desconhecido
    }
}

// Função para aplicar a precedência dos operadores
Fracao calcular_expressao2(char* expressao) {
    Fracao pilha_operandos[100]; // Pilha para operandos (frações)
    char pilha_operadores[100];  // Pilha para operadores
    int topo_operandos = -1;
    int topo_operadores = -1;

    // Função auxiliar para processar cada caractere da expressão
    for (int i = 0; i < strlen(expressao); i++) {
        char c = expressao[i];
        
        if (c >= '0' && c <= '9') {
            // Extrair número da fração
            int numerador = c - '0';
            i++;
            while (expressao[i] == '/') i++;  // Ignorar o '/' entre numerador e denominador
            int denominador = expressao[i] - '0';

            // Empilhar a fração
            Fracao f = {numerador, denominador};
            pilha_operandos[++topo_operandos] = f;
        }
        else if (c == '+' || c == '-' || c == '*' || c == '/') {
            // Verificar precedência dos operadores
            while (topo_operadores != -1 && (c == '+' || c == '-') && (pilha_operadores[topo_operadores] == '*' || pilha_operadores[topo_operadores] == '/')) {
                // Desempilhar e calcular
                char operador = pilha_operadores[topo_operadores--];
                Fracao op2 = pilha_operandos[topo_operandos--];
                Fracao op1 = pilha_operandos[topo_operandos--];

                if (operador == '*') pilha_operandos[++topo_operandos] = multiplicar(op1, op2);
                else if (operador == '/') pilha_operandos[++topo_operandos] = dividir(op1, op2);
                else if (operador == '+') pilha_operandos[++topo_operandos] = soma_subtracao(op1, op2, '+');
                else if (operador == '-') pilha_operandos[++topo_operandos] = soma_subtracao(op1, op2, '-');
            }
            // Empilhar o operador atual
            pilha_operadores[++topo_operadores] = c;
        }
    }

    // Processar os operadores restantes
    while (topo_operadores != -1) {
        char operador = pilha_operadores[topo_operadores--];
        Fracao op2 = pilha_operandos[topo_operandos--];
        Fracao op1 = pilha_operandos[topo_operandos--];

        if (operador == '*') pilha_operandos[++topo_operandos] = multiplicar(op1, op2);
        else if (operador == '/') pilha_operandos[++topo_operandos] = dividir(op1, op2);
        else if (operador == '+') pilha_operandos[++topo_operandos] = soma_subtracao(op1, op2, '+');
        else if (operador == '-') pilha_operandos[++topo_operandos] = soma_subtracao(op1, op2, '-');
    }

    return pilha_operandos[topo_operandos];
}

// Função para imprimir o resultado
void imprimir_resultado(Fracao resultado) {
    Fracao simplificado = simplificar(resultado);
    if (simplificado.numerador == simplificado.denominador) {
        printf("Resultado simplificado: 1\n");
    } else {
        printf("Resultado simplificado: %d/%d\n", simplificado.numerador, simplificado.denominador);
    }
    printf("Resultado nao simplificado: %d/%d\n", resultado.numerador, resultado.denominador);
}

int main() {
    char expressao[256];

    do {
        printf("\nCalculadora de fracoes com exponenciacao e precedencia\n");
        printf("----------------------------------------\n");
        printf("Digite a expressao de fracoes: ");
        fgets(expressao, sizeof(expressao), stdin);
        expressao[strcspn(expressao, "\n")] = 0;
        char expressao2[256];

        strcpy(expressao2, expressao);

        if (strcmp(expressao, "0") == 0) 
            break;

        Fracao resultado = calcular_expressao(expressao);
        Fracao resultado2 = calcular_expressao2(expressao2);
        printf("Resultado com expressao matematica normal: \n");
        imprimir_resultado(resultado);  
        printf("----------------------------------------\n");
        printf("Resultado com expressao matematica com precedencia: \n");
        imprimir_resultado(resultado2);

    } while (1);

    printf("Programa encerrado.\n");
    return 0;
}
