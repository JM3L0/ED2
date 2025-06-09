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

void menu_geral()
{
    
    ESTADOS *cabeca_estados;
    cabeca_estados = NULL;
    Arv23_PESSOAS *raiz_pessoas;
    raiz_pessoas = NULL;
    int opcao1, opcao2, sucesso, cep_usado, retorno;

    do
    {
        menu_print();
        printf("Digite a opcao desejada: ");
        opcao1 = digitar_int();

        switch (opcao1)
        {
        case 1:
        {

            ESTADOS *novo_estado;
            novo_estado = cadastro_estado();
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
                        do
                        { // verifica se o CEP já existe se já existir, pede para o usuário digitar novamente

                            sucesso = capturar_cep(cep_capital.cep); // Capturando o CEP da capital
                            cep_usado = percorre_estados_procurando_CEP(cabeca_estados, cep_capital.cep);

                            if (cep_usado)
                            {
                                printf("CEP %s ja se encontra em uso!\n", cep_capital.cep);
                                printf("Reiniciando o cadastro do CEP...\n");
                                sucesso = 0; // Reinicia o sucesso para forçar a nova captura do CEP
                            }
                        } while (cep_usado);

                        if (sucesso)
                        {
                            if (insere_23_CEP(&(cidade_capital.arv_cep), cep_capital))                            
                                printf("CEP %s cadastrado com sucesso!\n", cep_capital.cep);                            
                            else
                            {
                                printf("Erro ao cadastrar CEP!\n");
                                sucesso = 0;
                            }
                        }

                        if (sucesso)
                        {
                            if (insere_23_cidade(&(novo_estado->arv_cidades), cidade_capital))
                             // tenho que inserir somente apos o CEP por que é uma varivel comum e não mais um ponteiro
                                printf("Cidade %s cadastrada com sucesso!\n", cidade_capital.nome_cidade);
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
            pausar();
            break;
        }
        case 2:
        {
            opcao2 = 0;
            char nome_estado[100];
            printf("Digite o nome do estado para cadastrar a cidade: ");
            ler_string_simples(nome_estado, sizeof(nome_estado));
            ESTADOS *estado;
            estado = existe_estado(cabeca_estados, nome_estado);

            if (estado)
            {
                do
                {

                    CIDADES novaCidade;
                    novaCidade = cadastra_cidade(&sucesso);
                    if (sucesso)
                    {
                        // Cadastro de CEPs
                        do
                        {
                            CEP novoCEP;
                            do
                            {
                                sucesso = capturar_cep(novoCEP.cep);
                                cep_usado = percorre_estados_procurando_CEP(cabeca_estados, novoCEP.cep);
                                if (cep_usado)
                                {
                                    printf("CEP %s ja se encontra em uso!\n", novoCEP.cep);
                                    printf("Reiniciando o cadastro do CEP...\n");
                                    sucesso = 0; // Reinicia o sucesso para forçar a nova captura do CEP
                                }
                            } while (!sucesso);

                            if (sucesso)
                            {
                                sucesso = insere_23_CEP(&(novaCidade.arv_cep), novoCEP);
                                if (sucesso)                                
                                    printf("CEP %s cadastrado com sucesso!\n", novoCEP.cep);
                                else
                                    printf("Erro ao cadastrar CEP!\n");
                            }

                            if (sucesso)
                            {

                                printf("Deseja cadastrar outro CEP para %s? [1] - SIM [0] - NAO: ", novaCidade.nome_cidade);
                                opcao2 = digitar_int();
                            }
                            else
                            {
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
                            printf("Erro ao inserir cidade!\n");
                    }
                    else
                        printf("Erro ao cadastrar cidade!\n");
                    printf("Deseja cadastrar outra cidade para o estado %s? [1] - SIM [0] - NAO: ", nome_estado);
                    opcao2 = digitar_int();
                } while (opcao2 == 1);
            }
            else
                printf("Estado %s nao encontrado!\n", nome_estado);
            pausar();
            break;
        }
        case 3:
        {
            char nome_estado[100];
            char nome_cidade[100];
            CEP Cep;
            opcao2 = 0;

            printf("Digite o nome do estado para cadastrar o CEP: ");
            ler_string_simples(nome_estado, sizeof(nome_estado));

            ESTADOS *estado;
            estado = existe_estado(cabeca_estados, nome_estado);
            if (estado)
            {
                printf("Digite o nome da cidade para cadastrar o CEP: ");
                ler_string_simples(nome_cidade, sizeof(nome_cidade));

                CIDADES *cidade;
                cidade = buscar_info_cidade(estado->arv_cidades, nome_cidade);
                if (cidade)
                {
                    do
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
                                printf("CEP %s cadastrado com sucesso na cidade %s!\n", Cep.cep, cidade->nome_cidade);
                            else
                                printf("Erro ao cadastrar CEP!\n");
                        }

                        printf("Deseja cadastrar outro CEP para a cidade %s? [1] - SIM [0] - NAO: ", cidade->nome_cidade);
                        opcao2 = digitar_int();

                    } while (opcao2 == 1);
                }
                else
                    printf("Cidade %s nao encontrada no estado %s!\n", nome_cidade, nome_estado);
            }
            else
                printf("Estado %s nao encontrado!\n", nome_estado);
            pausar();
            break;
        }
        case 4:
        {
            PESSOAS novaPessoa;
            char cep_natal[10], cep_atual[10];
            opcao2 = 0;

            do
            {

                printf("Digite o CEP da natal da pessoa: ");
                sucesso = capturar_cep(cep_natal);
                if (sucesso)
                {
                    if (percorre_estados_procurando_CEP(cabeca_estados, cep_natal))
                    {
                        printf("Digite o CEP atual da pessoa: ");
                        sucesso = capturar_cep(cep_atual);

                        if (percorre_estados_procurando_CEP(cabeca_estados, cep_natal))
                        {

                            novaPessoa = cadastra_pessoa(cep_natal, cep_atual, &sucesso);
                            if (sucesso)
                            {
                                if (insere_23_PESSOAS(&raiz_pessoas, novaPessoa))
                                    printf("Pessoa %s cadastrada com sucesso!\n", novaPessoa.nome_pessoa);
                                else
                                    printf("Erro ao cadastrar pessoa!\n");
                            }
                        }
                        else
                            printf("CEP %s nao encontrado em nenhum estado!\n", cep_atual);
                    }
                    else
                        printf("CEP %s nao encontrado em nenhum estado!\n", cep_natal);
                }
                printf("Deseja cadastrar outra pessoa? [1] - SIM [0] - NAO: ");
                opcao2 = digitar_int();
            } while (opcao2 == 1);
            pausar();
            break;
        }
        case 5:
        {
            char estado_nome[100];
            char cidade_nome[100];
            CEP Cep;

            printf("Digite o nome do estado: ");
            ler_string_simples(estado_nome, sizeof(estado_nome));
            ESTADOS *estado;
            estado = existe_estado(cabeca_estados, estado_nome);

            if (estado)
            {
                printf("Digite o nome da cidade: ");
                ler_string_simples(cidade_nome, sizeof(cidade_nome));
                CIDADES *cidade;
                cidade = buscar_info_cidade(estado->arv_cidades, cidade_nome);
                if (cidade)
                {
                    printf("=== Remover CEP (da cidade %s) ===\n", cidade->nome_cidade);
                    sucesso = capturar_cep(Cep.cep);

                    if (!verificar_se_existe_pessoa_associada_a_um_CEP(raiz_pessoas, Cep.cep))
                    {
                        StatusRemocao status = remover_23_CEP(&(cidade->arv_cep), Cep);
                        mensagens_do_remover(status);
                    }
                    else
                        printf("Nao e possivel remover o CEP %s, pois ha pessoas associadas a ele!\n", Cep.cep);
                }
                else
                    printf("Cidade %s nao encontrada no estado %s!\n", cidade_nome, estado_nome);
            }
            else
                printf("Estado %s nao encontrado!\n", estado_nome);
            pausar();
            break;
        }
        case 6:
        {
            PESSOAS pessoa_remover;
            printf("=== Remover Pessoa (pelo CPF) ===\n");
            sucesso = capturar_cpf(pessoa_remover.CPF);

            StatusRemocao status = remover_23_PESSOAS(&raiz_pessoas, pessoa_remover);
            mensagens_do_remover(status);
            pausar();
            break;
        }
        case 7:
        {
            ESTADOS *estado_populoso = verifica_estado_mais_populoso(cabeca_estados);
            if (estado_populoso)
                printf("Estado mais populoso: %s com populacao de %d\n", estado_populoso->nome_estado, estado_populoso->populacao_estado);
            else
                printf("Nenhum estado cadastrado!\n");
            pausar();
            break;
        }
        case 8:
        {
            char nome_estado[100];
            printf("Digite o nome do estado: ");
            ler_string_simples(nome_estado, sizeof(nome_estado));
            ESTADOS *estado;
            estado = existe_estado(cabeca_estados, nome_estado);
            if (estado)
            {
                CIDADES *capital = buscar_info_cidade(estado->arv_cidades, estado->nome_capital);
                if (capital)
                    printf("Populacao da capital do Estado [%s]: Capital [%s]: Populacao %d\n", estado->nome_estado, capital->nome_cidade, capital->populacao_city);
                else
                    printf("Capital %s nao encontrada no estado %s!\n", estado->nome_capital, nome_estado);
            }
            else
                printf("Estado %s nao encontrado!\n", nome_estado);
            pausar();
            break;
        }
        case 9:
        {
            char nome_estado[100];
            printf("Digite o nome do estado: ");
            ler_string_simples(nome_estado, sizeof(nome_estado));
            ESTADOS *estado;
            estado = existe_estado(cabeca_estados, nome_estado);
            if (estado)
            {
                CIDADES *cidade_mais_populosa;
                cidade_mais_populosa = verifica_cidade_mais_populosa_nao_capital_23(estado->arv_cidades, estado->nome_capital);
                if (cidade_mais_populosa)
                {
                    printf("Cidade mais populosa (exceto a capital) no Estado [%s]: Cidade [%s] com populacao de %d\n",
                           estado->nome_estado, cidade_mais_populosa->nome_cidade, cidade_mais_populosa->populacao_city);
                }
                else
                    printf("Nenhuma cidade encontrada no estado %s que nao seja a capital!\n", nome_estado);
            }
            else
                printf("Estado %s nao encontrado!\n", nome_estado);
            pausar();
            break;
        }
        case 10:
        {
            printf("=== Imprimir quantas pessoas nao moram na cidade natal ===\n");
            retorno = quantas_pessoas_nao_moram_na_cidade_natal_ESTADO(cabeca_estados, raiz_pessoas);
            if (retorno > 0)
                printf("Total de pessoas que nao moram na cidade natal: %d\n", retorno);
            pausar();
            break;
        }
        case 11:
        {
            char cpf_pessoa[12];
            printf("Digite o CPF da pessoa: ");
            capturar_cpf(cpf_pessoa);

            PESSOAS *pessoa;
            pessoa = buscar_info_pessoa(raiz_pessoas, cpf_pessoa);
            if (pessoa){
                CIDADES *cidade_natal;
                cidade_natal = cidade_dado_cep(cabeca_estados->arv_cidades, pessoa->cep_city_natal);
                ESTADOS *aux;
                aux = cabeca_estados;
                do
                {
                    if (aux != NULL){
                        cidade_natal = cidade_dado_cep(aux->arv_cidades, pessoa->cep_city_natal);
                        aux = aux->prox;
                    }
                } while (aux != NULL && cidade_natal == NULL);

                if (cidade_natal)
                    printf("Cidade natal da pessoa %s: %s\n", pessoa->nome_pessoa, cidade_natal->nome_cidade);
                else
                    printf("Cidade natal nao encontrada para o CEP %s!\n", pessoa->cep_city_natal);
            }
            pausar();
            break;
        }
        case 12:
        {
            char nome_estado[100];
            char nome_cidade[100];
            printf("Digite o nome do estado: ");
            ler_string_simples(nome_estado, sizeof(nome_estado));
            ESTADOS *estado;
            estado = existe_estado(cabeca_estados, nome_estado);
            if (estado)
            {
                printf("Digite o nome da cidade: ");
                ler_string_simples(nome_cidade, sizeof(nome_cidade));
                CIDADES *cidade;
                cidade = buscar_info_cidade(estado->arv_cidades, nome_cidade);
                if (cidade)
                {
                    retorno = quantas_pessoas_nascidas_na_cidade_nao_moram_nela(cidade, raiz_pessoas);
                    printf("Total de pessoas que nasceram na cidade %s e nao moram na cidade natal: %d\n", cidade->nome_cidade, retorno);
                }
                else
                    printf("Cidade %s nao encontrada no estado %s!\n", nome_cidade, nome_estado);
            }
            else
                printf("Estado %s nao encontrado!\n", nome_estado);
            pausar();
            break;
        }
        case 13:
        {
            ESTADOS *estado;
            char nome_estado[100];
            char nome_cidade[100];
            printf("Digite o nome do estado: ");

            ler_string_simples(nome_estado, sizeof(nome_estado));
            estado = existe_estado(cabeca_estados, nome_estado);

            if (estado)
            {
                printf("Digite o nome da cidade: ");
                ler_string_simples(nome_cidade, sizeof(nome_cidade));
                CIDADES *cidade;
                cidade = buscar_info_cidade(estado->arv_cidades, nome_cidade);
                if (cidade)
                {
                    retorno = quantas_pessoas_moram_na_cidade_nao_nasceram_nela(cidade, raiz_pessoas);
                    printf("Total de pessoas que moram na cidade %s e nao nasceram nela: %d\n", cidade->nome_cidade, retorno);
                }
                else
                    printf("Cidade %s nao encontrada no estado %s!\n", nome_cidade, nome_estado);
            }
            else
                printf("Estado %s nao encontrado!\n", nome_estado);
            pausar();
            break;
        }
        case 14:
        {
            if (mostrar_todos_estados(cabeca_estados))
                printf("Todos os estados foram impressos com sucesso!\n");
            else
                printf("Nenhum estado cadastrado!\n");
            pausar();
            break;
        }
        case 15:
        {
            char nome_estado[100];
            printf("Digite o nome do estado: ");
            ler_string_simples(nome_estado, sizeof(nome_estado));
            ESTADOS *estado;
            estado = existe_estado(cabeca_estados, nome_estado);
            if (estado)
                imprime_23_em_ordem_cidade(estado->arv_cidades);
            else
                printf("Estado %s nao encontrado!\n", nome_estado);
            pausar();
            break;
        }
        case 16:
        {
            char nome_estado[100];
            char nome_cidade[100];
            printf("Digite o nome do estado: ");
            ler_string_simples(nome_estado, sizeof(nome_estado));
            ESTADOS *estado;
            estado = existe_estado(cabeca_estados, nome_estado);
            if (estado)
            {
                printf("Digite o nome da cidade: ");
                ler_string_simples(nome_cidade, sizeof(nome_cidade));
                CIDADES *cidade;
                cidade = buscar_info_cidade(estado->arv_cidades, nome_cidade);
                if (cidade)
                    imprime_23_em_ordem_CEP(cidade->arv_cep);
                else
                    printf("Cidade %s nao encontrada no estado %s!\n", nome_cidade, nome_estado);
            }
            else
                printf("Estado %s nao encontrado!\n", nome_estado);
            pausar();
            break;
        }
        case 17:
        {
            if (raiz_pessoas != NULL)
                imprime_23_em_ordem_PESSOAS(raiz_pessoas);
            else
                printf("Nenhuma pessoa cadastrada!\n");
            pausar();
            break;
        }
        case 0:{

            printf("Saindo...\n");
            // Libera a memória alocada para os estados e pessoas
            if (cabeca_estados) liberar_todos_estados(&cabeca_estados);
              
            if (raiz_pessoas) libera_arvore_PESSOAS(&raiz_pessoas);
            printf("Memoria liberada com sucesso!\n");
            break;
        }
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
