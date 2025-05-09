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
    char cep[9];
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
    struct CIDADES *cidade;
    struct ESTADOS *prox, *ant;

} ESTADOS;

typedef struct PESSOAS
{
    char CPF[12];
    char *nome_pessoa;
    char cep_city_natal[9];
    char cep_city_atual[9];
    int cor;
    DATA data_nasc;

} PESSOAS;