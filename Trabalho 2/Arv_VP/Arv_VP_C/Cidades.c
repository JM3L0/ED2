#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../Arv_VP_H/Cidades.h"
#include "../Arv_VP_H/utilitarios.h"
#include "../Arv_VP_H/STRUCTS.h"


/*---------------------------- Funções Arv Red Black ----------------------------*/

CIDADES *alocaCidade(){
    CIDADES *cidade;
    cidade = (CIDADES *)malloc(sizeof(CIDADES));
    if (cidade == NULL) {
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

CIDADES *cadastrarCidade(){
    CIDADES *novaCidade;
    novaCidade = NULL;
    char *nome_cidade;
    int populacao, erro = 0;

    printf("Digite o nome da cidade: "); 
    nome_cidade = ler_string();

    if (nome_cidade == NULL){
        erro = 1;
    }

    if (erro == 0) {
        printf("Digite a populacao da cidade: ");
        populacao = digitar_int();
    }

    if (erro == 0) {
        novaCidade = criaCidade(nome_cidade, populacao);
    } else{
        if (nome_cidade != NULL) {
            free(nome_cidade);
        }
    }
    
    return novaCidade;
}

int corCidade(CIDADES *cidade){
    return cidade == NULL ? BLACK : cidade->cor;
}

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

void trocar_cor(CIDADES *raiz)
{
    raiz->cor = !(raiz->cor);

    if(raiz->esq != NULL)
        raiz->esq->cor = !(raiz->esq->cor);

    if(raiz->dir != NULL)
        raiz->dir->cor =!(raiz->dir->cor);
}

void balancear_RB(CIDADES **raiz)
{
    if(*raiz != NULL)
    {
        if(retornar_cor((*raiz)->esq) == BLACK && retornar_cor((*raiz)->dir) == RED)
            rotacao_esquerda(raiz);

        if(retornar_cor((*raiz)->esq) == RED && retornar_cor((*raiz)->esq->esq) == RED)
            rotacao_direita(raiz);

        if(retornar_cor((*raiz)->esq) == RED && retornar_cor((*raiz)->dir) == RED)
            trocar_cor(*raiz);
    }
}

CIDADES *inserir_no_Cidade(CIDADES **raiz, CIDADES *novaCidade)
{
    if(*raiz == NULL)
    {
        *raiz = novaCidade;
    }
    else if(strcasecmp(novaCidade->nome_cidade, (*raiz)->nome_cidade) < 0)
    {
        inserir_no_Cidade(&((*raiz)->esq), novaCidade);
    }
    else if(strcasecmp(novaCidade->nome_cidade, (*raiz)->nome_cidade) > 0)
    {
        inserir_no_Cidade(&((*raiz)->dir), novaCidade);
    }

    balancear_RB(raiz);

    return *raiz;
}

CIDADES *inserir_Cidade(CIDADES **raiz, CIDADES *novaCidade)
{
    CIDADES *inseriu;
    inseriu = inserir_no_Cidade(raiz, novaCidade);

    if(*raiz != NULL)
        (*raiz)->cor = BLACK;

    return inseriu;
}

/*------------------------------- Tem que conferir a partir daqui ------------------------------------*/

// Função para buscar uma cidade na árvore pelo nome
CIDADES *buscar_cidade(CIDADES *raiz, char *nome_cidade) {
    CIDADES *resultado = NULL;
    
    if(raiz != NULL) {
        int comparacao = strcasecmp(nome_cidade, raiz->nome_cidade);
        
        if(comparacao == 0) {
            resultado = raiz;
        } else if(comparacao < 0) {
            resultado = buscar_cidade(raiz->esq, nome_cidade);
        } else {
            resultado = buscar_cidade(raiz->dir, nome_cidade);
        }
    }
    
    return resultado;
}

// Função auxiliar para imprimir informações de uma cidade
void imprimir_cidade(CIDADES *cidade) {
    if(cidade != NULL) {
        printf("Cidade: %s\n", cidade->nome_cidade);
        printf("População: %d\n", cidade->populacao_city);
        printf("Cor: %s\n", cidade->cor == RED ? "Vermelho" : "Preto");
        if(cidade->cep != NULL) {
            printf("CEP: %s\n", cidade->cep);
        }
        printf("------------------------\n");
    }
}

// Função para imprimir todas as cidades em ordem alfabética
void imprimir_cidades_em_ordem(CIDADES *raiz) {
    if(raiz != NULL) {
        imprimir_cidades_em_ordem(raiz->esq);
        imprimir_cidade(raiz);
        imprimir_cidades_em_ordem(raiz->dir);
    }
}

// Função para imprimir todas as cidades
void imprimir_todas_cidades(CIDADES *raiz) {
    if(raiz == NULL) {
        printf("Não há cidades cadastradas.\n");
    } else {
        printf("=== Lista de Cidades ===\n");
        imprimir_cidades_em_ordem(raiz);
    }
}

// Funções auxiliares para remoção
CIDADES *encontrar_menor_cidade(CIDADES *raiz) {
    CIDADES *menor = raiz;
    
    if(raiz != NULL) {
        while(menor->esq != NULL) {
            menor = menor->esq;
        }
    }
    
    return menor;
}

// Função para limpar o nó da cidade (apenas liberar o nome)
void limpar_no_cidade(CIDADES *cidade) {
    if (cidade != NULL && cidade->nome_cidade != NULL) {
        free(cidade->nome_cidade);
        cidade->nome_cidade = NULL;
    }
}

// Função para desalocar completamente uma cidade
void desalocar_cidade(CIDADES **raiz)
{
    if(*raiz != NULL)
    {
        if((*raiz)->nome_cidade != NULL) {
            free((*raiz)->nome_cidade);
            (*raiz)->nome_cidade = NULL;
        }
        
        if((*raiz)->cep != NULL) {
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

// Função para mover um nó vermelho para a esquerda durante a remoção
void mover_esquerda(CIDADES **raiz)
{
    trocar_cor(*raiz);

    if((*raiz)->dir != NULL && corCidade((*raiz)->dir->esq) == RED)
    {
        rotacao_direita(&((*raiz)->dir));
        rotacao_esquerda(raiz);
        trocar_cor(*raiz);
    }
}

// Função para mover um nó vermelho para a direita durante a remoção
void mover_direita(CIDADES **raiz)
{
    trocar_cor(*raiz);

    if((*raiz)->esq != NULL && corCidade((*raiz)->esq->esq) == RED)
    {
        rotacao_direita(raiz);
        trocar_cor(*raiz);
    }
}

// Função para remover a menor cidade da árvore
void remover_menor_cidade_arv(CIDADES **raiz)
{
    if((*raiz)->esq == NULL) {
        desalocar_cidade(raiz);
    }
    else
    {
        if(corCidade((*raiz)->esq) == BLACK && corCidade((*raiz)->esq->esq) == BLACK)
            mover_esquerda(raiz);

        remover_menor_cidade_arv(&((*raiz)->esq));
        balancear_RB(raiz);
    }
}

// Função para remover uma cidade da árvore
int remover_cidade_no(CIDADES **raiz, char *nome_cidade)
{
    int removeu = 1;

    if((*raiz) != NULL)
    {
        int resultado = strcasecmp(nome_cidade, (*raiz)->nome_cidade);

        if(resultado < 0)
        {
            if((*raiz)->esq != NULL)
            {
                if(corCidade((*raiz)->esq) == BLACK && corCidade((*raiz)->esq->esq) == BLACK)
                    mover_esquerda(raiz);
            }
                
            removeu = remover_cidade_no(&((*raiz)->esq), nome_cidade);
        }
        else {
            if(corCidade((*raiz)->esq) == RED)
                rotacao_direita(raiz);

            if(resultado == 0 && (*raiz)->dir == NULL) {
                desalocar_cidade(raiz);
            }
            else
            {
                if((*raiz)->dir != NULL)
                {
                    if(corCidade((*raiz)->dir) == BLACK && corCidade((*raiz)->dir->esq) == BLACK)
                        mover_direita(raiz);
                }

                if(resultado == 0)
                {
                    CIDADES *menor = encontrar_menor_cidade((*raiz)->dir);
                    
                    // Troca completa de informações entre os nós
                    char *temp_nome = (*raiz)->nome_cidade;
                    (*raiz)->nome_cidade = menor->nome_cidade;
                    menor->nome_cidade = temp_nome;
                    
                    int temp_pop = (*raiz)->populacao_city;
                    (*raiz)->populacao_city = menor->populacao_city;
                    menor->populacao_city = temp_pop;
                    
                    char *temp_cep = (*raiz)->cep;
                    (*raiz)->cep = menor->cep;
                    menor->cep = temp_cep;
                    
                    // Remove o menor (que agora contém os dados do nó que queríamos remover)
                    remover_menor_cidade_arv(&((*raiz)->dir));
                }
                else
                    removeu = remover_cidade_no(&((*raiz)->dir), nome_cidade);
            }
        }
        
        // Rebalanceia a árvore se o nó atual ainda existe
        if(*raiz != NULL) {
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
    int removeu = remover_cidade_no(raiz, nome_cidade);
    
    if(*raiz != NULL)
        (*raiz)->cor = BLACK;

    return removeu;
}

