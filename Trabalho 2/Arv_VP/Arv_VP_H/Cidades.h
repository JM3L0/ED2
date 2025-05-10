#pragma once
#include "../Arv_VP_H/STRUCTS.h"

CIDADES *alocaCidade();
CIDADES *criaCidade(char *nome_cidade, int populacao_city);
CIDADES *cadastrarCidade();
int corCidade(CIDADES *cidade);
void rotacao_esquerda(CIDADES **raiz);
void rotacao_direita(CIDADES **raiz);
void trocar_cor(CIDADES *raiz);
void balancear_RB(CIDADES **raiz);
CIDADES *inserir_no_Cidade(CIDADES **raiz, CIDADES *novaCidade);
CIDADES *inserir_Cidade(CIDADES **raiz, CIDADES *novaCidade);
CIDADES *buscar_cidade(CIDADES *raiz, char *nome_cidade);
void imprimir_cidade(CIDADES *cidade);
void imprimir_cidades_em_ordem(CIDADES *raiz);
void imprimir_todas_cidades(CIDADES *raiz);
void limpar_no_cidade(CIDADES *cidade);
void desalocar_cidade(CIDADES **raiz);
void desalocar_arvore_cidades(CIDADES **raiz);
void mover_esquerda(CIDADES **raiz);
void mover_direita(CIDADES **raiz);
void remover_menor_cidade_arv(CIDADES **raiz);
int remover_cidade_no(CIDADES **raiz, char *nome_cidade);
int remover_cidade_arvore(CIDADES **raiz, char *nome_cidade);