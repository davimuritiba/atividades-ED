#include <stdio.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

//criando a pilha
typedef struct
{
    int *dados;
    int topo;
    int capacidade;

}Pilha;

Pilha *startPilha(int capacidade_inicial)
    {
        Pilha *p = (Pilha*)malloc(sizeof(Pilha));
        p->dados = (int*)malloc(capacidade_inicial * sizeof(int));
        p->topo=0;
        p->capacidade = capacidade_inicial;
        return p;
    }
Pilha *Empilhar(int dado, Pilha *p)
    {
        if(p->topo == p->capacidade)
            {
                p->capacidade *=2;
                p->dados = (int*)realloc(p->dados, p->capacidade * sizeof(int));
            }
        p->dados[p->topo] = dado;
        p->topo++;
        return p;
    }
Pilha *Desempilhar(Pilha *p)
    {
        if(p->topo == 0)
            {
                printf("Pilha vazia...");
            }
        else
            {
                p->topo--;
            }
        return p;
    }
void printarPilha(Pilha *p)
    {
        if(p->topo == 0)
            {
                printf("Pilha vazia...");
            }
        for(int i = p->topo - 1; i >= 0; i--) 
            {
                    printf("%d ", p->dados[i]);
            }
        printf("\n");
    }
void freePilha(Pilha *p)
    {
        free(p->dados);
        free(p);
    }


void inverterStr(char *str)//funcao que inverte a string, pois o resultado da soma sera feito de forma inversa
    {
        int n = strlen(str);
        for(int i=0; i<n/2; i++)
            {
                char temp = str[i];
                str[i] = str[n-i-1];
                str[n-i-1]= temp;
            }
    }

char* somaBinaria (char* a, char* b)//funcao que soma dois numeros binarios lidos como string (a+b)
    {
        int tamanho_a = strlen(a);//tamanho do numero binario a
        int tamanho_b = strlen(b);//tamanho do numero binaro b
        int tamanho_maximo;//tamanho maximo que sera utilizado
        if(tamanho_a > tamanho_b)
            {
                tamanho_maximo = tamanho_a;
            }
        else
            {
                tamanho_maximo = tamanho_b;
            }

        char* result = (char*)malloc(tamanho_maximo+2);//aloca memoria do tamanho maximo utilizado +2, caso do incremento do carry e do final da string "\0"
        result[tamanho_maximo+1] = '\0';//verifica se a string termina em nulo

        int carry = 0;//introduz o carry, variavel utilizada caso a soma de dois numeros seja maior que 1.
        int i = tamanho_a-1, j = tamanho_b-1, k = tamanho_maximo;
 
        while(i>= 0|| j>=0)
            {
                int bitA, bitB;

                if (i>=0)//pega um bit de a caso ainda haja, caso contrario pega 0
                    {
                        bitA = a[i]- '0';
                    }
                else
                    {
                        bitA = 0;
                    }
                
                if (j>=0)//pega um bit de b caso ainda haja, caso contrario pega 0.
                    {
                        bitB = a[i] - '0';
                    }
                else
                    {
                        bitB = 0;
                    }

                int soma = bitA + bitB + carry;//soma dos bits com carry

                if(soma%2 == 0)//o bit de soma vai para o resultado
                    {
                        result[k] = '0';
                    }
                else
                    {
                        result[k] = '1';
                    }
                
                if(soma >= 2)//o carry vai pra proxima soma
                    {
                        carry=1;
                    }
                else
                    {
                        carry=0;
                    }

                i--;
                j--;
                k--;
            }

        if (carry)//se ainda houver carry, ele é adicionado ao inicio do resultado
            {
                result[k] = '1';
            }
        else
            {
                result[k] = '0';//caso nao haja carry, preenche com 0.
            }

        inverterStr(result);//inverte a string
        return(result);//retorna o resultado

    }
int main()
    {


        return 0;
    }
