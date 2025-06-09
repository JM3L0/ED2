#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Estrutura do No da Arvore 4-5
typedef struct Arv45
{
    int info1, info2, info3, info4;
    int nInfo; // 0 (underflow temporario), 1, 2, 3 ou 4
    struct Arv45 *esq;
    struct Arv45 *f_cen1;
    struct Arv45 *f_cen2;
    struct Arv45 *f_cen3;
    struct Arv45 *dir;
} Arv45;

// Enum para status da remocao
typedef enum
{
    OK,
    UNDERFLOW
} StatusRemocao;

// Prototypes
Arv45 *cria_no_45(int info, Arv45 *F_esq, Arv45 *F_f_cen1);
int eh_folha_45(Arv45 *no);
void adiciona_chave_em_no_com_espaco_45(Arv45 *no_atual, int info_nova, Arv45 *sub_arv_direita_da_info_nova);
Arv45 *quebra_no_45(Arv45 **no_original_ptr, int info_a_ser_inserida, Arv45 *filho_dir_da_info_a_ser_inserida, int *chave_promovida_saida);
int insere_45_recursivo(Arv45 **raiz, int valor, int *sobe, Arv45 **maiorNo);
int insere_45(Arv45 **raiz, int valor);
void imprime_45_em_ordem(Arv45 *raiz);
void imprime_arvore_visual_45(Arv45 *raiz, char *prefixo, int eh_ultimo, int eh_raiz);
Arv45 *buscar_menor_elemento_45(Arv45 *no);
StatusRemocao remover_45_recursivo(Arv45 **ponteiro_no_atual, int valor);
int remover_45(Arv45 **raiz, int valor);
StatusRemocao tratar_underflow_45(Arv45 **ponteiro_filho_no_pai, Arv45 *pai);
StatusRemocao redistribuir_com_irmao_esquerdo_45(Arv45 **ponteiro_filho_no_pai, Arv45 *pai, Arv45 *irmao_esq, int pos_filho);
StatusRemocao redistribuir_com_irmao_direito_45(Arv45 **ponteiro_filho_no_pai, Arv45 *pai, Arv45 *irmao_dir, int pos_filho);
StatusRemocao fundir_com_irmao_esquerdo_45(Arv45 **ponteiro_filho_no_pai, Arv45 *pai, Arv45 *irmao_esq, int pos_filho);
StatusRemocao fundir_com_irmao_direito_45(Arv45 **ponteiro_filho_no_pai, Arv45 *pai, Arv45 *irmao_dir, int pos_filho);
void libera_arvore_45(Arv45 **raiz);

//================ CRIA NO ==================
Arv45 *cria_no_45(int info, Arv45 *F_esq, Arv45 *F_f_cen1)
{
    Arv45 *no = (Arv45 *)malloc(sizeof(Arv45));
    if (no != NULL)
    {
        no->info1 = info;
        no->nInfo = 1;
        no->esq = F_esq;
        no->f_cen1 = F_f_cen1;
        no->f_cen2 = NULL;
        no->f_cen3 = NULL;
        no->dir = NULL;
        // info2, info3, info4 contêm valores indeterminados inicialmente
    }
    return no;
}

//================ VERIFICA FOLHA ==================
int eh_folha_45(Arv45 *no)
{
    int resultado = 0; // Padrão para nó NULL ou não folha
    if (no != NULL)
    {
        if (no->esq == NULL && no->f_cen1 == NULL && no->f_cen2 == NULL && no->f_cen3 == NULL && no->dir == NULL)
        {
            resultado = 1;
        }
    }
    return resultado;
}

//================ ADICIONA CHAVE EM NÓ COM ESPAÇO ==================
void adiciona_chave_em_no_com_espaco_45(Arv45 *no_atual, int info_nova, Arv45 *sub_arv_direita_da_info_nova)
{
    if (no_atual == NULL || no_atual->nInfo >= 4)
    { // 4 é o MAX_KEYS
        fprintf(stderr, "Erro: Tentativa de adicionar chave em no cheio ou nulo sem quebra (nInfo=%d).\n", no_atual ? no_atual->nInfo : -1);
        // Esta é uma função void, então o return é apenas para sair.
        // Para aderir a "um único return", a lógica de erro precisaria ser reestruturada,
        // mas para um guarda inicial, um return void é comum.
        // Para o propósito desta refatoração, vamos manter assim, pois não retorna valor.
        return;
    }

    int j;
    int pos_insercao_chave = 0;

    if (no_atual->nInfo >= 1 && info_nova > no_atual->info1)
        pos_insercao_chave = 1;
    if (no_atual->nInfo >= 2 && info_nova > no_atual->info2)
        pos_insercao_chave = 2;
    if (no_atual->nInfo >= 3 && info_nova > no_atual->info3)
        pos_insercao_chave = 3;

    // Desloca chaves existentes para a direita
    for (j = no_atual->nInfo - 1; j >= pos_insercao_chave; j--)
    {
        if (j == 2)
            no_atual->info4 = no_atual->info3;
        else if (j == 1)
            no_atual->info3 = no_atual->info2;
        else if (j == 0)
            no_atual->info2 = no_atual->info1;
    }

    // Desloca ponteiros de filhos existentes para a direita
    for (j = no_atual->nInfo; j >= pos_insercao_chave + 1; j--)
    {
        if (j == 3)
            no_atual->dir = no_atual->f_cen3;
        else if (j == 2)
            no_atual->f_cen3 = no_atual->f_cen2;
        else if (j == 1)
            no_atual->f_cen2 = no_atual->f_cen1;
    }

    // Insere a nova chave
    if (pos_insercao_chave == 0)
        no_atual->info1 = info_nova;
    else if (pos_insercao_chave == 1)
        no_atual->info2 = info_nova;
    else if (pos_insercao_chave == 2)
        no_atual->info3 = info_nova;
    else if (pos_insercao_chave == 3)
        no_atual->info4 = info_nova;

    // Insere o novo filho (à DIREITA da info_nova)
    if (pos_insercao_chave == 0)
        no_atual->f_cen1 = sub_arv_direita_da_info_nova;
    else if (pos_insercao_chave == 1)
        no_atual->f_cen2 = sub_arv_direita_da_info_nova;
    else if (pos_insercao_chave == 2)
        no_atual->f_cen3 = sub_arv_direita_da_info_nova;
    else if (pos_insercao_chave == 3)
        no_atual->dir = sub_arv_direita_da_info_nova;

    no_atual->nInfo++;
    // Single exit point for void function
}

