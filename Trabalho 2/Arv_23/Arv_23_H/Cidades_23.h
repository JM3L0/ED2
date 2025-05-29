#pragma once

#include "../Arv_23_H/STRUCTS_23.h"

// Prototipos das funcoes

// ======================= Operações Básicas =======================
// Função para cadastrar uma cidade
CIDADES cadastra_cidade(int *sucesso);

// ======================= Operações de Nós =======================
// Funções para manipulação dos nós da árvore 2-3
Arv23_CIDADES *cria_no_cidade(CIDADES info, Arv23_CIDADES *F_esq, Arv23_CIDADES *F_cen);
void adiciona_infos_cidade(Arv23_CIDADES **no, CIDADES info, Arv23_CIDADES *Sub_Arv_Info);
Arv23_CIDADES *quebra_no_cidade(Arv23_CIDADES **no, CIDADES info, CIDADES *sobe, Arv23_CIDADES *F_dir);

// ======================= Operações da Árvore =======================
// Funções para inserção e manipulação da árvore 2-3
int insere_23_cidade(Arv23_CIDADES **raiz, CIDADES valor);
int insere_23_recursivo_cidade(Arv23_CIDADES **raiz, CIDADES valor, CIDADES *info_sobe, Arv23_CIDADES **filho_dir);
void libera_arvore_cidade(Arv23_CIDADES **raiz);

//================ VERIFICA FOLHA ==================
int eh_folha_cidade(Arv23_CIDADES *no);

// ======================= Funções de Busca =======================
// Funções para busca na árvore 2-3
Arv23_CIDADES *buscar_menor_elemento_cidade(Arv23_CIDADES *no);
CIDADES *buscar_info_cidade(Arv23_CIDADES*raiz, char *info);