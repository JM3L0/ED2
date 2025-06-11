#pragma once

// Declarações antecipadas de estruturas
typedef struct DATA DATA;
typedef struct CEP CEP;
typedef struct CIDADES CIDADES;
typedef struct ESTADOS ESTADOS;
typedef struct PESSOAS PESSOAS;
typedef struct Arv23_CEP Arv23_CEP;
typedef struct Arv23_CIDADES Arv23_CIDADES;
typedef struct Arv23_PESSOAS Arv23_PESSOAS;

// ===== enum de status de remoção =====
typedef enum {
    OK,
    UNDERFLOW,
    INFO_NAO_ENCONTRADA,
    SUCESSOR_NAO_ENCONTRADO,
    ARVORE_VAZIA,
    NAO_FOI_POSSIVEL_T_UNDERFLOW
} StatusRemocao;

// ===== Lista Duplamente =====
typedef struct ESTADOS// lista duplamente encadeada
{
    char *nome_estado;
    char nome_capital[100];
    int quant_city;
    int populacao_estado;
    struct Arv23_CIDADES *arv_cidades;

    struct ESTADOS *prox, *ant;

} ESTADOS;

// ===== INFOS =====
typedef struct DATA// DATA DE NASCIMENTO
{
    int dia;
    int mes;
    int ano;

} DATA;

typedef struct CEP
{
    char cep[10];

} CEP;

typedef struct CIDADES
{
    char nome_cidade[100];
    int populacao_city;
    struct Arv23_CEP *arv_cep;

} CIDADES;


typedef struct PESSOAS
{
    char CPF[12];
    char nome_pessoa[100];
    char cep_city_natal[10];
    char cep_city_atual[10];
    DATA data_nasc;

} PESSOAS;

// ===== ARVORE 2-3 =====

typedef struct Arv23_CEP{

    CEP info1, info2;
    int nInfo;

    struct Arv23_CEP *esq, *cen, *dir;
} Arv23_CEP;

typedef struct Arv23_CIDADES{

    CIDADES info1, info2;
    int nInfo;

    struct Arv23_CIDADES *esq, *cen, *dir;
} Arv23_CIDADES;

typedef struct Arv23_PESSOAS{

    struct PESSOAS info1, info2;
    int nInfo;

    struct Arv23_PESSOAS *esq, *cen, *dir;
} Arv23_PESSOAS;