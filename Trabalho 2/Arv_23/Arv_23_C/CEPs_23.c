#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../Arv_23_H/STRUCTS_23.h"
#include "../Arv_23_H/CEPs_23.h"
#include "../Arv_23_H/utilitarios_23.h"

//==============================================================================
// FUNÇÕES BÁSICAS DA ÁRVORE 2-3
//==============================================================================
// Cria um nó da árvore 2-3 para CEP
Arv23_CEP *cria_no_CEP(CEP info, Arv23_CEP *F_esq, Arv23_CEP *F_cen)
{
    Arv23_CEP *no = (Arv23_CEP *)malloc(sizeof(Arv23_CEP));
    if (no != NULL)
    {
        strcpy(no->info1.cep, info.cep);
        no->nInfo = 1;
        no->esq = F_esq;
        no->cen = F_cen;
        no->dir = NULL;
        no->info2.cep[0] = '\0';
    }
    return no;
}

// Verifica se é folha
int eh_folha_CEP(Arv23_CEP *no)
{
    return no != NULL && no->esq == NULL;
}

// Busca o menor elemento
Arv23_CEP *buscar_menor_elemento_CEP(Arv23_CEP *no)
{
    Arv23_CEP *atual;
    atual = no;

    while (atual != NULL && atual->esq != NULL)
        atual = atual->esq;
    return atual;
}

//==============================================================================
// FUNÇÕES DE INSERÇÃO NA ÁRVORE 2-3
//==============================================================================
void adiciona_infos_CEP(Arv23_CEP **no, CEP info, Arv23_CEP *Sub_Arv_Info)
{
    Arv23_CEP *no_atual;
    no_atual = *no;
    if (strcmp(info.cep, no_atual->info1.cep) > 0)
    {
        strcpy(no_atual->info2.cep, info.cep);
        no_atual->dir = Sub_Arv_Info;
    }
    else
    {
        strcpy(no_atual->info2.cep, no_atual->info1.cep);
        strcpy(no_atual->info1.cep, info.cep);
        no_atual->dir = no_atual->cen;
        no_atual->cen = Sub_Arv_Info;
    }
    no_atual->nInfo = 2;
}

// Quebra um nó cheio
Arv23_CEP *quebra_no_CEP(Arv23_CEP **no, CEP info, CEP *sobe, Arv23_CEP *F_dir)
{
    Arv23_CEP *maior;
    Arv23_CEP *no_atual;
    no_atual = *no;
    maior = NULL;

    if (strcmp(info.cep, no_atual->info2.cep) > 0)
    {
        strcpy(sobe->cep, no_atual->info2.cep);
        maior = cria_no_CEP(info, no_atual->dir, F_dir);
        no_atual->nInfo = 1;
        no_atual->dir = NULL;
    }
    else if (strcmp(info.cep, no_atual->info1.cep) > 0)
    {
        strcpy(sobe->cep, info.cep);
        maior = cria_no_CEP(no_atual->info2, F_dir, no_atual->dir);
        no_atual->nInfo = 1;
        no_atual->dir = NULL;
    }
    else
    {
        strcpy(sobe->cep, no_atual->info1.cep);
        maior = cria_no_CEP(no_atual->info2, no_atual->cen, no_atual->dir);
        strcpy(no_atual->info1.cep, info.cep);
        no_atual->cen = F_dir;
        no_atual->nInfo = 1;
        no_atual->info2.cep[0] = '\0';
        no_atual->dir = NULL;
    }
    return maior;
}

