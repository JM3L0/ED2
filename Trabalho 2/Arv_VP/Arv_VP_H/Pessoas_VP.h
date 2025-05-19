#pragma once
#include "../Arv_VP_H/STRUCTS_VP.h"
// insercao de pessoas
int inserir_no_pessoa(PESSOAS **raiz, PESSOAS *novaPessoa);
int inserir_pessoa(PESSOAS **raiz, PESSOAS *novaPessoa);
PESSOAS *aloca_pessoa(char *nome_pessoa, char *CPF, char *cep_natal, char *cep_atual, DATA data_nasc);
PESSOAS *cadastra_pessoa(char *cep_natal, char *cep_atual);

// rotacao de pessoas
void rotacao_esquerda_pessoa(PESSOAS **raiz);
void rotacao_direita_pessoa(PESSOAS **raiz);

// propriedades de pessoas
int Cor_pessoa(PESSOAS *pessoa);
void trocar_cor_pessoa(PESSOAS *raiz);
void balancear_RB_pessoa(PESSOAS **raiz);

// busca e impressao de pessoas
PESSOAS *existe_pessoa(PESSOAS *raiz, char *CPF);
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
void mover2_esquerda_pessoa(PESSOAS **raiz);
void mover2_direita_pessoa(PESSOAS **raiz);
void remover_menor_pessoa_arv(PESSOAS **raiz);
int remover_pessoa_no(PESSOAS **raiz, char *CPF);
int remover_pessoa_arvore(PESSOAS **raiz, char *CPF);

// especificas do trabalho
int verifica_pessoa_nascida_ou_que_mora_na_cidade(PESSOAS *raiz_pessoa, CEP *raiz_CEP);
int quantas_pessoas_nao_moram_na_cidade_natal_ESTADO(ESTADOS *cabeca_estado, PESSOAS *raiz_pessoa);
int quantas_pessoas_nao_moram_na_cidade_natal_PESSOAS(PESSOAS *raiz_pessoa, CIDADES *raiz_cidade);
int quantas_pessoas_moram_na_cidade_nao_nasceram_nela(CIDADES *cidade, PESSOAS *raiz_pessoa);