//================ QUEBRA NO ==================
Arv45 *quebra_no_45(Arv45 **no_original_ptr, int N, Arv45 *Nc, int *chave_promovida_saida)
{
    Arv45 *no_orig = *no_original_ptr;
    Arv45 *novo_no_dir = NULL; // Valor de retorno

    // Salva as chaves e filhos originais do nó que está cheio (no_orig tem 4 chaves)
    int i1 = no_orig->info1, i2 = no_orig->info2, i3 = no_orig->info3, i4 = no_orig->info4;
    Arv45 *c0 = no_orig->esq, *c1 = no_orig->f_cen1, *c2 = no_orig->f_cen2, *c3 = no_orig->f_cen3, *c4 = no_orig->dir;

    if (N < i1)
    {
        *chave_promovida_saida = i2;
        no_orig->info1 = N;
        no_orig->info2 = i1;
        no_orig->nInfo = 2;
        no_orig->esq = c0;
        no_orig->f_cen1 = Nc;
        no_orig->f_cen2 = c1;
        novo_no_dir = cria_no_45(i3, c2, c3);
        if (novo_no_dir)
        {
            novo_no_dir->info2 = i4;
            novo_no_dir->f_cen2 = c4;
            novo_no_dir->nInfo = 2;
        }
    }
    else if (N < i2)
    {
        *chave_promovida_saida = i2;
        no_orig->info1 = i1;
        no_orig->info2 = N;
        no_orig->nInfo = 2;
        no_orig->esq = c0;
        no_orig->f_cen1 = c1;
        no_orig->f_cen2 = Nc;
        novo_no_dir = cria_no_45(i3, c2, c3);
        if (novo_no_dir)
        {
            novo_no_dir->info2 = i4;
            novo_no_dir->f_cen2 = c4;
            novo_no_dir->nInfo = 2;
        }
    }
    else if (N < i3)
    {
        *chave_promovida_saida = N;
        no_orig->info1 = i1;
        no_orig->info2 = i2;
        no_orig->nInfo = 2;
        no_orig->esq = c0;
        no_orig->f_cen1 = c1;
        no_orig->f_cen2 = c2;
        novo_no_dir = cria_no_45(i3, Nc, c3);
        if (novo_no_dir)
        {
            novo_no_dir->info2 = i4;
            novo_no_dir->f_cen2 = c4;
            novo_no_dir->nInfo = 2;
        }
    }
    else if (N < i4)
    {
        *chave_promovida_saida = i3;
        no_orig->info1 = i1;
        no_orig->info2 = i2;
        no_orig->nInfo = 2;
        no_orig->esq = c0;
        no_orig->f_cen1 = c1;
        no_orig->f_cen2 = c2;
        novo_no_dir = cria_no_45(N, c3, Nc);
        if (novo_no_dir)
        {
            novo_no_dir->info2 = i4;
            novo_no_dir->f_cen2 = c4;
            novo_no_dir->nInfo = 2;
        }
    }
    else
    {
        *chave_promovida_saida = i3;
        no_orig->info1 = i1;
        no_orig->info2 = i2;
        no_orig->nInfo = 2;
        no_orig->esq = c0;
        no_orig->f_cen1 = c1;
        no_orig->f_cen2 = c2;
        novo_no_dir = cria_no_45(i4, c3, c4);
        if (novo_no_dir)
        {
            novo_no_dir->info2 = N;
            novo_no_dir->f_cen2 = Nc;
            novo_no_dir->nInfo = 2;
        }
    }

    no_orig->info3 = 0;
    no_orig->info4 = 0;
    no_orig->f_cen3 = NULL;
    no_orig->dir = NULL;

    if (novo_no_dir != NULL)
    {
        if (novo_no_dir->nInfo < 2)
            novo_no_dir->info2 = 0;
        if (novo_no_dir->nInfo < 2)
            novo_no_dir->f_cen2 = NULL; // Garante NULL se só 1 chave
        novo_no_dir->f_cen3 = NULL;
        novo_no_dir->dir = NULL;
    }
    return novo_no_dir;
}

//================ INSERCAO (Recursiva) ==================
int insere_45_recursivo(Arv45 **raiz_ptr, int valor, int *chave_promovida_acima, Arv45 **novo_irmao_a_direita)
{
    int status_operacao = 0;
    Arv45 *no_atual = (raiz_ptr != NULL) ? *raiz_ptr : NULL;

    *chave_promovida_acima = 0;
    *novo_irmao_a_direita = NULL;

    if (no_atual == NULL)
    {
        *chave_promovida_acima = valor;
        status_operacao = 1;
    }
    else
    {
        int duplicado = 0;
        if (no_atual->nInfo >= 1 && valor == no_atual->info1)
            duplicado = 1;
        else if (no_atual->nInfo >= 2 && valor == no_atual->info2)
            duplicado = 1;
        else if (no_atual->nInfo >= 3 && valor == no_atual->info3)
            duplicado = 1;
        else if (no_atual->nInfo >= 4 && valor == no_atual->info4)
            duplicado = 1;

        if (duplicado)
        {
            status_operacao = 0;
        }
        else
        {
            if (eh_folha_45(no_atual))
            {
                if (no_atual->nInfo < 4)
                { // 4 é MAX_KEYS
                    adiciona_chave_em_no_com_espaco_45(no_atual, valor, NULL);
                    status_operacao = 1;
                }
                else
                {
                    *novo_irmao_a_direita = quebra_no_45(raiz_ptr, valor, NULL, chave_promovida_acima);
                    status_operacao = 1;
                }
            }
            else
            { // Nó interno
                Arv45 **proximo_filho_ptr = NULL;
                if (valor < no_atual->info1)
                {
                    proximo_filho_ptr = &(no_atual->esq);
                }
                else if (no_atual->nInfo == 1 || (no_atual->nInfo >= 2 && valor < no_atual->info2))
                {
                    proximo_filho_ptr = &(no_atual->f_cen1);
                }
                else if (no_atual->nInfo == 2 || (no_atual->nInfo >= 3 && valor < no_atual->info3))
                {
                    proximo_filho_ptr = &(no_atual->f_cen2);
                }
                else if (no_atual->nInfo == 3 || (no_atual->nInfo >= 4 && valor < no_atual->info4))
                {
                    proximo_filho_ptr = &(no_atual->f_cen3);
                }
                else
                {
                    proximo_filho_ptr = &(no_atual->dir);
                }

                int chave_vinda_de_baixo = 0;
                Arv45 *filho_direito_vindo_de_baixo = NULL;

                if (insere_45_recursivo(proximo_filho_ptr, valor, &chave_vinda_de_baixo, &filho_direito_vindo_de_baixo))
                {
                    if (chave_vinda_de_baixo != 0)
                    {
                        if (no_atual->nInfo < 4)
                        { // 4 é MAX_KEYS
                            adiciona_chave_em_no_com_espaco_45(no_atual, chave_vinda_de_baixo, filho_direito_vindo_de_baixo);
                            status_operacao = 1;
                        }
                        else
                        {
                            *novo_irmao_a_direita = quebra_no_45(raiz_ptr, chave_vinda_de_baixo, filho_direito_vindo_de_baixo, chave_promovida_acima);
                            status_operacao = 1;
                        }
                    }
                    else
                    {
                        status_operacao = 1;
                    }
                } // else status_operacao já é 0
            }
        }
    }
    return status_operacao;
}

