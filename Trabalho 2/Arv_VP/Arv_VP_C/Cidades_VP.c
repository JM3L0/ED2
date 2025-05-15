#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../Arv_VP_H/Cidades_VP.h"
#include "../Arv_VP_H/utilitarios_VP.h"
#include "../Arv_VP_H/STRUCTS_VP.h"

/*---------------------------- Funções Arv Red Black ----------------------------*/

void print_debug(CIDADES **raiz)
{
    printf("Esquerda nulo? %d \n", (*raiz)->esq == NULL);
    printf("Cor Raiz = %d \n", Cor_cidade(*raiz));
    printf("Nome = %s \n", (*raiz)->nome_cidade);
    printf("Cor Esquerda = %d \n", Cor_cidade((*raiz)->esq));
    printf("Cor Direita = %d \n", Cor_cidade((*raiz)->dir));

    //printf("\nPonteiro gambiarra = %p \n", (*raiz)->gambiarra);

    printf("Ponteiro Raiz = %p \n", (*raiz));
    printf("Ponteiro Esquerda = %p \n", (*raiz)->esq);
    printf("Ponteiro Direita = %p \n", (*raiz)->dir);
   
}

// =================================
// INSERCAO
// =================================

int inserir_no_Cidade(CIDADES **raiz, CIDADES *novaCidade)
{
    int inseriu = 0;

    printf("\n\nDentro do inserir no cidade, nova cidade antes de qualquer movimentacao\n");
    print_debug(&novaCidade);

    if (*raiz == NULL)
    {
        printf("\n\n(nova cidade) dentro do if raiz nula antes de raiz = nova cidade\n");
        print_debug(&novaCidade);

        // printf("\n\n(RAIZ) dentro do if raiz nula antes de raiz = nova cidade\n");
        // printf("RAIZ nulo? %d \n", (*raiz) == NULL);
        // print_debug(raiz);

        (*raiz) = novaCidade;//todo o mal esta aqui


        printf("\n\n(raiz) dentro do if raiz nula depois de raiz = nova cidade\n");
        print_debug(raiz);

        printf("\n\n(nova cidade) dentro do if raiz nula depois de raiz = nova cidade\n");
        print_debug(&novaCidade);

        printf("RAIZ nulo? %d \n", (*raiz) == NULL);
        //novaCidade->esq = NULL;
        printf("RAIZ nulo? %d \n", (*raiz) == NULL);

        printf("\n\n(nova cidade) dentro do if raiz nula depois de raiz = nova cidade e NC-> esq = NULL marra\n");
        print_debug(&novaCidade);
        inseriu = 1;
    }
    else if (strcasecmp(novaCidade->nome_cidade, (*raiz)->nome_cidade) < 0){
        printf("\n\nentrou no if menor antes do retorno\n");
        print_debug(raiz);
        inseriu = inserir_no_Cidade(&((*raiz)->esq), novaCidade);
        printf("\n\nentrou no if menor depois do retorno\n");
        print_debug(raiz);
    }
    else if (strcasecmp(novaCidade->nome_cidade, (*raiz)->nome_cidade) > 0)
        inseriu = inserir_no_Cidade(&((*raiz)->dir), novaCidade);

    if (inseriu){
        printf("\n\n(nova cidade) entrou no if inseriu antes do balancear\n");
        print_debug(&novaCidade);
        printf("\n\ndentro de inserir logo antes de balancear\n");
        printf("RAIZ nulo? %d \n", (*raiz) == NULL);
        print_debug(raiz);
        balancear_RB_cidade(raiz);
        printf("Saiu balancear\n");
    }
    return inseriu;
}

int inserir_Cidade(CIDADES **raiz, CIDADES *novaCidade)
{
    int inseriu;
    inseriu = inserir_no_Cidade(raiz, novaCidade);

    if (*raiz != NULL)
        (*raiz)->cor = BLACK;

    print_debug(&(*raiz));
    printf("Saiu debug\n");
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

    // printf("\n\nDentro do aloca_Cidade\n");
    // print_debug(&cidade);

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

    if (erro == 0){
        novaCidade = aloca_Cidade(nome_cidade, populacao);
        printf("\n\nDentro do cadastrarCidade\n");
        print_debug(&novaCidade);
    }
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
    printf("entrou rotacao dir\n");
    CIDADES *aux;

    aux = (*raiz)->esq;
    (*raiz)->esq = aux->dir;
    aux->dir = (*raiz);
    (*raiz) = aux;
    printf("entrou rotacao dir 2\n");
    
    (*raiz)->cor = (*raiz)->dir->cor;
    (*raiz)->dir->cor = RED;
}

// =================================
// PROPRIEDADES
// =================================

