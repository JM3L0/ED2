#pragma once

#include "../Arv_23_H/STRUCTS_23.h"

// Prototipos das funcoes

// ======================= OPERAÇÕES BASICAS =======================
// Função para cadastrar uma cidade
CIDADES cadastra_cidade(int *sucesso);
int eh_folha_cidade(Arv23_CIDADES *no);

// ======================= FUNÇÕES AUX DE INSERÇÃO =======================
Arv23_CIDADES *cria_no_cidade(CIDADES info, Arv23_CIDADES *F_esq, Arv23_CIDADES *F_cen);
void adiciona_infos_cidade(Arv23_CIDADES **no, CIDADES info, Arv23_CIDADES *Sub_Arv_Info);
Arv23_CIDADES *quebra_no_cidade(Arv23_CIDADES **no, CIDADES info, CIDADES *sobe, Arv23_CIDADES *F_dir);

// ======================= OPERAÇÃO DE INSERÇÃO =======================
int insere_23_cidade(Arv23_CIDADES **raiz, CIDADES valor);
int insere_23_recursivo_cidade(Arv23_CIDADES **raiz, CIDADES valor, CIDADES *info_sobe, Arv23_CIDADES **filho_dir);

////================= FUNÇÃO DE LIBERAÇÃO DA ÁRVORE ==================
void libera_arvore_cidade(Arv23_CIDADES **raiz);

// ======================= FUNÇÕES DE BUSCA =======================
Arv23_CIDADES *buscar_menor_elemento_cidade(Arv23_CIDADES *no);
CIDADES *buscar_info_cidade(Arv23_CIDADES*raiz, char *info);

//================= FUNÇÕES DE PRINTAR ==================
void printar_informacoes_cidade(CIDADES *cidade);
void imprime_23_em_ordem_cidade(Arv23_CIDADES *raiz);