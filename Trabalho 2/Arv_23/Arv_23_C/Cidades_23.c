#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../Arv_23_H/STRUCTS_23.h"
#include "../Arv_23_H/Cidades_23.h"
#include "../Arv_23_H/CEPs_23.h"
#include "../Arv_23_H/utilitarios_23.h"

//==============================================================================
// FUNÇÕES DE MANIPULAÇÃO DE DADOS
//==============================================================================
CIDADES cadastra_cidade(int *sucesso)
{
    CIDADES novaCidade;
    char nome_cidade[100];

    *sucesso = 0;                            // Inicializa como falha
    memset(&novaCidade, 0, sizeof(CIDADES)); // Inicializa a estrutura

    printf("Digite o nome da cidade: ");
    ler_string_simples(nome_cidade, sizeof(nome_cidade));

    printf("Digite a populacao da cidade: ");
    int populacao_city = digitar_int();

    if (nome_cidade[0] != '\0' && populacao_city >= 0)
    {
        strcpy(novaCidade.nome_cidade, nome_cidade);
        novaCidade.populacao_city = populacao_city;
        novaCidade.arv_cep = NULL; // CEPs serão cadastrados no menu
        *sucesso = 1;              // Marca como sucesso
    }

    return novaCidade;
}

//==============================================================================
// FUNÇÕES BÁSICAS DA ÁRVORE 2-3
//==============================================================================

Arv23_CIDADES *cria_no_cidade(CIDADES info, Arv23_CIDADES *F_esq, Arv23_CIDADES *F_cen)
{
    Arv23_CIDADES *no = (Arv23_CIDADES *)malloc(sizeof(Arv23_CIDADES));
    if (no != NULL)
    {
        no->info1 = info; // Copia a estrutura CIDADES
        no->nInfo = 1;
        no->esq = F_esq;
        no->cen = F_cen;
        no->dir = NULL;
    }
    return no;
}

int eh_folha_cidade(Arv23_CIDADES *no)
{
    int resultado = 0;
    if (no != NULL)
        resultado = (no->esq == NULL);
    return resultado;
}

CIDADES *buscar_info_cidade(Arv23_CIDADES *raiz, char *info)
{
    CIDADES *retorno;
    retorno = NULL;

    if (raiz != NULL)
    {
        if (strcasecmp(info, raiz->info1.nome_cidade) == 0)
            retorno = &(raiz->info1);
        
        else if (raiz->nInfo == 2 && strcasecmp(info, raiz->info2.nome_cidade) == 0)
            retorno = &(raiz->info2);
        
        else if (strcasecmp(info, raiz->info1.nome_cidade) < 0)
            retorno = buscar_info_cidade(raiz->esq, info);
    
        else if (raiz->nInfo == 1 || strcasecmp(info, raiz->info2.nome_cidade) < 0)
            retorno = buscar_info_cidade(raiz->cen, info);
        
        else
            retorno = buscar_info_cidade(raiz->dir, info);
    }

    return retorno;
}

//==============================================================================
// FUNÇÕES DE INSERÇÃO NA ÁRVORE 2-3
//==============================================================================
void adiciona_infos_cidade(Arv23_CIDADES **no, CIDADES info, Arv23_CIDADES *Sub_Arv_Info)
{
    Arv23_CIDADES *no_atual;
    no_atual = *no;
    if (strcmp(info.nome_cidade, no_atual->info1.nome_cidade) > 0)
    {
        no_atual->info2 = info;
        no_atual->dir = Sub_Arv_Info;
    }
    else
    {
        no_atual->info2 = no_atual->info1;
        no_atual->info1 = info;
        no_atual->dir = no_atual->cen;
        no_atual->cen = Sub_Arv_Info;
    }
    no_atual->nInfo = 2;
}

// Ajustado para CIDADES
Arv23_CIDADES *quebra_no_cidade(Arv23_CIDADES **no, CIDADES info, CIDADES *sobe, Arv23_CIDADES *F_dir)
{
    Arv23_CIDADES *no_atual;
    no_atual = *no;
    Arv23_CIDADES *maior;
    maior = NULL;
    if (strcmp(info.nome_cidade, no_atual->info2.nome_cidade) > 0)
    {
        *sobe = no_atual->info2;
        maior = cria_no_cidade(info, no_atual->dir, F_dir);
        no_atual->nInfo = 1;
        no_atual->dir = NULL;
    }
    else if (strcmp(info.nome_cidade, no_atual->info1.nome_cidade) > 0)
    {
        *sobe = info;
        maior = cria_no_cidade(no_atual->info2, F_dir, no_atual->dir);
        no_atual->nInfo = 1;
        no_atual->dir = NULL;
    }
    else
    {
        *sobe = no_atual->info1;
        maior = cria_no_cidade(no_atual->info2, no_atual->cen, no_atual->dir);
        no_atual->info1 = info;
        no_atual->cen = F_dir;
        no_atual->nInfo = 1;
        no_atual->dir = NULL;
    }
    return maior;
}

