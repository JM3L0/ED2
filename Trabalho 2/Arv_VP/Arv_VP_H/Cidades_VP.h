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
CIDADES *buscar_cidade(CIDADES *raiz, char *nome_cidade);
void imprimir_cidade(CIDADES *cidade);
void imprimir_cidades_em_ordem(CIDADES *raiz);
void imprimir_todas_cidades(CIDADES *raiz);
void limpar_info_no_cidade(CIDADES *cidade);

// Desalocação
void limpa_no_cidade(CIDADES **raiz);
void limpa_arvore_cidades(CIDADES **raiz);

//consulta
int consulta_cidade(CIDADES *raiz, char *nome_cidade);
 
// Remoção
CIDADES *encontrar_menor_cidade(CIDADES *raiz);
void trocar_informacoes_cidades(CIDADES *cidade1, CIDADES *cidade2);
void mover2_esquerda_cidade(CIDADES **raiz);
void mover2_direita_cidade(CIDADES **raiz);
void remover_menor_cidade_arv(CIDADES **raiz);
int remover_cidade_no(CIDADES **raiz, char *nome_cidade);
int remover_cidade_arvore(CIDADES **raiz, char *nome_cidade); 

// especificas do trabalho
CIDADES *verifica_cidade_mais_populosa_nao_capital(CIDADES *raiz, char *nome_capital);