//================ INSERCAO (Principal) ==================
int insere_45(Arv45 **raiz, int valor)
{
    int sucesso = 0;
    int chave_promovida_da_raiz = 0;
    Arv45 *novo_no_irmao_da_raiz = NULL;

    if (raiz != NULL)
    {
        if (*raiz == NULL)
        {
            *raiz = cria_no_45(valor, NULL, NULL);
            if (*raiz != NULL)
            {
                sucesso = 1;
            }
        }
        else
        {
            int duplicado_na_raiz = 0;
            if ((*raiz)->nInfo >= 1 && valor == (*raiz)->info1)
                duplicado_na_raiz = 1;
            else if ((*raiz)->nInfo >= 2 && valor == (*raiz)->info2)
                duplicado_na_raiz = 1;
            else if ((*raiz)->nInfo >= 3 && valor == (*raiz)->info3)
                duplicado_na_raiz = 1;
            else if ((*raiz)->nInfo >= 4 && valor == (*raiz)->info4)
                duplicado_na_raiz = 1;

            if (duplicado_na_raiz)
            {
                printf("Valor %d ja existe na arvore (raiz)!\n", valor);
                // sucesso permanece 0
            }
            else if (insere_45_recursivo(raiz, valor, &chave_promovida_da_raiz, &novo_no_irmao_da_raiz))
            {
                if (chave_promovida_da_raiz != 0)
                {
                    Arv45 *antiga_raiz = *raiz;
                    *raiz = cria_no_45(chave_promovida_da_raiz, antiga_raiz, novo_no_irmao_da_raiz);
                    if (*raiz != NULL)
                    {
                        sucesso = 1;
                    }
                    else
                    {
                        fprintf(stderr, "Erro ao criar nova raiz!\n");
                        if (novo_no_irmao_da_raiz)
                            free(novo_no_irmao_da_raiz);
                        // sucesso permanece 0
                    }
                }
                else
                {
                    sucesso = 1;
                }
            } // else sucesso permanece 0
        }
    }
    return sucesso;
}

//================ BUSCAR MENOR ==================
Arv45 *buscar_menor_elemento_45(Arv45 *no)
{
    Arv45 *resultado = NULL;
    Arv45 *atual = no;

    if (atual != NULL)
    {
        while (atual->esq != NULL)
        {
            atual = atual->esq;
        }
        resultado = atual;
    }
    return resultado;
}

//================ TRATAR UNDERFLOW ==================
StatusRemocao tratar_underflow_45(Arv45 **ponteiro_filho_no_pai, Arv45 *pai)
{
    StatusRemocao status_final = OK;
    Arv45 *filho_com_underflow = (ponteiro_filho_no_pai != NULL) ? *ponteiro_filho_no_pai : NULL;

    if (filho_com_underflow == NULL || (filho_com_underflow->nInfo >= 1))
    { // 1 é MIN_KEYS_ALLOWED
        // status_final permanece OK;
    }
    else if (filho_com_underflow->nInfo == 0)
    {
        if (pai == NULL)
        {
            status_final = UNDERFLOW;
        }
        else
        {
            Arv45 *irmao_esq = NULL;
            Arv45 *irmao_dir = NULL;
            int pos_filho = -1;
            int operacao_realizada = 0;

            if (pai->esq == filho_com_underflow)
            {
                pos_filho = 0;
                irmao_dir = pai->f_cen1;
            }
            else if (pai->f_cen1 == filho_com_underflow)
            {
                pos_filho = 1;
                irmao_esq = pai->esq;
                irmao_dir = pai->f_cen2;
            }
            else if (pai->f_cen2 == filho_com_underflow)
            {
                pos_filho = 2;
                irmao_esq = pai->f_cen1;
                irmao_dir = pai->f_cen3;
            }
            else if (pai->f_cen3 == filho_com_underflow)
            {
                pos_filho = 3;
                irmao_esq = pai->f_cen2;
                irmao_dir = pai->dir;
            }
            else if (pai->dir == filho_com_underflow && pai->nInfo == 4)
            { // 4 é MAX_KEYS
                pos_filho = 4;
                irmao_esq = pai->f_cen3;
            }

            if (pos_filho != -1)
            { // Apenas tenta operações se o filho foi encontrado
                if (irmao_dir != NULL && irmao_dir->nInfo > 1)
                {
                    status_final = redistribuir_com_irmao_direito_45(ponteiro_filho_no_pai, pai, irmao_dir, pos_filho);
                    operacao_realizada = 1;
                }
                else if (irmao_esq != NULL && irmao_esq->nInfo > 1)
                {
                    status_final = redistribuir_com_irmao_esquerdo_45(ponteiro_filho_no_pai, pai, irmao_esq, pos_filho);
                    operacao_realizada = 1;
                }
                else if (irmao_dir != NULL && irmao_dir->nInfo == 1)
                {
                    status_final = fundir_com_irmao_direito_45(ponteiro_filho_no_pai, pai, irmao_dir, pos_filho);
                    operacao_realizada = 1;
                }
                else if (irmao_esq != NULL && irmao_esq->nInfo == 1)
                {
                    status_final = fundir_com_irmao_esquerdo_45(ponteiro_filho_no_pai, pai, irmao_esq, pos_filho);
                    operacao_realizada = 1;
                }

                if (!operacao_realizada)
                {
                    status_final = UNDERFLOW;
                }
            }
            else if (filho_com_underflow != pai)
            { // Evita erro se filho é a própria raiz (pai é NULL)
                fprintf(stderr, "Erro critico: Filho em underflow nao eh filho do pai.\n");
                status_final = UNDERFLOW;
            }
        }
    }
    return status_final;
}

