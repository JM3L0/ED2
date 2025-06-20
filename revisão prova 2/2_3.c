#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// #include "../z_2-3_de_referencia/2-3_referencia.h"

// Estrutura do No da Arvore 2-3
typedef struct Arv23
{
    int info1, info2;
    int nInfo; // 0 (underflow temporario), 1 ou 2
    struct Arv23 *esq, *cen, *dir;
} Arv23;

// Enum para status da remocao
typedef enum {
    OK,
    UNDERFLOW
} StatusRemocao;


Arv23 *cria_no(int info, Arv23 *F_esq, Arv23 *F_cen);
int eh_folha(Arv23 *no);
void adiciona_infos(Arv23 **no, int info, Arv23 *Sub_Arv_Info);
Arv23 *quebra_no(Arv23 **no, int info, int *sobe, Arv23 *F_dir);
int insere_23_recursivo(Arv23 **raiz, int valor, int *sobe, Arv23 **maiorNo);
int insere_23(Arv23 **raiz, int valor);
void imprime_23_em_ordem(Arv23 *raiz);
void imprime_arvore_visual(Arv23 *raiz, char *prefixo, int eh_ultimo, int eh_raiz);
Arv23 *buscar_menor_elemento(Arv23 *no);
StatusRemocao remover_23_recursivo(Arv23 **ponteiro_no_atual, int valor);
int remover_23(Arv23 **raiz, int valor);
StatusRemocao tratar_underflow(Arv23 **ponteiro_filho_no_pai, Arv23 *pai);
StatusRemocao redistribuir_com_irmao_esquerdo(Arv23 **ponteiro_filho_no_pai, Arv23 *pai, Arv23 *irmao_esq, int pos_filho);
StatusRemocao redistribuir_com_irmao_direito(Arv23 **ponteiro_filho_no_pai, Arv23 *pai, Arv23 *irmao_dir, int pos_filho);
StatusRemocao fundir_com_irmao_esquerdo(Arv23 **ponteiro_filho_no_pai, Arv23 *pai, Arv23 *irmao_esq, int pos_filho);
StatusRemocao fundir_com_irmao_direito(Arv23 **ponteiro_filho_no_pai, Arv23 *pai, Arv23 *irmao_dir, int pos_filho);
void libera_arvore(Arv23 **raiz);


//================ CRIA NO ==================
Arv23 *cria_no(int info, Arv23 *F_esq, Arv23 *F_cen)
{
    Arv23 *no = (Arv23 *)malloc(sizeof(Arv23));
    if (no != NULL)
    {
        no->info1 = info;
        no->nInfo = 1;
        no->esq = F_esq;
        no->cen = F_cen;
        no->dir = NULL;
        no->info2 = 0;
    }
    return no;
}

//================ VERIFICA FOLHA ==================
int eh_folha(Arv23 *no)
{
    int resultado = 0;
    if (no != NULL)
    {
        resultado = (no->esq == NULL && no->cen == NULL && no->dir == NULL);
    }
    return resultado;
}

//================ BUSCAR MENOR ==================
Arv23 *buscar_menor_elemento(Arv23 *no)
{
    Arv23 *resultado = NULL;
    if (no != NULL)
    {
        Arv23 *atual = no;
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
StatusRemocao tratar_underflow(Arv23 **ponteiro_filho_no_pai, Arv23 *pai)
{
    StatusRemocao status_final = OK;
    Arv23 *filho_com_underflow = *ponteiro_filho_no_pai;

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
        printf("Tratando underflow no filho (nInfo=0, %p) do pai [%d%s%d] (%p)...\n",
               (void *)filho_com_underflow,
               pai->info1, pai->nInfo == 2 ? ", " : " ", pai->nInfo == 2 ? pai->info2 : -1,
               (void *)pai);

        Arv23 *irmao_esq = NULL;
        Arv23 *irmao_dir = NULL;
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
            fprintf(stderr, "Erro critico: Nao foi possivel tratar underflow por redistribuicao ou fusao. Pai: [%d%s%d], Filho pos: %d, irmao_esq: %p, irmao_dir: %p\n",
                    pai->info1, pai->nInfo == 2 ? ", " : " ", pai->nInfo == 2 ? pai->info2 : -1, pos_filho, (void *)irmao_esq, (void *)irmao_dir);
            // Retorna OK para evitar loop, mas a arvore pode estar invalida.
            status_final = OK;
        }
    }

    return status_final;
}

