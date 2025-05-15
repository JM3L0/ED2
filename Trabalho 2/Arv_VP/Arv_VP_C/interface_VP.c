/*
cd "c:\Users\raild\Downloads\UFPI\4° período\ED2\Trabalho 2\Arv_VP\Arv_VP_C"
gcc -o interface interface.c Estados.c Cidades.c CEPs.c Pessoas.c utilitarios.c -I "../Arv_VP_H"
.\interface
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../Arv_VP_H/CEPs_VP.h"
#include "../Arv_VP_H/Cidades_VP.h"
#include "../Arv_VP_H/Estados_VP.h"
#include "../Arv_VP_H/Pessoas_VP.h"
#include "../Arv_VP_H/STRUCTS_VP.h"
#include "../Arv_VP_H/utilitarios_VP.h"
#include "../Arv_VP_H/interface_VP.h"

void menu_print()
{
    printf("\n[1] - Cadastrar estado\n");
    printf("[2] - Cadastrar cidade\n");
    printf("[3] - Cadastrar CEP\n");
    printf("[4] - Cadastrar pessoa\n");
    printf("[5] - Remover CEP\n");
    printf("[6] - Remover pessoa\n");
    printf("[7] - Imprimir estado mais populoso\n");
    printf("[8] - Imprimir populacao da Capital de um determinado estado\n");
    printf("[9] - Imprimir cidade mais populosa de um estado sem ser a Capital\n");
    printf("[10] - Imprimir quantas pessoas nao moram na cidade natal\n");
    printf("[11] - Imprimir qual cidade natal de uma pessoa dado o CEP da cidade\n");
    printf("[12] - Imprimir quantas pessoas nascidas em uma determinada cidade nao mora na cidade natal\n");
    printf("[13] - Imprimir quantas pessoas que moram em uma determinada cidade nao nasceram na cidade\n");
    printf("[0] - Sair\n");
}

void menu_geral()
{
    ESTADOS *cabeca_estado;
    cabeca_estado = NULL;

    int opcao1, opcao2, retorno = 0;

    do
    {
        menu_print();
        printf("Escolha uma opcao: ");
        opcao1 = digitar_int();
        printf("\n[1] - SIM\n[0] - NAO\nDeseja prosseguir com a acao?: ");
        opcao2 = digitar_int();
        if (opcao2 == 1)
        {
            switch (opcao1)
            {
            case 1: {
                ESTADOS *novo_estado = cadastro_estado();
                CIDADES *capital = NULL;

                if (!novo_estado) {
                    printf("Erro ao criar estado.\n");
                } else if (!inserir_estado_rec(&cabeca_estado, novo_estado)) {
                    printf("Erro ao cadastrar o estado.\n");
                } else {
                    printf("== Cadastro de Capital ==\n");
                    capital = cadastrarCidade();
                    if (!capital) {
                        printf("Erro ao cadastrar a capital.\n");
                    } else if (!inserir_Cidade(&novo_estado->cidade, capital)) {
                        printf("Erro ao inserir a capital no estado.\n");
                    } else {
                        // Cadastro dos CEPs
                        do {
                            CEP *novoCEP = cadastrarCEP();
                            if (novoCEP) {
                                if (percorre_estados_procurando_CEP(cabeca_estado, novoCEP->cep)) {
                                    printf("Erro: CEP já existe no sistema.\n");
                                } else if (!inserir_CEP(&capital->cep, novoCEP)) {
                                    printf("Erro ao inserir o CEP.\n");
                                } else {
                                    printf("CEP cadastrado com sucesso.\n");
                                    novoCEP = NULL; // evita desalocar se tiver sido inserido
                                }
                                if (novoCEP) libera_no_CEP(&novoCEP);
                            } else {
                                printf("Erro ao cadastrar o CEP.\n");
                            }
                            printf("\nDeseja cadastrar outro CEP? [1] - SIM [0] - NAO: ");
                            opcao2 = digitar_int();
                        } while (opcao2 == 1);

                        // Atualiza informações do estado
                        novo_estado->quant_city = 1;
                        novo_estado->populacao_estado += capital->populacao_city;
                    }
                }

                // Desalocações consolidadas para estado e capital
                if (capital && !novo_estado->cidade) limpa_no_cidade(&capital);
                if (novo_estado && !novo_estado->quant_city) desalocar_estado(&novo_estado);

                break;
            }
            case 2:{
                ESTADOS *estado;
                char nome_estado[100];

                printf("Digite o nome do estado para qual a cidade sera inserida: ");
                ler_string_simples(nome_estado, sizeof(nome_estado));

                estado = existe_estado(cabeca_estado, nome_estado);
                if (estado){
                    CIDADES *novaCidade = cadastrarCidade();
                    if (novaCidade)
                    {
                        retorno = inserir_Cidade(&estado->cidade, novaCidade);
                        
                        if (retorno){

                            estado->quant_city++;
                            estado->populacao_estado += novaCidade->populacao_city;

                            printf("Cadastre o CEP da cidade: ");

                            do{
                                CEP *novoCEP = cadastrarCEP();
                                if (novoCEP)
                                {
                                    retorno = inserir_CEP(&(novaCidade->cep), novoCEP);
                                    if (retorno)
                                    {
                                        printf("CEP cadastrado com sucesso.\n");
                                        printf("Deseja cadastrar outro CEP? [1] - SIM [0] - NAO: ");
                                        opcao2 = digitar_int();
                                    }
                                    else
                                    {
                                        printf("Erro ao inserir o CEP.\n");
                                    }
                                }
                                else
                                {
                                    printf("Erro ao cadastrar o CEP.\n");
                                }
                            }while (opcao2 == 1);
                            
                        }
                    }
                    else
                    {
                        printf("Erro ao cadastrar a cidade.\n");
                    }
                }
                else
                {
                    printf("Estado nao encontrado.\n");
                }

                break;
            }
            case 3:{
                ESTADOS *estado;
                char nome_estado[100];

                printf("Digite o nome do estado para qual o CEP sera inserido: ");
                ler_string_simples(nome_estado, sizeof(nome_estado));
                estado = existe_estado(cabeca_estado, nome_estado);
                if (estado){

                    CIDADES *cidade;
                    char nome_cidade[100];
                    printf("Digite o nome da cidade para qual o CEP sera inserido: ");
                    ler_string_simples(nome_cidade, sizeof(nome_cidade));
                    cidade = buscar_cidade(estado->cidade, nome_cidade);
                    if (cidade)
                    {
                        CEP *novoCEP = cadastrarCEP();
                        if (novoCEP)
                        {
                            retorno = inserir_CEP(&cidade->cep, novoCEP);
                        }
                        else
                        {
                            printf("Erro ao cadastrar o CEP.\n");
                        }
                    }
                    else
                    {
                        printf("Cidade nao encontrada.\n");
                    }
                }else
                {
                    printf("Estado nao encontrado.\n");
                }

                break;
            }
            case 4:{
                ESTADOS *estado;
                char nome_estado[100];

                printf("Digite o nome do estado de onde o CEP sera removido: ");
                ler_string_simples(nome_estado, sizeof(nome_estado));
                estado = existe_estado(cabeca_estado, nome_estado);
                if (estado){

                    CIDADES *cidade;
                    char nome_cidade[100];
                    printf("Digite o nome da cidade de onde o CEP sera removido: ");
                    ler_string_simples(nome_cidade, sizeof(nome_cidade));
                    cidade = buscar_cidade(estado->cidade, nome_cidade);
                    if (cidade)
                    {
                        char str_cep[10];
                        printf("Digite o CEP a ser removido: ");
                        ler_string_simples(str_cep, sizeof(str_cep));
                        retorno = remover_CEP_arvore(&cidade->cep, str_cep);
                        if (retorno)
                        {
                            printf("CEP removido com sucesso.\n");
                        }
                        else
                        {
                            printf("Erro ao remover o CEP.\n");
                        }
                    }
                    else
                    {
                        printf("Cidade nao encontrada.\n");
                    }
                }else
                {
                    printf("Estado nao encontrado.\n");
                }
                break;
            }
            case 5:
                break;
            case 6:
                break;
            case 7:{
                ESTADOS *estado;
                estado = verifica_estado_mais_populoso(cabeca_estado);

                if (estado)
                {
                    printf("Estado mais populoso: %s\n", estado->nome_estado);
                    printf("Populacao: %d\n", estado->populacao_estado);
                }else
                    printf("Nenhum estado cadastrado.\n");
                break;
            } 
            case 8:{
                ESTADOS *estado;
                estado = NULL;
                char nome_estado[100];
                
                ler_string_simples(nome_estado, sizeof(nome_estado));

                estado = existe_estado(cabeca_estado, nome_estado);
                if(estado){
                    CIDADES *capital;
                    capital = NULL;

                    capital = buscar_cidade(estado->cidade, estado->nome_capital);
                    if(capital)
                        printf("A populacao da capital do estado %s eh: %d\n", estado->nome_estado, capital->populacao_city);
                    else
                        printf("ERRO: Capital nao encontrada\n");
                }
                else
                    printf("Estado nao encontrado.\n");
                break;
            }
            case 9:{
                char nome_estado[100];
                ler_string_simples(nome_estado, sizeof(nome_estado));
                ESTADOS *estado;

                estado = existe_estado(estado, nome_estado);
                if(estado){
                    CIDADES *cidade_mais_populosa;
                    cidade_mais_populosa = verifica_cidade_mais_populosa_nao_capital(estado->cidade, estado->nome_capital);

                    if(cidade_mais_populosa)
                        printf("A cidade mais populosa do estado %s sem ser a capital eh: %s\n", estado->nome_estado, cidade_mais_populosa->nome_cidade);
                    else
                        printf("Nenhuma cidade encontrada.\n");
                }
                break;
            }
            case 10:
                break;
            case 11:
                break;
            case 12:
                break;
            case 13:
                break;
            case 0:
                printf("Saindo do programa...\n");
                break;
            default:
                printf("Opção inválida! Tente novamente.\n");
                break;
            }
        }
    } while (opcao1 != 0);
}