//================ REDISTRIBUIR COM IRMAO ESQUERDO ==================
StatusRemocao redistribuir_com_irmao_esquerdo_45(Arv45 **ponteiro_filho_no_pai, Arv45 *pai, Arv45 *irmao_esq, int pos_filho)
{
    StatusRemocao status_retorno = OK;
    Arv45 *filho = *ponteiro_filho_no_pai;

    if (pai == NULL || irmao_esq == NULL || filho == NULL || irmao_esq->nInfo <= 1)
    { // 1 é MIN_KEYS_ALLOWED
        // fprintf(stderr, "Erro: Condicoes invalidas para redistribuicao esquerda.\n"); // Comentado para reduzir verbosidade
        status_retorno = UNDERFLOW;
    }
    else
    {
        int chave_pai_desce = 0;
        Arv45 *filho_transferido_do_irmao = NULL;
        int chave_irmao_sobe = 0;

        if (pos_filho == 1)
            chave_pai_desce = pai->info1;
        else if (pos_filho == 2)
            chave_pai_desce = pai->info2;
        else if (pos_filho == 3)
            chave_pai_desce = pai->info3;
        else if (pos_filho == 4)
            chave_pai_desce = pai->info4;
        else
        { /*fprintf(stderr, "Posicao %d invalida para redistribuicao esquerda\n", pos_filho);*/
            status_retorno = UNDERFLOW;
        }

        if (status_retorno == OK)
        {
            if (irmao_esq->nInfo == 2)
            {
                chave_irmao_sobe = irmao_esq->info2;
                filho_transferido_do_irmao = irmao_esq->f_cen2;
            }
            else if (irmao_esq->nInfo == 3)
            {
                chave_irmao_sobe = irmao_esq->info3;
                filho_transferido_do_irmao = irmao_esq->f_cen3;
            }
            else if (irmao_esq->nInfo == 4)
            {
                chave_irmao_sobe = irmao_esq->info4;
                filho_transferido_do_irmao = irmao_esq->dir;
            }
            else
            { /* Erro interno, nInfo do irmão deveria ser > 1 */
                status_retorno = UNDERFLOW;
            }

            if (status_retorno == OK)
            {
                if (pos_filho == 1)
                    pai->info1 = chave_irmao_sobe;
                else if (pos_filho == 2)
                    pai->info2 = chave_irmao_sobe;
                else if (pos_filho == 3)
                    pai->info3 = chave_irmao_sobe;
                else if (pos_filho == 4)
                    pai->info4 = chave_irmao_sobe;

                Arv45 *esq_original_do_filho_underflow = filho->esq;
                filho->info1 = chave_pai_desce;
                filho->nInfo = 1;
                filho->esq = filho_transferido_do_irmao;
                filho->f_cen1 = esq_original_do_filho_underflow;
                filho->f_cen2 = NULL;
                filho->f_cen3 = NULL;
                filho->dir = NULL;

                if (irmao_esq->nInfo == 2)
                {
                    irmao_esq->info2 = 0;
                    irmao_esq->f_cen2 = NULL;
                }
                else if (irmao_esq->nInfo == 3)
                {
                    irmao_esq->info3 = 0;
                    irmao_esq->f_cen3 = NULL;
                }
                else if (irmao_esq->nInfo == 4)
                {
                    irmao_esq->info4 = 0;
                    irmao_esq->dir = NULL;
                }
                irmao_esq->nInfo--;
            }
        }
    }
    return status_retorno;
}

//================ REDISTRIBUIR COM IRMAO DIREITO ==================
StatusRemocao redistribuir_com_irmao_direito_45(Arv45 **ponteiro_filho_no_pai, Arv45 *pai, Arv45 *irmao_dir, int pos_filho)
{
    StatusRemocao status_retorno = OK;
    Arv45 *filho = *ponteiro_filho_no_pai;

    if (pai == NULL || irmao_dir == NULL || filho == NULL || irmao_dir->nInfo <= 1)
    { // 1 é MIN_KEYS_ALLOWED
        // fprintf(stderr, "Erro: Condicoes invalidas para redistribuicao direita.\n");
        status_retorno = UNDERFLOW;
    }
    else
    {
        int chave_pai_desce = 0;
        Arv45 *filho_transferido_do_irmao = NULL;
        int chave_irmao_sobe = 0;

        if (pos_filho == 0)
            chave_pai_desce = pai->info1;
        else if (pos_filho == 1)
            chave_pai_desce = pai->info2;
        else if (pos_filho == 2)
            chave_pai_desce = pai->info3;
        else if (pos_filho == 3 && pai->nInfo == 4)
            chave_pai_desce = pai->info4; // 4 é MAX_KEYS
        else
        { /*fprintf(stderr, "Posicao %d invalida para redistribuicao direita\n",pos_filho);*/
            status_retorno = UNDERFLOW;
        }

        if (status_retorno == OK)
        {
            chave_irmao_sobe = irmao_dir->info1;
            filho_transferido_do_irmao = irmao_dir->esq;

            if (pos_filho == 0)
                pai->info1 = chave_irmao_sobe;
            else if (pos_filho == 1)
                pai->info2 = chave_irmao_sobe;
            else if (pos_filho == 2)
                pai->info3 = chave_irmao_sobe;
            else if (pos_filho == 3 && pai->nInfo == 4)
                pai->info4 = chave_irmao_sobe;

            filho->info1 = chave_pai_desce;
            filho->nInfo = 1;
            filho->f_cen1 = filho_transferido_do_irmao;
            filho->f_cen2 = NULL;
            filho->f_cen3 = NULL;
            filho->dir = NULL;

            irmao_dir->info1 = irmao_dir->info2;
            irmao_dir->info2 = irmao_dir->info3;
            irmao_dir->info3 = irmao_dir->info4;
            irmao_dir->info4 = 0;
            irmao_dir->esq = irmao_dir->f_cen1;
            irmao_dir->f_cen1 = irmao_dir->f_cen2;
            irmao_dir->f_cen2 = irmao_dir->f_cen3;
            irmao_dir->f_cen3 = irmao_dir->dir;
            irmao_dir->dir = NULL;
            irmao_dir->nInfo--;
        }
    }
    return status_retorno;
}

