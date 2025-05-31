#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../Arv_23_H/Estados_23.h"
#include "../Arv_23_H/Cidades_23.h"
#include "../Arv_23_H/CEPs_23.h"
#include "../Arv_23_H/Pessoas_23.h"

#include "../Arv_23_H/STRUCTS_23.h"
#include "../Arv_23_H/utilitarios_23.h"

//gcc -o a Arv_23_C/Interface_23.c Arv_23_C/Estados_23.c Arv_23_C/Cidades_23.c Arv_23_C/CEPs_23.c Arv_23_C/Pessoas_23.c Arv_23_C/utilitarios_23.c -IArv_23_H

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

void menu_geral()
{

    int opcao1, opcao2, sucesso, cep_usado;

    ESTADOS *cabeca_estados = NULL;
    Arv23_PESSOAS *raiz_pessoas = NULL;

    do
    {
        menu_print();
        printf("Digite a opcao desejada: ");
        opcao1 = digitar_int();

        switch (opcao1)
        {
        case 1:
        {

            ESTADOS *novo_estado = cadastro_estado();
            CIDADES cidade_capital;
            CEP cep_capital;

            if (novo_estado == NULL)
            {
                printf("Erro ao cadastrar estado!\n");
            }
            else
            {
                if (inserir_estado_rec(&cabeca_estados, novo_estado))
                { // Inserindo o estado na lista

                    printf("\n=== Cadastre a capital do estado %s ===\n", novo_estado->nome_estado);

                    cidade_capital = cadastra_cidade(&sucesso); // aqui pego só as informações da cidade
                    if (sucesso)
                    {
                        do {// verifica se o CEP já existe se já existir, pede para o usuário digitar novamente

                            sucesso = capturar_cep(cep_capital.cep); // Capturando o CEP da capital
                            cep_usado = percorre_estados_procurando_CEP(cabeca_estados, cep_capital.cep);

                            if (cep_usado){
                                printf("CEP %s ja se encontra em uso!\n", cep_capital.cep);
                                printf("Reiniciando o cadastro do CEP...\n");
                                sucesso = 0; // Reinicia o sucesso para forçar a nova captura do CEP
                            }
                        }while (cep_usado);

                        if (sucesso)
                        {
                            if (insere_23_CEP(&(cidade_capital.arv_cep), cep_capital))
                            {
                                printf("CEP %s cadastrado com sucesso!\n", cep_capital.cep);
                                novo_estado->populacao_estado += cidade_capital.populacao_city;
                                novo_estado->quant_city++;
                                strcpy(novo_estado->nome_capital, cidade_capital.nome_cidade);
                            }
                            else
                            {
                                printf("Erro ao cadastrar CEP!\n");
                                sucesso = 0;
                            }
                        }

                        if (sucesso)
                        {
                            if (insere_23_cidade(&(novo_estado->arv_cidades), cidade_capital))
                            { // tenho que inserir somente apos o CEP por que é uma varivel comum e não mais um ponteiro
                                printf("Cidade %s cadastrada com sucesso!\n", cidade_capital.nome_cidade);
                            }
                            else
                            {
                                printf("Erro ao inserir cidade!\n");
                                sucesso = 0;
                            }
                        }
                    }
                }
            }

            if (novo_estado && sucesso)
            {
                printf("Estado %s cadastrado com sucesso!\n", novo_estado->nome_estado);
                novo_estado->quant_city++;
                novo_estado->populacao_estado += cidade_capital.populacao_city;
                strcpy(novo_estado->nome_capital, cidade_capital.nome_cidade);
            }
            else
            {
                free(novo_estado->nome_estado);
                free(novo_estado);
                printf("Erro ao cadastrar estado!\n");
            }
            break;
        }
        case 2:
        {

            char nome_estado[100];
            printf("Digite o nome do estado para cadastrar a cidade: ");
            ler_string_simples(nome_estado, sizeof(nome_estado));
            ESTADOS *estado = existe_estado(cabeca_estados, nome_estado);

            if (estado)
            {
                do
                {

                    CIDADES novaCidade = cadastra_cidade(&sucesso);
                    if (sucesso)
                    {
                        // Cadastro de CEPs
                        do
                        {
                            CEP novoCEP;
                            do {
                                sucesso = capturar_cep(novoCEP.cep);
                                cep_usado = percorre_estados_procurando_CEP(cabeca_estados, novoCEP.cep);
                                if (cep_usado)
                                {
                                    printf("CEP %s ja se encontra em uso!\n", novoCEP.cep);
                                    printf("Reiniciando o cadastro do CEP...\n");
                                    sucesso = 0; // Reinicia o sucesso para forçar a nova captura do CEP
                                }
                            }while (!sucesso);

                            if (sucesso)
                            {
                                sucesso = insere_23_CEP(&(novaCidade.arv_cep), novoCEP);
                                if (sucesso)
                                {
                                    printf("CEP %s cadastrado com sucesso!\n", novoCEP.cep);
                                }
                                else
                                {
                                    printf("Erro ao cadastrar CEP!\n");
                                }
                            }

                            if (sucesso){

                                printf("Deseja cadastrar outro CEP para %s? [1] - SIM [0] - NAO: ", novaCidade.nome_cidade);
                                opcao2 = digitar_int();
                            }else{
                                printf("Erro ao cadastrar CEP!\n");
                                printf("Reinaiciando o cadastro do CEP...\n");
                                opcao2 = 1; // Força a repetição do cadastro de CEP
                            }
                        } while (opcao2 == 1);

                        if (insere_23_cidade(&(estado->arv_cidades), novaCidade))
                        {
                            estado->quant_city++;
                            estado->populacao_estado += novaCidade.populacao_city;
                            printf("Cidade %s cadastrada com sucesso!\n", novaCidade.nome_cidade);
                        }
                        else
                        {
                            printf("Erro ao inserir cidade!\n");
                        }
                    }
                    else
                    {
                        printf("Erro ao cadastrar cidade!\n");
                    }
                    printf("Deseja cadastrar outra cidade para o estado %s? [1] - SIM [0] - NAO: ", nome_estado);
                    opcao2 = digitar_int();
                } while (opcao2 == 1);
            }
            else
            {
                printf("Estado %s nao encontrado!\n", nome_estado);
            }

            break;
        }
        case 3:
        {
            char nome_estado[100];
            char nome_cidade[100];
            CEP Cep;

            printf("Digite o nome do estado para cadastrar o CEP: ");
            ler_string_simples(nome_estado, sizeof(nome_estado));

            ESTADOS *estado = existe_estado(cabeca_estados, nome_estado);
            if (estado)
            {
                printf("Digite o nome da cidade para cadastrar o CEP: ");
                ler_string_simples(nome_cidade, sizeof(nome_cidade));

                CIDADES *cidade = buscar_info_cidade(estado->arv_cidades, nome_cidade);
                if (cidade)
                {
                    do
                    {
                        sucesso = capturar_cep(Cep.cep);
                        cep_usado = percorre_estados_procurando_CEP(cabeca_estados, Cep.cep);

                        if (cep_usado)
                        {
                            printf("CEP %s ja se encontra em uso!\n", Cep.cep);
                            printf("Reiniciando o cadastro do CEP...\n");
                            sucesso = 0; // Reinicia o sucesso para forçar a nova captura do CEP
                        }
                    } while (!sucesso);

                    if (sucesso)
                    {
                        if (insere_23_CEP(&(cidade->arv_cep), Cep))
                        {
                            printf("CEP %s cadastrado com sucesso na cidade %s!\n", Cep.cep, cidade->nome_cidade);
                        }
                        else
                        {
                            printf("Erro ao cadastrar CEP!\n");
                        }
                    }
                }
                else
                {
                    printf("Cidade %s nao encontrada no estado %s!\n", nome_cidade, nome_estado);
                }
            }
            else
            {
                printf("Estado %s nao encontrado!\n", nome_estado);
            }

            break;
        }
        case 4:
        {
            ESTADOS *estado = NULL;
            char nome_estado[100];
            char nome_cidade[100];
            char cep_natal[10];
            char cep_atual[10];
            PESSOAS nova_pessoa;

            do
            {

                printf("Digite o nome do estado para qual a pessoa ira: ");
                ler_string_simples(nome_estado, sizeof(nome_estado));

                estado = existe_estado(cabeca_estados, nome_estado);
                if (estado)
                {

                    printf("Digite o nome da cidade para cadastrar o CEP: ");
                    ler_string_simples(nome_cidade, sizeof(nome_cidade));

                    CIDADES *cidade = buscar_info_cidade(estado->arv_cidades, nome_cidade);
                    if (cidade)
                    {
                        printf("Digite o CEP da cidade %s: ", nome_cidade);
                        sucesso = capturar_cep(cep_natal);
                        if (sucesso)
                        {
                            cep_usado = percorre_estados_procurando_CEP(cabeca_estados, cep_natal);

                            do
                            {

                                if (!cep_usado)
                                {
                                    printf("CEP %s nao encontrado em nenhum estado!\n", cep_natal);
                                }
                                else
                                {

                                    do
                                    {

                                        printf("Digite o CEP atual da pessoa: ");
                                        sucesso = capturar_cep(cep_atual);
                                        if (sucesso)
                                        {
                                            cep_usado = percorre_estados_procurando_CEP(cabeca_estados, cep_atual);
                                            if (!cep_usado)
                                            {
                                                printf("CEP %s nao encontrado em nenhum estado!\n", cep_atual);
                                            }
                                            else
                                            {
                                                nova_pessoa = cadastra_pessoa(cep_natal, cep_atual);
                                                if (nova_pessoa.CPF[0] != '\0')
                                                {
                                                    if (insere_23_PESSOAS(&raiz_pessoas, nova_pessoa))
                                                    {
                                                        printf("Pessoa [%s] cadastrada com sucesso!\n", nova_pessoa.nome_pessoa);
                                                    }
                                                    else
                                                    {
                                                        printf("Erro ao cadastrar pessoa!\n");
                                                    }
                                                }
                                                else
                                                {
                                                    printf("Dados invalidos para cadastro de pessoa!\n");
                                                }
                                            }
                                        }

                                        if (!sucesso)
                                        {
                                            printf("Deseja tentar novamente? [1] - SIM [0] - NAO: ");
                                            opcao2 = digitar_int();
                                        }
                                    } while (!sucesso && opcao2 == 1);
                                }
                                if (!sucesso)
                                {
                                    printf("Deseja tentar novamente? [1] - SIM [0] - NAO: ");
                                    opcao2 = digitar_int();
                                }
                            } while (!sucesso && opcao2 == 1);
                        }
                    }
                    else
                    {
                        printf("Cidade %s nao encontrada no estado %s!\n", nome_cidade, nome_estado);
                    }
                }
                else
                {
                    printf("Estado %s nao encontrado!\n", nome_estado);
                }

                if (sucesso)
                {
                    printf("Deseja cadastrar outra pessoa? [1] - SIM [0] - NAO: ");
                    opcao2 = digitar_int();
                }
                else
                {
                    printf("A operacao falhou!\n");
                    printf("Deseja tentar novamente? [1] - SIM [0] - NAO: ");
                    opcao2 = digitar_int();
                }
            } while (opcao2 == 1);
            break;
        }
        case 14:{
            if (mostrar_todos_estados(cabeca_estados))
            {
                printf("Todos os estados foram impressos com sucesso!\n");
            }
            else
            {
                printf("Nenhum estado cadastrado!\n");
            }
            break;
        }
        case 15:{
            char nome_estado[100];
            printf("Digite o nome do estado: ");
            ler_string_simples(nome_estado, sizeof(nome_estado));
            ESTADOS *estado = existe_estado(cabeca_estados, nome_estado);
            if (estado)
            {
                imprime_23_em_ordem_cidade(estado->arv_cidades);
                imprime_arvore_visual_cidade(estado->arv_cidades, "", 1, 1);
            }
            else
            {
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

int main()
{
    menu_geral();
    return 0;
}