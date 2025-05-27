#pragma once

#include "../Arv_23_H/STRUCTS_23.h"

CIDADES cadastra_cidade(int *sucesso);
Arv23_CIDADES *cria_no_cidade(CIDADES info, Arv23_CIDADES *F_esq, Arv23_CIDADES *F_cen);
void adiciona_infos_cidade(Arv23_CIDADES **no, CIDADES info, Arv23_CIDADES *Sub_Arv_Info);
Arv23_CIDADES *quebra_no_cidade(Arv23_CIDADES **no, CIDADES info, CIDADES *sobe, Arv23_CIDADES *F_dir);
int insere_23_cidade(Arv23_CIDADES **raiz, CIDADES valor);
int insere_23_recursivo_cidade(Arv23_CIDADES **raiz, CIDADES valor, Arv23_CIDADES *pai, CIDADES *info_sobe, Arv23_CIDADES **filho_dir);
void libera_arvore_cidade(Arv23_CIDADES **raiz);
// void libera_arvore_cep(Arv23_CEP **raiz);//implementar posteriormente
int eh_folha_cidade(Arv23_CIDADES *no);
Arv23_CIDADES *buscar_menor_elemento_cidade(Arv23_CIDADES *no);
StatusRemocao tratar_underflow_cidade(Arv23_CIDADES **ponteiro_filho_no_pai, Arv23_CIDADES *pai);
StatusRemocao redistribuir_com_irmao_esquerdo(Arv23_CIDADES **ponteiro_filho_no_pai, Arv23_CIDADES *pai, Arv23_CIDADES *irmao_esq, int pos_filho);
StatusRemocao redistribuir_com_irmao_direito(Arv23_CIDADES **ponteiro_filho_no_pai, Arv23_CIDADES *pai, Arv23_CIDADES *irmao_dir, int pos_filho);
StatusRemocao fundir_com_irmao_esquerdo(Arv23_CIDADES **ponteiro_filho_no_pai, Arv23_CIDADES *pai, Arv23_CIDADES *irmao_esq, int pos_filho);
StatusRemocao fundir_com_irmao_direito(Arv23_CIDADES **ponteiro_filho_no_pai, Arv23_CIDADES *pai, Arv23_CIDADES *irmao_dir, int pos_filho);
StatusRemocao remover_23_recursivo_cidade(Arv23_CIDADES **ponteiro_no_atual, CIDADES valor);
int remover_23_cidade(Arv23_CIDADES **raiz, CIDADES valor);