// Redistribui do irmao esquerdo (3-no) para o filho (underflow)
StatusRemocao redistribuir_com_irmao_esquerdo(Arv23 **ponteiro_filho_no_pai, Arv23 *pai, Arv23 *irmao_esq, int pos_filho)
{
    printf("Redistribuindo com irmao esquerdo...\n");
    Arv23 *filho = *ponteiro_filho_no_pai;
    int chave_pai_desce;
    Arv23 *filho_transferido = irmao_esq->dir;
    Arv23 *filho_original_unico = (filho->esq != NULL) ? filho->esq : filho->cen;

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
    irmao_esq->info2 = 0;
    irmao_esq->dir = NULL; // Filho dir foi transferido

    return OK;
}

// Redistribui do irmao direito (3-no) para o filho (underflow)
StatusRemocao redistribuir_com_irmao_direito(Arv23 **ponteiro_filho_no_pai, Arv23 *pai, Arv23 *irmao_dir, int pos_filho)
{
    printf("Redistribuindo com irmao direito...\n");
    Arv23 *filho = *ponteiro_filho_no_pai;
    int chave_pai_desce;
    Arv23 *filho_transferido = irmao_dir->esq;
    Arv23 *filho_original_unico = (filho->esq != NULL) ? filho->esq : filho->cen;

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
    irmao_dir->info2 = 0;
    irmao_dir->esq = irmao_dir->cen;
    irmao_dir->cen = irmao_dir->dir;
    irmao_dir->dir = NULL;

    return OK;
}

