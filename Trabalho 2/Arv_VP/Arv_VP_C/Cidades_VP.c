#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../Arv_VP_H/Cidades_VP.h"
#include "../Arv_VP_H/utilitarios_VP.h"
#include "../Arv_VP_H/STRUCTS_VP.h"

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
    {
        inseriu = inserir_no_Cidade(&((*raiz)->esq), novaCidade);
    }
    else if (strcasecmp(novaCidade->nome_cidade, (*raiz)->nome_cidade) > 0)
    {
        inseriu = inserir_no_Cidade(&((*raiz)->dir), novaCidade);
    }

    if (inseriu)
    {
        balancear_RB(raiz);
    }

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

CIDADES *alocaCidade()
{
    CIDADES *cidade;
    cidade = (CIDADES *)malloc(sizeof(CIDADES));
    if (cidade == NULL)
    {
        printf("Erro ao alocar memoria para a cidade.\n");
        exit(EXIT_FAILURE);
    }
    return cidade;
}

CIDADES *criaCidade(char *nome_cidade, int populacao_city)
{
    CIDADES *cidade;
    cidade = alocaCidade();

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
    {
        erro = 1;
    }

    if (erro == 0)
    {
        printf("Digite a populacao da cidade: ");
        populacao = digitar_int();
    }

    if (erro == 0)
    {
        novaCidade = criaCidade(nome_cidade, populacao);
    }
    else
    {
        if (nome_cidade != NULL)
        {
            free(nome_cidade);
        }
    }

    return novaCidade;
}

// =================================
// ROTAÇÃO
// =================================

void rotacao_esquerda(CIDADES **raiz)
{
    CIDADES *aux;

    aux = (*raiz)->dir;
    (*raiz)->dir = aux->esq;
    aux->esq = (*raiz);
    (*raiz) = aux;

    (*raiz)->cor = (*raiz)->esq->cor;
    (*raiz)->esq->cor = RED;
}

void rotacao_direita(CIDADES **raiz)
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

int Cor(CIDADES *cidade)
{
    return cidade == NULL ? BLACK : cidade->cor;
}

void trocar_cor(CIDADES *raiz)
{
    raiz->cor = !(raiz->cor);

    if (raiz->esq != NULL)
        raiz->esq->cor = !(raiz->esq->cor);

    if (raiz->dir != NULL)
        raiz->dir->cor = !(raiz->dir->cor);
}

void balancear_RB(CIDADES **raiz)
{
    if (*raiz != NULL)
    {
        if (Cor((*raiz)->esq) == BLACK && Cor((*raiz)->dir) == RED)
            rotacao_esquerda(raiz);

        if (Cor((*raiz)->esq) == RED && Cor((*raiz)->esq->esq) == RED)
            rotacao_direita(raiz);

        if (Cor((*raiz)->esq) == RED && Cor((*raiz)->dir) == RED)
            trocar_cor(*raiz);
    }
}

// =================================
// BUSCA E IMPRESSÃO
// =================================

// Função para buscar uma cidade na árvore pelo nome
CIDADES *buscar_cidade(CIDADES *raiz, char *nome_cidade)
{
    CIDADES *resultado = NULL;

    if (raiz != NULL)
    {
        int comparacao = strcasecmp(nome_cidade, raiz->nome_cidade);

        if (comparacao == 0)
        {
            resultado = raiz;
        }
        else if (comparacao < 0)
        {
            resultado = buscar_cidade(raiz->esq, nome_cidade);
        }
        else
        {
            resultado = buscar_cidade(raiz->dir, nome_cidade);
        }
    }

    return resultado;
}