int insere_23_cidade(Arv23_CIDADES **raiz, CIDADES valor)
{
    CIDADES sobe;
    Arv23_CIDADES *maiorNo;
    maiorNo = NULL;
    int sucesso = 0;

    if (raiz != NULL)
    {
        sucesso = insere_23_recursivo_cidade(raiz, valor, &sobe, &maiorNo);

        if (maiorNo != NULL)
        {
            Arv23_CIDADES *nova_raiz;
            nova_raiz = cria_no_cidade(sobe, *raiz, maiorNo);

            if (nova_raiz != NULL)
                *raiz = nova_raiz;
            else
            {
                sucesso = 0;
                if (maiorNo)
                    free(maiorNo);
            }
        }
    }
    return sucesso;
}

int insere_23_recursivo_cidade(Arv23_CIDADES **raiz, CIDADES valor, CIDADES *sobe, Arv23_CIDADES **maiorNo)
{
    int sucesso = 0;
    Arv23_CIDADES *no_atual;
    no_atual = *raiz;

    if (no_atual == NULL)
    {
        *raiz = cria_no_cidade(valor, NULL, NULL);
        *maiorNo = NULL;
        memset(sobe, 0, sizeof(CIDADES)); // Inicializa sobe
        sucesso = (*raiz != NULL);
    }
    else if (strcasecmp(valor.nome_cidade, no_atual->info1.nome_cidade) == 0 || (no_atual->nInfo == 2 && strcasecmp(valor.nome_cidade, no_atual->info2.nome_cidade) == 0))
    {
        *maiorNo = NULL;
        memset(sobe, 0, sizeof(CIDADES));// Inicializa sobe como vazio
        sucesso = 0;
    }
    else
    {
        if (eh_folha_cidade(no_atual))
        {
            if (no_atual->nInfo == 1)
            {
                adiciona_infos_cidade(raiz, valor, NULL);
                *maiorNo = NULL;
                memset(sobe, 0, sizeof(CIDADES));
                sucesso = 1;
            }
            else
            {
                *maiorNo = quebra_no_cidade(raiz, valor, sobe, NULL);
                sucesso = (*maiorNo != NULL);
            }
        }
        else
        {
            Arv23_CIDADES **proximo_filho;
            if (strcmp(valor.nome_cidade, no_atual->info1.nome_cidade) < 0)
                proximo_filho = &(no_atual->esq);
            
            else if (no_atual->nInfo == 1 || strcasecmp(valor.nome_cidade, no_atual->info2.nome_cidade) < 0)
                proximo_filho = &(no_atual->cen);
            
            else
                proximo_filho = &(no_atual->dir);
            
            int sucesso_rec = insere_23_recursivo_cidade(proximo_filho, valor, sobe, maiorNo);

            if (*maiorNo != NULL)
            {
                CIDADES chave_que_subiu;
                chave_que_subiu = *sobe;
                Arv23_CIDADES *novo_filho_dir;
                novo_filho_dir = *maiorNo;
                *maiorNo = NULL;
                memset(sobe, 0, sizeof(CIDADES));

                if (no_atual->nInfo == 1)
                {
                    adiciona_infos_cidade(raiz, chave_que_subiu, novo_filho_dir);
                    sucesso = 1;
                }
                else
                {
                    *maiorNo = quebra_no_cidade(raiz, chave_que_subiu, sobe, novo_filho_dir);
                    sucesso = (*maiorNo != NULL);
                }
            }
            else
                sucesso = sucesso_rec;
        }
    }
    return sucesso;
}

//==============================================================================
// FUNÇÕES DE GERENCIAMENTO DE MEMÓRIA
//==============================================================================

void libera_arvore_cidade(Arv23_CIDADES **raiz)
{
    if (raiz != NULL && *raiz != NULL)
    {
        libera_arvore_cidade(&(*raiz)->esq);
        libera_arvore_cidade(&(*raiz)->cen);
        
        libera_arvore_CEP(&(*raiz)->info1.arv_cep); // Libera árvore (juntamente com a árvore de CEPs)
        
        if ((*raiz)->nInfo == 2){
            libera_arvore_CEP(&(*raiz)->info2.arv_cep);
            libera_arvore_cidade(&(*raiz)->dir);
        }
        free(*raiz);
        *raiz = NULL;
    }
}

//==============================================================================
// FUNÇÕES DE EXIBIÇÃO E PERCURSO DA ÁRVORE 2-3
//==============================================================================
void printar_informacoes_cidade(CIDADES *cidade)
{
    if (cidade != NULL)
    {
        printf("\n==========================\n");
        printf("Cidade: %s\n", cidade->nome_cidade);
        printf("Populacao: %d\n", cidade->populacao_city);
        printf("CEP(s): ");
        printf("\n==========================\n");
    }
}

void imprime_23_em_ordem_cidade(Arv23_CIDADES *raiz)
{
    if (raiz != NULL)
    {
        imprime_23_em_ordem_cidade(raiz->esq);
        printar_informacoes_cidade(&raiz->info1);
        imprime_23_em_ordem_cidade(raiz->cen);
        if (raiz->nInfo == 2)
        {
            printar_informacoes_cidade(&raiz->info2);
            imprime_23_em_ordem_cidade(raiz->dir);
        }
    }
}

