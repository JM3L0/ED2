#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../Arv_23_H/STRUCTS_23.h"
#include "../Arv_23_H/PESSOAS_23.h"
#include "../Arv_23_H/utilitarios_23.h"

//==============================================================================
// FUNÇÕES DE MANIPULAÇÃO DE DADOS
//==============================================================================

PESSOAS cadastra_pessoa(char *cep_natal, char *cep_atual)
{
    PESSOAS pessoa = {0}; // Inicializa com zeros
    DATA data_nasc = {0};
    char CPF[12] = {0};
    char nome_buffer[100] = {0};
    int retorno = 0;

    printf("Digite o nome da pessoa: ");
    ler_string_simples(nome_buffer, sizeof(nome_buffer));

    if (nome_buffer[0] != '\0')
    {
        retorno = capturar_cpf(CPF);
        if (retorno)
        {
            // Captura da data de nascimento
            retorno = capturar_data(&data_nasc);
            if (retorno)
            {
                // Inicializa a estrutura pessoa
                strncpy(pessoa.CPF, CPF, sizeof(pessoa.CPF) - 1);
                pessoa.CPF[sizeof(pessoa.CPF) - 1] = '\0';
                
                strncpy(pessoa.nome_pessoa, nome_buffer, sizeof(pessoa.nome_pessoa) - 1);
                pessoa.nome_pessoa[sizeof(pessoa.nome_pessoa) - 1] = '\0';
                
                strncpy(pessoa.cep_city_natal, cep_natal, sizeof(pessoa.cep_city_natal) - 1);
                pessoa.cep_city_natal[sizeof(pessoa.cep_city_natal) - 1] = '\0';
                
                strncpy(pessoa.cep_city_atual, cep_atual, sizeof(pessoa.cep_city_atual) - 1);
                pessoa.cep_city_atual[sizeof(pessoa.cep_city_atual) - 1] = '\0';
                
                pessoa.data_nasc = data_nasc;
            }
        }
    }

    // Se retorno == 0 ou nome_buffer vazio, pessoa.CPF permanece vazio, indicando falha
    return pessoa;
}

//==============================================================================
// FUNÇÕES BÁSICAS DA ÁRVORE 2-3
//==============================================================================

// Cria um novo nó
Arv23_PESSOAS *cria_no_PESSOAS(PESSOAS info, Arv23_PESSOAS *F_esq, Arv23_PESSOAS *F_cen)
{
    Arv23_PESSOAS *no = (Arv23_PESSOAS *)malloc(sizeof(Arv23_PESSOAS));
    if (no != NULL)
    {
        no->info1 = (PESSOAS *)malloc(sizeof(PESSOAS));
        if (no->info1 == NULL)
        {
            free(no);
            return NULL;
        }
        *no->info1 = info; // Copia todos os campos
        no->info2 = NULL;
        no->nInfo = 1;
        no->esq = F_esq;
        no->cen = F_cen;
        no->dir = NULL;
    }
    return no;
}

// Verifica se é folha
int eh_folha_PESSOAS(Arv23_PESSOAS *no)
{
    return no != NULL && no->esq == NULL && no->cen == NULL && no->dir == NULL;
}

// Busca o menor elemento
Arv23_PESSOAS *buscar_menor_elemento_PESSOAS(Arv23_PESSOAS *no)
{
    Arv23_PESSOAS *atual = no;
    while (atual != NULL && atual->esq != NULL)
    atual = atual->esq;
    return atual;
}

//==============================================================================
// FUNÇÕES DE INSERÇÃO NA ÁRVORE 2-3
//==============================================================================

// Adiciona uma nova informação ao nó
void adiciona_infos_PESSOAS(Arv23_PESSOAS **no, PESSOAS info, Arv23_PESSOAS *Sub_Arv_Info)
{
    Arv23_PESSOAS *no_atual = *no;
    no_atual->info2 = (PESSOAS *)malloc(sizeof(PESSOAS));
    if (no_atual->info2 == NULL)
    return;
    
    if (strcmp(info.CPF, no_atual->info1->CPF) > 0)
    {
        *no_atual->info2 = info;
        no_atual->dir = Sub_Arv_Info;
    }
    else
    {
        *no_atual->info2 = *no_atual->info1;
        *no_atual->info1 = info;
        no_atual->dir = no_atual->cen;
        no_atual->cen = Sub_Arv_Info;
    }
    no_atual->nInfo = 2;
}

