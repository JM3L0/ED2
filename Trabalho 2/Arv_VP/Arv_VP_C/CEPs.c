#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../Arv_VP_H/utilitarios.h"
#include "../Arv_VP_H/STRUCTS.h"
#include "../Arv_VP_H/CEPs.h"


/*---------------------------- Funções Arv Red Black ----------------------------*/

// =================================
// INSERCAO
// =================================

int inserir_no_CEP(CEP **raiz, CEP *novoCEP)
{
    int inseriu = 0;

    if(*raiz == NULL)
    {
        *raiz = novoCEP;
        inseriu = 1;
    }
    else if(strcasecmp(novoCEP->cep, (*raiz)->cep) < 0)
    {
        inseriu = inserir_no_CEP(&((*raiz)->esq), novoCEP);
    }
    else if(strcasecmp(novoCEP->cep, (*raiz)->cep) > 0)
    {
        inseriu = inserir_no_CEP(&((*raiz)->dir), novoCEP);
    }

    if (inseriu) {
        balancear_RB(raiz);
    }

    return inseriu;
}

int inserir_CEP(CEP **raiz, CEP *novoCEP)
{
    int inseriu;
    inseriu = inserir_no_CEP(raiz, novoCEP);

    if(*raiz != NULL)
        (*raiz)->cor = BLACK;

    return inseriu;
}

// =================================
// CRIACAO
// =================================

CEP *alocaCEP(){
    CEP *cep;
    cep = (CEP *)malloc(sizeof(CEP));
    if (cep == NULL) {
        printf("Erro ao alocar memoria para a cep.\n");
        exit(EXIT_FAILURE);
    }
    return cep;
}

CEP *criaCEP(char *str_cep)
{
    CEP *no_cep;
    no_cep = alocaCEP();

    strcpy(no_cep->cep, str_cep);
    no_cep->cor = RED;
    no_cep->esq = NULL; 
    no_cep->dir = NULL;
 
    return no_cep;
}

CEP *cadastrarCEP(){

    CEP *novoCEP;
    novoCEP = NULL;
    char str_cep[10];
    int erro = 0;

    printf("Digite o CEP: ");
    erro = capturar_cep(str_cep);

    if (erro == 0) {
        novoCEP = criaCEP(str_cep);
    }
    
    return novoCEP;
}

// =================================
// ROTAÇÃO
// =================================

void rotacao_esquerda(CEP **raiz)
{
    CEP *aux;

    aux = (*raiz)->dir;
    (*raiz)->dir = aux->esq;
    aux->esq = (*raiz);
    (*raiz) = aux;

    (*raiz)->cor = (*raiz)->esq->cor;
    (*raiz)->esq->cor = RED;
    
}

