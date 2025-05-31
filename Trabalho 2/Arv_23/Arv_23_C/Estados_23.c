#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../Arv_23_H/Estados_23.h"
#include "../Arv_23_H/Cidades_23.h"
#include "../Arv_23_H/utilitarios_23.h"
#include "../Arv_23_H/STRUCTS_23.h"
// #include "../Arv_23_H/Cidades_23.h"

// =================================
// PESQUISA E INSERÇÃO
// =================================

ESTADOS *existe_estado(ESTADOS *cabeca, char *nome_estado)
{
    ESTADOS *resultado;
    ESTADOS *atual;
    atual = cabeca;
    resultado = NULL;

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
    ESTADOS *novo_estado;
    novo_estado = (ESTADOS *)malloc(sizeof(ESTADOS));
    
    if (novo_estado != NULL)
    {
        novo_estado->nome_estado = nome_estado;
        novo_estado->quant_city = 0;
        novo_estado->populacao_estado = 0;
        novo_estado->arv_cidades = NULL;
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
        liberar_no_estado(&novo_estado);
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
    if (nome_estado == NULL)
    {
        erro = 1;
        free(nome_estado);
    }
    // a capital sera add na interface
    if (!erro)
        estado = aloca_estado(nome_estado);

    return estado;
}

// =================================
// LIBERAÇÃO
// =================================

// Função para liberar todos os estados
void liberar_todos_estados(ESTADOS **cabeca)
{
    ESTADOS *atual;
    ESTADOS *proximo;
    atual = *cabeca;

    while (atual != NULL)
    {
        proximo = atual->prox;
        liberar_no_estado(&atual);
        atual = proximo;
    }
    *cabeca = NULL;
}

// Função para desalocar um estado
void liberar_no_estado(ESTADOS **estado)
{
    if (estado != NULL)
    {
        free((*estado)->nome_estado);
        (*estado)->nome_estado = NULL;
        
        if ((*estado)->arv_cidades != NULL)
            // Desalocar a árvore de cidades associada ao estado
            libera_arvore_cidade(&((*estado)->arv_cidades));
        free(*estado);
    }
}

// =================================
// IMPRESSÃO
// =================================

int mostrar_estado(ESTADOS *estado)
{
    int impresso = 0;
    
    if (estado != NULL)
    {
        printf("Estado: %s, Capital: %s\n\n", estado->nome_estado, estado->nome_capital);
        impresso = 1;
    }
    return impresso;
}

int mostrar_todos_estados(ESTADOS *cabeca)
{
    int impresso = 0, i = 0;
    ESTADOS *atual;
    atual = cabeca;

    while (atual != NULL)
    {
        printf("%d. ", ++i);
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