// Quebra um nó cheio
Arv23_PESSOAS *quebra_no_PESSOAS(Arv23_PESSOAS **no, PESSOAS info, PESSOAS *sobe, Arv23_PESSOAS *F_dir)
{
    Arv23_PESSOAS *no_atual = *no;
    Arv23_PESSOAS *maior = NULL;
    if (strcmp(info.CPF, no_atual->info2->CPF) > 0)
    {
        *sobe = *no_atual->info2;
        maior = cria_no_PESSOAS(info, no_atual->dir, F_dir);
        no_atual->nInfo = 1;
        free(no_atual->info2);
        no_atual->info2 = NULL;
        no_atual->dir = NULL;
    }
    else if (strcmp(info.CPF, no_atual->info1->CPF) > 0)
    {
        *sobe = info;
        maior = cria_no_PESSOAS(*no_atual->info2, F_dir, no_atual->dir);
        no_atual->nInfo = 1;
        free(no_atual->info2);
        no_atual->info2 = NULL;
        no_atual->dir = NULL;
    }
    else
    {
        *sobe = *no_atual->info1;
        maior = cria_no_PESSOAS(*no_atual->info2, no_atual->cen, no_atual->dir);
        *no_atual->info1 = info;
        no_atual->cen = F_dir;
        no_atual->nInfo = 1;
        free(no_atual->info2);
        no_atual->info2 = NULL;
        no_atual->dir = NULL;
    }
    return maior;
}

// Inserção recursiva
int insere_23_recursivo_PESSOAS(Arv23_PESSOAS **raiz, PESSOAS valor, PESSOAS *sobe, Arv23_PESSOAS **maiorNo)
{
    int sucesso = 0;
    Arv23_PESSOAS *no_atual = *raiz;
    
    if (no_atual == NULL)
    {
        *raiz = cria_no_PESSOAS(valor, NULL, NULL);
        *maiorNo = NULL;
        memset(sobe, 0, sizeof(PESSOAS));
        sucesso = (*raiz != NULL);
    }
    else if (strcmp(valor.CPF, no_atual->info1->CPF) == 0 ||(no_atual->nInfo == 2 && strcmp(valor.CPF, no_atual->info2->CPF) == 0))
    {
        printf("CPF %s ja existe na arvore!\n", valor.CPF);
        *maiorNo = NULL;
        memset(sobe, 0, sizeof(PESSOAS));
        sucesso = 0;
    }
    else
    {
        if (eh_folha_PESSOAS(no_atual))
        {
            if (no_atual->nInfo == 1)
            {
                adiciona_infos_PESSOAS(raiz, valor, NULL);
                *maiorNo = NULL;
                memset(sobe, 0, sizeof(PESSOAS));
                sucesso = 1;
            }
            else
            {
                *maiorNo = quebra_no_PESSOAS(raiz, valor, sobe, NULL);
                sucesso = (*maiorNo != NULL);
            }
        }
        else
        {
            Arv23_PESSOAS **proximo_filho;
            if (strcmp(valor.CPF, no_atual->info1->CPF) < 0)
            proximo_filho = &(no_atual->esq);
            else if (no_atual->nInfo == 1 || strcmp(valor.CPF, no_atual->info2->CPF) < 0)
            proximo_filho = &(no_atual->cen);
            else
            proximo_filho = &(no_atual->dir);
            
            int sucesso_rec = insere_23_recursivo_PESSOAS(proximo_filho, valor, sobe, maiorNo);
            
            if (*maiorNo != NULL)
            {
                PESSOAS chave_que_subiu = *sobe;
                Arv23_PESSOAS *novo_filho_dir = *maiorNo;
                *maiorNo = NULL;
                memset(sobe, 0, sizeof(PESSOAS));
                
                if (no_atual->nInfo == 1)
                {
                    adiciona_infos_PESSOAS(raiz, chave_que_subiu, novo_filho_dir);
                    sucesso = 1;
                }
                else
                {
                    *maiorNo = quebra_no_PESSOAS(raiz, chave_que_subiu, sobe, novo_filho_dir);
                    sucesso = (*maiorNo != NULL);
                }
            }
            else
            {
                sucesso = sucesso_rec;
            }
        }
    }
    return sucesso;
}