// Função auxiliar para imprimir informações de uma cidade
void imprimir_cidade(CIDADES *cidade)
{
    if (cidade != NULL)
    {
        printf("Cidade: %s\n", cidade->nome_cidade);
        printf("População: %d\n", cidade->populacao_city);
        printf("Cor: %s\n", cidade->cor == RED ? "Vermelho" : "Preto");
        // if(cidade->cep != NULL) {
        //     printf("CEP: %s\n", cidade->cep); // pode imprimir a arvore de CEP (pensar nisso posteriormente)
        // }
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
    {
        printf("Não há cidades cadastradas.\n");
    }
    else
    {
        printf("=== Lista de Cidades ===\n");
        imprimir_cidades_em_ordem(raiz);
    }
}

// Função para limpar o nó da cidade (apenas liberar o nome)
void limpar_no_cidade(CIDADES *cidade)
{
    if (cidade != NULL && cidade->nome_cidade != NULL)
    {
        free(cidade->nome_cidade);
        cidade->nome_cidade = NULL;
    }
}

// =================================
// DESALOCAÇÃO
// =================================

// Função para desalocar completamente uma cidade
void desalocar_cidade(CIDADES **raiz)
{
    if (*raiz != NULL)
    {
        if ((*raiz)->nome_cidade != NULL)
        {
            free((*raiz)->nome_cidade);
            (*raiz)->nome_cidade = NULL;
        }

        if ((*raiz)->cep != NULL)
        {
            free((*raiz)->cep);
            (*raiz)->cep = NULL;
        }

        free(*raiz);
        *raiz = NULL;
    }
}

// Função para desalocar toda a árvore de cidades
void desalocar_arvore_cidades(CIDADES **raiz)
{
    if (*raiz != NULL)
    {
        if ((*raiz)->esq != NULL)
            desalocar_arvore_cidades(&((*raiz)->esq));

        if ((*raiz)->dir != NULL)
            desalocar_arvore_cidades(&((*raiz)->dir));

        desalocar_cidade(raiz);
    }
}

// =================================
// CONSULTA ÁRVORE
// =================================

int consulta_cidade(CIDADES *raiz, char *nome_cidade)
{
    int resultado = 0;

    if (raiz != NULL)
    {
        int comparacao = strcasecmp(nome_cidade, raiz->nome_cidade);

        if (comparacao == 0)
        {
            resultado = 1;
        }
        else if (comparacao < 0)
        {
            resultado = consulta_cidade(raiz->esq, nome_cidade);
        }
        else
        {
            resultado = consulta_cidade(raiz->dir, nome_cidade);
        }
    }

    return resultado;
}

// =================================
// REMOÇÃO
// =================================

// Funções auxiliares para remoção
CIDADES *encontrar_menor_cidade(CIDADES *raiz)
{
    CIDADES *menor = raiz;

    if (raiz != NULL)
    {
        while (menor->esq != NULL)
        {
            menor = menor->esq;
        }
    }

    return menor;
}

void trocar_informacoes_cidades(CIDADES *cidade1, CIDADES *cidade2)
{
    if (cidade1 != NULL && cidade2 != NULL)
    {
        // Troca os nomes
        char *temp_nome = cidade1->nome_cidade;
        cidade1->nome_cidade = cidade2->nome_cidade;
        cidade2->nome_cidade = temp_nome;

        // Troca as populações
        int temp_pop = cidade1->populacao_city;
        cidade1->populacao_city = cidade2->populacao_city;
        cidade2->populacao_city = temp_pop;

        // Troca os CEPs
        CEP *temp_cep = cidade1->cep;
        cidade1->cep = cidade2->cep;
        cidade2->cep = temp_cep;
    }
}

// Função para mover um nó vermelho para a esquerda durante a remoção
void mover2_esquerda(CIDADES **raiz)
{
    trocar_cor(*raiz);

    if ((*raiz)->dir != NULL && cor((*raiz)->dir->esq) == RED)
    {
        rotacao_direita(&((*raiz)->dir));
        rotacao_esquerda(raiz);
        trocar_cor(*raiz);
    }
}

// Função para mover um nó vermelho para a direita durante a remoção
void mover2_direita(CIDADES **raiz)
{
    trocar_cor(*raiz);

    if ((*raiz)->esq != NULL && Cor((*raiz)->esq->esq) == RED)
    {
        rotacao_direita(raiz);
        trocar_cor(*raiz);
    }
}

// Função para remover a menor cidade da árvore
void remover_menor_cidade_arv(CIDADES **raiz)
{
    if ((*raiz)->esq == NULL)
    {
        desalocar_cidade(raiz);
    }
    else
    {
        if (cor((*raiz)->esq) == BLACK && cor((*raiz)->esq->esq) == BLACK)
            mover2_esquerda(raiz);

        remover_menor_cidade_arv(&((*raiz)->esq));
        balancear_RB(raiz);
    }
}

// Função para remover uma cidade da árvore
int remover_cidade_no(CIDADES **raiz, char *nome_cidade)
{
    int removeu = 1;

    if ((*raiz) != NULL)
    {
        int resultado = strcasecmp(nome_cidade, (*raiz)->nome_cidade);

        if (resultado < 0)
        {
            if ((*raiz)->esq != NULL)
            {
                if (Cor((*raiz)->esq) == BLACK && Cor((*raiz)->esq->esq) == BLACK)
                    mover2_esquerda(raiz);
            }

            removeu = remover_cidade_no(&((*raiz)->esq), nome_cidade);
        }
        else
        {
            if (Cor((*raiz)->esq) == RED)
                rotacao_direita(raiz);

            if (resultado == 0 && (*raiz)->dir == NULL)
            {
                desalocar_cidade(raiz);
            }
            else
            {
                if ((*raiz)->dir != NULL)
                {
                    if (Cor((*raiz)->dir) == BLACK && Cor((*raiz)->dir->esq) == BLACK)
                        mover2_direita(raiz);
                }

                if (resultado == 0)
                {
                    CIDADES *menor = encontrar_menor_cidade((*raiz)->dir);
                    trocar_informacoes_cidades(*raiz, menor);
                    // Remove o menor (que agora contém os dados do nó que queríamos remover)
                    remover_menor_cidade_arv(&((*raiz)->dir));
                }
                else
                    removeu = remover_cidade_no(&((*raiz)->dir), nome_cidade);
            }
        }

        // Rebalanceia a árvore se o nó atual ainda existe
        if (*raiz != NULL)
        {
            balancear_RB(raiz);
        }
    }
    else
        removeu = 0;

    return removeu;
}

// Função principal para remoção - mantém a raiz preta
int remover_cidade_arvore(CIDADES **raiz, char *nome_cidade)
{
    int removeu = consulta_cidade(*raiz, nome_cidade);

    if (removeu)
    {
        removeu = remover_cidade_no(raiz, nome_cidade);
    }

    if (*raiz != NULL)
        (*raiz)->cor = BLACK;

    return removeu;
}

// =================================
// ESPECÍFICAS DO TRABALHO
// =================================

CIDADES *verifica_cidade_mais_populosa_nao_capital(CIDADES *raiz, char *nome_capital){
    CIDADES *cidade_mais_populosa;
    cidade_mais_populosa = NULL;
    int maior_populacao = -1;
    int comp = strcasecmp(nome_capital, raiz->nome_cidade);

    if(raiz != NULL){
        if(raiz->populacao_city > maior_populacao && comp != 0){
            maior_populacao = raiz->populacao_city;
            cidade_mais_populosa = raiz;
        }
        cidade_mais_populosa = verifica_cidade_mais_populosa_nao_capital(raiz->esq, nome_capital);
        cidade_mais_populosa = verifica_cidade_mais_populosa_nao_capital(raiz->dir, nome_capital);
    }
    return (cidade_mais_populosa);
}