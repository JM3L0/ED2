#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../Arv_VP_H/utilitarios_VP.h"
#include "../Arv_VP_H/STRUCTS_VP.h"
#include "../Arv_VP_H/Pessoas_VP.h"

/*---------------------------- Funções Arv Red Black ----------------------------*/

// =================================
// INSERCAO
// =================================

int inserir_no_pessoa(PESSOAS **raiz, PESSOAS *novapessoa)
{
    int inseriu = 0;

    if (*raiz == NULL)
    {
        *raiz = novapessoa;
        inseriu = 1;
    }
    else if (strcasecmp(novapessoa->CPF, (*raiz)->CPF) < 0)
        inseriu = inserir_no_pessoa(&((*raiz)->esq), novapessoa);
    else if (strcasecmp(novapessoa->CPF, (*raiz)->CPF) > 0)
        inseriu = inserir_no_pessoa(&((*raiz)->dir), novapessoa);

    if (inseriu)
        balancear_RB(raiz);
    return inseriu;
}

int inserir_pessoa(PESSOAS **raiz, PESSOAS *novapessoa)
{
    int inseriu;
    inseriu = inserir_no_pessoa(raiz, novapessoa);

    if (*raiz != NULL)
        (*raiz)->cor = BLACK;

    return inseriu;
}

PESSOAS *aloca_pessoa(char *nome_pessoa, char *CPF, char *cep_natal, char *cep_atual, DATA data_nasc)
{
    PESSOAS *pessoa;
    pessoa = (PESSOAS *)malloc(sizeof(PESSOAS));

    pessoa->nome_pessoa = nome_pessoa;
    strcpy(pessoa->CPF, CPF);
    strcpy(pessoa->cep_city_natal, cep_natal);
    strcpy(pessoa->cep_city_atual, cep_atual);
    pessoa->data_nasc = data_nasc;

    pessoa->cor = RED;
    pessoa->esq = NULL;
    pessoa->dir = NULL;

    return pessoa;
}

PESSOAS *cadastra_pessoa(char *cep_natal, char *cep_atual)
{
    PESSOAS *pessoa = NULL;
    DATA data_nasc;
    char CPF[12];
    char *nome_pessoa = NULL;
    int retorno = 0;

    printf("Digite o nome da pessoa: ");
    nome_pessoa = ler_string();

    if (nome_pessoa != NULL)
    {
        retorno = capturar_cpf(CPF);

        if (retorno)
        {
            // Alocação de memória para a data de nascimento

            retorno = capturar_data(&data_nasc);

            pessoa = aloca_pessoa(nome_pessoa, CPF, cep_natal, cep_atual, data_nasc);
        }
    }
    // Limpeza em caso de falha
    if (!retorno)
    {
        if (nome_pessoa != NULL)
            free(nome_pessoa);
    }

    return pessoa;
}

// =================================
// ROTAÇÃO
// =================================

void rotacao_esquerda(PESSOAS **raiz)
{
    PESSOAS *aux;

    aux = (*raiz)->dir;
    (*raiz)->dir = aux->esq;
    aux->esq = (*raiz);
    (*raiz) = aux;

    (*raiz)->cor = (*raiz)->esq->cor;
    (*raiz)->esq->cor = RED;
}

