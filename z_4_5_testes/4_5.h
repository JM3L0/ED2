#pragma once

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