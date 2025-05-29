#pragma once

#include "../Arv_23_H/STRUCTS_23.h"

// ================ CRIAÇÃO E INICIALIZAÇÃO ================
Arv23_CEP *cria_no_CEP(CEP info, Arv23_CEP *F_esq, Arv23_CEP *F_cen);
int eh_folha_CEP(Arv23_CEP *no);

// ================ OPERAÇÕES DE INSERÇÃO ================
void adiciona_infos_CEP(Arv23_CEP **no, CEP info, Arv23_CEP *Sub_Arv_Info);
Arv23_CEP *quebra_no_CEP(Arv23_CEP **no, CEP info, CEP *sobe, Arv23_CEP *F_dir);
int insere_23_recursivo_CEP(Arv23_CEP **raiz, CEP valor, CEP *sobe, Arv23_CEP **maiorNo);
int insere_23_CEP(Arv23_CEP **raiz, CEP valor);

// ================ OPERAÇÕES DE TRAVESSIA E VISUALIZAÇÃO ================
void imprime_23_em_ordem_CEP(Arv23_CEP *raiz);
void imprime_arvore_visual_CEP(Arv23_CEP *raiz, char *prefixo, int eh_ultimo, int eh_raiz);

// ================ OPERAÇÕES DE BUSCA ================
Arv23_CEP *buscar_menor_elemento_CEP(Arv23_CEP *no);

// ================ OPERAÇÕES DE REMOÇÃO ================
StatusRemocao remover_23_CEP_recursivo_CEP(Arv23_CEP **ponteiro_no_atual, CEP valor);
StatusRemocao remover_23_CEP(Arv23_CEP **raiz, CEP valor);
StatusRemocao tratar_underflow_CEP(Arv23_CEP **ponteiro_filho_no_pai, Arv23_CEP *pai);
StatusRemocao redistribuir_com_irmao_esquerdo_CEP(Arv23_CEP **ponteiro_filho_no_pai, Arv23_CEP *pai, Arv23_CEP *irmao_esq, int pos_filho);
StatusRemocao redistribuir_com_irmao_direito_CEP(Arv23_CEP **ponteiro_filho_no_pai, Arv23_CEP *pai, Arv23_CEP *irmao_dir, int pos_filho);
StatusRemocao fundir_com_irmao_esquerdo_CEP(Arv23_CEP **ponteiro_filho_no_pai, Arv23_CEP *pai, Arv23_CEP *irmao_esq, int pos_filho);
StatusRemocao fundir_com_irmao_direito_CEP(Arv23_CEP **ponteiro_filho_no_pai, Arv23_CEP *pai, Arv23_CEP *irmao_dir, int pos_filho);

// ================ GERENCIAMENTO DE MEMÓRIA ================
void libera_arvore_CEP(Arv23_CEP **raiz);

// ================ CONSELTA DE CEP ================
int consulta_CEP(Arv23_CEP *raiz, char *str_cep);
int percorre_estados_procurando_CEP(ESTADOS *cabeca, char *cep);
int percorre_cidades_procurando_CEP(Arv23_CIDADES *raiz, char *cep);
