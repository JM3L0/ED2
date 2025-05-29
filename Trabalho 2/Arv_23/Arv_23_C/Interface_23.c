#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../Arv_23_H/Estados_23.h"
#include "../Arv_23_H/Cidades_23.h"
#include "../Arv_23_H/CEPs_23.h"
#include "../Arv_23_H/Pessoas_23.h"

#include "../Arv_23_H/STRUCTS_23.h"
#include "../Arv_23_H/utilitarios_23.h"

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
    printf("[14] - Imprimir todos os estados cadastrados\n");
    printf("[15] - Imprimir todas as cidades de um estado\n");
    printf("[16] - Imprimir todos os CEPs de uma cidade\n");
    printf("[17] - Imprimir todas as pessoas cadastradas\n");
    printf("[0] - Sair\n");
}

void menu_geral(){

    int opcao1, opcao2, sucesso;

    ESTADOS *cabeca_estados = NULL;
    PESSOAS *raiz_pessoas = NULL;

    do
    {
        menu_print();
        printf("Digite a opcao desejada: ");
        opcao1 = digitar_int();

        switch (opcao1)
        {
            case 1:{

                ESTADOS *novo_estado = cadastro_estado();
                CIDADES cidade_capital;
                CEP cep_capital;

                if (novo_estado == NULL){
                    printf("Erro ao cadastrar estado!\n");
                }else{
                    if (inserir_estado_rec(&cabeca_estados, novo_estado)){// Inserindo o estado na lista
                        
                        cidade_capital = cadastra_cidade(&sucesso);// aqui pego só as informações da cidade
                        if (sucesso){
                            
                            sucesso = capturar_cep(cep_capital.cep);// Capturando o CEP da capital
                            if (sucesso){
                                if (insere_23_cep(&(cidade_capital.arv_cep), cep_capital)){
                                    printf("CEP %s cadastrado com sucesso!\n", cep_capital.cep);
                                    novo_estado->populacao_estado += cidade_capital.populacao_city;
                                    novo_estado->quant_city ++;
                                    strcpy(novo_estado->nome_capital, cidade_capital.nome_cidade);
                                }else{
                                    printf("Erro ao cadastrar CEP!\n");
                                    sucesso = 0;
                                }
                            }

                            if (sucesso){
                                if (insere_23_cidade(&(novo_estado->arv_cidades), cidade_capital)){// tenho que inserir somente apos o CEP por que é uma varivel comum e não mais um ponteiro
                                    printf("Cidade %s cadastrada com sucesso!\n", cidade_capital.nome_cidade);
                                }else{
                                    printf("Erro ao inserir cidade!\n");
                                    sucesso = 0;
                                }
                            }
                            
                        }
                    }
                }
                    
                if (novo_estado && sucesso){
                    printf("Estado %s cadastrado com sucesso!\n", novo_estado->nome_estado);
                    novo_estado->quant_city ++;
                    novo_estado->populacao_estado += cidade_capital.populacao_city;
                    strcpy(novo_estado->nome_capital, cidade_capital.nome_cidade);
                }else{
                    free(novo_estado->nome_estado);
                    free(novo_estado);
                    printf("Erro ao cadastrar estado!\n");
                }
                break;
            }
            case 2:{

                char nome_estado[100];
                printf("Digite o nome do estado para cadastrar a cidade: ");
                ler_string_simples(nome_estado, sizeof(nome_estado));
                ESTADOS *estado = existe_estado(cabeca_estados, nome_estado);

                if (estado){
                    do {

                        CIDADES novaCidade = cadastra_cidade(&sucesso);
                        if (sucesso){
                            // Cadastro de CEPs
                            do {
                                CEP novoCEP;
                                sucesso = capturar_cep(novoCEP.cep);
                                if (sucesso){
                                    if (insere_23_cep(&(novaCidade.arv_cep), novoCEP)){
                                        printf("CEP %s cadastrado com sucesso!\n", novoCEP.cep);
                                    } else {
                                        printf("Erro ao cadastrar CEP!\n");
                                    }
                                }
                                printf("Deseja cadastrar outro CEP para %s? [1] - SIM [0] - NAO: ", novaCidade.nome_cidade);
                                opcao2 = digitar_int();
                            } while (opcao2 == 1 || !sucesso);

                            if (insere_23_cidade(&(estado->arv_cidades), novaCidade)){
                                estado->quant_city++;
                                estado->populacao_estado += novaCidade.populacao_city;
                                printf("Cidade %s cadastrada com sucesso!\n", novaCidade.nome_cidade);
                                
                            } else {
                                printf("Erro ao inserir cidade!\n");
                            }
                        } else {
                            printf("Erro ao cadastrar cidade!\n");
                        }
                        printf("Deseja cadastrar outra cidade para o estado %s? [1] - SIM [0] - NAO: ", nome_estado);
                        opcao2 = digitar_int();
                    }while (opcao2 == 1);
                } else {
                    printf("Estado %s nao encontrado!\n", nome_estado);
                }
                
                break;
            }
            case 0:
                printf("Saindo...\n");
                break;
            default:
                printf("Opcao invalida! Tente novamente.\n");
        }
    } while (opcao1 != 0);
}