// Funde o filho (underflow) com o irmao esquerdo (2-no)
StatusRemocao fundir_com_irmao_esquerdo(Arv23 **ponteiro_filho_no_pai, Arv23 *pai, Arv23 *irmao_esq, int pos_filho)
{
    printf("Fundindo filho (%p) com irmao esquerdo [%d] (%p)...\n", (void *)*ponteiro_filho_no_pai, irmao_esq->info1, (void *)irmao_esq);
    Arv23 *filho_underflow = *ponteiro_filho_no_pai;
    int chave_pai_desce;
    StatusRemocao status_pai = OK;

    Arv23 *filho_u_remanescente = (filho_underflow->esq != NULL) ? filho_underflow->esq : filho_underflow->cen;
    Arv23 *irmao_e_esq = irmao_esq->esq;
    Arv23 *irmao_e_cen = irmao_esq->cen;

    // Determina a chave que desce do pai e ajusta o pai
    if (pai->nInfo == 1)
    {
        chave_pai_desce = pai->info1;
        pai->nInfo = 0; // Pai entra em underflow
        pai->info1 = 0;
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
        pai->info2 = 0;
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
StatusRemocao fundir_com_irmao_direito(Arv23 **ponteiro_filho_no_pai, Arv23 *pai, Arv23 *irmao_dir, int pos_filho)
{
    printf("Fundindo filho (%p) com irmao direito [%d] (%p)...\n", (void *)*ponteiro_filho_no_pai, irmao_dir->info1, (void *)irmao_dir);
    Arv23 *filho_underflow = *ponteiro_filho_no_pai;
    int chave_pai_desce;
    StatusRemocao status_pai = OK;

    Arv23 *filho_u_remanescente = (filho_underflow->esq != NULL) ? filho_underflow->esq : filho_underflow->cen;
    Arv23 *irmao_d_esq = irmao_dir->esq;
    Arv23 *irmao_d_cen = irmao_dir->cen;

    // Determina a chave que desce do pai e ajusta o pai
    if (pai->nInfo == 1)
    {
        chave_pai_desce = pai->info1;
        pai->nInfo = 0;
        pai->info1 = 0;
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
        pai->info2 = 0;
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
StatusRemocao remover_23_recursivo(Arv23 **ponteiro_no_atual, int valor)
{
    StatusRemocao status_final = OK;
    Arv23 *no_atual = *ponteiro_no_atual;

    if (no_atual == NULL)
    {
        printf("Valor %d nao encontrado na subarvore.\n", valor);
        status_final = OK;
    }
    else
    {
        // printf("Visitando no [%d%s%d] (%p) para remover %d (Pai: %p)\n", no_atual->info1,
        //        no_atual->nInfo==2 ? ", " : " ", no_atual->nInfo==2 ? no_atual->info2 : -1,
        //        (void*)no_atual, valor, (void*)pai_do_no_atual);

        Arv23 **proximo_ponteiro_recursao = NULL;
        int valor_encontrado_neste_no = 0;
        int indice_valor_removido = -1;

        if (valor == no_atual->info1)
        {
            valor_encontrado_neste_no = 1;
            indice_valor_removido = 0;
        }
        else if (no_atual->nInfo == 2 && valor == no_atual->info2)
        {
            valor_encontrado_neste_no = 1;
            indice_valor_removido = 1;
        }
        else if (valor < no_atual->info1)
        {
            proximo_ponteiro_recursao = &(no_atual->esq);
        }
        else if (no_atual->nInfo == 1 || valor < no_atual->info2)
        {
            proximo_ponteiro_recursao = &(no_atual->cen);
        }
        else
        {
            proximo_ponteiro_recursao = &(no_atual->dir);
        }

        if (valor_encontrado_neste_no)
        {
            printf("Valor %d encontrado no no [%d%s%d]\n", valor, no_atual->info1,
                   no_atual->nInfo == 2 ? ", " : " ", no_atual->nInfo == 2 ? no_atual->info2 : -1);

            if (eh_folha(no_atual))
            {
                printf("Removendo %d de um no FOLHA.\n", valor);
                if (no_atual->nInfo == 2)
                {
                    printf("Folha e 3-no. Remocao simples.\n");
                    if (indice_valor_removido == 0)
                    {
                        no_atual->info1 = no_atual->info2;
                    }
                    no_atual->info2 = 0;
                    no_atual->nInfo = 1;
                    status_final = OK;
                }
                else
                {
                    printf("Folha e 2-no. Remocao causa UNDERFLOW.\n");
                    no_atual->nInfo = 0;
                    no_atual->info1 = 0;
                    status_final = UNDERFLOW;
                }
            }
            else
            {
                printf("Removendo %d de um no INTERNO. Substituindo por sucessor.\n", valor);
                Arv23 *sucessor_node = NULL;
                int valor_sucessor;
                Arv23 **ponteiro_subarvore_sucessor;

                if (indice_valor_removido == 0)
                {
                    sucessor_node = buscar_menor_elemento(no_atual->cen);
                    ponteiro_subarvore_sucessor = &(no_atual->cen);
                }
                else
                {
                    sucessor_node = buscar_menor_elemento(no_atual->dir);
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
                        no_atual->info1 = valor_sucessor;
                    }
                    else
                    {
                        no_atual->info2 = valor_sucessor;
                    }

                    printf("Removendo recursivamente o sucessor %d da subarvore apropriada...\n", valor_sucessor);
                    StatusRemocao status_remocao_sucessor = remover_23_recursivo(ponteiro_subarvore_sucessor, valor_sucessor);

                    if (status_remocao_sucessor == UNDERFLOW)
                    {
                        printf("Underflow apos remover sucessor %d. Tratando na subarvore (pai=%p)...\n", valor_sucessor, (void *)no_atual);
                        status_final = tratar_underflow(ponteiro_subarvore_sucessor, no_atual);
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
            StatusRemocao status_recursao = remover_23_recursivo(proximo_ponteiro_recursao, valor);

            if (status_recursao == UNDERFLOW)
            {
                printf("Underflow retornado da subarvore. Tratando no nivel atual (pai=%p)...\n", (void *)no_atual);
                status_final = tratar_underflow(proximo_ponteiro_recursao, no_atual);
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

int remover_23(Arv23 **raiz, int valor)
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
        StatusRemocao status_geral = remover_23_recursivo(raiz, valor);

        // CORRECAO: Verifica se a raiz ficou vazia (nInfo=0) APOS a remocao recursiva,
        // independentemente do status retornado. Isso trata o caso onde a ultima fusao
        // ocorreu no nivel superior e deixou a raiz vazia, mas retornou OK, ou
        // se a remocao na propria raiz (sendo 2-no folha) causou underflow.
        if (*raiz != NULL && (*raiz)->nInfo == 0)
        {
            printf("Raiz (%p) entrou em underflow (nInfo=0). Ajustando a raiz da arvore...\n", (void *)*raiz);
            Arv23 *raiz_antiga = *raiz;
            Arv23 *nova_raiz = NULL;

            // A nova raiz e o unico filho nao-NULL da raiz antiga.
            // Apos uma fusao que causa underflow na raiz, ou remocao da unica chave da raiz,
            // ela tera apenas um filho, que deve estar em ->esq.
            if (raiz_antiga->esq != NULL)
            {
                nova_raiz = raiz_antiga->esq;
                printf("Nova raiz sera o filho esquerdo [%d%s%d] (%p) da antiga raiz.\n",
                       nova_raiz->info1, nova_raiz->nInfo == 2 ? ", " : " ", nova_raiz->nInfo == 2 ? nova_raiz->info2 : -1,
                       (void *)nova_raiz);
            }
            else if (raiz_antiga->cen != NULL)
            {
                // Este caso nao deveria ocorrer se a logica de fusao estiver correta.
                nova_raiz = raiz_antiga->cen;
                printf("Nova raiz sera o filho central [%d%s%d] (%p) da antiga raiz (Inesperado!).\n",
                       nova_raiz->info1, nova_raiz->nInfo == 2 ? ", " : " ", nova_raiz->nInfo == 2 ? nova_raiz->info2 : -1,
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

//================ INSERCAO (Codigo Original Mantido) ==================

void adiciona_infos(Arv23 **no, int info, Arv23 *Sub_Arv_Info)
{
    Arv23 *no_atual = *no;
    if (info > no_atual->info1)
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

Arv23 *quebra_no(Arv23 **no, int info, int *sobe, Arv23 *F_dir)
{
    Arv23 *no_atual = *no;
    Arv23 *maior = NULL;
    if (info > no_atual->info2)
    {
        *sobe = no_atual->info2;
        maior = cria_no(info, no_atual->dir, F_dir);
        no_atual->nInfo = 1;
        no_atual->info2 = 0;
        no_atual->dir = NULL;
    }
    else if (info > no_atual->info1)
    {
        *sobe = info;
        maior = cria_no(no_atual->info2, F_dir, no_atual->dir);
        no_atual->nInfo = 1;
        no_atual->info2 = 0;
        no_atual->dir = NULL;
    }
    else
    {
        *sobe = no_atual->info1;
        maior = cria_no(no_atual->info2, no_atual->cen, no_atual->dir);
        no_atual->info1 = info;
        no_atual->cen = F_dir;
        no_atual->nInfo = 1;
        no_atual->info2 = 0;
        no_atual->dir = NULL;
    }
    return maior;
}

int insere_23_recursivo(Arv23 **raiz, int valor, int *sobe, Arv23 **maiorNo)
{

    int sucesso = 0;
    Arv23 *no_atual = *raiz;

    if (no_atual == NULL)
    {
        *raiz = cria_no(valor, NULL, NULL);
        *maiorNo = NULL;
        *sobe = 0;
        sucesso = (*raiz != NULL);
    }
    else if (valor == no_atual->info1 || (no_atual->nInfo == 2 && valor == no_atual->info2))
    {
        printf("Valor %d ja existe na arvore!\n", valor);
        *maiorNo = NULL;
        *sobe = 0;
        sucesso = 0;
    }
    else
    {
        if (eh_folha(no_atual))
        {
            if (no_atual->nInfo == 1)
            {
                adiciona_infos(raiz, valor, NULL); // Passa o endereco do ponteiro
                *maiorNo = NULL;
                *sobe = 0;
                sucesso = 1;
            }
            else
            {
                *maiorNo = quebra_no(raiz, valor, sobe, NULL); // Passa o endereco do ponteiro
                sucesso = (*maiorNo != NULL);
            }
        }
        else
        {
            Arv23 **proximo_filho;
            if (valor < no_atual->info1)
            {
                proximo_filho = &(no_atual->esq);
            }
            else if ((no_atual->nInfo == 1) || (valor < no_atual->info2))
            {
                proximo_filho = &(no_atual->cen);
            }
            else
            {
                proximo_filho = &(no_atual->dir);
            }
            int sucesso_rec = insere_23_recursivo(proximo_filho, valor, sobe, maiorNo);

            if (*maiorNo != NULL)
            {
                int chave_que_subiu = *sobe;
                Arv23 *novo_filho_dir = *maiorNo;
                *maiorNo = NULL;
                *sobe = 0;

                if (no_atual->nInfo == 1)
                {
                    adiciona_infos(raiz, chave_que_subiu, novo_filho_dir); // Passa o endereco do ponteiro
                    sucesso = 1;
                }
                else
                {
                    *maiorNo = quebra_no(raiz, chave_que_subiu, sobe, novo_filho_dir); // Passa o endereco do ponteiro
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

int insere_23(Arv23 **raiz, int valor)
{
    int sobe = 0;
    Arv23 *maiorNo = NULL;
    int sucesso = 0;

    if (raiz != NULL)
    {
        sucesso = insere_23_recursivo(raiz, valor, &sobe, &maiorNo);

        if (maiorNo != NULL)
        {
            printf("Raiz original quebrou. Criando nova raiz com valor %d.\n", sobe);
            Arv23 *nova_raiz = cria_no(sobe, *raiz, maiorNo);
            if (nova_raiz != NULL)
            {
                *raiz = nova_raiz;
            }
            else
            {
                fprintf(stderr, "Erro ao criar nova raiz!\n");
                sucesso = 0;
                // Idealmente, deveria tentar liberar maiorNo aqui
                if (maiorNo)
                    free(maiorNo); // Tenta liberar
            }
        }
    }
    return sucesso;
}

//=============== IMPRIMIR ==================
void imprime_23_em_ordem(Arv23 *raiz)
{
    if (raiz != NULL)
    {
        // Adiciona verificacao defensiva contra nInfo invalido
        if (raiz->nInfo <= 0)
        {
            fprintf(stderr, "Erro: Tentando imprimir no com nInfo=%d (%p)\n", raiz->nInfo, (void *)raiz);
            return;
        }
        imprime_23_em_ordem(raiz->esq);
        printf("%d ", raiz->info1);
        imprime_23_em_ordem(raiz->cen);
        if (raiz->nInfo == 2)
        {
            printf("%d ", raiz->info2);
            imprime_23_em_ordem(raiz->dir);
        }
    }
}

void imprime_arvore_visual(Arv23 *raiz, char *prefixo, int eh_ultimo, int eh_raiz)
{
    if (raiz != NULL)
    {
        printf("%s", prefixo);
        if (!eh_raiz)
        {
            printf("%s", eh_ultimo ? "`------ " : "+------ ");
        }
        else
        {
            printf(" Raiz--> ");
        }

        if (raiz->nInfo == 1)
        {
            printf("[%d]\n", raiz->info1);
            // printf("[%d] (nfo=%d, %p)\n", raiz->info1, raiz->nInfo, (void*)raiz);
        }
        else if (raiz->nInfo == 2)
        {
            printf("[%d, %d]\n", raiz->info1, raiz->info2);
            // printf("[%d, %d] (nfo=%d, %p)\n", raiz->info1, raiz->info2, raiz->nInfo, (void*)raiz);
        }
        else
        {
            // Este caso (nInfo=0) so deve ocorrer temporariamente durante a remocao
            // Se aparecer na impressao final, indica um erro.
            // printf("[UNDERFLOW!] (nfo=%d, %p)\n", raiz->nInfo, (void*)raiz);
        }

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
            imprime_arvore_visual(raiz->esq, novo_prefixo, filhos_impressos == num_filhos, 0);
        }
        if (raiz->cen != NULL)
        {
            filhos_impressos++;
            imprime_arvore_visual(raiz->cen, novo_prefixo, filhos_impressos == num_filhos, 0);
        }
        if (raiz->dir != NULL)
        {
            filhos_impressos++;
            imprime_arvore_visual(raiz->dir, novo_prefixo, filhos_impressos == num_filhos, 0);
        }
    }
}

//================ LIBERAR MEMORIA ==================
void libera_arvore(Arv23 **raiz)
{
    if (raiz != NULL && *raiz != NULL)
    {

        libera_arvore(&(*raiz)->esq);
        libera_arvore(&(*raiz)->cen);
        libera_arvore(&(*raiz)->dir);

        free(*raiz);
        *raiz = NULL;
    }
}



int altura_23(Arv23 *raiz){
    int altura = -1;
    while (raiz != NULL){
        altura++;
        raiz = raiz->esq; // Segue sempre o filho esquerdo
    }
    return altura;
}

int qunatas_folhas(Arv23 *raiz){
    int folhas = 0;
    if (raiz != NULL) {
        if (eh_folha(raiz)) {
            folhas = 1; // Conta esta folha
        } else {
            folhas += qunatas_folhas(raiz->esq);
            folhas += qunatas_folhas(raiz->cen);
            if (raiz->nInfo == 2) folhas += qunatas_folhas(raiz->dir);
        }
    }
    return folhas;
}

int profundidade_e_altura(Arv23 *raiz, int buscado, int *altura){
    int profundidade = 0;
    if (raiz != NULL){
        if (raiz->info1 == buscado || (raiz->nInfo == 2 && raiz->info2 == buscado)){
            (*altura) = altura_23(raiz);
        }
        else if (buscado < raiz->info1){
            profundidade = profundidade_e_altura(raiz->esq, buscado, altura) + 1;
        }
        else if (raiz->nInfo == 1 || buscado < raiz->info2){
            profundidade = profundidade_e_altura(raiz->cen, buscado, altura) + 1;
        }
        else {
            profundidade = profundidade_e_altura(raiz->dir, buscado, altura) + 1;
        }
    }
    return profundidade;
}

//================ MAIN   ==================

int main()
{
    Arv23 *raiz = NULL;
    int opcao = 0, valor;
    char prefixo_inicial[10] = "";
    int altura = 0;

    do {
        printf("\n=========================\n");
        printf("      ARVORE 2-3 MENU    \n");
        printf("=========================\n");
        
        if (raiz != NULL) {
            printf("Arvore 2-3 Atual:\n");
            imprime_arvore_visual(raiz, prefixo_inicial, 1, 1);
            printf("\nEm ordem: ");
            imprime_23_em_ordem(raiz);
            printf("\n");
        } else {
            printf("Arvore vazia\n");
        }
        
        printf("------------------------\n");
        printf("1. Inserir um valor\n");
        printf("2. Remover um valor\n");
        printf("3. Inserir valores de 1 a 30\n");
        printf("4. Buscar um valor (nao implementado)\n");
        printf("5. Limpar a arvore\n");
        printf("7. Altura da arvore\n");
        printf("8. Qunatas folhas\n");
        printf("9. Profundidade e altura de um valor\n");
        printf("6. Sair\n");
        printf("Escolha uma opcao: ");

        if (scanf("%d", &opcao) != 1) {
            printf("Entrada invalida. Por favor, insira um numero.\n");
            while (getchar() != '\n');
            opcao = 0;
            continue;
        }
        while (getchar() != '\n');

        switch (opcao) {
            case 1:
                printf("Digite o valor a ser inserido: ");
                if (scanf("%d", &valor) != 1) {
                    printf("Entrada invalida.\n");
                    while (getchar() != '\n');
                } else {
                    while (getchar() != '\n');
                    if (insere_23(&raiz, valor)) {
                        printf("Valor %d inserido com sucesso.\n", valor);
                    }
                }
                break;

            case 2:
                printf("Digite o valor a ser removido: ");
                if (scanf("%d", &valor) != 1) {
                    printf("Entrada invalida.\n");
                    while (getchar() != '\n');
                } else {
                    while (getchar() != '\n');
                    if (raiz == NULL) {
                        printf("Arvore esta vazia, nao e possivel remover.\n");
                    } else {
                        remover_23(&raiz, valor);
                    }
                }
                break;
                
            case 3:
                printf("Inserindo valores de 1 a 30...\n");
                for (int i = 1; i <= 100; i++) {
                    insere_23(&raiz, i);
                }
                printf("Valores 1-30 inseridos com sucesso.\n");
                break;
                
            case 4:
                printf("Funcionalidade de busca nao implementada.\n");
                break;
                
            case 5:
                if (raiz != NULL) {
                    libera_arvore(&raiz);
                    printf("Arvore esvaziada com sucesso.\n");
                } else {
                    printf("Arvore ja esta vazia.\n");
                }
                break;

            case 6:
                printf("Saindo...\n");
                break;
            case 7:
                altura = altura_23(raiz);
                printf("Altura da arvore: %d\n", altura);
                break;
            case 8:{
                int folhas = qunatas_folhas(raiz);
                printf("Quantidade de folhas na arvore: %d\n", folhas);
                break;
            }
            case 9:
                printf("Digite o valor para buscar profundidade e altura: ");
                if (scanf("%d", &valor) != 1) {
                    printf("Entrada invalida.\n");
                    while (getchar() != '\n');
                } else {
                    while (getchar() != '\n');
                    int profundidade = profundidade_e_altura(raiz, valor, &altura);
                    if (altura >= 0) {
                        printf("Valor %d encontrado. Profundidade: %d, Altura: %d\n", valor, profundidade, altura);
                    } else {
                        printf("Valor %d nao encontrado na arvore.\n", valor);
                    }
                }
                break;
            default:
                printf("Opcao invalida. Tente novamente.\n");
                break;
        }
    } while (opcao != 6);

    libera_arvore(&raiz); // Libera a memoria da arvore antes de sair
    printf("\nPrograma encerrado. Memoria liberada.\n");

    return 0;
}