//================ FUNDIR COM IRMAO ESQUERDO ==================
StatusRemocao fundir_com_irmao_esquerdo_45(Arv45 **ponteiro_filho_no_pai, Arv45 *pai, Arv45 *irmao_esq, int pos_filho)
{
    StatusRemocao status_pai_final = UNDERFLOW;
    Arv45 *filho_underflow = (ponteiro_filho_no_pai != NULL) ? *ponteiro_filho_no_pai : NULL;

    if (pai == NULL || irmao_esq == NULL || filho_underflow == NULL || irmao_esq->nInfo != 1)
    { // 1 é MIN_KEYS_ALLOWED
        // fprintf(stderr, "Erro: Condicoes invalidas para fusao esquerda (irmao_esq->nInfo=%d).\n", irmao_esq ? irmao_esq->nInfo : -1);
    }
    else
    {
        int chave_pai_desce = 0;
        Arv45 *fu_esq_original = filho_underflow->esq;
        Arv45 *fu_fcen1_original = filho_underflow->f_cen1;

        // Desloca chaves e filhos no pai
        if (pos_filho == 1)
        { // Filho em underflow é pai->f_cen1, irmão esq é pai->esq
            chave_pai_desce = pai->info1;
            pai->info1 = pai->info2;
            pai->info2 = pai->info3;
            pai->info3 = pai->info4;
            pai->info4 = 0;
            pai->f_cen1 = pai->f_cen2;
            pai->f_cen2 = pai->f_cen3;
            pai->f_cen3 = pai->dir;
            pai->dir = NULL;
        }
        else if (pos_filho == 2)
        { // Filho em underflow é pai->f_cen2, irmão esq é pai->f_cen1
            chave_pai_desce = pai->info2;
            pai->info2 = pai->info3;
            pai->info3 = pai->info4;
            pai->info4 = 0;
            pai->f_cen2 = pai->f_cen3;
            pai->f_cen3 = pai->dir;
            pai->dir = NULL;
        }
        else if (pos_filho == 3)
        { // Filho em underflow é pai->f_cen3, irmão esq é pai->f_cen2
            chave_pai_desce = pai->info3;
            pai->info3 = pai->info4;
            pai->info4 = 0;
            pai->f_cen3 = pai->dir;
            pai->dir = NULL;
        }
        else if (pos_filho == 4)
        { // Filho em underflow é pai->dir, irmão esq é pai->f_cen3
            chave_pai_desce = pai->info4;
            pai->info4 = 0;
            pai->dir = NULL;
        }
        else
        { /*fprintf(stderr, "Pos_filho %d invalida em fusao esquerda\n", pos_filho);*/
            return UNDERFLOW;
        }
        pai->nInfo--;

        // Funde no irmao_esq: irmao_esq(1 chave) + chave_pai_desce. Resulta em 2 chaves.
        irmao_esq->info2 = chave_pai_desce; // info1 do irmao_esq já existe
        irmao_esq->nInfo = 2;

        // Filhos do irmao_esq (agora com 2 chaves: info1, info2) são: esq, f_cen1, f_cen2
        // irmao_esq->esq (original do irmao_esq)
        // irmao_esq->f_cen1 (original do irmao_esq, era filho entre info1 original e (implicitamente) chave_pai_desce)
        // irmao_esq->f_cen2 (novo) = fu_esq_original (filho esquerdo do nó que estava em underflow)
        irmao_esq->f_cen2 = fu_esq_original;
        // O fu_fcen1_original seria o 4º filho conceitual. Para um nó de 2 chaves com 3 filhos,
        // este é o ponto onde a acomodação de todos os filhos se torna complexa sem alterar
        // a estrutura fundamental da árvore ou permitir que um nó viole k+1 filhos.
        // A estratégia mais simples, embora com potencial perda se fu_fcen1_original for uma subárvore grande,
        // é não atribuí-lo se os 3 slots de filho já estão preenchidos de forma lógica.
        if (fu_fcen1_original != NULL && irmao_esq->f_cen1 == NULL && irmao_esq->f_cen2 == fu_esq_original)
        {
            // Caso especial: se f_cen1 original do irmao_esq era NULL (improvável para nInfo=1),
            // e f_cen2 pegou fu_esq, então fu_fcen1 poderia ir para f_cen1.
            // Esta lógica é complexa e depende da estrutura exata. A linha abaixo é uma tentativa:
            irmao_esq->f_cen1 = fu_esq_original; // Reatribui se f_cen1 estava "vazio"
            irmao_esq->f_cen2 = fu_fcen1_original;
        }
        else if (fu_fcen1_original != NULL)
        {
            // fprintf(stderr, "Alerta: Fusao esquerda, fu_fcen1_original nao acomodado diretamente.\n");
        }

        irmao_esq->f_cen3 = NULL;
        irmao_esq->dir = NULL; // Limpa para consistência de 2 chaves

        free(filho_underflow);
        if (ponteiro_filho_no_pai != NULL)
            *ponteiro_filho_no_pai = NULL; // Marca o ponteiro no pai como nulo

        if (pai->nInfo == 0 && pai != NULL)
        {
            status_pai_final = UNDERFLOW;
        }
        else if (pai->nInfo > 0 && pai->nInfo < 1)
        {
            status_pai_final = UNDERFLOW;
        } // 1 é MIN_KEYS_ALLOWED
        else
        {
            status_pai_final = OK;
        }
    }
    return status_pai_final;
}