//==============================================================================
// FUNÇÕES DE REMOÇÃO NA ÁRVORE 2-3
//==============================================================================

// Remoção recursiva
StatusRemocao remover_23_PESSOAS_recursivo(Arv23_PESSOAS **ponteiro_no_atual, PESSOAS valor)
{
    StatusRemocao status_final = OK;
    Arv23_PESSOAS *no_atual = *ponteiro_no_atual;

    if (no_atual == NULL)
    {
        status_final = INFO_NAO_ENCONTRADA;
    }
    else
    {
        Arv23_PESSOAS **proximo_ponteiro_recursao = NULL;
        int valor_encontrado_neste_no = 0;
        int indice_valor_removido = -1;

        if (strcmp(valor.CPF, no_atual->info1->CPF) == 0)
        {
            valor_encontrado_neste_no = 1;
            indice_valor_removido = 0;
        }
        else if (no_atual->nInfo == 2 && strcmp(valor.CPF, no_atual->info2->CPF) == 0)
        {
            valor_encontrado_neste_no = 1;
            indice_valor_removido = 1;
        }
        else if (strcmp(valor.CPF, no_atual->info1->CPF) < 0)
            proximo_ponteiro_recursao = &(no_atual->esq);
        else if (no_atual->nInfo == 1 || strcmp(valor.CPF, no_atual->info2->CPF) < 0)
            proximo_ponteiro_recursao = &(no_atual->cen);
        else
            proximo_ponteiro_recursao = &(no_atual->dir);

        if (valor_encontrado_neste_no)
        {
            if (eh_folha_PESSOAS(no_atual))
            {
                if (no_atual->nInfo == 2)
                {
                    if (indice_valor_removido == 0)
                        *no_atual->info1 = *no_atual->info2;
                    free(no_atual->info2);
                    no_atual->info2 = NULL;
                    no_atual->nInfo = 1;
                    status_final = OK;
                }
                else
                {
                    no_atual->nInfo = 0;
                    free(no_atual->info1);
                    no_atual->info1 = NULL;
                    status_final = UNDERFLOW;
                }
            }
            else
            {
                Arv23_PESSOAS *sucessor_node = NULL;
                PESSOAS valor_sucessor;
                Arv23_PESSOAS **ponteiro_subarvore_sucessor;

                if (indice_valor_removido == 0)
                {
                    sucessor_node = buscar_menor_elemento_PESSOAS(no_atual->cen);
                    ponteiro_subarvore_sucessor = &(no_atual->cen);
                }
                else
                {
                    sucessor_node = buscar_menor_elemento_PESSOAS(no_atual->dir);
                    ponteiro_subarvore_sucessor = &(no_atual->dir);
                }

                if (sucessor_node == NULL)
                {
                    status_final = SUCESSOR_NAO_ENCONTRADO;
                }
                else
                {
                    valor_sucessor = *sucessor_node->info1;
                    if (indice_valor_removido == 0)
                        *no_atual->info1 = valor_sucessor;
                    else
                        *no_atual->info2 = valor_sucessor;

                    StatusRemocao status_remocao_sucessor = remover_23_PESSOAS_recursivo(ponteiro_subarvore_sucessor, valor_sucessor);
                    if (status_remocao_sucessor == UNDERFLOW)
                        status_final = tratar_underflow_PESSOAS(ponteiro_subarvore_sucessor, no_atual);
                    else
                        status_final = status_remocao_sucessor;
                }
            }
        }
        else
        {
            StatusRemocao status_recursao = remover_23_PESSOAS_recursivo(proximo_ponteiro_recursao, valor);
            if (status_recursao == UNDERFLOW)
                status_final = tratar_underflow_PESSOAS(proximo_ponteiro_recursao, no_atual);
            else
                status_final = status_recursao;
        }
    }
    return status_final;
}

