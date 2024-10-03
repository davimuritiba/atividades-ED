#include <stdio.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

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
        for (int i = p->topo - 1; i >= 0; i--) 
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
int main()
    {


        return 0;
    }
