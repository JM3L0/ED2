#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../Arv_VP_H/Cidades_VP.h"
#include "../Arv_VP_H/utilitarios_VP.h"
#include "../Arv_VP_H/STRUCTS_VP.h"
#include "../Arv_VP_H/CEPs_VP.h"

/*---------------------------- Funções Arv Red Black ----------------------------*/

// =================================
// INSERCAO
// =================================

int inserir_no_Cidade(CIDADES **raiz, CIDADES *novaCidade)
{
    int inseriu = 0;

    if (*raiz == NULL)
    {
        *raiz = novaCidade;
        inseriu = 1;
    }
    else if (strcasecmp(novaCidade->nome_cidade, (*raiz)->nome_cidade) < 0)
        inseriu = inserir_no_Cidade(&((*raiz)->esq), novaCidade);
    else if (strcasecmp(novaCidade->nome_cidade, (*raiz)->nome_cidade) > 0)
        inseriu = inserir_no_Cidade(&((*raiz)->dir), novaCidade);

    if (inseriu)
        balancear_RB_cidade(raiz);
    return inseriu;
}

int inserir_Cidade(CIDADES **raiz, CIDADES *novaCidade)
{
    int inseriu;
    inseriu = inserir_no_Cidade(raiz, novaCidade);

    if (*raiz != NULL)
        (*raiz)->cor = BLACK;

    return inseriu;
}

CIDADES *aloca_Cidade(char *nome_cidade, int populacao_city)
{
    CIDADES *cidade;
    cidade = (CIDADES *)malloc(sizeof(CIDADES));

    cidade->nome_cidade = nome_cidade;
    cidade->populacao_city = populacao_city;
    cidade->cor = RED;
    cidade->cep = NULL;
    cidade->esq = NULL;
    cidade->dir = NULL;

    return cidade;
}

CIDADES *cadastrarCidade()
{
    CIDADES *novaCidade;
    novaCidade = NULL;
    char *nome_cidade;
    int populacao, erro = 0;

    printf("Digite o nome da cidade: ");
    nome_cidade = ler_string();

    if (nome_cidade == NULL)
        erro = 1;

    if (erro == 0)
    {
        printf("Digite a populacao da cidade: ");
        populacao = digitar_int();
    }

    if (erro == 0)
        novaCidade = aloca_Cidade(nome_cidade, populacao);
    else
    {
        if (nome_cidade != NULL)
            free(nome_cidade);
    }
    return novaCidade;
}

// =================================
// ROTAÇÃO
// =================================

void rotacao_esquerda_cidade(CIDADES **raiz)
{
    CIDADES *aux;

    aux = (*raiz)->dir;
    (*raiz)->dir = aux->esq;
    aux->esq = (*raiz);
    (*raiz) = aux;

    (*raiz)->cor = (*raiz)->esq->cor;
    (*raiz)->esq->cor = RED;
}

void rotacao_direita_cidade(CIDADES **raiz)
{
    CIDADES *aux;

    aux = (*raiz)->esq;
    (*raiz)->esq = aux->dir;
    aux->dir = (*raiz);
    (*raiz) = aux;

    (*raiz)->cor = (*raiz)->dir->cor;
    (*raiz)->dir->cor = RED;
}

// =================================
// PROPRIEDADES
// =================================

int Cor_cidade(CIDADES *cidade)
{
    return (cidade == NULL) ? BLACK : cidade->cor;
}

void trocar_cor_cidade(CIDADES *raiz)
{
    raiz->cor = !(raiz->cor);

    if (raiz->esq != NULL)
        raiz->esq->cor = !(raiz->esq->cor);

    if (raiz->dir != NULL)
        raiz->dir->cor = !(raiz->dir->cor);
}

void balancear_RB_cidade(CIDADES **raiz)
{
    if (*raiz != NULL)
    {
        if (Cor_cidade((*raiz)->esq) == BLACK && Cor_cidade((*raiz)->dir) == RED)
            rotacao_esquerda_cidade(raiz);
        if ((Cor_cidade((*raiz)->esq) == RED) && Cor_cidade((*raiz)->esq->esq) == RED)
            rotacao_direita_cidade(raiz);
        if (Cor_cidade((*raiz)->esq) == RED && Cor_cidade((*raiz)->dir) == RED)
            trocar_cor_cidade(*raiz);
    }
}

// =================================
// BUSCA E IMPRESSÃO
// =================================

// Função para buscar uma cidade na árvore pelo nome
CIDADES *existe_cidade(CIDADES *raiz, char *nome_cidade)
{
    CIDADES *resultado;
    resultado = NULL;

    if (raiz != NULL)
    {
        int comparacao = strcasecmp(nome_cidade, raiz->nome_cidade);

        if (comparacao == 0)
            resultado = raiz;
        else if (comparacao < 0)
            resultado = existe_cidade(raiz->esq, nome_cidade);
        else
            resultado = existe_cidade(raiz->dir, nome_cidade);
    }

    return resultado;
}