// Remoção principal
StatusRemocao remover_23_PESSOAS(Arv23_PESSOAS **raiz, PESSOAS valor)
{
    StatusRemocao status_geral = OK;
    if (raiz == NULL || *raiz == NULL)
    {
        status_geral = ARVORE_VAZIA;
    }
    else
    {
        status_geral = remover_23_PESSOAS_recursivo(raiz, valor);

        if (*raiz != NULL && (*raiz)->nInfo == 0)
        {
            Arv23_PESSOAS *raiz_antiga = *raiz;
            Arv23_PESSOAS *nova_raiz = raiz_antiga->esq ? raiz_antiga->esq : raiz_antiga->cen;
            *raiz = nova_raiz;
            free(raiz_antiga);
            status_geral = OK;
        }
    }
    return status_geral;
}
// Inserção principal
int insere_23_PESSOAS(Arv23_PESSOAS **raiz, PESSOAS valor)
{
    PESSOAS sobe = {0};
    Arv23_PESSOAS *maiorNo = NULL;
    int sucesso = 0;

    if (raiz != NULL)
    {
        sucesso = insere_23_recursivo_PESSOAS(raiz, valor, &sobe, &maiorNo);

        if (maiorNo != NULL)
        {
            Arv23_PESSOAS *nova_raiz = cria_no_PESSOAS(sobe, *raiz, maiorNo);
            if (nova_raiz != NULL)
            {
                *raiz = nova_raiz;
            }
            else
            {
                sucesso = 0;
                if (maiorNo)
                    libera_arvore_PESSOAS(&maiorNo);
            }
        }
    }
    return sucesso;
}

//==============================================================================
// FUNÇÕES DE REBALANCEAMENTO DA ÁRVORE 2-3
//==============================================================================

// Redistribuição com irmão esquerdo
StatusRemocao redistribuir_com_irmao_esquerdo_PESSOAS(Arv23_PESSOAS **ponteiro_filho_no_pai, Arv23_PESSOAS *pai, Arv23_PESSOAS *irmao_esq, int pos_filho)
{
    Arv23_PESSOAS *filho = *ponteiro_filho_no_pai;
    PESSOAS *chave_pai_desce = (PESSOAS *)malloc(sizeof(PESSOAS));
    Arv23_PESSOAS *filho_transferido = irmao_esq->dir;
    Arv23_PESSOAS *filho_original_unico = (filho->esq != NULL) ? filho->esq : filho->cen;

    if (pos_filho == 1)
    {
        *chave_pai_desce = *pai->info1;
        *pai->info1 = *irmao_esq->info2;
    }
    else
    {
        *chave_pai_desce = *pai->info2;
        *pai->info2 = *irmao_esq->info2;
    }

    *filho->info1 = *chave_pai_desce;
    filho->nInfo = 1;
    filho->esq = filho_transferido;
    filho->cen = filho_original_unico;
    filho->dir = NULL;

    irmao_esq->nInfo = 1;
    free(irmao_esq->info2);
    irmao_esq->info2 = NULL;
    irmao_esq->dir = NULL;

    free(chave_pai_desce);
    return OK;
}

// Redistribuição com irmão direito
StatusRemocao redistribuir_com_irmao_direito_PESSOAS(Arv23_PESSOAS **ponteiro_filho_no_pai, Arv23_PESSOAS *pai, Arv23_PESSOAS *irmao_dir, int pos_filho)
{
    Arv23_PESSOAS *filho = *ponteiro_filho_no_pai;
    PESSOAS *chave_pai_desce = (PESSOAS *)malloc(sizeof(PESSOAS));
    Arv23_PESSOAS *filho_transferido = irmao_dir->esq;
    Arv23_PESSOAS *filho_original_unico = (filho->esq != NULL) ? filho->esq : filho->cen;

    if (pos_filho == 0)
    {
        *chave_pai_desce = *pai->info1;
        *pai->info1 = *irmao_dir->info1;
    }
    else
    {
        *chave_pai_desce = *pai->info2;
        *pai->info2 = *irmao_dir->info1;
    }

    *filho->info1 = *chave_pai_desce;
    filho->nInfo = 1;
    filho->esq = filho_original_unico;
    filho->cen = filho_transferido;
    filho->dir = NULL;

    *irmao_dir->info1 = *irmao_dir->info2;
    irmao_dir->nInfo = 1;
    free(irmao_dir->info2);
    irmao_dir->info2 = NULL;
    irmao_dir->esq = irmao_dir->cen;
    irmao_dir->cen = irmao_dir->dir;
    irmao_dir->dir = NULL;

    free(chave_pai_desce);
    return OK;
}