void imprime_arvore_visual_cidade(Arv23_CIDADES *raiz, char *prefixo, int eh_ultimo, int eh_raiz)
{
    if (raiz != NULL)
    {
        printf("%s", prefixo);
        if (!eh_raiz)        
            printf("%s", eh_ultimo ? "`------ " : "+------ ");        
        else        
            printf(" \nRaiz--> ");        

        if (raiz->nInfo == 1)        
            printf("[%s]\n", raiz->info1.nome_cidade);        
        else if (raiz->nInfo == 2)        
            printf("[%s, %s]\n", raiz->info1.nome_cidade, raiz->info2.nome_cidade);        

        char novo_prefixo[1024];
        sprintf(novo_prefixo, "%s%s", prefixo, eh_raiz ? "         " : (eh_ultimo ? "         " : "|        "));

        int num_filhos = 0;
        if (raiz->esq)
            num_filhos++;
        if (raiz->cen)
            num_filhos++;
        if (raiz->dir)
            num_filhos++;

        int filhos_impressos = 0;
        if (raiz->esq != NULL)
        {
            filhos_impressos++;
            imprime_arvore_visual_cidade(raiz->esq, novo_prefixo, filhos_impressos == num_filhos, 0);
        }
        if (raiz->cen != NULL)
        {
            filhos_impressos++;
            imprime_arvore_visual_cidade(raiz->cen, novo_prefixo, filhos_impressos == num_filhos, 0);
        }
        if (raiz->dir != NULL)
        {
            filhos_impressos++;
            imprime_arvore_visual_cidade(raiz->dir, novo_prefixo, filhos_impressos == num_filhos, 0);
        }
    }
}

//==============================================================================
// FUNÇÕES ESPECÍFICAS DO TRABALHO
//==============================================================================

CIDADES *verifica_cidade_mais_populosa_nao_capital_23(Arv23_CIDADES *raiz, char *nome_capital)
{
    CIDADES *cidade_mais_populosa;
    cidade_mais_populosa = NULL;

    if (raiz != NULL)
    {
        // Verifica as cidades no nó atual
        if (strcasecmp(raiz->info1.nome_cidade, nome_capital) != 0)
            cidade_mais_populosa = &raiz->info1;

        if (raiz->nInfo == 2 && strcasecmp(raiz->info2.nome_cidade, nome_capital) != 0)
        {
            if (cidade_mais_populosa == NULL || raiz->info2.populacao_city > cidade_mais_populosa->populacao_city)
                cidade_mais_populosa = &raiz->info2;
        }

        // Processa recursivamente os filhos
        CIDADES *filhos[3] = {
            raiz->esq ? verifica_cidade_mais_populosa_nao_capital_23(raiz->esq, nome_capital) : NULL,
            raiz->cen ? verifica_cidade_mais_populosa_nao_capital_23(raiz->cen, nome_capital) : NULL,
            raiz->dir ? verifica_cidade_mais_populosa_nao_capital_23(raiz->dir, nome_capital) : NULL
        };

        // Compara com os resultados dos filhos
        for (int i = 0; i < 3; i++)
        {
            if (filhos[i] && (cidade_mais_populosa == NULL || filhos[i]->populacao_city > cidade_mais_populosa->populacao_city))
                cidade_mais_populosa = filhos[i];
        }
    }

    return cidade_mais_populosa;
}

int cep_pertence_a_cidade(Arv23_CEP *raiz, char *cep)
{
    int resultado = 0;

    if (raiz != NULL)
    {
        // Verifica se o CEP está no nó atual
        if (strcasecmp(raiz->info1.cep, cep) == 0)
            resultado = 1;
        
        else if (raiz->nInfo == 2 && strcasecmp(raiz->info2.cep, cep) == 0)
            resultado = 1;
        
        else
        {

            if (strcasecmp(cep, raiz->info1.cep) < 0)
                resultado = cep_pertence_a_cidade(raiz->esq, cep);
            
            else if (raiz->nInfo == 1 || strcasecmp(cep, raiz->info2.cep) < 0)
                resultado = cep_pertence_a_cidade(raiz->cen, cep);
            
            else
                resultado = cep_pertence_a_cidade(raiz->dir, cep);
        }
    }

    return resultado;
}

CIDADES *cidade_dado_cep(Arv23_CIDADES *raiz, char *cep)
{
    CIDADES *resultado;
    resultado = NULL;

    if (raiz != NULL)
    {
        if (cep_pertence_a_cidade(raiz->info1.arv_cep, cep) == 1)
            resultado = &raiz->info1;
        else if (raiz->nInfo == 2 && cep_pertence_a_cidade(raiz->info2.arv_cep, cep) == 1)
            resultado = &raiz->info2;
        else
        {
            if (!resultado)
                resultado = cidade_dado_cep(raiz->esq, cep);
            if (!resultado)
                resultado = cidade_dado_cep(raiz->cen, cep);
            if (!resultado)
                resultado = cidade_dado_cep(raiz->dir, cep);
        }
    }
    return resultado;
}