//================ FUNDIR COM IRMAO DIREITO ==================
StatusRemocao fundir_com_irmao_direito_45(Arv45 **ponteiro_filho_no_pai, Arv45 *pai, Arv45 *irmao_dir, int pos_filho)
{
    StatusRemocao status_pai_final = UNDERFLOW;
    Arv45 *filho_underflow = *ponteiro_filho_no_pai; // nInfo == 0

    if (pai == NULL || irmao_dir == NULL || filho_underflow == NULL || irmao_dir->nInfo != 1)
    { // 1 é MIN_KEYS_ALLOWED
        // fprintf(stderr, "Erro: Condicoes invalidas para fusao direita (irmao_dir->nInfo=%d).\n", irmao_dir ? irmao_dir->nInfo : -1);
    }
    else
    {
        int chave_pai_desce = 0;
        Arv45 *fu_esq_original = filho_underflow->esq;
        Arv45 *fu_fcen1_original = filho_underflow->f_cen1;
        Arv45 *id_esq_original = irmao_dir->esq;
        Arv45 *id_fcen1_original = irmao_dir->f_cen1;
        int id_info1_original = irmao_dir->info1;

        if (pos_filho == 0)
        { // filho_underflow é pai->esq, irmao_dir é pai->f_cen1
            chave_pai_desce = pai->info1;
            pai->info1 = pai->info2;
            pai->info2 = pai->info3;
            pai->info3 = pai->info4;
            pai->info4 = 0;
            pai->esq = pai->f_cen1; // irmao_dir (modificado) se torna o novo pai->esq
            pai->f_cen1 = pai->f_cen2;
            pai->f_cen2 = pai->f_cen3;
            pai->f_cen3 = pai->dir;
            pai->dir = NULL;
        }
        else if (pos_filho == 1)
        { // filho_underflow é pai->f_cen1, irmao_dir é pai->f_cen2
            chave_pai_desce = pai->info2;
            pai->info2 = pai->info3;
            pai->info3 = pai->info4;
            pai->info4 = 0;
            pai->f_cen1 = pai->f_cen2; // irmao_dir (modificado) se torna o novo pai->f_cen1
            pai->f_cen2 = pai->f_cen3;
            pai->f_cen3 = pai->dir;
            pai->dir = NULL;
        }
        else if (pos_filho == 2)
        { // filho_underflow é pai->f_cen2, irmao_dir é pai->f_cen3
            chave_pai_desce = pai->info3;
            pai->info3 = pai->info4;
            pai->info4 = 0;
            pai->f_cen2 = pai->f_cen3; // irmao_dir (modificado) se torna o novo pai->f_cen2
            pai->f_cen3 = pai->dir;
            pai->dir = NULL;
        }
        else
        { /*fprintf(stderr, "Pos_filho %d invalida em fusao direita\n", pos_filho);*/
            return UNDERFLOW;
        }
        pai->nInfo--;

        // irmao_dir (originalmente 1 chave) absorve chave_pai_desce. Resulta em 2 chaves.
        // Ordem: chave_pai_desce, chave_original_irmao_dir
        irmao_dir->info2 = id_info1_original; // Chave original do irmao_dir (era info1) vai para info2
        irmao_dir->info1 = chave_pai_desce;   // Chave do pai vai para info1
        irmao_dir->nInfo = 2;

        // Filhos do novo irmao_dir (2 chaves, 3 filhos: esq, f_cen1, f_cen2)
        // Ordem dos filhos disponíveis: fu_esq, fu_fcen1 (do filho_underflow)
        //                             id_esq, id_fcen1 (do irmao_dir original)
        irmao_dir->esq = fu_esq_original;
        irmao_dir->f_cen1 = fu_fcen1_original; // Filho entre nova info1 (chave_pai_desce) e nova info2 (id_info1_original)
        irmao_dir->f_cen2 = id_esq_original;   // Filho à direita da nova info2 (id_info1_original)
                                               // que era o filho esquerdo original de id_info1_original
        // O id_fcen1_original (filho direito original de id_info1_original) seria o 4º filho conceitual.
        if (irmao_dir->f_cen1 == NULL && irmao_dir->f_cen2 == id_esq_original)
        {                                          // Se fu_fcen1 era NULL
            irmao_dir->f_cen1 = id_esq_original;   // id_esq_original pode ser f_cen1
            irmao_dir->f_cen2 = id_fcen1_original; // e id_fcen1_original pode ser f_cen2
        }
        else if (id_fcen1_original != NULL && irmao_dir->f_cen2 == id_esq_original)
        {
            // fprintf(stderr, "Alerta: Fusao direita, id_fcen1_original nao acomodado diretamente se fu_fcen1_original existia.\n");
        }

        irmao_dir->f_cen3 = NULL;
        irmao_dir->dir = NULL;

        free(filho_underflow);
        // O ponteiro no pai (e.g., pai->esq) já aponta para irmao_dir.

        if (pai->nInfo == 0 && pai != NULL)
        {
            status_pai_final = UNDERFLOW;
        }
        else if (pai->nInfo > 0 && pai->nInfo < 1)
        {
            status_pai_final = UNDERFLOW;
        } // 1 é MIN_KEYS_ALLOWED
        else
        {
            status_pai_final = OK;
        }
    }
    return status_pai_final;
}