// Fusão com irmão esquerdo
StatusRemocao fundir_com_irmao_esquerdo_PESSOAS(Arv23_PESSOAS **ponteiro_filho_no_pai, Arv23_PESSOAS *pai, Arv23_PESSOAS *irmao_esq, int pos_filho)
{
    Arv23_PESSOAS *filho_underflow = *ponteiro_filho_no_pai;
    PESSOAS *chave_pai_desce = (PESSOAS *)malloc(sizeof(PESSOAS));
    StatusRemocao status_pai = OK;

    Arv23_PESSOAS *filho_u_remanescente = (filho_underflow->esq != NULL) ? filho_underflow->esq : filho_underflow->cen;
    Arv23_PESSOAS *irmao_e_esq = irmao_esq->esq;
    Arv23_PESSOAS *irmao_e_cen = irmao_esq->cen;

    if (pai->nInfo == 1)
    {
        *chave_pai_desce = *pai->info1;
        free(pai->info1);
        pai->info1 = NULL;
        pai->nInfo = 0;
        status_pai = UNDERFLOW;
    }
    else
    {
        if (pos_filho == 1)
        {
            *chave_pai_desce = *pai->info1;
            *pai->info1 = *pai->info2;
        }
        else
        {
            *chave_pai_desce = *pai->info2;
        }
        free(pai->info2);
        pai->info2 = NULL;
        pai->nInfo = 1;
    }

    irmao_esq->info2 = chave_pai_desce;
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

    free(filho_underflow->info1);
    free(filho_underflow);
    return status_pai;
}

// Fusão com irmão direito
StatusRemocao fundir_com_irmao_direito_PESSOAS(Arv23_PESSOAS **ponteiro_filho_no_pai, Arv23_PESSOAS *pai, Arv23_PESSOAS *irmao_dir, int pos_filho)
{
    Arv23_PESSOAS *filho_underflow = *ponteiro_filho_no_pai;
    PESSOAS *chave_pai_desce = (PESSOAS *)malloc(sizeof(PESSOAS));
    StatusRemocao status_pai = OK;

    Arv23_PESSOAS *filho_u_remanescente = (filho_underflow->esq != NULL) ? filho_underflow->esq : filho_underflow->cen;
    Arv23_PESSOAS *irmao_d_esq = irmao_dir->esq;
    Arv23_PESSOAS *irmao_d_cen = irmao_dir->cen;

    if (pai->nInfo == 1)
    {
        *chave_pai_desce = *pai->info1;
        free(pai->info1);
        pai->info1 = NULL;
        pai->nInfo = 0;
        status_pai = UNDERFLOW;
    }
    else
    {
        if (pos_filho == 0)
        {
            *chave_pai_desce = *pai->info1;
            *pai->info1 = *pai->info2;
        }
        else
        {
            *chave_pai_desce = *pai->info2;
        }
        free(pai->info2);
        pai->info2 = NULL;
        pai->nInfo = 1;
    }

    irmao_dir->info2 = (PESSOAS *)malloc(sizeof(PESSOAS));
    *irmao_dir->info2 = *irmao_dir->info1;
    *irmao_dir->info1 = *chave_pai_desce;
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

    free(filho_underflow->info1);
    free(filho_underflow);
    free(chave_pai_desce);
    return status_pai;
}

// Tratamento de underflow
StatusRemocao tratar_underflow_PESSOAS(Arv23_PESSOAS **ponteiro_filho_no_pai, Arv23_PESSOAS *pai)
{
    StatusRemocao status_final = OK;
    Arv23_PESSOAS *filho_com_underflow = *ponteiro_filho_no_pai;

    if (filho_com_underflow == NULL || filho_com_underflow->nInfo > 0)
    {
        status_final = OK;
    }
    else if (pai == NULL)
    {
        status_final = UNDERFLOW;
    }
    else
    {
        Arv23_PESSOAS *irmao_esq = NULL;
        Arv23_PESSOAS *irmao_dir = NULL;
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
            if (pai->nInfo == 2) irmao_dir = pai->dir;
        }
        else if (pai->nInfo == 2 && pai->dir == filho_com_underflow)
        {
            pos_filho = 2;
            irmao_esq = pai->cen;
        }

        if (irmao_dir != NULL && irmao_dir->nInfo == 2)
            status_final = redistribuir_com_irmao_direito_PESSOAS(ponteiro_filho_no_pai, pai, irmao_dir, pos_filho);
        else if (irmao_esq != NULL && irmao_esq->nInfo == 2)
            status_final = redistribuir_com_irmao_esquerdo_PESSOAS(ponteiro_filho_no_pai, pai, irmao_esq, pos_filho);
        else if (irmao_dir != NULL && irmao_dir->nInfo == 1)
            status_final = fundir_com_irmao_direito_PESSOAS(ponteiro_filho_no_pai, pai, irmao_dir, pos_filho);
        else if (irmao_esq != NULL && irmao_esq->nInfo == 1)
            status_final = fundir_com_irmao_esquerdo_PESSOAS(ponteiro_filho_no_pai, pai, irmao_esq, pos_filho);
        else
        {
            fprintf(stderr, "Erro critico: Nao foi possivel tratar underflow.\n");
            status_final = OK;
        }
    }
    return status_final;
}

