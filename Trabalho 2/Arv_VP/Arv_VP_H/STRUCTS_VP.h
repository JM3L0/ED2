#pragma once

#define BLACK 0
#define RED 1

typedef struct DATA// DATA DE NASCIMENTO
{
    int dia;
    int mes;
    int ano;

} DATA;

typedef struct CEP
{
    char cep[10];
    int cor;

    struct CEP *esq, *dir;

} CEP;

typedef struct CIDADES
{
    char *nome_cidade;
    int populacao_city;
    int cor;
    struct CEP *cep;
    
    struct CIDADES *esq, *dir;

} CIDADES;

typedef struct ESTADOS// lista duplamente encadeada
{
    char *nome_estado;
    char *nome_capital;
    int quant_city;
    int populacao_estado;
    struct CIDADES *cidade;

    struct ESTADOS *prox, *ant;

} ESTADOS;

typedef struct PESSOAS
{
    char CPF[12];
    char *nome_pessoa;
    char cep_city_natal[10];
    char cep_city_atual[10];
    int cor;
    DATA data_nasc;

    struct PESSOAS *esq, *dir;

} PESSOAS;