void rotacao_direita(CEP **raiz)
{
    CEP *aux;

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

int cor(CEP *cep){
    return cep == NULL ? BLACK : cep->cor;
}

void trocar_cor(CEP *raiz)
{
    raiz->cor = !(raiz->cor);

    if(raiz->esq != NULL)
        raiz->esq->cor = !(raiz->esq->cor);

    if(raiz->dir != NULL)
        raiz->dir->cor =!(raiz->dir->cor);
}

void balancear_RB(CEP **raiz)
{
    if(*raiz != NULL)
    {
        if(cor((*raiz)->esq) == BLACK && cor((*raiz)->dir) == RED)
            rotacao_esquerda(raiz);

        if(cor((*raiz)->esq) == RED && cor((*raiz)->esq->esq) == RED)
            rotacao_direita(raiz);

        if(cor((*raiz)->esq) == RED && cor((*raiz)->dir) == RED)
            trocar_cor(*raiz);
    }
}


// =================================
// BUSCA E IMPRESSÃO
// =================================

// Função para buscar uma cep na árvore pelo nome
CEP *buscar_CEP(CEP *raiz, char *str_cep) {
    CEP *resultado = NULL;
    
    if(raiz != NULL) {
        int comparacao = strcasecmp(str_cep, raiz->cep);
        
        if(comparacao == 0) {
            resultado = raiz;
        } else if(comparacao < 0) {
            resultado = buscar_CEP(raiz->esq, str_cep);
        } else {
            resultado = buscar_CEP(raiz->dir, str_cep);
        }
    }
    
    return resultado;
}

// Função auxiliar para imprimir informações de uma cep
void imprimir_CEP(CEP *Cep) {
    if(Cep != NULL) {
        printf("Cidade: %s\n", Cep->cep);
        printf("Cor: %s\n", Cep->cor == RED ? "Vermelho" : "Preto");
        printf("------------------------\n");
    }
}

// Função para imprimir todas as cidades em ordem alfabética
void imprimir_CEP_em_ordem(CEP *raiz) {
    if(raiz != NULL) {
        imprimir_CEP_em_ordem(raiz->esq);
        imprimir_CEP(raiz);
        imprimir_CEP_em_ordem(raiz->dir);
    }
}

// Função para imprimir todas as cidades
void imprimir_todos_CEP(CEP *raiz) {
    if(raiz == NULL) {
        printf("Não há CEPs cadastrados.\n");
    } else {
        printf("=== Lista de CEPs ===\n");
        imprimir_CEP_em_ordem(raiz);
    }
}


// =================================
// DESALOCAÇÃO
// =================================

// Função para desalocar um cep
void desalocar_CEP(CEP **raiz)
{
    if(*raiz != NULL) {
        free(*raiz);
        *raiz = NULL;
    }
}

// Função para desalocar toda a árvore de CEPs
void desalocar_arvore_CEP(CEP **raiz)
{
    if (*raiz != NULL)
    {
        if ((*raiz)->esq != NULL)
            desalocar_arvore_CEP(&((*raiz)->esq));

        if ((*raiz)->dir != NULL)
            desalocar_arvore_CEP(&((*raiz)->dir));

        desalocar_CEP(raiz);
    }
}

// =================================
// CONSULTA ÁRVORE
// =================================

int consulta_CEP(CEP *raiz, char *str_cep) {
    int resultado = 0;
    
    if (raiz != NULL) {
        int comparacao = strcasecmp(str_cep, raiz->cep);

        if (comparacao == 0) {
            resultado = 1;
        } else if (comparacao < 0) {
            resultado = consulta_CEP(raiz->esq, str_cep);
        } else {
            resultado = consulta_CEP(raiz->dir, str_cep);
        }
    }
    
    return resultado;
}

// =================================
// REMOÇÃO
// =================================

// Funções auxiliares para remoção
CEP *encontrar_menor_CEP(CEP *raiz) {
    CEP *menor = raiz;
    
    if(raiz != NULL) {
        while(menor->esq != NULL) {
            menor = menor->esq;
        }
    }
    
    return menor;
}

void trocar_informacoes_CEP(CEP *cep1, CEP *cep2) {
    if (cep1 != NULL && cep2 != NULL) {
        // Troca os valores de CEP
        strcpy(cep1->cep, cep2->cep);
    }
}

// Função para mover um nó vermelho para a esquerda durante a remoção
void mover2_esquerda(CEP **raiz)
{
    trocar_cor(*raiz);
 
    if((*raiz)->dir != NULL && cor((*raiz)->dir->esq) == RED)
    {
        rotacao_direita(&((*raiz)->dir));
        rotacao_esquerda(raiz);
        trocar_cor(*raiz);
    }
}

// Função para mover um nó vermelho para a direita durante a remoção
void mover2_direita(CEP **raiz)
{
    trocar_cor(*raiz);

    if((*raiz)->esq != NULL && cor((*raiz)->esq->esq) == RED)
    {
        rotacao_direita(raiz);
        trocar_cor(*raiz);
    }
}

// Função para remover a menor cep da árvore
void remover_menor_CEP_arv(CEP **raiz)
{
    if((*raiz)->esq == NULL) {
        desalocar_CEP(raiz);
    }
    else
    {
        if(cor((*raiz)->esq) == BLACK && cor((*raiz)->esq->esq) == BLACK)
            mover2_esquerda(raiz);

        remover_menor_CEP_arv(&((*raiz)->esq));
        balancear_RB(raiz);
    }
}

// Função para remover uma cep da árvore
int remover_CEP_no(CEP **raiz, char *str_cep)
{
    int removeu = 1;

    if((*raiz) != NULL)
    {
        int resultado = strcasecmp(str_cep, (*raiz)->cep);

        if(resultado < 0)
        {
            if((*raiz)->esq != NULL)
            {
                if(cor((*raiz)->esq) == BLACK && cor((*raiz)->esq->esq) == BLACK)
                    mover2_esquerda(raiz);
            }
                
            removeu = remover_CEP_no(&((*raiz)->esq), str_cep);
        }
        else {
            if(cor((*raiz)->esq) == RED)
                rotacao_direita(raiz);

            if(resultado == 0 && (*raiz)->dir == NULL) {
                desalocar_CEP(raiz);
            }
            else
            {
                if((*raiz)->dir != NULL)
                {
                    if(cor((*raiz)->dir) == BLACK && cor((*raiz)->dir->esq) == BLACK)
                        mover2_direita(raiz);
                }

                if(resultado == 0)
                {
                    CEP *menor = encontrar_menor_CEP((*raiz)->dir);
                    trocar_informacoes_CEP(*raiz, menor);
                    // Remove o menor (que agora contém os dados do nó que queríamos remover)
                    remover_menor_CEP_arv(&((*raiz)->dir));
                }
                else
                    removeu = remover_CEP_no(&((*raiz)->dir), str_cep);
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
int remover_CEP_arvore(CEP **raiz, char *str_cep)
{
    int removeu = consulta_CEP(*raiz, str_cep);

    if (removeu) {
        removeu = remover_CEP_no(raiz, str_cep);
    }

    if(*raiz != NULL)
        (*raiz)->cor = BLACK;

    return removeu;
}

// =================================
// ESPECÍFICAS DO TRABALHO
// =================================