//==============================================================================
// FUNÇÕES DE GERENCIAMENTO DE MEMÓRIA
//==============================================================================

// Liberação de memória
void libera_arvore_PESSOAS(Arv23_PESSOAS **raiz)
{
    if (raiz != NULL && *raiz != NULL)
    {
        libera_arvore_PESSOAS(&(*raiz)->esq);
        libera_arvore_PESSOAS(&(*raiz)->cen);
        libera_arvore_PESSOAS(&(*raiz)->dir);
        if ((*raiz)->info1)
            free((*raiz)->info1);
        if ((*raiz)->info2)
            free((*raiz)->info2);
        free(*raiz);
        *raiz = NULL;
    }
}
//==============================================================================
// FUNÇÕES DE EXIBIÇÃO E PERCURSO DA ÁRVORE 2-3
//==============================================================================

void imprimir_dados_PESSOAS(PESSOAS *pessoa)
{
    if (pessoa != NULL)
    {
        printf("CPF: %s\n", pessoa->CPF);
        printf("Nome: %s\n", pessoa->nome_pessoa);
        printf("CEP Cidade Natal: %s\n", pessoa->cep_city_natal);
        printf("CEP Cidade Atual: %s\n", pessoa->cep_city_atual);
        printf("Data de Nascimento: %02d/%02d/%04d\n",
               pessoa->data_nasc.dia, pessoa->data_nasc.mes, pessoa->data_nasc.ano);
    }
}

// Impressão em ordem
void imprime_23_em_ordem_PESSOAS(Arv23_PESSOAS *raiz)
{
    if (raiz != NULL)
    {

        imprime_23_em_ordem_PESSOAS(raiz->esq);
        imprimir_dados_PESSOAS(raiz->info1);
        imprime_23_em_ordem_PESSOAS(raiz->cen);
        if (raiz->nInfo == 2)
        {
            imprimir_dados_PESSOAS(raiz->info2);
            imprime_23_em_ordem_PESSOAS(raiz->dir);
        }
    }
}

// Impressão visual
void imprime_arvore_visual_PESSOAS(Arv23_PESSOAS *raiz, char *prefixo, int eh_ultimo, int eh_raiz)
{
    if (raiz != NULL)
    {
        printf("%s", prefixo);
        printf("%s", eh_raiz ? " Raiz--> " : (eh_ultimo ? "`------ " : "+------ "));

        if (raiz->nInfo == 1)
            printf("[%s]\n", raiz->info1->CPF);
        else if (raiz->nInfo == 2)
            printf("[%s, %s]\n", raiz->info1->CPF, raiz->info2->CPF);

        char novo_prefixo[1024];
        sprintf(novo_prefixo, "%s%s", prefixo, eh_raiz ? "         " : (eh_ultimo ? "         " : "|        "));

        int num_filhos = 0;
        if (raiz->esq) num_filhos++;
        if (raiz->cen) num_filhos++;
        if (raiz->dir) num_filhos++;

        int filhos_impressos = 0;
        if (raiz->esq)
            imprime_arvore_visual_PESSOAS(raiz->esq, novo_prefixo, ++filhos_impressos == num_filhos, 0);
        if (raiz->cen)
            imprime_arvore_visual_PESSOAS(raiz->cen, novo_prefixo, ++filhos_impressos == num_filhos, 0);
        if (raiz->dir)
            imprime_arvore_visual_PESSOAS(raiz->dir, novo_prefixo, ++filhos_impressos == num_filhos, 0);
    }
}