// Inserção principal
int insere_23_CEP(Arv23_CEP **raiz, CEP valor)
{
    int sucesso = 0;
    CEP sobe = {{0}};
    Arv23_CEP *maiorNo;
    maiorNo = NULL;

    if (raiz != NULL)
    {
        sucesso = insere_23_recursivo_CEP(raiz, valor, &sobe, &maiorNo);

        if (maiorNo != NULL)
        {
            Arv23_CEP *nova_raiz;
            nova_raiz = cria_no_CEP(sobe, *raiz, maiorNo);

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

// Inserção recursiva
int insere_23_recursivo_CEP(Arv23_CEP **raiz, CEP valor, CEP *sobe, Arv23_CEP **maiorNo)
{
    int sucesso = 0;
    Arv23_CEP *no_atual;
    no_atual = *raiz;

    if (no_atual == NULL)
    {
        *raiz = cria_no_CEP(valor, NULL, NULL);
        *maiorNo = NULL;
        sobe->cep[0] = '\0';
        sucesso = (*raiz != NULL);
    }
    else if (strcmp(valor.cep, no_atual->info1.cep) == 0 || (no_atual->nInfo == 2 && strcmp(valor.cep, no_atual->info2.cep) == 0))
    {
        *maiorNo = NULL;
        sobe->cep[0] = '\0';
        sucesso = 0;
    }
    else
    {
        if (eh_folha_CEP(no_atual))
        {
            if (no_atual->nInfo == 1)
            {
                adiciona_infos_CEP(raiz, valor, NULL);
                *maiorNo = NULL;
                sobe->cep[0] = '\0';
                sucesso = 1;
            }
            else
            {
                *maiorNo = quebra_no_CEP(raiz, valor, sobe, NULL);
                sucesso = (*maiorNo != NULL);
            }
        }
        else
        {
            Arv23_CEP **proximo_filho;
            if (strcmp(valor.cep, no_atual->info1.cep) < 0)
                proximo_filho = &(no_atual->esq);
            else if (no_atual->nInfo == 1 || strcmp(valor.cep, no_atual->info2.cep) < 0)
                proximo_filho = &(no_atual->cen);
            else
                proximo_filho = &(no_atual->dir);

            int sucesso_rec = insere_23_recursivo_CEP(proximo_filho, valor, sobe, maiorNo);

            if (*maiorNo != NULL)
            {
                Arv23_CEP *novo_filho_dir;
                CEP chave_que_subiu;
                chave_que_subiu = *sobe;
                novo_filho_dir = *maiorNo;
                *maiorNo = NULL;
                sobe->cep[0] = '\0';

                if (no_atual->nInfo == 1)
                {
                    adiciona_infos_CEP(raiz, chave_que_subiu, novo_filho_dir);
                    sucesso = 1;
                }
                else
                {
                    *maiorNo = quebra_no_CEP(raiz, chave_que_subiu, sobe, novo_filho_dir);
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
// FUNÇÕES DE REMOÇÃO NA ÁRVORE 2-3
//==============================================================================
StatusRemocao remover_23_CEP_recursivo_CEP(Arv23_CEP **ponteiro_no_atual, CEP valor)
{
    Arv23_CEP *no_atual;
    StatusRemocao status_final;
    status_final = OK;
    no_atual = *ponteiro_no_atual;

    if (no_atual == NULL)
        status_final = INFO_NAO_ENCONTRADA;
    else
    {
        Arv23_CEP **proximo_ponteiro_recursao = NULL;// ponteiro para o próximo nó a ser visitado
        int valor_encontrado_neste_no = 0;
        int indice_valor_removido = -1;

        if (strcmp(valor.cep, no_atual->info1.cep) == 0)
        {
            valor_encontrado_neste_no = 1;
            indice_valor_removido = 0;
        }
        else if (no_atual->nInfo == 2 && strcmp(valor.cep, no_atual->info2.cep) == 0)
        {
            valor_encontrado_neste_no = 1;
            indice_valor_removido = 1;
        }
        else if (strcmp(valor.cep, no_atual->info1.cep) < 0)
            proximo_ponteiro_recursao = &(no_atual->esq);

        else if (no_atual->nInfo == 1 || strcmp(valor.cep, no_atual->info2.cep) < 0)
            proximo_ponteiro_recursao = &(no_atual->cen);

        else
            proximo_ponteiro_recursao = &(no_atual->dir);

        if (valor_encontrado_neste_no)
        {
            if (eh_folha_CEP(no_atual))
            {
                if (no_atual->nInfo == 2)
                {
                    if (indice_valor_removido == 0)
                        strcpy(no_atual->info1.cep, no_atual->info2.cep);

                    no_atual->info2.cep[0] = '\0';
                    no_atual->nInfo = 1;
                    status_final = OK;
                }
                else
                {
                    no_atual->nInfo = 0;
                    no_atual->info1.cep[0] = '\0';
                    status_final = UNDERFLOW;
                }
            }
            else
            {
                CEP valor_sucessor;
                Arv23_CEP **ponteiro_subarvore_sucessor;
                Arv23_CEP *sucessor_node;
                sucessor_node = NULL;

                if (indice_valor_removido == 0)
                {
                    sucessor_node = buscar_menor_elemento_CEP(no_atual->cen);
                    ponteiro_subarvore_sucessor = &(no_atual->cen);
                }
                else
                {
                    sucessor_node = buscar_menor_elemento_CEP(no_atual->dir);
                    ponteiro_subarvore_sucessor = &(no_atual->dir);
                }

                if (sucessor_node == NULL)
                {
                    status_final = SUCESSOR_NAO_ENCONTRADO;
                }
                else
                {
                    strcpy(valor_sucessor.cep, sucessor_node->info1.cep);

                    if (indice_valor_removido == 0)
                        strcpy(no_atual->info1.cep, valor_sucessor.cep);

                    else
                        strcpy(no_atual->info2.cep, valor_sucessor.cep);

                    StatusRemocao status_remocao_sucessor = remover_23_CEP_recursivo_CEP(ponteiro_subarvore_sucessor, valor_sucessor);
                    if (status_remocao_sucessor == UNDERFLOW)
                        status_final = tratar_underflow_CEP(ponteiro_subarvore_sucessor, no_atual);
                    else
                        status_final = status_remocao_sucessor;
                }
            }
        }
        else
        {
            StatusRemocao status_recursao = remover_23_CEP_recursivo_CEP(proximo_ponteiro_recursao, valor);
            if (status_recursao == UNDERFLOW)
                status_final = tratar_underflow_CEP(proximo_ponteiro_recursao, no_atual);
            else
                status_final = status_recursao;
        }
    }
    return status_final;
}

// Remoção principal
StatusRemocao remover_23_CEP(Arv23_CEP **raiz, CEP valor)
{
    int OPERACAO = 0;
    
    if (raiz == NULL || *raiz == NULL)
    {
        // printf("Arvore vazia. Nao e possivel remover %s.\n", valor.cep);
        OPERACAO = ARVORE_VAZIA;
    }
    else
    {
        StatusRemocao status_geral = remover_23_CEP_recursivo_CEP(raiz, valor);

        if (*raiz != NULL && (*raiz)->nInfo == 0)
        {
            Arv23_CEP *raiz_antiga;
            raiz_antiga = *raiz;
            Arv23_CEP *nova_raiz;
            nova_raiz = raiz_antiga->esq ? raiz_antiga->esq : raiz_antiga->cen;
            *raiz = nova_raiz;
            free(raiz_antiga);
            status_geral = OK;
        }

        OPERACAO = status_geral;
    }
    return OPERACAO;
}

//==============================================================================
// FUNÇÕES DE REBALANCEAMENTO DA ÁRVORE 2-3
//==============================================================================

// Redistribuição com irmão esquerdo
StatusRemocao redistribuir_com_irmao_esquerdo_CEP(Arv23_CEP **ponteiro_filho_no_pai, Arv23_CEP *pai, Arv23_CEP *irmao_esq, int pos_filho)
{
    Arv23_CEP *filho;
    filho = *ponteiro_filho_no_pai;
    CEP chave_pai_desce;
    Arv23_CEP *filho_transferido;
    filho_transferido = irmao_esq->dir;
    Arv23_CEP *filho_original_unico;
    filho_original_unico = (filho->esq != NULL) ? filho->esq : filho->cen;

    if (pos_filho == 1)
    {
        strcpy(chave_pai_desce.cep, pai->info1.cep);
        strcpy(pai->info1.cep, irmao_esq->info2.cep);
    }
    else
    {
        strcpy(chave_pai_desce.cep, pai->info2.cep);
        strcpy(pai->info2.cep, irmao_esq->info2.cep);
    }

    strcpy(filho->info1.cep, chave_pai_desce.cep);
    filho->nInfo = 1;
    filho->esq = filho_transferido;
    filho->cen = filho_original_unico;
    filho->dir = NULL;

    irmao_esq->nInfo = 1;
    irmao_esq->dir = NULL;

    return OK;
}

// Redistribuição com irmão direito
StatusRemocao redistribuir_com_irmao_direito_CEP(Arv23_CEP **ponteiro_filho_no_pai, Arv23_CEP *pai, Arv23_CEP *irmao_dir, int pos_filho)
{
    Arv23_CEP *filho;
    filho = *ponteiro_filho_no_pai;
    CEP chave_pai_desce;
    Arv23_CEP *filho_transferido;
    filho_transferido = irmao_dir->esq;
    Arv23_CEP *filho_original_unico;
    filho_original_unico = (filho->esq != NULL) ? filho->esq : filho->cen;

    if (pos_filho == 0)
    {
        strcpy(chave_pai_desce.cep, pai->info1.cep);
        strcpy(pai->info1.cep, irmao_dir->info1.cep);
    }
    else
    {
        strcpy(chave_pai_desce.cep, pai->info2.cep);
        strcpy(pai->info2.cep, irmao_dir->info1.cep);
    }

    strcpy(filho->info1.cep, chave_pai_desce.cep);
    filho->nInfo = 1;
    filho->esq = filho_original_unico;
    filho->cen = filho_transferido;
    filho->dir = NULL;

    strcpy(irmao_dir->info1.cep, irmao_dir->info2.cep);
    irmao_dir->nInfo = 1;
    irmao_dir->esq = irmao_dir->cen;
    irmao_dir->cen = irmao_dir->dir;
    irmao_dir->dir = NULL;

    return OK;
}

// Fusão com irmão esquerdo
StatusRemocao fundir_com_irmao_esquerdo_CEP(Arv23_CEP **ponteiro_filho_no_pai, Arv23_CEP *pai, Arv23_CEP *irmao_esq, int pos_filho)
{
    Arv23_CEP *filho_underflow;
    filho_underflow = *ponteiro_filho_no_pai;
    CEP chave_pai_desce;
    StatusRemocao status_pai = OK;

    Arv23_CEP *filho_u_remanescente;
    filho_u_remanescente = (filho_underflow->esq != NULL) ? filho_underflow->esq : filho_underflow->cen;
    Arv23_CEP *irmao_e_esq;
    irmao_e_esq = irmao_esq->esq;
    Arv23_CEP *irmao_e_cen;
    irmao_e_cen = irmao_esq->cen;

    if (pai->nInfo == 1)
    {
        strcpy(chave_pai_desce.cep, pai->info1.cep);
        pai->nInfo = 0;
        pai->info1.cep[0] = '\0';
        status_pai = UNDERFLOW;
    }
    else
    {
        if (pos_filho == 1)
        {
            strcpy(chave_pai_desce.cep, pai->info1.cep);
            strcpy(pai->info1.cep, pai->info2.cep);
        }
        else
            strcpy(chave_pai_desce.cep, pai->info2.cep);
        
        pai->nInfo = 1;
    }

    strcpy(irmao_esq->info2.cep, chave_pai_desce.cep);
    irmao_esq->nInfo = 2;
    irmao_esq->esq = irmao_e_esq;
    irmao_esq->cen = irmao_e_cen;
    irmao_esq->dir = filho_u_remanescente;

    if (pos_filho == 1)
    {
        pai->cen = pai->dir;
        pai->dir = NULL;
    }
    else
    {
        pai->dir = NULL;
    }
    if (status_pai == UNDERFLOW)
    {
        pai->cen = NULL;
        pai->dir = NULL;
    }

    free(filho_underflow);
    return status_pai;
}

// Fusão com irmão direito
StatusRemocao fundir_com_irmao_direito_CEP(Arv23_CEP **ponteiro_filho_no_pai, Arv23_CEP *pai, Arv23_CEP *irmao_dir, int pos_filho)
{
    Arv23_CEP *filho_underflow;
    filho_underflow = *ponteiro_filho_no_pai;
    CEP chave_pai_desce;
    StatusRemocao status_pai = OK;

    Arv23_CEP *filho_u_remanescente;
    filho_u_remanescente = (filho_underflow->esq != NULL) ? filho_underflow->esq : filho_underflow->cen;
    Arv23_CEP *irmao_d_esq;
    irmao_d_esq = irmao_dir->esq;
    Arv23_CEP *irmao_d_cen;
    irmao_d_cen = irmao_dir->cen;

    if (pai->nInfo == 1)
    {
        strcpy(chave_pai_desce.cep, pai->info1.cep);
        pai->nInfo = 0;
        pai->info1.cep[0] = '\0';
        status_pai = UNDERFLOW;
    }
    else
    {
        if (pos_filho == 0)
        {
            strcpy(chave_pai_desce.cep, pai->info1.cep);
            strcpy(pai->info1.cep, pai->info2.cep);
        }
        else
            strcpy(chave_pai_desce.cep, pai->info2.cep);

        pai->info2.cep[0] = '\0';
        pai->nInfo = 1;
    }

    strcpy(irmao_dir->info2.cep, irmao_dir->info1.cep);
    strcpy(irmao_dir->info1.cep, chave_pai_desce.cep);
    irmao_dir->nInfo = 2;
    irmao_dir->esq = filho_u_remanescente;
    irmao_dir->cen = irmao_d_esq;
    irmao_dir->dir = irmao_d_cen;

    if (pos_filho == 0)
    {
        pai->esq = pai->cen;
        pai->cen = pai->dir;
        pai->dir = NULL;
    }
    else
    {
        pai->cen = pai->dir;
        pai->dir = NULL;
    }
    if (status_pai == UNDERFLOW)
    {
        pai->cen = NULL;
        pai->dir = NULL;
    }

    free(filho_underflow);
    return status_pai;
}

// Tratamento de underflow
StatusRemocao tratar_underflow_CEP(Arv23_CEP **ponteiro_filho_no_pai, Arv23_CEP *pai)
{
    StatusRemocao status_final = OK;
    Arv23_CEP *filho_com_underflow;
    filho_com_underflow = *ponteiro_filho_no_pai;

    if (filho_com_underflow == NULL || filho_com_underflow->nInfo > 0)
        status_final = OK;
    else if (pai == NULL)
        status_final = UNDERFLOW;
    else
    {
        Arv23_CEP *irmao_esq;
        irmao_esq = NULL;
        Arv23_CEP *irmao_dir;
        irmao_dir = NULL;
        int pos_filho = -1;

        if (pai->esq == filho_com_underflow)
        {
            pos_filho = 0;
            irmao_dir = pai->cen;
        }
        else if (pai->cen == filho_com_underflow)
        {
            pos_filho = 1;
            irmao_esq = pai->esq;
            if (pai->nInfo == 2)
                irmao_dir = pai->dir;
        }
        else if (pai->nInfo == 2 && pai->dir == filho_com_underflow)
        {
            pos_filho = 2;
            irmao_esq = pai->cen;
        }

        if (irmao_dir != NULL && irmao_dir->nInfo == 2)
            status_final = redistribuir_com_irmao_direito_CEP(ponteiro_filho_no_pai, pai, irmao_dir, pos_filho);

        else if (irmao_esq != NULL && irmao_esq->nInfo == 2)
            status_final = redistribuir_com_irmao_esquerdo_CEP(ponteiro_filho_no_pai, pai, irmao_esq, pos_filho);

        else if (irmao_dir != NULL && irmao_dir->nInfo == 1)
            status_final = fundir_com_irmao_direito_CEP(ponteiro_filho_no_pai, pai, irmao_dir, pos_filho);

        else if (irmao_esq != NULL && irmao_esq->nInfo == 1)
            status_final = fundir_com_irmao_esquerdo_CEP(ponteiro_filho_no_pai, pai, irmao_esq, pos_filho);

        else
            status_final = NAO_FOI_POSSIVEL_T_UNDERFLOW;
    }
    return status_final;
}

//==============================================================================
// FUNÇÕES DE GERENCIAMENTO DE MEMÓRIA
//==============================================================================

// Liberação de memória
void libera_arvore_CEP(Arv23_CEP **raiz)
{
    if (raiz != NULL && *raiz != NULL)
    {
        libera_arvore_CEP(&(*raiz)->esq);
        libera_arvore_CEP(&(*raiz)->cen);

        if ((*raiz)->nInfo == 2)
            libera_arvore_CEP(&(*raiz)->dir);

        free(*raiz);
        *raiz = NULL;
    }
}

//==============================================================================
// FUNÇÕES DE EXIBIÇÃO E PERCURSO DA ÁRVORE 2-3
//==============================================================================

// Impressão em ordem
void imprime_23_em_ordem_CEP(Arv23_CEP *raiz)
{
    if (raiz != NULL)
    {
        imprime_23_em_ordem_CEP(raiz->esq);
        printf("%s ", raiz->info1.cep);
        imprime_23_em_ordem_CEP(raiz->cen);
        if (raiz->nInfo == 2)
        {
            printf("%s ", raiz->info2.cep);
            imprime_23_em_ordem_CEP(raiz->dir);
        }
    }
}

// Impressão visual
void imprime_arvore_visual_CEP(Arv23_CEP *raiz, char *prefixo, int eh_ultimo, int eh_raiz)
{
    if (raiz != NULL)
    {
        printf("%s", prefixo);
        printf("%s", eh_raiz ? " \nRaiz--> " : (eh_ultimo ? "`------ " : "+------ "));

        if (raiz->nInfo == 1)
            printf("[%s]\n", raiz->info1.cep);
        else if (raiz->nInfo == 2)
            printf("[%s, %s]\n", raiz->info1.cep, raiz->info2.cep);

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
        if (raiz->esq)
            imprime_arvore_visual_CEP(raiz->esq, novo_prefixo, ++filhos_impressos == num_filhos, 0);
        if (raiz->cen)
            imprime_arvore_visual_CEP(raiz->cen, novo_prefixo, ++filhos_impressos == num_filhos, 0);
        if (raiz->dir)
            imprime_arvore_visual_CEP(raiz->dir, novo_prefixo, ++filhos_impressos == num_filhos, 0);
    }
}

//==============================================================================
// ESPECIFICAS DO TRABALHO
//==============================================================================

//====== para percorrer estados e cidades procurando CEPs ======

// Consulta CEP na árvore 2-3
int consulta_CEP(Arv23_CEP *raiz, char *str_cep)
{

    int resultado = 0;

    if (raiz != NULL && raiz->nInfo > 0)
    {
        int comparacao1 = strcasecmp(str_cep, raiz->info1.cep);
        int comparacao2;

        if (comparacao1 == 0)
            resultado = 1; // CEP encontrado em info1
        else if (raiz->nInfo == 2)
        {
            comparacao2 = strcasecmp(str_cep, raiz->info2.cep);
            if (comparacao2 == 0)
                resultado = 1; // CEP encontrado em info2
            
            else if (comparacao2 < 0)
                // str_cep < info2.cep, busca em cen
                resultado = consulta_CEP(raiz->cen, str_cep);
            
            else
                // str_cep > info2.cep, busca em dir
                resultado = consulta_CEP(raiz->dir, str_cep);
        }
        else if (comparacao1 < 0)
            // str_cep < info1.cep, busca em esq
            resultado = consulta_CEP(raiz->esq, str_cep);
        
        else
            // str_cep > info1.cep e nInfo == 1, busca em cen
            resultado = consulta_CEP(raiz->cen, str_cep);
    }
    return resultado;
}

// percorre estados procurando CEP para validar o cadastro de pessoas

int percorre_estados_procurando_CEP(ESTADOS *cabeca, char *cep)
{
    ESTADOS *atual;
    atual = cabeca;
    int encontrado = 0;

    while (atual != NULL && encontrado == 0)
    {
        encontrado |= percorre_cidades_procurando_CEP(atual->arv_cidades, cep);
        if (encontrado == 0)
            atual = atual->prox;
    }

    return encontrado;
}

int percorre_cidades_procurando_CEP(Arv23_CIDADES *raiz, char *cep)
{
    int encontrado = 0;

    if (raiz != NULL && raiz->nInfo > 0)
    {
        // Verifica a árvore de CEPs da primeira cidade (info1)
        if (raiz->info1.arv_cep != NULL)
            encontrado |= consulta_CEP(raiz->info1.arv_cep, cep);

        // Se ainda não encontrado e o nó tem duas cidades, verifica info2
        if (encontrado == 0 && raiz->nInfo == 2)
        {
            if (raiz->info2.arv_cep != NULL)
                encontrado |= consulta_CEP(raiz->info2.arv_cep, cep);
        }

        // Se ainda não encontrado, percorre as subárvores
        if (encontrado == 0)
            encontrado |= percorre_cidades_procurando_CEP(raiz->esq, cep);

        if (encontrado == 0)
            encontrado |= percorre_cidades_procurando_CEP(raiz->cen, cep);

        if (encontrado == 0 && raiz->nInfo == 2)
            encontrado |= percorre_cidades_procurando_CEP(raiz->dir, cep);
    }

    return encontrado;
}