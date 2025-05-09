#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../Arv_VP_H/Estados.h"
#include "../Arv_VP_H/utilitarios.h"
#include "../Arv_VP_H/STRUCTS.h"

ESTADOS *criar_estado(char *nome_estado, char *nome_capital)
{
    ESTADOS *novo_estado = (ESTADOS *)malloc(sizeof(ESTADOS));
    if (novo_estado != NULL)
    {

        novo_estado->nome_estado = nome_estado;
        novo_estado->nome_capital = nome_capital;
        novo_estado->quant_city = 0;
        novo_estado->cidade = NULL;
        novo_estado->prox = NULL;
        novo_estado->ant = NULL;
    }

    return novo_estado;
}

int inserir_estado_rec(ESTADOS **cabeca, ESTADOS *novo_estado)
{
    int resultado = 0;

    if (*cabeca == NULL)
    {
        *cabeca = novo_estado;
        resultado = 1;
    }
    else if (strcasecmp((*cabeca)->nome_estado, novo_estado->nome_estado) == 0)
    {
        limpar_estado(novo_estado);
        free(novo_estado);
        resultado = 0; // Estado já existe
    }
    else if (strcasecmp((*cabeca)->nome_estado, novo_estado->nome_estado) > 0)
    {
        novo_estado->prox = *cabeca;
        (*cabeca)->ant = novo_estado;
        *cabeca = novo_estado;
        resultado = 1;
    }
    else
    {
        resultado = inserir_estado_rec(&((*cabeca)->prox), novo_estado);
    }

    return resultado;
}

int cadastro_estado(ESTADOS **cabeca)
{
    int erro = 0;
    char *nome_estado;
    char *nome_capital;
    ESTADOS *estado = NULL;

    printf("Digite o nome do Estado: ");
    nome_estado = ler_string();
    if (nome_estado == NULL)
        erro = 1;

    if (!erro)
    {
        printf("Digite o nome da Capital: ");
        nome_capital = ler_string();
        if (nome_capital == NULL)
        {
            free(nome_estado);
            erro = 1;
        }
    }

    if (!erro)
    {
        estado = criar_estado(nome_estado, nome_capital);
        if (estado == NULL)
            erro = 1;
        else if (!inserir_estado_rec(cabeca, estado))
            erro = 1;
    }

    return erro;
}

void limpar_estado(ESTADOS *estado)
{
    if (estado != NULL)
    {
        free(estado->nome_estado);
        estado->nome_estado = NULL;
        free(estado->nome_capital);
        estado->nome_capital = NULL;
    }
}

int remover_estado_rec(ESTADOS **cabeca, char *nome_estado)
{
    int resultado = 0;
    
    if (*cabeca != NULL)
    {
        if (strcasecmp((*cabeca)->nome_estado, nome_estado) == 0)
        {
            ESTADOS *temp = *cabeca;
            
            *cabeca = temp->prox; // Update the head pointer
            
            if (*cabeca != NULL)
                (*cabeca)->ant = temp->ant; // Update prev pointer of next node
                
            limpar_estado(temp);
            free(temp);
            
            resultado = 1; // Estado removido com sucesso
        }
        else
        {
            resultado = remover_estado_rec(&((*cabeca)->prox), nome_estado);
        }
    }
    
    return resultado;
}

int mostrar_estado(ESTADOS *estado)
{
    int impresso = 0;
    if (estado != NULL)
    {
        printf("Estado: %s, Capital: %s\n", estado->nome_estado, estado->nome_capital);
        impresso = 1;
    }
    return impresso;
}

int mostrar_todos_estados(ESTADOS *cabeca)
{
    int impresso = 0;
    ESTADOS *atual = cabeca;
    while (atual != NULL)
    {
        if (mostrar_estado(atual))
            impresso = 1;
        atual = atual->prox;
    }
    return impresso;
}