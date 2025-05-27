#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../Arv_23_H/STRUCTS_23.h"
#include "../Arv_23_H/Cidades_23.h"
#include "../Arv_23_H/utilitarios_23.h"

// Prototipos das funcoes
int cadastra_cidade(Arv23_CIDADES **raiz);
Arv23_CIDADES *cria_no_cidade(CIDADES info, Arv23_CIDADES *F_esq, Arv23_CIDADES *F_cen);
void adiciona_infos_cidade(Arv23_CIDADES **no, CIDADES info, Arv23_CIDADES *Sub_Arv_Info);
Arv23_CIDADES *quebra_no_cidade(Arv23_CIDADES **no, CIDADES info, CIDADES *sobe, Arv23_CIDADES *F_dir);
int insere_23_cidade(Arv23_CIDADES **raiz, CIDADES valor);
int insere_23_recursivo_cidade(Arv23_CIDADES **raiz, CIDADES valor, Arv23_CIDADES *pai, CIDADES *info_sobe, Arv23_CIDADES **filho_dir);
void libera_arvore_cidade(Arv23_CIDADES **raiz);
void libera_arvore_cep(Arv23_CEP **raiz);
int eh_folha_cidade(Arv23_CIDADES *no);
Arv23_CIDADES *buscar_menor_elemento_cidade(Arv23_CIDADES *no);
StatusRemocao tratar_underflow_cidade(Arv23_CIDADES **ponteiro_filho_no_pai, Arv23_CIDADES *pai);
StatusRemocao redistribuir_com_irmao_esquerdo(Arv23_CIDADES **ponteiro_filho_no_pai, Arv23_CIDADES *pai, Arv23_CIDADES *irmao_esq, int pos_filho);
StatusRemocao redistribuir_com_irmao_direito(Arv23_CIDADES **ponteiro_filho_no_pai, Arv23_CIDADES *pai, Arv23_CIDADES *irmao_dir, int pos_filho);
StatusRemocao fundir_com_irmao_esquerdo(Arv23_CIDADES **ponteiro_filho_no_pai, Arv23_CIDADES *pai, Arv23_CIDADES *irmao_esq, int pos_filho);
StatusRemocao fundir_com_irmao_direito(Arv23_CIDADES **ponteiro_filho_no_pai, Arv23_CIDADES *pai, Arv23_CIDADES *irmao_dir, int pos_filho);
StatusRemocao remover_23_recursivo_cidade(Arv23_CIDADES **ponteiro_no_atual, CIDADES valor);
int remover_23(Arv23_CIDADES **raiz, CIDADES valor);

// Funçao para cadastrar cidade (sem alocaçao dinâmica para CIDADES)
int cadastra_cidade(Arv23_CIDADES **raiz)
{
    CIDADES novaCidade;
    char nome_cidade[100];
    int populacao_city, retorno;

    printf("Digite o nome da cidade: ");
    ler_string_simples(nome_cidade, sizeof(nome_cidade));
    printf("Digite a populacao da cidade: ");
    populacao_city = digitar_int();

    if (nome_cidade[0] == '\0') {
        printf("Erro: Nome da cidade nao pode ser vazio.\n");
        return 0;
    }

    strcpy(novaCidade.nome_cidade, nome_cidade);
    novaCidade.populacao_city = populacao_city;
    novaCidade.arv_cep = NULL;

    if (insere_23_cidade(raiz, novaCidade)) {
        printf("Cidade %s cadastrada com sucesso.\n", novaCidade.nome_cidade);
        return 1;
    } else {
        printf("Falha ao cadastrar a cidade %s.\n", novaCidade.nome_cidade);
        return 0;
    }
}

// Cria nó (sem alocaçao para info1/info2)
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