void rotacao_direita(PESSOAS **raiz)
{
    PESSOAS *aux;

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

static int Cor(PESSOAS *pessoa)
{
    return pessoa == NULL ? BLACK : pessoa->cor;
}

static void trocar_cor(PESSOAS *raiz)
{
    raiz->cor = !(raiz->cor);

    if (raiz->esq != NULL)
        raiz->esq->cor = !(raiz->esq->cor);

    if (raiz->dir != NULL)
        raiz->dir->cor = !(raiz->dir->cor);
}

static void balancear_RB(PESSOAS **raiz)
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

// Função para buscar uma pessoa na árvore pelo nome
PESSOAS *buscar_pessoa(PESSOAS *raiz, char *CPF)
{
    PESSOAS *resultado = NULL;

    if (raiz != NULL)
    {
        int comparacao = strcasecmp(CPF, raiz->CPF);

        if (comparacao == 0)
            resultado = raiz;
        else if (comparacao < 0)
            resultado = buscar_pessoa(raiz->esq, CPF);
        else
            resultado = buscar_pessoa(raiz->dir, CPF);
    }

    return resultado;
}

// Função auxiliar para imprimir informações de uma pessoa
void imprimir_pessoa(PESSOAS *pessoa)
{
    if (pessoa != NULL)
    {

        printf("NOME: %s\n", pessoa->nome_pessoa);
        printf("CPF: %s\n", pessoa->CPF);
        printf("CEP Natal: %s\n", pessoa->cep_city_natal);
        printf("CEP Atual: %s\n", pessoa->cep_city_atual);
        imprimir_data(pessoa->data_nasc);
        printf("Cor: %s\n", pessoa->cor == RED ? "Vermelho" : "Preto");

        printf("------------------------\n");
    }
}

// Função para imprimir todas as pessoas em ordem alfabética
void imprimir_pessoas_em_ordem(PESSOAS *raiz)
{
    if (raiz != NULL)
    {
        imprimir_pessoas_em_ordem(raiz->esq);
        imprimir_pessoa(raiz);
        imprimir_pessoas_em_ordem(raiz->dir);
    }
}

// Função para imprimir todas as pessoas
void imprimir_todas_pessoas(PESSOAS *raiz)
{
    if (raiz == NULL)
        printf("Nao ha pessoas cadastradas.\n");
    else
    {
        printf("=== Lista de pessoas ===\n");
        imprimir_pessoas_em_ordem(raiz);
    }
}

// =================================
// DESALOCAÇÃO
// =================================

// Função para desalocar completamente uma pessoa
void liberar_no_pessoa(PESSOAS **raiz)
{
    if (*raiz != NULL)
    {
        if ((*raiz)->nome_pessoa != NULL)
        {
            free((*raiz)->nome_pessoa);
            (*raiz)->nome_pessoa = NULL;
        }

        free(*raiz);
        *raiz = NULL;
    }
}

// Função para desalocar toda a árvore de pessoas
void liberar_arvore_pessoas(PESSOAS **raiz)
{
    if (*raiz != NULL)
    {
        if ((*raiz)->esq != NULL)
            liberar_arvore_pessoas(&((*raiz)->esq));

        if ((*raiz)->dir != NULL)
            liberar_arvore_pessoas(&((*raiz)->dir));

        liberar_no_pessoa(raiz);
    }
}

// =================================
// CONSULTA ÁRVORE
// =================================

int consulta_pessoa(PESSOAS *raiz, char *CPF)
{
    int resultado = 0;

    if (raiz != NULL)
    {
        int comparacao = strcasecmp(CPF, raiz->CPF);

        if (comparacao == 0)
            resultado = 1;
        else if (comparacao < 0)
            resultado = consulta_pessoa(raiz->esq, CPF);
        else
            resultado = consulta_pessoa(raiz->dir, CPF);
    }
    return resultado;
}

// =================================
// REMOÇÃO
// =================================

// Funções auxiliares para remoção
PESSOAS *encontrar_menor_pessoa(PESSOAS *raiz)
{
    PESSOAS *menor = raiz;

    if (raiz != NULL)
    {
        while (menor->esq != NULL)
            menor = menor->esq;
    }

    return menor;
}

void trocar_informacoes_pessoas(PESSOAS *pessoa1, PESSOAS *pessoa2)
{
    if (pessoa1 != NULL && pessoa2 != NULL)
    {
        // Troca os nomes
        char *temp_nome = pessoa1->nome_pessoa;
        pessoa1->nome_pessoa = pessoa2->nome_pessoa;
        pessoa2->nome_pessoa = temp_nome;

        // Troca os CPFs
        strcpy(pessoa1->CPF, pessoa2->CPF);

        // Troca os CEPs natais
        strcpy(pessoa1->cep_city_natal, pessoa2->cep_city_natal);

        // Troca os CEPs atuais
        strcpy(pessoa1->cep_city_atual, pessoa2->cep_city_atual);

        // Troca as datas de nascimento
        pessoa1->data_nasc = pessoa2->data_nasc;
    }
}

// Função para mover um nó vermelho para a esquerda durante a remoção
void mover2_esquerda(PESSOAS **raiz)
{
    trocar_cor(*raiz);

    if ((*raiz)->dir != NULL && Cor((*raiz)->dir->esq) == RED)
    {
        rotacao_direita(&((*raiz)->dir));
        rotacao_esquerda(raiz);
        trocar_cor(*raiz);
    }
}

// Função para mover um nó vermelho para a direita durante a remoção
void mover2_direita(PESSOAS **raiz)
{
    trocar_cor(*raiz);

    if ((*raiz)->esq != NULL && Cor((*raiz)->esq->esq) == RED)
    {
        rotacao_direita(raiz);
        trocar_cor(*raiz);
    }
}

// Função para remover a menor pessoa da árvore
void remover_menor_pessoa_arv(PESSOAS **raiz)
{
    if ((*raiz)->esq == NULL)
        liberar_no_pessoa(raiz);
    else
    {
        if (Cor((*raiz)->esq) == BLACK && Cor((*raiz)->esq->esq) == BLACK)
            mover2_esquerda(raiz);

        remover_menor_pessoa_arv(&((*raiz)->esq));
        balancear_RB(raiz);
    }
}

// Função para remover uma pessoa da árvore
int remover_pessoa_no(PESSOAS **raiz, char *CPF)
{
    int removeu = 1;

    if ((*raiz) != NULL)
    {
        int resultado = strcasecmp(CPF, (*raiz)->CPF);

        if (resultado < 0)
        {
            if ((*raiz)->esq != NULL)
            {
                if (Cor((*raiz)->esq) == BLACK && Cor((*raiz)->esq->esq) == BLACK)
                    mover2_esquerda(raiz);
            }

            removeu = remover_pessoa_no(&((*raiz)->esq), CPF);
        }
        else
        {
            if (Cor((*raiz)->esq) == RED)
                rotacao_direita(raiz);

            if (resultado == 0 && (*raiz)->dir == NULL)
            {
                liberar_no_pessoa(raiz);
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
                    PESSOAS *menor = encontrar_menor_pessoa((*raiz)->dir);
                    trocar_informacoes_pessoas(*raiz, menor);
                    // Remove o menor (que agora contém os dados do nó que queríamos remover)
                    remover_menor_pessoa_arv(&((*raiz)->dir));
                }
                else
                    removeu = remover_pessoa_no(&((*raiz)->dir), CPF);
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
int remover_pessoa_arvore(PESSOAS **raiz, char *CPF)
{
    int removeu = consulta_pessoa(*raiz, CPF);

    if (removeu)
    {
        removeu = remover_pessoa_no(raiz, CPF);
    }

    if (*raiz != NULL)
        (*raiz)->cor = BLACK;

    return removeu;
}

// =================================
// ESPECÍFICAS DO TRABALHO
// =================================