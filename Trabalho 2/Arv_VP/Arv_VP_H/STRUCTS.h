#pragma once

#define RED 1
#define BLACK 0

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
    char *cidade;
    int cor;
    struct CEP *cep;
    struct CIDADES *esq, *dir;

} CIDADES;

typedef struct ESTADOS// lista duplamente encadeada
{
    char *estado;
    int cor;
    struct CIDADES *cidade;
    struct ESTADOS *prox, *ant;

} ESTADOS;

typedef struct PESSOAS
{
    char CPF[12];
    char *nome_pessoa;
    char city_natal[9];
    char city_atual[9];
    DATA data_nasc;

} PESSOAS;