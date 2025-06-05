#pragma once

#include "../Arv_23_H/STRUCTS_23.h"

//==============================================================================
// FUNÇÕES DE MANIPULAÇÃO DE DADOS
//==============================================================================
PESSOAS cadastra_pessoa(char *cep_natal, char *cep_atual, int *sucesso);

//==============================================================================
// FUNÇÕES BÁSICAS DA ÁRVORE 2-3
//==============================================================================
Arv23_PESSOAS *cria_no_PESSOAS(PESSOAS info, Arv23_PESSOAS *F_esq, Arv23_PESSOAS *F_cen);
int eh_folha_PESSOAS(Arv23_PESSOAS *no);
Arv23_PESSOAS *buscar_menor_elemento_PESSOAS(Arv23_PESSOAS *no);
PESSOAS *buscar_info_pessoa(Arv23_PESSOAS *raiz, char *info);

//==============================================================================
// FUNÇÕES DE INSERÇÃO NA ÁRVORE 2-3
//==============================================================================
void adiciona_infos_PESSOAS(Arv23_PESSOAS **no, PESSOAS info, Arv23_PESSOAS *Sub_Arv_Info);
Arv23_PESSOAS *quebra_no_PESSOAS(Arv23_PESSOAS **no, PESSOAS info, PESSOAS *sobe, Arv23_PESSOAS *F_dir);
int insere_23_PESSOAS(Arv23_PESSOAS **raiz, PESSOAS valor);
int insere_23_recursivo_PESSOAS(Arv23_PESSOAS **raiz, PESSOAS valor, PESSOAS *sobe, Arv23_PESSOAS **maiorNo);

//==============================================================================
// FUNÇÕES DE REMOÇÃO NA ÁRVORE 2-3
//==============================================================================
StatusRemocao remover_23_PESSOAS_recursivo(Arv23_PESSOAS **ponteiro_no_atual, PESSOAS valor);
StatusRemocao remover_23_PESSOAS(Arv23_PESSOAS **raiz, PESSOAS valor);

//==============================================================================
// FUNÇÕES DE REBALANCEAMENTO DA ÁRVORE 2-3
//==============================================================================
StatusRemocao redistribuir_com_irmao_esquerdo_PESSOAS(Arv23_PESSOAS **ponteiro_filho_no_pai, Arv23_PESSOAS *pai, Arv23_PESSOAS *irmao_esq, int pos_filho);
StatusRemocao redistribuir_com_irmao_direito_PESSOAS(Arv23_PESSOAS **ponteiro_filho_no_pai, Arv23_PESSOAS *pai, Arv23_PESSOAS *irmao_dir, int pos_filho);
StatusRemocao fundir_com_irmao_esquerdo_PESSOAS(Arv23_PESSOAS **ponteiro_filho_no_pai, Arv23_PESSOAS *pai, Arv23_PESSOAS *irmao_esq, int pos_filho);
StatusRemocao fundir_com_irmao_direito_PESSOAS(Arv23_PESSOAS **ponteiro_filho_no_pai, Arv23_PESSOAS *pai, Arv23_PESSOAS *irmao_dir, int pos_filho);
StatusRemocao tratar_underflow_PESSOAS(Arv23_PESSOAS **ponteiro_filho_no_pai, Arv23_PESSOAS *pai);

//==============================================================================
// FUNÇÕES DE GERENCIAMENTO DE MEMÓRIA
//==============================================================================
void libera_arvore_PESSOAS(Arv23_PESSOAS **raiz);

//==============================================================================
// FUNÇÕES DE EXIBIÇÃO E PERCURSO DA ÁRVORE 2-3
//==============================================================================
void imprimir_dados_PESSOAS(PESSOAS *pessoa);
void imprime_23_em_ordem_PESSOAS(Arv23_PESSOAS *raiz);
void imprime_arvore_visual_PESSOAS(Arv23_PESSOAS *raiz, char *prefixo, int eh_ultimo, int eh_raiz);
int verificar_se_existe_pessoa_associada_a_um_CEP(Arv23_PESSOAS *raiz_pessoas, char *CEP);

//==============================================================================
// FUNÇÕES ESPECIFÍCAS DO TRABALHO
//==============================================================================
int verificar_se_existe_pessoa_associada_a_um_CEP(Arv23_PESSOAS *raiz_pessoas, char *CEP);
int quantas_pessoas_nao_moram_na_cidade_natal_ESTADO(ESTADOS *cabeca_estado, Arv23_PESSOAS *raiz_pessoa);
// int quantas_pessoas_nao_moram_na_cidade_natal_PESSOAS(Arv23_PESSOAS *raiz_pessoa, Arv23_CIDADES *raiz_cidade);
int quantas_pessoas_nascidas_na_cidade_nao_moram_nela(CIDADES *cidade, Arv23_PESSOAS *raiz_pessoa);
int quantas_pessoas_moram_na_cidade_nao_nasceram_nela(CIDADES *cidade, Arv23_PESSOAS *raiz_pessoa);