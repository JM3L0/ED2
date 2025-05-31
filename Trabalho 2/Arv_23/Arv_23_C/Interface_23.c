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

void povoar_sistema(ESTADOS **cabeca_estados, Arv23_PESSOAS **raiz_pessoas);

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

    povoar_sistema(&cabeca_estados, &raiz_pessoas);

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
            opcao2 = 0;

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
                    do {

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

                        printf("Deseja cadastrar outro CEP para a cidade %s? [1] - SIM [0] - NAO: ", cidade->nome_cidade);
                        opcao2 = digitar_int();

                    }while (opcao2 == 1);
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
        case 4:{
            PESSOAS novaPessoa;
            char cep_natal[10], cep_atual[10];
            opcao2 = 0;

            do {

                printf("Digite o CEP da natal da pessoa: ");
                sucesso = capturar_cep(cep_natal);
                if (sucesso){
                    if (percorre_estados_procurando_CEP(cabeca_estados, cep_natal)){
                        printf("Digite o CEP atual da pessoa: ");
                        sucesso = capturar_cep(cep_atual);
    
                        if(percorre_estados_procurando_CEP(cabeca_estados, cep_natal)){
    
                            novaPessoa = cadastra_pessoa(cep_natal, cep_atual, &sucesso);
                            if (sucesso)
                            {
                                if (insere_23_PESSOAS(&raiz_pessoas, novaPessoa))
                                {
                                    printf("Pessoa %s cadastrada com sucesso!\n", novaPessoa.nome_pessoa);
                                }
                                else
                                {
                                    printf("Erro ao cadastrar pessoa!\n");
                                }
                            }
    
                        }else{
                            printf("CEP %s nao encontrado em nenhum estado!\n", cep_atual);
                            
                        }
                    }else{
                        printf("CEP %s nao encontrado em nenhum estado!\n", cep_natal);
                    }
                }
                printf("Deseja cadastrar outra pessoa? [1] - SIM [0] - NAO: ");
            }while (opcao2 == 1);
        }
        case 5:{
            char estado_nome[100];
            char cidade_nome[100];
            CEP Cep;

            printf("Digite o nome do estado: ");
            ler_string_simples(estado_nome, sizeof(estado_nome));
            ESTADOS *estado = existe_estado(cabeca_estados, estado_nome);

            if (estado)
            {
                printf("Digite o nome da cidade: ");
                ler_string_simples(cidade_nome, sizeof(cidade_nome));
                CIDADES *cidade = buscar_info_cidade(estado->arv_cidades, cidade_nome);
                if (cidade)
                {
                    printf("=== Remover CEP (da cidade %s) ===\n", cidade->nome_cidade);
                    sucesso = capturar_cep(Cep.cep);

                    StatusRemocao status = remover_23_CEP(&(cidade->arv_cep), Cep);
                    mensagens_do_remover(status);
                }
                else
                {
                    printf("Cidade %s nao encontrada no estado %s!\n", cidade_nome, estado_nome);
                }
            }
            else
            {
                printf("Estado %s nao encontrado!\n", estado_nome);
            }
            break;
        }
        case 6:{
            imprime_arvore_visual_PESSOAS(raiz_pessoas, "", 1, 1);
            PESSOAS pessoa_remover;
            printf("=== Remover Pessoa (pelo CPF) ===\n");
            sucesso = capturar_cpf(pessoa_remover.CPF);

            StatusRemocao status = remover_23_PESSOAS(&raiz_pessoas, pessoa_remover);
            mensagens_do_remover(status);

            imprime_arvore_visual_PESSOAS(raiz_pessoas, "", 1, 1);
           
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
        case 16:{
            char nome_estado[100];
            char nome_cidade[100];
            printf("Digite o nome do estado: ");
            ler_string_simples(nome_estado, sizeof(nome_estado));
            ESTADOS *estado = existe_estado(cabeca_estados, nome_estado);
            if (estado)
            {
                printf("Digite o nome da cidade: ");
                ler_string_simples(nome_cidade, sizeof(nome_cidade));
                CIDADES *cidade = buscar_info_cidade(estado->arv_cidades, nome_cidade);
                if (cidade)
                {
                    imprime_23_em_ordem_CEP(cidade->arv_cep);
                    imprime_arvore_visual_CEP(cidade->arv_cep, "", 1, 1);
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
        case 17:{
            if (raiz_pessoas != NULL)
            {
                imprime_23_em_ordem_PESSOAS(raiz_pessoas);
                imprime_arvore_visual_PESSOAS(raiz_pessoas, "", 1, 1);
            }
            else
            {
                printf("Nenhuma pessoa cadastrada!\n");
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

void povoar_sistema(ESTADOS **cabeca_estados, Arv23_PESSOAS **raiz_pessoas) {
    int i, j, k, sucesso;
    
    printf("Povoando sistema com dados iniciais...\n");
    
    // Arrays com nomes para povoar o sistema (30 de cada)
    const char *estados[] = {
        "Sao Paulo", "Rio de Janeiro", "Minas Gerais", "Bahia", "Parana", 
        "Rio Grande do Sul", "Pernambuco", "Ceara", "Para", "Santa Catarina",
        "Goias", "Maranhao", "Amazonas", "Espirito Santo", "Paraiba",
        "Mato Grosso", "Rio Grande do Norte", "Alagoas", "Piaui", "Distrito Federal",
        "Mato Grosso do Sul", "Sergipe", "Rondonia", "Tocantins", "Acre",
        "Amapa", "Roraima", "Brasilia", "Fernando de Noronha", "Palmas"
    };
    
    const char *cidades[] = {
        "Capital", "Cidade Norte", "Cidade Sul", "Cidade Leste", "Cidade Oeste",
        "Vila Central", "Vila Serrana", "Centro Urbano", "Metropole", "Distrito Rural",
        "Regiao Litoral", "Area Comercial", "Polo Industrial", "Cidade Turistica", "Porto Principal",
        "Cidade Nova", "Vila Antiga", "Zona Portuaria", "Distrito Federal", "Regiao Periferica",
        "Vale Central", "Cidade Alta", "Cidade Baixa", "Polo Tecnologico", "Regiao Administrativa",
        "Conjunto Habitacional", "Distrito Residencial", "Area Metropolitana", "Cidade Satelite", "Bairro Central"
    };
    
    const char *nomes[] = {
        "Joao Silva", "Maria Souza", "Carlos Oliveira", "Ana Santos", "Pedro Lima",
        "Julia Pereira", "Marcos Ferreira", "Lucia Costa", "Roberto Almeida", "Fernanda Rodrigues",
        "Ricardo Gomes", "Patricia Martins", "Fernando Ribeiro", "Camila Carvalho", "Antonio Nascimento",
        "Amanda Barbosa", "Lucas Mendes", "Mariana Cardoso", "Bruno Correia", "Juliana Torres",
        "Felipe Nunes", "Beatriz Rocha", "Rafael Moraes", "Larissa Duarte", "Vitor Castro",
        "Carolina Farias", "Diego Vieira", "Bianca Teixeira", "Gabriel Monteiro", "Leticia Campos"
    };
    
    // Criar estados
    int num_estados = sizeof(estados) / sizeof(estados[0]);
    int num_cidades = sizeof(cidades) / sizeof(cidades[0]);
    
    for (i = 0; i < num_estados; i++) {
        ESTADOS *estado = (ESTADOS *)malloc(sizeof(ESTADOS));
        if (!estado) continue;
        
        estado->nome_estado = (char *)malloc(100 * sizeof(char));
        strcpy(estado->nome_estado, estados[i]);
        estado->arv_cidades = NULL;
        estado->populacao_estado = 0;
        estado->quant_city = 0;
        
        if (!inserir_estado_rec(cabeca_estados, estado)) {
            free(estado->nome_estado);
            free(estado);
            continue;
        }
        
        // Adicionar cidades para cada estado (até 30 cidades)
        int max_cidades = (num_cidades < 30) ? num_cidades : 30;
        for (j = 0; j < max_cidades; j++) {
            char nome_cidade[100];
            if (j == 0) {
                sprintf(nome_cidade, "Capital de %s", estados[i]);
            } else {
                sprintf(nome_cidade, "%s de %s", cidades[j], estados[i]);
            }
            
            CIDADES cidade;
            strcpy(cidade.nome_cidade, nome_cidade);
            cidade.populacao_city = 100000 + (i * 10000) + (j * 1000);  // População determinística
            cidade.arv_cep = NULL;
            
            // Adicionar 30 CEPs para cada cidade
            for (k = 0; k < 30; k++) {
                CEP cep;
                sprintf(cep.cep, "%02d%03d-%03d", (i%99)+1, (j%999)+1, (k%999)+1);  // Formato: EECCC-SSS (estado, cidade, sequencial)
                
                if (insere_23_CEP(&(cidade.arv_cep), cep)) {
                    if (k == 0) {
                        printf("CEP %s adicionado para %s\n", cep.cep, nome_cidade);
                    }
                }
            }
            
            if (insere_23_cidade(&(estado->arv_cidades), cidade)) {
                estado->quant_city++;
                estado->populacao_estado += cidade.populacao_city;
                
                if (j == 0) {
                    strcpy(estado->nome_capital, cidade.nome_cidade);
                }
            }
        }
    }
    
    // Adicionar pessoas (30)
    int num_pessoas = sizeof(nomes) / sizeof(nomes[0]);
    
    // Criar um pool de CEPs para usar
    char pool_ceps[100][10]; // Aumentado para comportar mais CEPs
    int cep_count = 0;
    
    // Preencher o pool com alguns CEPs existentes
    for (i = 0; i < 10 && i < num_estados; i++) {
        for (j = 0; j < 10 && j < num_cidades; j++) {
            sprintf(pool_ceps[cep_count], "%02d%03d-%03d", (i%99)+1, (j%999)+1, 1);
            cep_count++;
            if (cep_count >= 100) break; // Limite de segurança
        }
        if (cep_count >= 100) break;
    }
    
    for (i = 0; i < num_pessoas; i++) {
        PESSOAS pessoa;
        strcpy(pessoa.nome_pessoa, nomes[i]);
        
        // Gerar CPF único (formato: xxxxxxxxxxx, 11 dígitos sem separadores)
        sprintf(pessoa.CPF, "%03d%03d%03d%02d", i+100, i+200, i+300, i+10);
        
        // Data de nascimento variada
        pessoa.data_nasc.dia = (i % 28) + 1;
        pessoa.data_nasc.mes = ((i % 12) + 1);
        pessoa.data_nasc.ano = 1970 + (i % 50);
        
        // Escolher CEPs do pool
        int idx_natal = i % cep_count;
        int idx_atual = (i + 5) % cep_count;  // Deslocamento para ter alguns diferentes
        strcpy(pessoa.cep_city_natal, pool_ceps[idx_natal]);
        strcpy(pessoa.cep_city_atual, pool_ceps[idx_atual]);
        
        if (insere_23_PESSOAS(raiz_pessoas, pessoa)) {
            printf("Pessoa %s adicionada com sucesso.\n", pessoa.nome_pessoa);
        }
    }
    
    printf("Sistema povoado com sucesso com 30 itens de cada categoria!\n");
}
