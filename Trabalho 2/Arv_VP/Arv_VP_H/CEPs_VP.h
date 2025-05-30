#pragma once
#include "../Arv_VP_H/STRUCTS_VP.h"

// insercao de CEP

int inserir_no_CEP(CEP **raiz, CEP *novoCEP);
int inserir_CEP(CEP **raiz, CEP *novoCEP);

// criacao de CEP
CEP *alocaCEP(char *str_cep);
CEP *cadastrarCEP();

// rotacao de CEP
void rotacao_esquerda_CEP(CEP **raiz);
void rotacao_direita_CEP(CEP **raiz);

// propriedades
int Cor_CEP(CEP *cep);
void trocar_cor_CEP(CEP *raiz);
void balancear_RB_CEP(CEP **raiz);

// busca e impressao
CEP *existe_CEP(CEP *raiz, char *str_cep);
void imprimir_CEP(CEP *cep);
void imprimir_CEP_em_ordem(CEP *raiz);
void imprimir_todos_CEP(CEP *raiz);

// desalocacao
void libera_no_CEP(CEP **raiz);
void desalocar_arvore_CEP(CEP **raiz);

//consulta
int consulta_CEP(CEP *raiz, char *str_cep);

// removal
CEP *encontrar_menor_CEP(CEP *raiz);
void trocar_informacoes_CEP(CEP *cep1, CEP *cep2);
void mover2_esquerda_CEP(CEP **raiz);
void mover2_direita_CEP(CEP **raiz);
void remover_menor_CEP_arv(CEP **raiz);
int remover_CEP_no(CEP **raiz, char *str_cep);
int remover_CEP_arvore(CEP **raiz, char *str_cep);

// especificas do trabalho
int percorre_estados_procurando_CEP(ESTADOS *cabeca, char *cep);
int percorre_cidades_procurando_CEP(CIDADES *raiz, char *cep);