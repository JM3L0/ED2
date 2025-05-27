
#pragma once

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
int insere_23_recursivo(Arv23 **raiz, int valor, Arv23 *pai, int *sobe, Arv23 **maiorNo);
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