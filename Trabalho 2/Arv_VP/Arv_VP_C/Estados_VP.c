#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../Arv_VP_H/Estados_VP.h"
#include "../Arv_VP_H/utilitarios_VP.h"
#include "../Arv_VP_H/STRUCTS_VP.h"
#include "../Arv_VP_H/Cidades_VP.h"

ESTADOS *existe_estado(ESTADOS *cabeca, char *nome_estado){
    ESTADOS *resultado = NULL;
    ESTADOS *atual = cabeca;

    while (atual != NULL && resultado == NULL)
    {
        if (strcasecmp(atual->nome_estado, nome_estado) == 0)
            resultado = atual; // Estado encontrado
        else
            atual = atual->prox;
    }
    return resultado;
}

ESTADOS *aloca_estado(char *nome_estado)
{
    ESTADOS *novo_estado = (ESTADOS *)malloc(sizeof(ESTADOS));
    if (novo_estado != NULL)
    {

        novo_estado->nome_estado = nome_estado;
        novo_estado->nome_capital = NULL;
        novo_estado->quant_city = 0;
        novo_estado->populacao_estado = 0;
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
        // limpar_estado(novo_estado);
        // free(novo_estado);
        desalocar_estado(&novo_estado);
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
        resultado = inserir_estado_rec(&((*cabeca)->prox), novo_estado);

    return resultado;
}

ESTADOS *cadastro_estado()
{
    int erro = 0;
    char *nome_estado;
    ESTADOS *estado;
    estado = NULL;

    printf("Digite o nome do Estado: ");
    nome_estado = ler_string();
    if (nome_estado == NULL){
        erro = 1;
        free(nome_estado);
    }
    // a capital sera add na interface 
    if (!erro)
    {
        estado = aloca_estado(nome_estado);
    }
    //o resto do código deve ser feito na interface.c
    return estado;
}

// Função para liberar todos os estados

void liberar_todos_estados(ESTADOS **cabeca)
{
    ESTADOS *atual = *cabeca;
    ESTADOS *proximo;

    while (atual != NULL)
    {
        proximo = atual->prox;
        desalocar_estado(&atual);
        atual = proximo;
    }
    *cabeca = NULL;
}

// Função para desalocar um estado

void desalocar_estado(ESTADOS **estado)
{
    if (estado != NULL)
    {
        free((*estado)->nome_estado);
        (*estado)->nome_estado = NULL;
        free((*estado)->nome_capital);
        (*estado)->nome_capital = NULL;

        if ((*estado)->cidade != NULL)
        {
            // Desalocar a árvore de cidades associada ao estado
            limpa_arvore_cidades(&((*estado)->cidade));
        }
        free(*estado);
    }
}

int remover_estado_rec(ESTADOS **cabeca, char *nome_estado)
{
    int resultado = 0;

    if (*cabeca != NULL)
    {
        if (strcasecmp((*cabeca)->nome_estado, nome_estado) == 0)
        {
            ESTADOS *temp;
            temp = *cabeca;

            *cabeca = temp->prox; // Atualiza o ponteiro da cabeça

            if (*cabeca != NULL)
                (*cabeca)->ant = temp->ant; // Atualiza o ponteiro anterior do próximo nó
            if (temp->ant != NULL)
                temp->ant->prox = *cabeca; // Atualiza o ponteiro próximo do nó anterior
            
            desalocar_estado(&temp);
            // limpar_estado(temp);
            // free(temp);

            resultado = 1; // Estado removido com sucesso
        }
        else
            resultado = remover_estado_rec(&((*cabeca)->prox), nome_estado);
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
    ESTADOS *atual;
    atual = cabeca;

    while (atual != NULL)
    {
        if (mostrar_estado(atual))
            impresso = 1;
        atual = atual->prox;
    }
    return impresso;
}

// =================================
// ESPECÍFICAS DO TRABALHO
// =================================

ESTADOS *verifica_estado_mais_populoso(ESTADOS *cabeca)
{
    ESTADOS *estado_mais_populoso;
    estado_mais_populoso = NULL;
    int maior_populacao = -1;

    while (cabeca != NULL)
    {
        if (cabeca->populacao_estado > maior_populacao)
        {
            maior_populacao = cabeca->populacao_estado;
            estado_mais_populoso = cabeca;
        }
        cabeca = cabeca->prox;
    }
    return (estado_mais_populoso);
}