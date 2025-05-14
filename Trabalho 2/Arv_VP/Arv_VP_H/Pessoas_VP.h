#pragma once
#include "../Arv_VP_H/STRUCTS_VP.h"
// insercao de pessoas
int inserir_no_pessoa(PESSOAS **raiz, PESSOAS *novaPessoa);
int inserir_pessoa(PESSOAS **raiz, PESSOAS *novaPessoa);
PESSOAS *aloca_pessoa(char* nome_pessoa, char *CPF, char *cep_natal, char *cep_atual, DATA data_nasc);
PESSOAS *cadastra_pessoa(char *cep_natal, char *cep_atual);

// rotacao de pessoas
void rotacao_esquerda(PESSOAS **raiz);
void rotacao_direita(PESSOAS **raiz);

// propriedades de pessoas
int Cor(PESSOAS *pessoa);
void trocar_cor(PESSOAS *raiz);
void balancear_RB(PESSOAS **raiz);

// busca e impressao de pessoas
PESSOAS *buscar_pessoa(PESSOAS *raiz, char *CPF);
void imprimir_pessoa(PESSOAS *pessoa);
void imprimir_pessoas_em_ordem(PESSOAS *raiz);
void imprimir_todas_pessoas(PESSOAS *raiz);

// desalocacao de pessoas
void liberar_no_pessoa(PESSOAS **raiz);
void liberar_arvore_pessoas(PESSOAS **raiz);

// consulta de pessoas
int consulta_pessoa(PESSOAS *raiz, char *CPF);

// removal de pessoas
PESSOAS *encontrar_menor_pessoa(PESSOAS *raiz);
void trocar_informacoes_pessoas(PESSOAS *pessoa1, PESSOAS *pessoa2);
void mover2_esquerda(PESSOAS **raiz);
void mover2_direita(PESSOAS **raiz);
void remover_menor_pessoa_arv(PESSOAS **raiz);
int remover_pessoa_no(PESSOAS **raiz, char *CPF);
int remover_pessoa_arvore(PESSOAS **raiz, char *CPF);

// especificas do trabalho