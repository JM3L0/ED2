#pragma once
#include "../Arv_VP_H/STRUCTS_VP.h"

// Incerções e alocações
int inserir_no_Cidade(CIDADES **raiz, CIDADES *novaCidade);
int inserir_Cidade(CIDADES **raiz, CIDADES *novaCidade);
CIDADES *aloca_Cidade(char *nome_cidade, int populacao_city);
CIDADES *cadastrarCidade();

// Rotações
void rotacao_esquerda_cidade(CIDADES **raiz);
void rotacao_direita_cidade(CIDADES **raiz);

// Propriedades da árvore
int Cor_cidade(CIDADES *cidade);
void trocar_cor_cidade(CIDADES *raiz);
void balancear_RB_cidade(CIDADES **raiz);

// Busca e impressão
CIDADES *existe_cidade(CIDADES *raiz, char *nome_cidade);
void imprimir_cidade(CIDADES *cidade);
void imprimir_cidades_em_ordem(CIDADES *raiz);
void imprimir_todas_cidades(CIDADES *raiz);
void limpar_info_no_cidade(CIDADES *cidade);

// Desalocação
void limpar_no_cidade(CIDADES **raiz);
void limpar_arvore_cidades(CIDADES **raiz);

// especificas do trabalho
CIDADES *verifica_cidade_mais_populosa_nao_capital(CIDADES *raiz, char *nome_capital);
int cep_pertence_a_cidade(CEP *raiz, char *cep);
CIDADES *cidade_natal_dado_cep(CIDADES *raiz, char *cep);
int quantas_pessoas_nascidas_na_cidade_nao_moram_na_cidade(CIDADES *cidade, PESSOAS *raiz_pessoa);