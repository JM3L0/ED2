#pragma once

#include "../Arv_23_H/STRUCTS_23.h"


//==============================================================================
// FUNÇÕES DE MANIPULAÇÃO DE DADOS
//==============================================================================
CIDADES cadastra_cidade(int *sucesso);

//==============================================================================
// FUNÇÕES BÁSICAS DA ÁRVORE 2-3
//==============================================================================
Arv23_CIDADES *cria_no_cidade(CIDADES info, Arv23_CIDADES *F_esq, Arv23_CIDADES *F_cen);
int eh_folha_cidade(Arv23_CIDADES *no);
// Arv23_CIDADES *buscar_menor_elemento_cidade(Arv23_CIDADES *no);
CIDADES *buscar_info_cidade(Arv23_CIDADES*raiz, char *info);

//==============================================================================
// FUNÇÕES DE INSERÇÃO NA ÁRVORE 2-3
//==============================================================================
void adiciona_infos_cidade(Arv23_CIDADES **no, CIDADES info, Arv23_CIDADES *Sub_Arv_Info);
Arv23_CIDADES *quebra_no_cidade(Arv23_CIDADES **no, CIDADES info, CIDADES *sobe, Arv23_CIDADES *F_dir);
int insere_23_cidade(Arv23_CIDADES **raiz, CIDADES valor);
int insere_23_recursivo_cidade(Arv23_CIDADES **raiz, CIDADES valor, CIDADES *info_sobe, Arv23_CIDADES **filho_dir);

//==============================================================================
// FUNÇÕES DE GERENCIAMENTO DE MEMÓRIA
//==============================================================================
void libera_arvore_cidade(Arv23_CIDADES **raiz);

//==============================================================================
// FUNÇÕES DE EXIBIÇÃO E PERCURSO DA ÁRVORE 2-3
//==============================================================================
void printar_informacoes_cidade(CIDADES *cidade);
void imprime_23_em_ordem_cidade(Arv23_CIDADES *raiz);
void imprime_arvore_visual_cidade(Arv23_CIDADES *raiz, char *prefixo, int eh_ultimo, int eh_raiz);

//==============================================================================
// FUNÇÕES ESPECÍFICAS DO TRABALHO
//==============================================================================
CIDADES *verifica_cidade_mais_populosa_nao_capital_23(Arv23_CIDADES *raiz, char *nome_capital);
int cep_pertence_a_cidade(Arv23_CEP *raiz, char *cep);
CIDADES *cidade_dado_cep(Arv23_CIDADES *raiz, char *cep);