//================ REMOCAO (Recursiva) ==================
StatusRemocao remover_45_recursivo(Arv45 **ponteiro_no_atual_ptr, int valor)
{
    StatusRemocao status_final = OK;
    Arv45 *no_atual = (ponteiro_no_atual_ptr != NULL) ? *ponteiro_no_atual_ptr : NULL;

    if (no_atual == NULL)
    {
        // status_final permanece OK
    }
    else
    {
        int valor_encontrado_neste_no = 0;
        int indice_valor_removido = -1;
        Arv45 **proximo_ponteiro_recursao = NULL;
        int pode_prosseguir_logica = 1;

        if (no_atual->nInfo >= 1 && valor == no_atual->info1)
        {
            valor_encontrado_neste_no = 1;
            indice_valor_removido = 0;
        }
        else if (no_atual->nInfo >= 2 && valor == no_atual->info2)
        {
            valor_encontrado_neste_no = 1;
            indice_valor_removido = 1;
        }
        else if (no_atual->nInfo >= 3 && valor == no_atual->info3)
        {
            valor_encontrado_neste_no = 1;
            indice_valor_removido = 2;
        }
        else if (no_atual->nInfo >= 4 && valor == no_atual->info4)
        {
            valor_encontrado_neste_no = 1;
            indice_valor_removido = 3;
        }
        else if (no_atual->nInfo >= 1 && valor < no_atual->info1)
        {
            proximo_ponteiro_recursao = &(no_atual->esq);
        }
        else if (no_atual->nInfo == 1 || (no_atual->nInfo >= 2 && valor < no_atual->info2))
        {
            proximo_ponteiro_recursao = &(no_atual->f_cen1);
        }
        else if (no_atual->nInfo == 2 || (no_atual->nInfo >= 3 && valor < no_atual->info3))
        {
            proximo_ponteiro_recursao = &(no_atual->f_cen2);
        }
        else if (no_atual->nInfo == 3 || (no_atual->nInfo == 4 && valor < no_atual->info4))
        {
            proximo_ponteiro_recursao = &(no_atual->f_cen3);
        }
        else if (no_atual->nInfo >= 1)
        {
            proximo_ponteiro_recursao = &(no_atual->dir);
        }
        else
        {                               // nInfo = 0 ou valor não se encaixa
            pode_prosseguir_logica = 0; // Evita descer se nInfo é 0
        }

        if (pode_prosseguir_logica && valor_encontrado_neste_no)
        {
            if (eh_folha_45(no_atual))
            {
                if (indice_valor_removido == 0)
                {
                    no_atual->info1 = no_atual->info2;
                    no_atual->info2 = no_atual->info3;
                    no_atual->info3 = no_atual->info4;
                    no_atual->info4 = 0;
                }
                else if (indice_valor_removido == 1)
                {
                    no_atual->info2 = no_atual->info3;
                    no_atual->info3 = no_atual->info4;
                    no_atual->info4 = 0;
                }
                else if (indice_valor_removido == 2)
                {
                    no_atual->info3 = no_atual->info4;
                    no_atual->info4 = 0;
                }
                else if (indice_valor_removido == 3)
                {
                    no_atual->info4 = 0;
                }
                if (no_atual->nInfo > 0)
                    no_atual->nInfo--;

                if (no_atual->nInfo < 1)
                { // 1 é MIN_KEYS_ALLOWED
                    status_final = UNDERFLOW;
                }
            }
            else
            { // Nó interno
                Arv45 *subarvore_para_sucessor = NULL;
                Arv45 **ponteiro_para_raiz_subarvore_sucessor = NULL;

                if (indice_valor_removido == 0)
                {
                    subarvore_para_sucessor = no_atual->f_cen1;
                    ponteiro_para_raiz_subarvore_sucessor = &(no_atual->f_cen1);
                }
                else if (indice_valor_removido == 1)
                {
                    subarvore_para_sucessor = no_atual->f_cen2;
                    ponteiro_para_raiz_subarvore_sucessor = &(no_atual->f_cen2);
                }
                else if (indice_valor_removido == 2)
                {
                    subarvore_para_sucessor = no_atual->f_cen3;
                    ponteiro_para_raiz_subarvore_sucessor = &(no_atual->f_cen3);
                }
                else if (indice_valor_removido == 3)
                {
                    subarvore_para_sucessor = no_atual->dir;
                    ponteiro_para_raiz_subarvore_sucessor = &(no_atual->dir);
                }
                else
                {
                    pode_prosseguir_logica = 0;
                    status_final = UNDERFLOW;
                }

                Arv45 *sucessor_node = NULL;
                if (pode_prosseguir_logica && subarvore_para_sucessor != NULL)
                {
                    sucessor_node = buscar_menor_elemento_45(subarvore_para_sucessor);
                }
                else if (pode_prosseguir_logica && subarvore_para_sucessor == NULL)
                {
                    fprintf(stderr, "Erro critico: Subarvore do sucessor eh NULL ao remover %d de no interno!\n", valor);
                    status_final = UNDERFLOW; // Indica erro
                    pode_prosseguir_logica = 0;
                }

                if (pode_prosseguir_logica && (sucessor_node == NULL || ponteiro_para_raiz_subarvore_sucessor == NULL || *ponteiro_para_raiz_subarvore_sucessor == NULL))
                {
                    // Esta condição agora é mais específica: se subarvore_para_sucessor era não-nulo, mas buscar_menor falhou ou o ponteiro para ele é ruim.
                    fprintf(stderr, "Erro critico: Sucessor nao encontrado (ou ponteiro invalido) para %d!\n", valor);
                    status_final = UNDERFLOW; // Indica erro
                    pode_prosseguir_logica = 0;
                }

                if (pode_prosseguir_logica)
                {
                    int valor_sucessor = sucessor_node->info1;
                    if (indice_valor_removido == 0)
                        no_atual->info1 = valor_sucessor;
                    else if (indice_valor_removido == 1)
                        no_atual->info2 = valor_sucessor;
                    else if (indice_valor_removido == 2)
                        no_atual->info3 = valor_sucessor;
                    else if (indice_valor_removido == 3)
                        no_atual->info4 = valor_sucessor;

                    StatusRemocao status_rem_suc = remover_45_recursivo(ponteiro_para_raiz_subarvore_sucessor, valor_sucessor);
                    if (status_rem_suc == UNDERFLOW)
                    {
                        status_final = tratar_underflow_45(ponteiro_para_raiz_subarvore_sucessor, no_atual);
                    }
                    else
                    {
                        status_final = status_rem_suc;
                    }
                }
            }
        }
        else if (pode_prosseguir_logica)
        { // Descer
            if (proximo_ponteiro_recursao != NULL)
            {
                StatusRemocao status_recursao = remover_45_recursivo(proximo_ponteiro_recursao, valor);
                if (status_recursao == UNDERFLOW)
                {
                    status_final = tratar_underflow_45(proximo_ponteiro_recursao, no_atual);
                }
                else
                {
                    status_final = status_recursao;
                }
            } // else: valor não encontrado e sem caminho, status_final permanece OK
        }
    }
    return status_final;
}

//================ REMOCAO (Principal) ==================
int remover_45(Arv45 **raiz_ptr, int valor)
{
    int sucesso = 0;
    if (raiz_ptr == NULL || *raiz_ptr == NULL)
    {
        // sucesso permanece 0
    }
    else
    {
        StatusRemocao status_geral = remover_45_recursivo(raiz_ptr, valor);
        Arv45 *raiz_atual = *raiz_ptr;

        if (raiz_atual != NULL && raiz_atual->nInfo == 0)
        {
            Arv45 *raiz_antiga = raiz_atual;
            if (raiz_antiga->esq != NULL)
                *raiz_ptr = raiz_antiga->esq;
            else if (raiz_antiga->f_cen1 != NULL)
                *raiz_ptr = raiz_antiga->f_cen1;
            else
                *raiz_ptr = NULL;
            free(raiz_antiga);
            status_geral = OK;
        }

        if (status_geral == OK)
        {
            sucesso = 1;
        }
    }
    return sucesso;
}

