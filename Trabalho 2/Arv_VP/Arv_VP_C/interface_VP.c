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
    printf("[14] - Imprimir todos os estados cadastrados\n");
    printf("[15] - Imprimir todas as cidades de um estado\n");
    printf("[16] - Imprimir todos os CEPs de uma cidade\n");
    printf("[17] - Imprimir todas as pessoas cadastradas\n");
    printf("[0] - Sair\n");
}

void menu_geral()
{
    ESTADOS *cabeca_estado;
    cabeca_estado = NULL;

    PESSOAS *raiz_pessoa;
    raiz_pessoa = NULL;

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
            case 1:
            {
                printf("\n== Cadastro de Estado ==\n");
                ESTADOS *novo_estado = cadastro_estado();
                CIDADES *capital = NULL;

                if (!novo_estado)
                {
                    printf("\nERRO: Erro ao criar estado.\n");
                }
                else if (!inserir_estado_rec(&cabeca_estado, novo_estado))
                {
                    printf("\nERRO: Erro ao cadastrar o estado.\n");
                }
                else
                {
                    printf("\n== Cadastro de Capital ==\n");
                    capital = cadastrarCidade();
                    if (!capital)
                    {
                        printf("\nERRO: Erro ao cadastrar a capital.\n");
                    }
                    else if (!inserir_Cidade(&novo_estado->cidade, capital))
                    {
                        printf("\nERRO: Erro ao inserir a capital no estado.\n");
                    }
                    else
                    {
                        // Cadastro dos CEPs
                        do
                        {
                            CEP *novoCEP = cadastrarCEP();
                            if (novoCEP)
                            {
                                if (percorre_estados_procurando_CEP(cabeca_estado, novoCEP->cep))
                                {
                                    printf("\nErro: CEP ja existe no sistema.\n");
                                }
                                else if (!inserir_CEP(&capital->cep, novoCEP))
                                {
                                    printf("\nERRO: Erro ao inserir o CEP.\n");
                                }
                                else
                                {
                                    printf("\nCEP cadastrado com sucesso.\n");
                                    novoCEP = NULL; // evita desalocar se tiver sido inserido
                                }
                                if (novoCEP)
                                    libera_no_CEP(&novoCEP);
                            }
                            else
                            {
                                printf("\nERRO: Erro ao cadastrar o CEP.\n");
                            }
                            printf("\nDeseja cadastrar outro CEP? [1] - SIM [0] - NAO: ");
                            opcao2 = digitar_int();
                        } while (opcao2 == 1);

                        // Atualiza informações do estado
                        novo_estado->quant_city = 1;
                        novo_estado->populacao_estado += capital->populacao_city;
                        novo_estado->nome_capital = capital->nome_cidade;
                    }
                }

                // Desalocações consolidadas para estado e capital
                if (capital && !novo_estado->cidade)
                    limpa_no_cidade(&capital);
                if (novo_estado && !novo_estado->quant_city)
                    desalocar_estado(&novo_estado);
                pausar();
                break;
            }
            case 2:
            {
                char nome_estado[100];
                CIDADES *novaCidade;
                ESTADOS *estado;
                novaCidade = NULL;
                estado = NULL;

                printf("\n== Cadastro de Cidade ==\n");
                printf("Digite o nome do estado para qual a cidade sera inserida: ");
                ler_string_simples(nome_estado, sizeof(nome_estado));

                estado = existe_estado(cabeca_estado, nome_estado);
                if (estado)
                {
                    do
                    {

                        novaCidade = cadastrarCidade();
                        if (novaCidade && inserir_Cidade(&estado->cidade, novaCidade))
                        {
                            // Cidade inserida com sucesso
                            estado->quant_city++;
                            estado->populacao_estado += novaCidade->populacao_city;
                            printf("\nCidade cadastrada com sucesso.\n\n");

                            // Cadastro de CEPs
                            do
                            {
                                do {

                                    CEP *novoCEP = cadastrarCEP();
                                    retorno = percorre_estados_procurando_CEP(cabeca_estado, novoCEP->cep);
    
                                    if (retorno)
                                    {
                                        printf("\nErro: CEP ja existe no sistema.\n");
                                        libera_no_CEP(&novoCEP);
                                    }
                                    else
                                    {
                                        if (novoCEP)
                                        {
                                            if (inserir_CEP(&(novaCidade->cep), novoCEP))
                                                printf("\nCEP cadastrado com sucesso.\n");
                                            else
                                            {
                                                printf("\nERRO: Erro ao inserir o CEP.\n");
                                                libera_no_CEP(&novoCEP);
                                            }
                                        }
                                        else
                                        {
                                            printf("\nERRO: Erro ao cadastrar o CEP.\n");
                                        }
                                    }
                                }while (novaCidade->cep == NULL);
                                
                                printf("\nDeseja cadastrar outro CEP para %s? [1] - SIM [0] - NAO: ", novaCidade->nome_cidade);
                                opcao2 = digitar_int();
                            } while (opcao2 == 1);
                        }
                        else
                        {
                            printf("\nERRO: Erro ao %s a cidade.\n", novaCidade ? "inserir" : "cadastrar");
                            if (novaCidade)
                                limpa_no_cidade(&novaCidade);
                        }
                        printf("\nDeseja cadastrar outra cidade para %s? [1] - SIM [0] - NAO: ", estado->nome_estado);
                        opcao2 = digitar_int();
                    } while (opcao2 == 1);
                }
                else
                {
                    printf("\nERRO: Estado nao encontrado.\n");
                }
                pausar();
                break;
            }
            case 3:
            {
                ESTADOS *estado;
                char nome_estado[100];
                printf("\n=== Cadastro de CEP ===\n");

                do {

                    printf("Digite o nome do estado para qual o CEP sera inserido: ");
                    ler_string_simples(nome_estado, sizeof(nome_estado));
                    estado = existe_estado(cabeca_estado, nome_estado);
                    if (estado)
                    {
                        CIDADES *cidade;
    
                        char nome_cidade[100];
                        printf("Digite o nome da cidade para qual o CEP sera inserido: ");
                        ler_string_simples(nome_cidade, sizeof(nome_cidade));
                        cidade = buscar_cidade(estado->cidade, nome_cidade);
                        if (cidade)
                        {
                            do{

                                CEP *novoCEP = cadastrarCEP();
                                retorno = percorre_estados_procurando_CEP(cabeca_estado, novoCEP->cep);

                                if (retorno){
                                    printf("\nERRO: CEP ja existe no sistema.\n");
                                    libera_no_CEP(&novoCEP);
                                }else{

                                    if (novoCEP)
                                    {
                                        retorno = inserir_CEP(&cidade->cep, novoCEP);
                                        if (retorno)
                                        {
                                            printf("\nCEP cadastrado com sucesso.\n");
                                        }
                                        else
                                        {
                                            printf("\nERRO: Erro ao inserir o CEP.\n");
                                        }
                                    }
                                    else
                                    {
                                        printf("\nERRO: Erro ao cadastrar o CEP.\n");
                                    }
                                    if (!retorno) libera_no_CEP(&novoCEP);
                                }
                                printf("\nDeseja cadastrar outro CEP para %s? [1] - SIM [0] - NAO: ", cidade->nome_cidade);
                                opcao2 = digitar_int();
                            }while (opcao2 == 1);
                        }
                        else
                        {
                            printf("\nERRO: Cidade nao encontrada.\n");
                        }
                    }
                    else
                    {
                        printf("\nERRO: Estado nao encontrado.\n");
                    }

                    printf("\nDeseja cadastrar outro CEP para %s? [1] - SIM [0] - NAO: ", nome_estado);
                    opcao2 = digitar_int();
                }while (opcao2 == 1);
                pausar();
                break;
            }
            case 4:
            {

                PESSOAS *novaPessoa = NULL;
                char cep_natal[10], cep_atual[10];
                printf("\n=== Cadastro de Pessoa ===\n");

                do
                {
                    printf("\n== Digite o CEP natal ==\n");
                    retorno = capturar_cep(cep_natal);
                    retorno = percorre_estados_procurando_CEP(cabeca_estado, cep_natal);

                    if (retorno)
                    {

                        printf("== Digite o CEP atual ==\n");
                        retorno = capturar_cep(cep_atual);
                        retorno = percorre_estados_procurando_CEP(cabeca_estado, cep_atual);

                        if (retorno)
                        {
                            novaPessoa = cadastra_pessoa(cep_natal, cep_atual);
                            if (novaPessoa)
                            {
                                
                                retorno = inserir_pessoa(&raiz_pessoa, novaPessoa);
                                
                                if (retorno)
                                {
                                    printf("\nPessoa cadastrada com sucesso.\n\n");
                                    imprimir_pessoa(novaPessoa);
                                }
                                else
                                {
                                    printf("\nERRO: Erro ao inserir a pessoa.\n");
                                    liberar_no_pessoa(&novaPessoa);
                                }
                                
                            }
                            else
                            {
                                printf("\nERRO: Erro ao cadastrar a pessoa.\n");
                            }
                        }
                        else
                        {
                            printf("\nERRO: CEP atual nao encontrado.\n");
                        }
                    }
                    else
                    {
                        printf("\nERRO: CEP natal nao encontrado.\n");
                    }

                    printf("\nDeseja cadastrar outra pessoa? [1] - SIM [0] - NAO: ");
                    opcao2 = digitar_int();
                } while (opcao2 == 1);
                pausar();
                break;
            }
            case 5:
            {
                char nome_estado[100], nome_cidade[100], str_cep[10];
                ESTADOS *estado = NULL;
                CIDADES *cidade = NULL;

                printf("\n=== Remocao de CEP ===\n");

                printf("Digite o nome do estado: ");
                ler_string_simples(nome_estado, sizeof(nome_estado));
                estado = existe_estado(cabeca_estado, nome_estado);

                if (estado)
                {
                    printf("Digite o nome da cidade: ");
                    ler_string_simples(nome_cidade, sizeof(nome_cidade));
                    cidade = buscar_cidade(estado->cidade, nome_cidade);

                    if (cidade)
                    {
                        printf("Digite o CEP a ser removido: ");
                        retorno = capturar_cep(str_cep);

                        retorno = verifica_pessoa_nascida_ou_que_mora_na_cidade(raiz_pessoa, cidade->cep);

                        if (!retorno)
                        {

                            if (remover_CEP_arvore(&cidade->cep, str_cep))
                            {
                                printf("\nCEP removido com sucesso\n");
                            }
                            else
                            {
                                printf("\nERRO: CEP nao encontrado\n");
                            }
                        }
                        else
                        {
                            printf("\nERRO: CEP nao pode ser removido\n");
                        }
                    }
                    else
                    {
                        printf("\nERRO: Cidade nao encontrada.\n");
                    }
                }
                else
                {
                    printf("\nERRO: Estado nao encontrado.\n");
                }

                pausar();
                break;
            }
            case 6:{
                char CPF[12];
                printf("\n=== Remocao de Pessoa ===\n");
                printf("Digite o CPF da pessoa a ser removida: ");
                retorno = capturar_cpf(CPF);
                if (retorno){

                    retorno = remover_pessoa_arvore(&raiz_pessoa, CPF);
                    if (retorno)
                    {
                        printf("\nPessoa removida com sucesso.\n");
                    }
                    else
                    {
                        printf("\nERRO: Pessoa nao encontrada.\n");
                    }
                }else{
                    printf("\nERRO: problema ao capturar CPF\n");
                }
                pausar();
                break;
            }
            case 7:
            {
                ESTADOS *estado;

                printf("\n=== Estado Mais Populoso ===\n");

                estado = verifica_estado_mais_populoso(cabeca_estado);

                if (estado)
                {
                    printf("\n+----------------------------------------+\n");
                    printf("| Estado: %-30s |\n", estado->nome_estado);
                    printf("+----------------------------------------+\n");
                    printf("| Populacao: %-28d |\n", estado->populacao_estado);
                    printf("| Quantidade de cidades: %-16d |\n", estado->quant_city);
                    printf("| Capital: %-30s |\n", estado->nome_capital);
                    printf("+----------------------------------------+\n");
                }
                else
                {
                    printf("\nERRO: Nenhum estado cadastrado.\n");
                }
                pausar();
                break;
            }
            case 8:
            {
                ESTADOS *estado = NULL;
                char nome_estado[100];

                printf("\n=== Populacao da Capital ===\n");
                printf("Digite o nome do estado: ");
                ler_string_simples(nome_estado, sizeof(nome_estado));
                estado = existe_estado(cabeca_estado, nome_estado);

                if (estado)
                {
                    CIDADES *capital = NULL;
                    capital = buscar_cidade(estado->cidade, estado->nome_capital);

                    if (capital)
                    {
                        printf("\n----------------------------------------\n");
                        printf("| Estado: %-30s |\n", estado->nome_estado);
                        printf("----------------------------------------\n");
                        printf("Capital: %-30s \n", estado->nome_capital);
                        printf("Populacao: %-28d \n", capital->populacao_city);
                        imprimir_CEP_em_ordem(capital->cep);
                        printf("Percentual do estado: %-17.1f%% \n",
                               (float)capital->populacao_city / estado->populacao_estado * 100);
                        printf("+----------------------------------------+\n");
                    }
                    else
                    {
                        printf("\nERRO: Capital nao encontrada\n");
                    }
                }
                else
                {
                    printf("\nERRO: Estado nao encontrado.\n");
                }

                pausar();
                break;
            }
            case 9:
            {
                char nome_estado[100];
                ESTADOS *estado = NULL;

                printf("\n=== Cidade mais populosa (exceto a capital) ===\n");
                printf("Digite o nome do estado: ");
                ler_string_simples(nome_estado, sizeof(nome_estado));
                estado = existe_estado(cabeca_estado, nome_estado);

                if (estado)
                {
                    CIDADES *cidade_mais_populosa;
                    cidade_mais_populosa = verifica_cidade_mais_populosa_nao_capital(estado->cidade, estado->nome_capital);

                    if (cidade_mais_populosa)
                    {
                        printf("\n+----------------------------------------+\n");
                        printf("| Estado: %-30s |\n", estado->nome_estado);
                        printf("+----------------------------------------+\n");
                        printf("| Cidade mais populosa: %-16s |\n", cidade_mais_populosa->nome_cidade);
                        printf("| Populacao: %-28d |\n", cidade_mais_populosa->populacao_city);
                        printf("| Percentual do estado: %-17.1f%% |\n",
                               (float)cidade_mais_populosa->populacao_city / estado->populacao_estado * 100);
                        printf("+----------------------------------------+\n");
                    }
                    else
                    {
                        printf("\nNao existem outras cidades alem da capital neste estado.\n");
                    }
                }
                else
                {
                    printf("\nERRO: Estado nao encontrado.\n");
                }
                pausar();
                break;
            }
            case 10:{

                printf("\n=== Quantas pessoas nao moram na cidade natal ===\n");
                retorno = quantas_pessoas_nao_moram_na_cidade_natal_ESTADO(cabeca_estado, raiz_pessoa);
                if (retorno > 0)
                {
                    printf("\nTotal de pessoas que nao moram na cidade natal: %d\n", retorno);
                }
                else
                {
                    printf("\nNenhuma pessoa.\n");
                }
                pausar();
                break;
            }
            case 11:{
                char cep[10];
                printf("\n=== Cidade natal de uma pessoa dado o CEP ===\n");
                printf("Digite o CEP: ");
                retorno = capturar_cep(cep);

                CIDADES *cidade = cidade_natal_dado_cep(cabeca_estado->cidade, cep);
                if (cidade)
                {
                    printf("\nCidade natal: %s\n", cidade->nome_cidade);
                }
                else
                {
                    printf("\nciade natal nao encontrada\n");
                }
                pausar();
                break;
            }
            case 12:{

                printf("\n=== Quantas pessoas nascidas em uma cidade nao moram na cidade natal ===\n");
                char nome_cidade[100];
                printf("Digite o nome da cidade: ");
                ler_string_simples(nome_cidade, sizeof(nome_cidade));
                CIDADES *cidade = buscar_cidade(cabeca_estado->cidade, nome_cidade);
                if (cidade)
                {
                    retorno = quantas_pessoas_nascidas_na_cidade_nao_moram_na_cidade(cidade, raiz_pessoa);
                    if (retorno > 0)
                    {
                        printf("\nTotal de pessoas que nasceram na cidade %s e nao moram na cidade natal: %d\n", cidade->nome_cidade, retorno);
                    }
                    else
                    {
                        printf("\nNenhuma pessoa cadastrada.\n");
                    }
                }
                else
                {
                    printf("\nERRO: Cidade nao encontrada.\n");
                }
                pausar();
                break;
            }
            case 13:{
                printf("\n=== Quantas pessoas que moram em uma cidade nao nasceram na cidade ===\n");
                char nome_cidade[100];
                printf("Digite o nome da cidade: ");
                ler_string_simples(nome_cidade, sizeof(nome_cidade));
                CIDADES *cidade = buscar_cidade(cabeca_estado->cidade, nome_cidade);
                if (cidade)
                {
                    retorno = quantas_pessoas_moram_na_cidade_nao_nasceram_nela(cidade, raiz_pessoa);
                    if (retorno > 0)
                    {
                        printf("\nTotal de pessoas que moram na cidade %s e nao nasceram na cidade: %d\n", cidade->nome_cidade, retorno);
                    }
                    else
                    {
                        printf("\nNenhuma pessoa cadastrada.\n");
                    }
                }
                else
                {
                    printf("\nERRO: Cidade nao encontrada.\n");
                }
                pausar();
                break;
            }
            case 14:{
                printf("\n=== IMPRIMIR TODOS OS ESTADOS ===\n");
                mostrar_todos_estados(cabeca_estado);
                pausar();
                break;
            }
            case 15:{
                char nome_estado[100];
                printf("\nDigite o nome do estado\n");
                ler_string_simples(nome_estado, sizeof(nome_estado));
                ESTADOS *estado = existe_estado(cabeca_estado, nome_estado);
                if (estado != NULL)
                {
                    printf("\n=== IMPRIMIR TODAS AS CIDADES ===\n");
                    imprimir_todas_cidades(estado->cidade);
                }else{
                    printf("\nERRO: Estado nao encontrado.\n");
                }
                pausar();
                break;
            }
            case 16:{
                char nome_cidade[100];
                char nome_estado[100];
                printf("\nDigite o nome do estado\n");
                ler_string_simples(nome_estado, sizeof(nome_estado));
                ESTADOS *estado = existe_estado(cabeca_estado, nome_estado);
                if (estado != NULL)
                {
                    printf("\nDigite o nome da cidade\n");
                    ler_string_simples(nome_cidade, sizeof(nome_cidade));
                    CIDADES *cidade = buscar_cidade(estado->cidade, nome_cidade);
                    if (cidade != NULL)
                    {
                        printf("\n=== IMPRIMIR TODOS OS CEPs ===\n");
                        imprimir_todos_CEP(cidade->cep);
                    }else{
                        printf("\nERRO: Cidade nao encontrada.\n");
                    }
                }
                pausar();
                break;
            }
            case 17:{
                printf("\n=== IMPRIMIR TODAS AS PESSOAS ===\n");
                imprimir_todas_pessoas(raiz_pessoa);
                pausar();
                break;
            }
            case 0:
                printf("\nSaindo do programa...\n");
                printf("Liberando memoria...\n");
                liberar_arvore_pessoas(&raiz_pessoa);
                liberar_todos_estados(&cabeca_estado);
                printf("Liberando memoria concluido.\n");
                printf("Sistema finalizado com sucesso!\n");
                break;
            default:
                printf("Opcao invalida! Tente novamente.\n");
                break;
            }
        }
    } while (opcao1 != 0);
}

int main(){
    menu_geral();
    return 0;
}