#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../Arv_VP_H/utilitarios_VP.h"
#include "../Arv_VP_H/STRUCTS_VP.h"
#include "../Arv_VP_H/Pessoas_VP.h"
#include "../Arv_VP_H/Cidades_VP.h"

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
        balancear_RB_pessoa(raiz);
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
    PESSOAS *pessoa;
    DATA data_nasc;
    char CPF[12];
    char *nome_pessoa;
    int retorno = 0;
    pessoa = NULL;
    nome_pessoa = NULL;

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

void rotacao_esquerda_pessoa(PESSOAS **raiz)
{
    PESSOAS *aux;

    aux = (*raiz)->dir;
    (*raiz)->dir = aux->esq;
    aux->esq = (*raiz);
    (*raiz) = aux;

    (*raiz)->cor = (*raiz)->esq->cor;
    (*raiz)->esq->cor = RED;
}

void rotacao_direita_pessoa(PESSOAS **raiz)
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

int Cor_pessoa(PESSOAS *pessoa)
{
    return pessoa == NULL ? BLACK : pessoa->cor;
}

void trocar_cor_pessoa(PESSOAS *raiz)
{
    raiz->cor = !(raiz->cor);

    if (raiz->esq != NULL)
        raiz->esq->cor = !(raiz->esq->cor);

    if (raiz->dir != NULL)
        raiz->dir->cor = !(raiz->dir->cor);
}

void balancear_RB_pessoa(PESSOAS **raiz)
{
    if (*raiz != NULL)
    {
        if (Cor_pessoa((*raiz)->esq) == BLACK && Cor_pessoa((*raiz)->dir) == RED)
            rotacao_esquerda_pessoa(raiz);

        if (Cor_pessoa((*raiz)->esq) == RED && Cor_pessoa((*raiz)->esq->esq) == RED)
            rotacao_direita_pessoa(raiz);

        if (Cor_pessoa((*raiz)->esq) == RED && Cor_pessoa((*raiz)->dir) == RED)
            trocar_cor_pessoa(*raiz);
    }
}

// =================================
// BUSCA E IMPRESSÃO
// =================================

// Função para buscar uma pessoa na árvore pelo nome
PESSOAS *existe_pessoa(PESSOAS *raiz, char *CPF)
{
    PESSOAS *resultado;
    resultado = NULL;

    if (raiz != NULL)
    {
        int comparacao = strcasecmp(CPF, raiz->CPF);

        if (comparacao == 0)
            resultado = raiz;
        else if (comparacao < 0)
            resultado = existe_pessoa(raiz->esq, CPF);
        else
            resultado = existe_pessoa(raiz->dir, CPF);
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
        // printf("Cor: %s\n", pessoa->cor == RED ? "Vermelho" : "Preto");

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
        liberar_arvore_pessoas(&((*raiz)->esq));
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
    PESSOAS *menor;
    menor = raiz;

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
        // pessoa1->data_nasc = pessoa2->data_nasc;
        pessoa1->data_nasc = pessoa2->data_nasc;
    }
}

// Função para mover um nó vermelho para a esquerda durante a remoção
void mover2_esquerda_pessoa(PESSOAS **raiz)
{
    trocar_cor_pessoa(*raiz);

    if ((*raiz)->dir != NULL && Cor_pessoa((*raiz)->dir->esq) == RED)
    {
        rotacao_direita_pessoa(&((*raiz)->dir));
        rotacao_esquerda_pessoa(raiz);
        trocar_cor_pessoa(*raiz);
    }
}

// Função para mover um nó vermelho para a direita durante a remoção
void mover2_direita_pessoa(PESSOAS **raiz)
{
    trocar_cor_pessoa(*raiz);

    if ((*raiz)->esq != NULL && Cor_pessoa((*raiz)->esq->esq) == RED)
    {
        rotacao_direita_pessoa(raiz);
        trocar_cor_pessoa(*raiz);
    }
}