int Cor_cidade(CIDADES *cidade)
{
    printf("entrou cor\n");
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
    printf("\n\nantes de fazer qualquer coisa no balancear\n");
    print_debug(raiz);
    printf("entrou balancear\n");
    if (*raiz != NULL)
    {
        printf("\n\nantes do primeiro if de balanceamento\n");
        print_debug(raiz);
        
        if (Cor_cidade((*raiz)->esq) == BLACK && Cor_cidade((*raiz)->dir) == RED){
            printf("passou rot esq\n");
            rotacao_esquerda_cidade(raiz);
        }
       
        if ((Cor_cidade((*raiz)->esq) == RED) && Cor_cidade((*raiz)->esq->esq) == RED){
            printf("entrou rot dir\n");
            rotacao_direita_cidade(raiz);
            printf("saiu rot dir\n");
        }
        if (Cor_cidade((*raiz)->esq) == RED && Cor_cidade((*raiz)->dir) == RED){
            printf("passou trocar cor\n");
            trocar_cor_cidade(*raiz);
        }
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
            resultado = raiz;
        else if (comparacao < 0)
            resultado = buscar_cidade(raiz->esq, nome_cidade);
        else
            resultado = buscar_cidade(raiz->dir, nome_cidade);
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
        printf("Não há cidades cadastradas.\n");
    else
    {
        printf("=== Lista de Cidades ===\n");
        imprimir_cidades_em_ordem(raiz);
    }
}

// Função para limpar o nó da cidade (apenas liberar o nome)
void limpar_info_no_cidade(CIDADES *cidade)
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
void limpa_no_cidade(CIDADES **raiz)
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
void limpa_arvore_cidades(CIDADES **raiz)
{
    if (*raiz != NULL)
    {
        if ((*raiz)->esq != NULL)
            limpa_arvore_cidades(&((*raiz)->esq));

        if ((*raiz)->dir != NULL)
            limpa_arvore_cidades(&((*raiz)->dir));

        limpa_no_cidade(raiz);
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
            resultado = 1;
        else if (comparacao < 0)
            resultado = consulta_cidade(raiz->esq, nome_cidade);
        else
            resultado = consulta_cidade(raiz->dir, nome_cidade);
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
            menor = menor->esq;
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
void mover2_esquerda_cidade(CIDADES **raiz)
{
    trocar_cor_cidade(*raiz);

    if ((*raiz)->dir != NULL && Cor_cidade((*raiz)->dir->esq) == RED)
    {
        rotacao_direita_cidade(&((*raiz)->dir));
        rotacao_esquerda_cidade(raiz);
        trocar_cor_cidade(*raiz);
    }
}

// Função para mover um nó vermelho para a direita durante a remoção
void mover2_direita_cidade(CIDADES **raiz)
{
    trocar_cor_cidade(*raiz);

    if ((*raiz)->esq != NULL && Cor_cidade((*raiz)->esq->esq) == RED)
    {
        rotacao_direita_cidade(raiz);
        trocar_cor_cidade(*raiz);
    }
}

// Função para remover a menor cidade da árvore
void remover_menor_cidade_arv(CIDADES **raiz)
{
    if ((*raiz)->esq == NULL)
        limpa_no_cidade(raiz);
    else
    {
        if (Cor_cidade((*raiz)->esq) == BLACK && Cor_cidade((*raiz)->esq->esq) == BLACK)
            mover2_esquerda_cidade(raiz);

        remover_menor_cidade_arv(&((*raiz)->esq));
        balancear_RB_cidade(raiz);
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
                if (Cor_cidade((*raiz)->esq) == BLACK && Cor_cidade((*raiz)->esq->esq) == BLACK)
                    mover2_esquerda_cidade(raiz);
            }

            removeu = remover_cidade_no(&((*raiz)->esq), nome_cidade);
        }
        else
        {
            if (Cor_cidade((*raiz)->esq) == RED)
                rotacao_direita_cidade(raiz);

            if (resultado == 0 && (*raiz)->dir == NULL)
            {
                limpa_no_cidade(raiz);
            }
            else
            {
                if ((*raiz)->dir != NULL)
                {
                    if (Cor_cidade((*raiz)->dir) == BLACK && Cor_cidade((*raiz)->dir->esq) == BLACK)
                        mover2_direita_cidade(raiz);
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
            balancear_RB_cidade(raiz);
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
        removeu = remover_cidade_no(raiz, nome_cidade);

    if (*raiz != NULL)
        (*raiz)->cor = BLACK;

    return removeu;
}

// =================================
// ESPECÍFICAS DO TRABALHO
// =================================

CIDADES *verifica_cidade_mais_populosa_nao_capital(CIDADES *raiz, char *nome_capital)
{
    CIDADES *mais_populosa = NULL;

    if (raiz)
    {
        mais_populosa = verifica_cidade_mais_populosa_nao_capital(raiz->esq, nome_capital);
        CIDADES *dir = verifica_cidade_mais_populosa_nao_capital(raiz->dir, nome_capital);

        if (strcasecmp(nome_capital, raiz->nome_cidade) != 0 && (!mais_populosa || raiz->populacao_city > mais_populosa->populacao_city))
            mais_populosa = raiz;

        if (dir != NULL && (mais_populosa == NULL || dir->populacao_city > mais_populosa->populacao_city))
            mais_populosa = dir;
    }

    return mais_populosa;
}