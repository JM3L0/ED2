#pragma once

// insercao de CEP

int inserir_no_CEP(CEP **raiz, CEP *novoCEP);
int inserir_CEP(CEP **raiz, CEP *novoCEP);

// criacao de CEP
CEP *alocaCEP();
CEP *criaCEP(char *str_cep);
CEP *cadastrarCEP();

// rotacao de CEP
void rotacao_esquerda(CEP **raiz);
void rotacao_direita(CEP **raiz);

// propriedades
int Cor(CEP *cep);
void trocar_cor(CEP *raiz);
void balancear_RB(CEP **raiz);

// busca e impressao
CEP *buscar_CEP(CEP *raiz, char *str_cep);
void imprimir_CEP(CEP *cep);
void imprimir_CEP_em_ordem(CEP *raiz);
void imprimir_todos_CEP(CEP *raiz);

// desalocacao
void desalocar_CEP(CEP **raiz);
void desalocar_arvore_CEP(CEP **raiz);

//consulta
int consulta_CEP(CEP *raiz, char *str_cep);

// removal
CEP *encontrar_menor_CEP(CEP *raiz);
void trocar_informacoes_CEP(CEP *cep1, CEP *cep2);
void mover2_esquerda(CEP **raiz);
void mover2_direita(CEP **raiz);
void remover_menor_CEP_arv(CEP **raiz);
int remover_CEP_no(CEP **raiz, char *str_cep);
int remover_CEP_arvore(CEP **raiz, char *str_cep);

// especificas do trabalho