// Função para remover a menor pessoa da árvore
void remover_menor_pessoa_arv(PESSOAS **raiz)
{
    if ((*raiz)->esq == NULL)
        liberar_no_pessoa(raiz);
    else
    {
        if (Cor_pessoa((*raiz)->esq) == BLACK && Cor_pessoa((*raiz)->esq->esq) == BLACK)
            mover2_esquerda_pessoa(raiz);

        remover_menor_pessoa_arv(&((*raiz)->esq));
        balancear_RB_pessoa(raiz);
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
                if (Cor_pessoa((*raiz)->esq) == BLACK && Cor_pessoa((*raiz)->esq->esq) == BLACK)
                    mover2_esquerda_pessoa(raiz);
            }

            removeu = remover_pessoa_no(&((*raiz)->esq), CPF);
        }
        else
        {
            if (Cor_pessoa((*raiz)->esq) == RED)
                rotacao_direita_pessoa(raiz);

            if (resultado == 0 && (*raiz)->dir == NULL)            
                liberar_no_pessoa(raiz);            
            else
            {
                if ((*raiz)->dir != NULL)
                {
                    if (Cor_pessoa((*raiz)->dir) == BLACK && Cor_pessoa((*raiz)->dir->esq) == BLACK)
                        mover2_direita_pessoa(raiz);
                }

                if (resultado == 0)
                {
                    PESSOAS *menor;
                    menor = encontrar_menor_pessoa((*raiz)->dir);

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
            balancear_RB_pessoa(raiz);        
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
        removeu = remover_pessoa_no(raiz, CPF);

    if (*raiz != NULL)
        (*raiz)->cor = BLACK;

    return removeu;
}

// =================================
// ESPECÍFICAS DO TRABALHO
// =================================

int verifica_pessoa_nascida_ou_que_mora_na_cidade(PESSOAS *raiz_pessoa, CEP *raiz_CEP)
{ // retorna 1 se alguma pessoa nasceu ou mora na cidade, 0 caso contrario
    int resultado = 0;

    if (raiz_pessoa != NULL && raiz_CEP != NULL)
    {
        // Verifica se a pessoa atual tem relação com o CEP atual
        if (strcasecmp(raiz_CEP->cep, raiz_pessoa->cep_city_natal) == 0 || strcasecmp(raiz_CEP->cep, raiz_pessoa->cep_city_atual) == 0)
            resultado = 1;
        else
        {
            // Verifica a pessoa atual com os CEPs filhos
            resultado = verifica_pessoa_nascida_ou_que_mora_na_cidade(raiz_pessoa, raiz_CEP->esq) || 
                        verifica_pessoa_nascida_ou_que_mora_na_cidade(raiz_pessoa, raiz_CEP->dir);
            
            if (!resultado)
            {
                // Se não encontrou com a pessoa atual e CEPs, verifica com as pessoas filhas
                resultado = verifica_pessoa_nascida_ou_que_mora_na_cidade(raiz_pessoa->esq, raiz_CEP) || 
                            verifica_pessoa_nascida_ou_que_mora_na_cidade(raiz_pessoa->dir, raiz_CEP);
            }
        }
    }
    
    return resultado;
}

///////////////////////////

int quantas_pessoas_nao_moram_na_cidade_natal_ESTADO(ESTADOS *cabeca_estado, PESSOAS *raiz_pessoa)
{ // camada de estados
    int resultado = 0;
    ESTADOS *atual;
    atual = cabeca_estado;

    while (atual != NULL)
    {
        resultado += quantas_pessoas_nao_moram_na_cidade_natal_PESSOAS(raiz_pessoa, atual->cidade);
        atual = atual->prox;
    }

    return resultado;
}

int quantas_pessoas_nao_moram_na_cidade_natal_PESSOAS(PESSOAS *raiz_pessoa, CIDADES *raiz_cidade)
{ // integra a camada de pessoas
    int retorno, soma = 0;

    if (raiz_pessoa != NULL)
    {
        CIDADES *cidade_natal;
        cidade_natal = cidade_natal_dado_cep(raiz_cidade, raiz_pessoa->cep_city_natal);

        if (cidade_natal)
        {
            retorno = cep_pertence_a_cidade(cidade_natal->cep, raiz_pessoa->cep_city_atual);
            if (retorno == 0)
            {
                imprimir_pessoa(raiz_pessoa);
                soma += 1;
            }
        }

        soma += quantas_pessoas_nao_moram_na_cidade_natal_PESSOAS(raiz_pessoa->esq, raiz_cidade);
        soma += quantas_pessoas_nao_moram_na_cidade_natal_PESSOAS(raiz_pessoa->dir, raiz_cidade);
    }

    return soma;
}

int quantas_pessoas_moram_na_cidade_nao_nasceram_nela(CIDADES *cidade, PESSOAS *raiz_pessoa)
{
    int resultado = 0;

    if (cidade != NULL && raiz_pessoa != NULL)
    {
        // Percorre todas as pessoas na árvore
        if (raiz_pessoa != NULL)
        {
            // Verifica se a pessoa mora na cidade especificada
            CIDADES *cidade_atual;
            cidade_atual = cidade_natal_dado_cep(cidade, raiz_pessoa->cep_city_atual);

            if (cidade_atual == cidade) // A pessoa mora nesta cidade
            {
                // Verifica se a pessoa NÃO nasceu na cidade onde mora atualmente
                CIDADES *cidade_natal;
                cidade_natal = cidade_natal_dado_cep(cidade, raiz_pessoa->cep_city_natal);

                if (cidade_natal != cidade) // A pessoa não nasceu na cidade onde mora
                    resultado += 1;              
            }

            // Verifica recursivamente para as demais pessoas na árvore
            resultado += quantas_pessoas_moram_na_cidade_nao_nasceram_nela(cidade, raiz_pessoa->esq);
            resultado += quantas_pessoas_moram_na_cidade_nao_nasceram_nela(cidade, raiz_pessoa->dir);
        }
    }
    return resultado;
}