// Função auxiliar para imprimir informações de uma cidade
void imprimir_cidade(CIDADES *cidade)
{
    if (cidade != NULL)
    {
        printf("Cidade: %s\n", cidade->nome_cidade);
        printf("Populacao: %d\n", cidade->populacao_city);
        if(cidade->cep != NULL)
            imprimir_todos_CEP(cidade->cep); // imprime a árvore de cep da cidade
        printf("------------------------\n");
    }
}

// Função para imprimir todas as cidades em ordem alfabética
void imprimir_cidades_em_ordem(CIDADES *raiz)
{
    if (raiz != NULL)
    {
        imprimir_cidades_em_ordem(raiz->esq);
        imprimir_cidade(raiz);
        imprimir_cidades_em_ordem(raiz->dir);
    }
}

// Função para imprimir todas as cidades
void imprimir_todas_cidades(CIDADES *raiz)
{
    if (raiz == NULL)
        printf("Nao ha cidades cadastradas.\n");
    else
    {
        printf("=== Lista de Cidades ===\n");
        imprimir_cidades_em_ordem(raiz);
    }
}


// =================================
// DESALOCAÇÃO
// =================================

// Função para desalocar completamente uma cidade
void limpar_no_cidade(CIDADES **raiz)
{
    if (*raiz != NULL)
    {
        if ((*raiz)->nome_cidade != NULL)
        {
            free((*raiz)->nome_cidade);
            (*raiz)->nome_cidade = NULL;
        }

        if ((*raiz)->cep != NULL)
            desalocar_arvore_CEP(&((*raiz)->cep));

        free(*raiz);
        *raiz = NULL;
    }
}

// Função para desalocar toda a árvore de cidades
void limpar_arvore_cidades(CIDADES **raiz)
{
    if (*raiz != NULL)
    {
        limpar_arvore_cidades(&((*raiz)->esq));
        limpar_arvore_cidades(&((*raiz)->dir));
        limpar_no_cidade(raiz);
    }
}

// =================================
// ESPECÍFICAS DO TRABALHO
// =================================

CIDADES *verifica_cidade_mais_populosa_nao_capital(CIDADES *raiz, char *nome_capital)
{
    CIDADES *mais_populosa;
    mais_populosa = NULL;

    if (raiz)
    {
        mais_populosa = verifica_cidade_mais_populosa_nao_capital(raiz->esq, nome_capital);
        CIDADES *dir;
        dir = verifica_cidade_mais_populosa_nao_capital(raiz->dir, nome_capital);

        if (strcasecmp(nome_capital, raiz->nome_cidade) != 0 && (!mais_populosa || raiz->populacao_city > mais_populosa->populacao_city))
            mais_populosa = raiz;

        if (dir != NULL && (mais_populosa == NULL || dir->populacao_city > mais_populosa->populacao_city))
            mais_populosa = dir;
    }

    return mais_populosa;
}

int cep_pertence_a_cidade(CEP *raiz, char *cep)
{
    int resultado = 0;

    if (raiz != NULL)
    {
        if (strcasecmp(raiz->cep, cep) == 0)
            resultado = 1;
        else
        {
            resultado |= cep_pertence_a_cidade(raiz->esq, cep);
            resultado |= cep_pertence_a_cidade(raiz->dir, cep);
        }
    }
    return resultado;
}

CIDADES *cidade_natal_dado_cep(CIDADES *raiz, char *cep)
{
    CIDADES *resultado;
    resultado = NULL;

    if (raiz != NULL)
    {
        if (cep_pertence_a_cidade(raiz->cep, cep) == 1)
            resultado = raiz;
        else
        {
            resultado = cidade_natal_dado_cep(raiz->esq, cep);
            if (resultado == NULL)
                resultado = cidade_natal_dado_cep(raiz->dir, cep);
        }
    }
    return resultado;
}

int quantas_pessoas_nascidas_na_cidade_nao_moram_na_cidade(CIDADES *cidade, PESSOAS *raiz_pessoa)
{
    int resultado = 0;

    if (cidade != NULL && raiz_pessoa != NULL)
    {
        // Percorre todas as pessoas na árvore
        if (raiz_pessoa != NULL)
        {
            // Verifica se a pessoa nasceu na cidade especificada
            if (cep_pertence_a_cidade(cidade->cep, raiz_pessoa->cep_city_natal))
            {
                // Verifica se a pessoa não mora mais na cidade
                if (!cep_pertence_a_cidade(cidade->cep, raiz_pessoa->cep_city_atual))
                {
                    resultado += 1; // Conta a pessoa que nasceu na cidade mas não mora lá
                }
            }

            // Verifica recursivamente para as demais pessoas na árvore
            resultado += quantas_pessoas_nascidas_na_cidade_nao_moram_na_cidade(cidade, raiz_pessoa->esq);
            resultado += quantas_pessoas_nascidas_na_cidade_nao_moram_na_cidade(cidade, raiz_pessoa->dir);
        }
    }
    return resultado;
}