// Adiciona infos (copia estruturas CIDADES)
void adiciona_infos_cidade(Arv23_CIDADES **no, CIDADES info, Arv23_CIDADES *Sub_Arv_Info)
{
    Arv23_CIDADES *no_atual = *no;
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
    Arv23_CIDADES *no_atual = *no;
    Arv23_CIDADES *maior = NULL;
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

// Inserçao ajustada
int insere_23_cidade(Arv23_CIDADES **raiz, CIDADES valor)
{
    CIDADES sobe;
    Arv23_CIDADES *maiorNo = NULL;
    int sucesso = 0;

    if (raiz != NULL)
    {
        sucesso = insere_23_recursivo_cidade(raiz, valor, NULL, &sobe, &maiorNo);

        if (maiorNo != NULL)
        {
            printf("Raiz original quebrou. Criando nova raiz com cidade %s.\n", sobe.nome_cidade);
            Arv23_CIDADES *nova_raiz = cria_no_cidade(sobe, *raiz, maiorNo);
            if (nova_raiz != NULL)
            {
                *raiz = nova_raiz;
            }
            else
            {
                fprintf(stderr, "Erro ao criar nova raiz!\n");
                sucesso = 0;
                if (maiorNo) free(maiorNo);
            }
        }
    }
    return sucesso;
}

int insere_23_recursivo_cidade(Arv23_CIDADES **raiz, CIDADES valor, Arv23_CIDADES *pai, CIDADES *sobe, Arv23_CIDADES **maiorNo)
{
    (void)pai; // Silencia warning
    int sucesso = 0;
    Arv23_CIDADES *no_atual = *raiz;

    if (no_atual == NULL)
    {
        *raiz = cria_no_cidade(valor, NULL, NULL);
        *maiorNo = NULL;
        memset(sobe, 0, sizeof(CIDADES)); // Inicializa sobe
        sucesso = (*raiz != NULL);
    }
    else if (strcasecmp(valor.nome_cidade, no_atual->info1.nome_cidade) == 0 ||
             (no_atual->nInfo == 2 && strcasecmp(valor.nome_cidade, no_atual->info2.nome_cidade) == 0))
    {
        printf("Cidade %s ja existe na arvore!\n", valor.nome_cidade);
        *maiorNo = NULL;
        memset(sobe, 0, sizeof(CIDADES));
        sucesso = 0;
    }
    else
    {
        if (eh_folha(no_atual))
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
            {
                proximo_filho = &(no_atual->esq);
            }
            else if (no_atual->nInfo == 1 || strcasecmp(valor.nome_cidade, no_atual->info2.nome_cidade) < 0)
            {
                proximo_filho = &(no_atual->cen);
            }
            else
            {
                proximo_filho = &(no_atual->dir);
            }
            int sucesso_rec = insere_23_recursivo_cidade(proximo_filho, valor, no_atual, sobe, maiorNo);

            if (*maiorNo != NULL)
            {
                CIDADES chave_que_subiu = *sobe;
                Arv23_CIDADES *novo_filho_dir = *maiorNo;
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
            {
                sucesso = sucesso_rec;
            }
        }
    }
    return sucesso;
}

// Libera árvore (sem liberar info1/info2 separadamente)
void libera_arvore_cidade(Arv23_CIDADES **raiz)
{
    if (raiz != NULL && *raiz != NULL)
    {
        libera_arvore_cidade(&(*raiz)->esq);
        libera_arvore_cidade(&(*raiz)->cen);
        libera_arvore_cidade(&(*raiz)->dir);
        libera_arvore_cep(&(*raiz)->info1.arv_cep);
        if ((*raiz)->nInfo == 2) libera_arvore_cep(&(*raiz)->info2.arv_cep);
        free(*raiz);
        *raiz = NULL;
    }
}

//================ VERIFICA FOLHA ==================
int eh_folha_cidade(Arv23_CIDADES *no)
{
    int resultado = 0;
    if (no != NULL)
    {
        resultado = (no->esq == NULL && no->cen == NULL && no->dir == NULL);
    }
    return resultado;
}

//================ BUSCAR MENOR ==================
Arv23_CIDADES *buscar_menor_elemento_cidade(Arv23_CIDADES *no)
{
    Arv23_CIDADES *resultado = NULL;
    if (no != NULL)
    {
        Arv23_CIDADES *atual = no;
        while (atual != NULL && atual->esq != NULL)
        {
            atual = atual->esq;
        }
        resultado = atual;
    }
    return resultado;
}

//================ FUNCOES AUXILIARES REMOCAO ==================

// Funcao principal para tratar underflow
StatusRemocao tratar_underflow_cidade(Arv23_CIDADES **ponteiro_filho_no_pai, Arv23_CIDADES *pai)
{
    StatusRemocao status_final = OK;
    Arv23_CIDADES *filho_com_underflow = *ponteiro_filho_no_pai;

    if (filho_com_underflow == NULL || filho_com_underflow->nInfo > 0)
    {
        status_final = OK;
    }
    else if (pai == NULL)
    {
        status_final = UNDERFLOW; // Underflow na raiz sera tratado em remover_23
    }
    else
    {
        printf("Tratando underflow no filho (nInfo=0, %p) do pai [%s%s%s] (%p)...\n",
               (void *)filho_com_underflow,
               pai->info1.nome_cidade, pai->nInfo == 2 ? ", " : " ", pai->nInfo == 2 ? pai->info2.nome_cidade : "",(void *)pai);

        Arv23_CIDADES *irmao_esq = NULL;
        Arv23_CIDADES *irmao_dir = NULL;
        int pos_filho = -1;

        // Identifica a posicao do filho e seus irmaos
        if (pai->esq == filho_com_underflow)
        {
            pos_filho = 0;
            irmao_dir = pai->cen;
            printf("Filho em underflow e o ESQUERDO. Irmao direito: %p\n", (void *)irmao_dir);
        }
        else if (pai->cen == filho_com_underflow)
        {
            pos_filho = 1;
            irmao_esq = pai->esq;
            if (pai->nInfo == 2)
            {
                irmao_dir = pai->dir;
            }
            printf("Filho em underflow e o CENTRAL. Irmao esquerdo: %p, Irmao direito: %p\n", (void *)irmao_esq, (void *)irmao_dir);
        }
        else if (pai->nInfo == 2 && pai->dir == filho_com_underflow)
        {
            pos_filho = 2;
            irmao_esq = pai->cen;
            printf("Filho em underflow e o DIREITO. Irmao esquerdo: %p\n", (void *)irmao_esq);
        }

        // Prioridade: Redistribuicao
        if (irmao_dir != NULL && irmao_dir->nInfo == 2)
        {
            printf("Tentando redistribuicao com irmao direito [%d, %d]\n", irmao_dir->info1, irmao_dir->info2);
            status_final = redistribuir_com_irmao_direito(ponteiro_filho_no_pai, pai, irmao_dir, pos_filho);
        }
        else if (irmao_esq != NULL && irmao_esq->nInfo == 2)
        {
            printf("Tentando redistribuicao com irmao esquerdo [%d, %d]\n", irmao_esq->info1, irmao_esq->info2);
            status_final = redistribuir_com_irmao_esquerdo(ponteiro_filho_no_pai, pai, irmao_esq, pos_filho);
        }
        // Se nao foi possivel redistribuir, tenta fusao
        else if (irmao_dir != NULL && irmao_dir->nInfo == 1)
        {
            printf("Tentando fusao com irmao direito [%d]\n", irmao_dir->info1);
            status_final = fundir_com_irmao_direito(ponteiro_filho_no_pai, pai, irmao_dir, pos_filho);
        }
        else if (irmao_esq != NULL && irmao_esq->nInfo == 1)
        {
            printf("Tentando fusao com irmao esquerdo [%d]\n", irmao_esq->info1);
            status_final = fundir_com_irmao_esquerdo(ponteiro_filho_no_pai, pai, irmao_esq, pos_filho);
        }
        // Caso critico: Nao ha irmaos ou os irmaos nao podem ajudar (nao deveria ocorrer em arvore valida, exceto raiz)
        else
        {
            // Este caso pode ocorrer se o pai for um 2-no e o outro filho ja foi fundido/removido
            // Se o pai tem nInfo=0, o underflow sera propagado.
            // Se o pai tem nInfo=1, e este filho e o unico, o pai entrara em underflow.
            // A logica de fusao ja deve ter tratado isso e retornado UNDERFLOW.
            // Se chegamos aqui, algo esta errado.
            fprintf(stderr, "Erro critico: Nao foi possivel tratar underflow por redistribuicao ou fusao. Pai: [%s%s%s], Filho pos: %d, irmao_esq: %p, irmao_dir: %p\n",
                    pai->info1.nome_cidade, 
                    pai->nInfo == 2 ? ", " : " ", 
                    pai->nInfo == 2 ? pai->info2.nome_cidade : "", 
                    pos_filho, (void *)irmao_esq, (void *)irmao_dir);
            // Retorna OK para evitar loop, mas a arvore pode estar invalida.
            status_final = OK;
        }
    }

    return status_final;
}

// Redistribui do irmao esquerdo (3-no) para o filho (underflow)
StatusRemocao redistribuir_com_irmao_esquerdo(Arv23_CIDADES **ponteiro_filho_no_pai, Arv23_CIDADES *pai, Arv23_CIDADES *irmao_esq, int pos_filho)
{
    printf("Redistribuindo com irmao esquerdo...\n");
    Arv23_CIDADES *filho = *ponteiro_filho_no_pai;
    CIDADES chave_pai_desce;
    Arv23_CIDADES *filho_transferido = irmao_esq->dir;
    Arv23_CIDADES *filho_original_unico = (filho->esq != NULL) ? filho->esq : filho->cen;

    if (pos_filho == 1)
    {
        chave_pai_desce = pai->info1;
        pai->info1 = irmao_esq->info2;
    }
    else
    { // pos_filho == 2
        chave_pai_desce = pai->info2;
        pai->info2 = irmao_esq->info2;
    }

    filho->info1 = chave_pai_desce;
    filho->nInfo = 1;
    filho->esq = filho_transferido;
    filho->cen = filho_original_unico;
    filho->dir = NULL;

    irmao_esq->nInfo = 1;
    memset(&irmao_esq->info2, 0, sizeof(CIDADES));// zerar os dados da info2
    irmao_esq->dir = NULL; // Filho dir foi transferido

    return OK;
}

// Redistribui do irmao direito (3-no) para o filho (underflow)
StatusRemocao redistribuir_com_irmao_direito(Arv23_CIDADES **ponteiro_filho_no_pai, Arv23_CIDADES *pai, Arv23_CIDADES *irmao_dir, int pos_filho)
{
    printf("Redistribuindo com irmao direito...\n");
    Arv23_CIDADES *filho = *ponteiro_filho_no_pai;
    CIDADES chave_pai_desce;
    Arv23_CIDADES *filho_transferido = irmao_dir->esq;
    Arv23_CIDADES *filho_original_unico = (filho->esq != NULL) ? filho->esq : filho->cen;

    if (pos_filho == 0)
    {
        chave_pai_desce = pai->info1;
        pai->info1 = irmao_dir->info1;
    }
    else
    { // pos_filho == 1
        chave_pai_desce = pai->info2;
        pai->info2 = irmao_dir->info1;
    }

    filho->info1 = chave_pai_desce;
    filho->nInfo = 1;
    filho->esq = filho_original_unico;
    filho->cen = filho_transferido;
    filho->dir = NULL;

    irmao_dir->info1 = irmao_dir->info2;
    irmao_dir->nInfo = 1;
    memset(&irmao_dir->info2, 0, sizeof(CIDADES));//memset serve para inicializar ou limpar blocos de memória, geralmente colocando tudo em zero.
    irmao_dir->esq = irmao_dir->cen;
    irmao_dir->cen = irmao_dir->dir;
    irmao_dir->dir = NULL;

    return OK;
}

// Funde o filho (underflow) com o irmao esquerdo (2-no)
StatusRemocao fundir_com_irmao_esquerdo(Arv23_CIDADES **ponteiro_filho_no_pai, Arv23_CIDADES *pai, Arv23_CIDADES *irmao_esq, int pos_filho)
{
    printf("Fundindo filho (%p) com irmao esquerdo [%d] (%p)...\n", (void *)*ponteiro_filho_no_pai, irmao_esq->info1, (void *)irmao_esq);
    Arv23_CIDADES *filho_underflow = *ponteiro_filho_no_pai;
    CIDADES chave_pai_desce;
    StatusRemocao status_pai = OK;

    Arv23_CIDADES *filho_u_remanescente = (filho_underflow->esq != NULL) ? filho_underflow->esq : filho_underflow->cen;
    Arv23_CIDADES *irmao_e_esq = irmao_esq->esq;
    Arv23_CIDADES *irmao_e_cen = irmao_esq->cen;

    // Determina a chave que desce do pai e ajusta o pai
    if (pai->nInfo == 1)
    {
        chave_pai_desce = pai->info1;
        pai->nInfo = 0; // Pai entra em underflow
        memset(&pai->info1, 0, sizeof(CIDADES));  // Limpa todos os campos adequadamente //verificar isso melhor
        status_pai = UNDERFLOW;
        printf("Pai [2-no] entra em underflow. Chave descendo: %d\n", chave_pai_desce);
    }
    else
    { // Pai era 3-no, ficara 2-no
        if (pos_filho == 1)
        { // Filho era o central (pai->cen)
            chave_pai_desce = pai->info1;
            pai->info1 = pai->info2;
            printf("Pai [3-no], filho=cen. Chave descendo: %d. Pai vira [%d]\n", chave_pai_desce, pai->info1);
        }
        else
        { // pos_filho == 2 (Filho era o direito, pai->dir)
            chave_pai_desce = pai->info2;
            printf("Pai [3-no], filho=dir. Chave descendo: %d. Pai vira [%d]\n", chave_pai_desce, pai->info1);
        }
        memset(&pai->info2, 0, sizeof(CIDADES));
        pai->nInfo = 1;
        status_pai = OK;
    }

    // Realiza a fusao no irmao esquerdo (que se torna um 3-no)
    irmao_esq->info2 = chave_pai_desce;
    irmao_esq->nInfo = 2;
    irmao_esq->esq = irmao_e_esq;
    irmao_esq->cen = irmao_e_cen;
    irmao_esq->dir = filho_u_remanescente;
    printf("Irmao esquerdo [%d] fundido -> [%d, %d]. Filhos: %p, %p, %p\n",
           irmao_esq->info1, irmao_esq->info1, irmao_esq->info2,
           (void *)irmao_esq->esq, (void *)irmao_esq->cen, (void *)irmao_esq->dir);

    // Ajusta os ponteiros do PAI para refletir a fusao
    // O irmao esquerdo (que absorveu o filho) permanece onde estava (pai->esq ou pai->cen)
    if (pos_filho == 1)
    { // Filho era pai->cen. Irmao esq era pai->esq.
        // Remove o filho do centro, movendo o da direita (se existir) para o centro.
        pai->cen = pai->dir;
        pai->dir = NULL;
        printf("Ajuste pai (filho=cen): pai->cen=%p, pai->dir=NULL\n", (void *)pai->cen);
    }
    else
    { // pos_filho == 2. Filho era pai->dir. Irmao esq era pai->cen.
        // Remove o filho da direita.
        pai->dir = NULL;
        printf("Ajuste pai (filho=dir): pai->dir=NULL\n");
    }
    // Se pai entrou em underflow (era 2-no), ele tera apenas pai->esq apontando para irmao_esq
    if (status_pai == UNDERFLOW)
    {
        // O unico filho remanescente e o irmao_esq, que esta em pai->esq
        pai->cen = NULL;
        pai->dir = NULL;
        printf("Ajuste pai (underflow): pai->cen=NULL, pai->dir=NULL\n");
    }

    printf("Liberando no em underflow (%p) (anteriormente filho na pos %d do pai)\n", (void *)filho_underflow, pos_filho);
    free(filho_underflow);
    // *ponteiro_filho_no_pai = NULL; // Nao necessario, ponteiro no pai foi ajustado

    printf("Fusao com irmao esquerdo concluida. No resultante [%d, %d]. Status pai: %s\n",
           irmao_esq->info1, irmao_esq->info2, status_pai == OK ? "OK" : "UNDERFLOW");

    return status_pai;
}

// Funde o filho (underflow) com o irmao direito (2-no)
StatusRemocao fundir_com_irmao_direito(Arv23_CIDADES **ponteiro_filho_no_pai, Arv23_CIDADES *pai, Arv23_CIDADES *irmao_dir, int pos_filho)
{
    printf("Fundindo filho (%p) com irmao direito [%d] (%p)...\n", (void *)*ponteiro_filho_no_pai, irmao_dir->info1, (void *)irmao_dir);
    Arv23_CIDADES *filho_underflow = *ponteiro_filho_no_pai;
    CIDADES chave_pai_desce;
    StatusRemocao status_pai = OK;

    Arv23_CIDADES *filho_u_remanescente = (filho_underflow->esq != NULL) ? filho_underflow->esq : filho_underflow->cen;
    Arv23_CIDADES *irmao_d_esq = irmao_dir->esq;
    Arv23_CIDADES *irmao_d_cen = irmao_dir->cen;

    // Determina a chave que desce do pai e ajusta o pai
    if (pai->nInfo == 1)
    {
        chave_pai_desce = pai->info1;
        pai->nInfo = 0;
        memset(&pai->info1, 0, sizeof(CIDADES));
        status_pai = UNDERFLOW;
        printf("Pai [2-no] entra em underflow. Chave descendo: %d\n", chave_pai_desce);
    }
    else
    { // Pai era 3-no, ficara 2-no
        if (pos_filho == 0)
        { // Filho era o esquerdo (pai->esq)
            chave_pai_desce = pai->info1;
            pai->info1 = pai->info2;
            printf("Pai [3-no], filho=esq. Chave descendo: %d. Pai vira [%d]\n", chave_pai_desce, pai->info1);
        }
        else
        { // pos_filho == 1 (Filho era o central, pai->cen)
            chave_pai_desce = pai->info2;
            printf("Pai [3-no], filho=cen. Chave descendo: %d. Pai vira [%d]\n", chave_pai_desce, pai->info1);
        }
        memset(&pai->info2, 0, sizeof(CIDADES));
        pai->nInfo = 1;
        status_pai = OK;
    }

    // Realiza a fusao no irmao direito (que se torna um 3-no)
    irmao_dir->info2 = irmao_dir->info1;
    irmao_dir->info1 = chave_pai_desce;
    irmao_dir->nInfo = 2;
    irmao_dir->esq = filho_u_remanescente;
    irmao_dir->cen = irmao_d_esq;
    irmao_dir->dir = irmao_d_cen;
    printf("Irmao direito [%d] fundido -> [%d, %d]. Filhos: %p, %p, %p\n",
           irmao_dir->info2, // info original
           irmao_dir->info1, irmao_dir->info2,
           (void *)irmao_dir->esq, (void *)irmao_dir->cen, (void *)irmao_dir->dir);

    // Ajusta os ponteiros do PAI para refletir a fusao
    // O irmao direito (que absorveu o filho) substitui a si mesmo e ao filho.
    if (pos_filho == 0)
    { // Filho era pai->esq. Irmao dir era pai->cen.
        // Remove o filho da esquerda, movendo o irmao (centro) para a esquerda
        // e o da direita (se existir) para o centro.
        pai->esq = pai->cen; // irmao_dir estava em pai->cen
        pai->cen = pai->dir;
        pai->dir = NULL;
        printf("Ajuste pai (filho=esq): pai->esq=%p, pai->cen=%p, pai->dir=NULL\n", (void *)pai->esq, (void *)pai->cen);
    }
    else
    { // pos_filho == 1. Filho era pai->cen. Irmao dir era pai->dir.
        // Remove o filho do centro, movendo o irmao (direita) para o centro.
        pai->cen = pai->dir; // irmao_dir estava em pai->dir
        pai->dir = NULL;
        printf("Ajuste pai (filho=cen): pai->cen=%p, pai->dir=NULL\n", (void *)pai->cen);
    }
    // Se pai entrou em underflow (era 2-no), ele tera apenas pai->esq apontando para irmao_dir
    if (status_pai == UNDERFLOW)
    {
        // O unico filho remanescente e o irmao_dir, que agora esta em pai->esq
        pai->cen = NULL;
        pai->dir = NULL;
        printf("Ajuste pai (underflow): pai->esq=%p, pai->cen=NULL, pai->dir=NULL\n", (void *)pai->esq);
    }

    printf("Liberando no em underflow (%p) (anteriormente filho na pos %d do pai)\n", (void *)filho_underflow, pos_filho);
    free(filho_underflow);
    // *ponteiro_filho_no_pai = NULL; // Nao necessario, ponteiro no pai foi ajustado

    printf("Fusao com irmao direito concluida. No resultante [%d, %d]. Status pai: %s\n",
           irmao_dir->info1, irmao_dir->info2, status_pai == OK ? "OK" : "UNDERFLOW");

    return status_pai;
}

//================ REMOCAO (Recursiva) ==================
StatusRemocao remover_23_recursivo_cidade(Arv23_CIDADES **ponteiro_no_atual, CIDADES valor)
{
    StatusRemocao status_final = OK;
    Arv23_CIDADES *no_atual = *ponteiro_no_atual;

    if (no_atual == NULL)
    {
        printf("Valor %d nao encontrado na subarvore.\n", valor);
        status_final = OK;
    }
    else
    {
        Arv23_CIDADES **proximo_ponteiro_recursao = NULL;
        int valor_encontrado_neste_no = 0;
        int indice_valor_removido = -1;

        if (strcasecmp(valor.nome_cidade, no_atual->info1.nome_cidade) == 0)
        {
            valor_encontrado_neste_no = 1;
            indice_valor_removido = 0;
        }
        else if (no_atual->nInfo == 2 && strcasecmp(valor.nome_cidade, no_atual->info2.nome_cidade) == 0)
        {
            valor_encontrado_neste_no = 1;
            indice_valor_removido = 1;
        }
        else if (strcasecmp(valor.nome_cidade, no_atual->info1.nome_cidade) < 0)
        {
            proximo_ponteiro_recursao = &(no_atual->esq);
        }
        else if (no_atual->nInfo == 1 || strcasecmp(valor.nome_cidade, no_atual->info2.nome_cidade) < 0)
        {
            proximo_ponteiro_recursao = &(no_atual->cen);
        }
        else
        {
            proximo_ponteiro_recursao = &(no_atual->dir);
        }

        if (valor_encontrado_neste_no)
        {
            printf("Cidade '%s' encontrada no no ['%s'%s'%s']\n", 
                   valor.nome_cidade, 
                   no_atual->info1.nome_cidade,
                   no_atual->nInfo == 2 ? ", " : " ", 
                   no_atual->nInfo == 2 ? no_atual->info2.nome_cidade : "");

            if (eh_folha_cidade(no_atual))
            {
                printf("Removendo %d de um no FOLHA.\n", valor);
                if (no_atual->nInfo == 2)
                {
                    printf("Folha e 3-no. Remocao simples.\n");
                    if (indice_valor_removido == 0)
                    {
                        libera_arvore_cep(&no_atual->info1.arv_cep);////////// OLHE PRA CAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA
                        no_atual->info1 = no_atual->info2;
                    }
                    else{
                        libera_arvore_cep(&no_atual->info2.arv_cep);
                    }
                    memset(&no_atual->info2, 0, sizeof(CIDADES));
                    no_atual->nInfo = 1;
                    status_final = OK;
                }
                else
                {
                    printf("Folha e 2-no. Remocao causa UNDERFLOW.\n");
                    no_atual->nInfo = 0;
                    memset(&no_atual->info1, 0, sizeof(CIDADES));
                    status_final = UNDERFLOW;
                }
            }
            else
            {
                printf("Removendo %d de um no INTERNO. Substituindo por sucessor.\n", valor);
                Arv23_CIDADES *sucessor_node = NULL;
                CIDADES valor_sucessor;
                Arv23_CIDADES **ponteiro_subarvore_sucessor;

                if (indice_valor_removido == 0)
                {
                    sucessor_node = buscar_menor_elemento_cidade(no_atual->cen);
                    ponteiro_subarvore_sucessor = &(no_atual->cen);
                }
                else
                {
                    sucessor_node = buscar_menor_elemento_cidade(no_atual->dir);
                    ponteiro_subarvore_sucessor = &(no_atual->dir);
                }

                if (sucessor_node == NULL)
                {
                    fprintf(stderr, "Erro critico: Sucessor nao encontrado para valor %d no no interno!\n", valor);
                    status_final = OK;
                }
                else
                {
                    valor_sucessor = sucessor_node->info1;
                    printf("Sucessor encontrado: %d. Substituindo %d por %d no no atual.\n", valor_sucessor, valor, valor_sucessor);

                    if (indice_valor_removido == 0)
                    {
                        libera_arvore_cep(&no_atual->info1.arv_cep); // Libera a arvore de CEP da info1
                        no_atual->info1 = valor_sucessor;
                    }
                    else
                    {
                        libera_arvore_cep(&no_atual->info2.arv_cep); // Libera a arvore de CEP da info2
                        no_atual->info2 = valor_sucessor;
                    }

                    printf("Removendo recursivamente o sucessor %d da subarvore apropriada...\n", valor_sucessor);
                    StatusRemocao status_remocao_sucessor = remover_23_recursivo_cidade(ponteiro_subarvore_sucessor, valor_sucessor);

                    if (status_remocao_sucessor == UNDERFLOW)
                    {
                        printf("Underflow apos remover sucessor %d. Tratando na subarvore (pai=%p)...\n", valor_sucessor, (void *)no_atual);
                        status_final = tratar_underflow_cidade(ponteiro_subarvore_sucessor, no_atual);
                    }
                    else
                    {
                        status_final = status_remocao_sucessor;
                    }
                }
            }
        }
        else
        {
            // printf("Valor %d nao encontrado neste no. Descendo para a subarvore...\n", valor);
            StatusRemocao status_recursao = remover_23_recursivo_cidade(proximo_ponteiro_recursao, valor);

            if (status_recursao == UNDERFLOW)
            {
                printf("Underflow retornado da subarvore. Tratando no nivel atual (pai=%p)...\n", (void *)no_atual);
                status_final = tratar_underflow_cidade(proximo_ponteiro_recursao, no_atual);
            }
            else
            {
                status_final = status_recursao;
            }
        }
    }

    return status_final;
}

//================ REMOCAO (Principal - CORRIGIDO) ==================

int remover_23(Arv23_CIDADES **raiz, CIDADES valor)
{
    int sucesso = 0; // 0 para falha, 1 para sucesso

    if (raiz == NULL || *raiz == NULL)
    {
        printf("Arvore vazia. Nao e possivel remover %d.\n", valor);
        sucesso = 0;
    }
    else
    {
        printf("\n--- Iniciando remocao de %d ---\n", valor);
        // Chama a funcao recursiva, passando a raiz e NULL como pai inicial
        StatusRemocao status_geral = remover_23_recursivo_cidade(raiz, valor);

        // CORRECAO: Verifica se a raiz ficou vazia (nInfo=0) APOS a remocao recursiva,
        // independentemente do status retornado. Isso trata o caso onde a ultima fusao
        // ocorreu no nivel superior e deixou a raiz vazia, mas retornou OK, ou
        // se a remocao na propria raiz (sendo 2-no folha) causou underflow.
        if (*raiz != NULL && (*raiz)->nInfo == 0)
        {
            printf("Raiz (%p) entrou em underflow (nInfo=0). Ajustando a raiz da arvore...\n", (void *)*raiz);
            Arv23_CIDADES *raiz_antiga = *raiz;
            Arv23_CIDADES *nova_raiz = NULL;

            // A nova raiz e o unico filho nao-NULL da raiz antiga.
            // Apos uma fusao que causa underflow na raiz, ou remocao da unica chave da raiz,
            // ela tera apenas um filho, que deve estar em ->esq.
            if (raiz_antiga->esq != NULL)
            {
                nova_raiz = raiz_antiga->esq;
                printf("Nova raiz sera o filho esquerdo ['%s'%s'%s'] (%p) da antiga raiz.\n",
                       nova_raiz->info1.nome_cidade, 
                       nova_raiz->nInfo == 2 ? ", " : " ", 
                       nova_raiz->nInfo == 2 ? nova_raiz->info2.nome_cidade : "",
                       (void *)nova_raiz);
            }
            else if (raiz_antiga->cen != NULL)
            {
                // Este caso nao deveria ocorrer se a logica de fusao estiver correta.
                nova_raiz = raiz_antiga->cen;
                printf("Nova raiz sera o filho central ['%s'%s'%s'] (%p) da antiga raiz (Inesperado!).\n",
                       nova_raiz->info1.nome_cidade, 
                       nova_raiz->nInfo == 2 ? ", " : " ", 
                       nova_raiz->nInfo == 2 ? nova_raiz->info2.nome_cidade : "",
                       (void *)nova_raiz);
            }
            else
            {
                // Se nao tem nem esq nem cen, a arvore ficou vazia
                nova_raiz = NULL;
                printf("Arvore ficou vazia apos remocao da raiz.\n");
            }

            *raiz = nova_raiz; // Atualiza o ponteiro da raiz

            printf("Liberando no da raiz antiga em underflow (%p).\n", (void *)raiz_antiga);
            free(raiz_antiga);
            status_geral = OK; // Considera o underflow da raiz resolvido
        }

        // Define o sucesso baseado no status final E se a raiz e valida
        if (status_geral == OK)
        {
            printf("--- Remocao de %d concluida com sucesso ---\n", valor);
            sucesso = 1;
        }
        else
        {
            fprintf(stderr, "--- Erro: Remocao de %d falhou ou deixou a arvore invalida (raiz->nInfo=%d) ---\n",
                    valor, (*raiz != NULL ? (*raiz)->nInfo : -1));
            sucesso = 0;
        }
    }

    return sucesso;
}