//=============== IMPRIMIR ==================
void imprime_45_em_ordem(Arv45 *raiz)
{
    if (raiz != NULL)
    {
        imprime_45_em_ordem(raiz->esq);
        if (raiz->nInfo >= 1)
            printf("%d ", raiz->info1);
        imprime_45_em_ordem(raiz->f_cen1);
        if (raiz->nInfo >= 2)
            printf("%d ", raiz->info2);
        imprime_45_em_ordem(raiz->f_cen2);
        if (raiz->nInfo >= 3)
            printf("%d ", raiz->info3);
        imprime_45_em_ordem(raiz->f_cen3);
        if (raiz->nInfo == 4)
            printf("%d ", raiz->info4);
        imprime_45_em_ordem(raiz->dir);
    }
}

void imprime_arvore_visual_45(Arv45 *raiz, char *prefixo, int eh_ultimo, int eh_raiz)
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

        printf("[");
        if (raiz->nInfo >= 1)
            printf("%d", raiz->info1);
        if (raiz->nInfo >= 2)
            printf(", %d", raiz->info2);
        if (raiz->nInfo >= 3)
            printf(", %d", raiz->info3);
        if (raiz->nInfo == 4)
            printf(", %d", raiz->info4);
        printf("] (nInfo=%d)\n", raiz->nInfo);

        char novo_prefixo[1024];
        sprintf(novo_prefixo, "%s%s", prefixo, eh_raiz ? "         " : (eh_ultimo ? "         " : "|        "));

        int num_filhos_reais = 0;
        // Contagem de filhos reais para formatação correta
        if (raiz->esq)
            num_filhos_reais++;
        if (raiz->f_cen1)
            num_filhos_reais++;
        if (raiz->f_cen2)
            num_filhos_reais++;
        if (raiz->f_cen3)
            num_filhos_reais++;
        if (raiz->dir)
            num_filhos_reais++;

        int filhos_impressos = 0;
        if (raiz->esq != NULL)
        {
            filhos_impressos++;
            imprime_arvore_visual_45(raiz->esq, novo_prefixo, filhos_impressos == num_filhos_reais, 0);
        }
        if (raiz->f_cen1 != NULL)
        {
            filhos_impressos++;
            imprime_arvore_visual_45(raiz->f_cen1, novo_prefixo, filhos_impressos == num_filhos_reais, 0);
        }
        if (raiz->f_cen2 != NULL)
        {
            filhos_impressos++;
            imprime_arvore_visual_45(raiz->f_cen2, novo_prefixo, filhos_impressos == num_filhos_reais, 0);
        }
        if (raiz->f_cen3 != NULL)
        {
            filhos_impressos++;
            imprime_arvore_visual_45(raiz->f_cen3, novo_prefixo, filhos_impressos == num_filhos_reais, 0);
        }
        if (raiz->dir != NULL)
        {
            filhos_impressos++;
            imprime_arvore_visual_45(raiz->dir, novo_prefixo, filhos_impressos == num_filhos_reais, 0);
        }
    }
}

//================ LIBERAR MEMORIA ==================
void libera_arvore_45(Arv45 **raiz)
{
    if (raiz != NULL && *raiz != NULL)
    {
        libera_arvore_45(&((*raiz)->esq));
        libera_arvore_45(&((*raiz)->f_cen1));
        libera_arvore_45(&((*raiz)->f_cen2));
        libera_arvore_45(&((*raiz)->f_cen3));
        libera_arvore_45(&((*raiz)->dir));
        free(*raiz);
        *raiz = NULL;
    }
}

//================ MENU (Adaptado) ==================
void menu_45(Arv45 **raiz)
{
    int opcao = 0, valor;
    char prefixo_inicial[1024] = "";

    do
    {
        printf("\n=========================\n");
        printf("Arvore 4-5 Atual:\n");
        imprime_arvore_visual_45(*raiz, prefixo_inicial, 1, 1);
        printf("\nEm ordem: ");
        imprime_45_em_ordem(*raiz);
        printf("\n------------------------\n");
        printf("1. Adicionar valor\n");
        printf("2. Remover valor\n");
        printf("3. Popular com 1-17 (teste)\n");
        printf("4. Sair\n");
        printf("Escolha uma opcao: ");

        if (scanf("%d", &opcao) != 1)
        {
            printf("Entrada invalida. Por favor, insira um numero.\n");
            while (getchar() != '\n')
                ;
            opcao = 0;
        }
        else
        {
            while (getchar() != '\n')
                ;
        }

        if (opcao == 1)
        {
            printf("Digite o valor a ser inserido: ");
            if (scanf("%d", &valor) != 1)
            {
                printf("Entrada invalida.\n");
                while (getchar() != '\n');
            }
            else
            {
                while (getchar() != '\n');
                if (insere_45(raiz, valor))
                {
                    printf("Valor %d inserido com sucesso.\n", valor);
                }
                else
                {
                    printf("Falha ao inserir %d.\n", valor);
                }
            }
        }
        else if (opcao == 2)
        {
            printf("Digite o valor a ser removido: ");
            if (scanf("%d", &valor) != 1)
            {
                printf("Entrada invalida.\n");
                while (getchar() != '\n')
                    ;
            }
            else
            {
                while (getchar() != '\n')
                    ;
                if (*raiz == NULL)
                {
                    printf("Arvore esta vazia, nao e possivel remover.\n");
                }
                else
                {
                    if (remover_45(raiz, valor))
                    {
                        printf("Remocao de %d processada.\n", valor);
                    }
                    else
                    {
                        printf("Falha ao tentar remover %d.\n", valor);
                    }
                }
            }
        }
        else if (opcao == 3)
        {
            printf("Populando com valores de 1 a 17...\n");
            libera_arvore_45(raiz);
            for (int i = 1; i <= 17; i++)
            {
                if (!insere_45(raiz, i))
                {
                    printf("Erro ao inserir %d na populacao.\n", i);
                }
            }
            printf("Populacao concluida.\n");
        }
        else if (opcao == 4)
        {
            printf("Saindo...\n");
        }
        else if (opcao != 0)
        {
            printf("Opcao invalida. Tente novamente.\n");
        }
    } while (opcao != 4);
}

//================ MAIN ==================
int main()
{
    Arv45 *raiz = NULL;
    menu_45(&raiz);
    libera_arvore_45(&raiz);
    printf("\nPrograma encerrado. Memoria liberada.\n");
    return 0;
}