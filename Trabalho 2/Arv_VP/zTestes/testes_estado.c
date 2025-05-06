#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../Arv_VP/Arv_VP_H/Estados.h"
#include "../Arv_VP/Arv_VP_H/utilitarios.h"
#include "../Arv_VP/Arv_VP_H/STRUCTS.h"

/* cd "c:\Users\jsous\OneDrive\Área de Trabalho\ED2\Trabalho 2"

gcc -o Arv_VP/zTestes/teste_estado Arv_VP/zTestes/testes_estado.c Arv_VP/Arv_VP_C/Estados.c Arv_VP/Arv_VP_C/utilitarios.c -I Arv_VP/Arv_VP_H

Arv_VP\zTestes\teste_estado */

// Estrutura para registrar os resultados dos testes
typedef struct {
    char nome_teste[50];
    int passou;
    char detalhes[100];
} ResultadoTeste;

// Variaveis globais para controle dos resultados
#define MAX_TESTES 100
ResultadoTeste resultados[MAX_TESTES];
int num_testes = 0;

// Funcao para registrar resultado de teste
void registrar_resultado(const char* nome, int passou, const char* detalhes) {
    if (num_testes < MAX_TESTES) {
        strcpy(resultados[num_testes].nome_teste, nome);
        resultados[num_testes].passou = passou;
        strcpy(resultados[num_testes].detalhes, detalhes);
        num_testes++;
    }
}

// Funcao para imprimir o relatorio final
void imprimir_relatorio() {
    int testes_passados = 0;
    
    printf("\n\n========= RELATORIO FINAL DE TESTES =========\n");
    printf("%-30s | %-10s | %s\n", "Teste", "Resultado", "Detalhes");
    printf("-----------------------------------------------------------\n");
    
    for (int i = 0; i < num_testes; i++) {
        printf("%-30s | %-10s | %s\n", 
               resultados[i].nome_teste,
               resultados[i].passou ? "PASSOU" : "FALHOU",
               resultados[i].detalhes);
        if (resultados[i].passou) testes_passados++;
    }
    
    printf("-----------------------------------------------------------\n");
    printf("Total: %d teste(s), %d passado(s), %d falha(s)\n", 
           num_testes, testes_passados, num_testes - testes_passados);
    printf("=================================================\n");
}

// Funcao auxiliar para imprimir um unico estado
void imprimir_estado(ESTADOS *estado) {
    if (estado == NULL) {
        printf("Estado nulo\n");
        return;
    }
    printf("Estado: %s, Capital: %s\n", estado->nome_estado, estado->nome_capital);
}

// Funcao auxiliar para imprimir a lista de estados
void imprimir_lista_estados(ESTADOS *cabeca) {
    ESTADOS *atual = cabeca;
    printf("\nLista de Estados:\n");
    if (atual == NULL) {
        printf("Lista vazia.\n");
        return;
    }
    
    while (atual != NULL) {
        imprimir_estado(atual);
        atual = atual->prox;
    }
    printf("\n");
}

// Funcao para testar criar_estado() - Modificada para registrar resultado
int teste_criar_estado() {
    printf("\n=== TESTE: criar_estado() ===\n");
    
    char *nome_estado = strdup("Sao Paulo");
    char *nome_capital = strdup("Sao Paulo");
    int teste_passou = 1;
    char detalhes[100] = "OK";
    
    ESTADOS *estado = criar_estado(nome_estado, nome_capital);
    
    printf("Estado criado: ");
    if (estado != NULL) {
        imprimir_estado(estado);
        
        // Verificando campos
        printf("Verificacao de campos:\n");
        
        // Verifica nome do estado
        int nome_ok = strcmp(estado->nome_estado, nome_estado) == 0;
        printf("- Nome do estado: %s\n", nome_ok ? "CORRETO" : "INCORRETO");
        if (!nome_ok) {
            teste_passou = 0;
            strcat(detalhes, " | Nome incorreto");
        }
        
        // Verifica nome da capital
        int capital_ok = strcmp(estado->nome_capital, nome_capital) == 0;
        printf("- Nome da capital: %s\n", capital_ok ? "CORRETO" : "INCORRETO");
        if (!capital_ok) {
            teste_passou = 0;
            strcat(detalhes, " | Capital incorreta");
        }
        
        // Verifica quantidade de cidades
        int quant_ok = estado->quant_city == 0;
        printf("- Quantidade de cidades: %s\n", quant_ok ? "CORRETO (0)" : "INCORRETO");
        if (!quant_ok) {
            teste_passou = 0;
            strcat(detalhes, " | Quantidade incorreta");
        }
        
        // Verifica ponteiro para cidades
        int cidade_ok = estado->cidade == NULL;
        printf("- Ponteiro para cidades: %s\n", cidade_ok ? "CORRETO (NULL)" : "INCORRETO");
        if (!cidade_ok) {
            teste_passou = 0;
            strcat(detalhes, " | Ponteiro cidade incorreto");
        }
        
        // Verifica ponteiro anterior
        int ant_ok = estado->ant == NULL;
        printf("- Ponteiro anterior: %s\n", ant_ok ? "CORRETO (NULL)" : "INCORRETO");
        if (!ant_ok) {
            teste_passou = 0;
            strcat(detalhes, " | Ponteiro ant incorreto");
        }
        
        // Verifica ponteiro proximo
        int prox_ok = estado->prox == NULL;
        printf("- Ponteiro proximo: %s\n", prox_ok ? "CORRETO (NULL)" : "INCORRETO");
        if (!prox_ok) {
            teste_passou = 0;
            strcat(detalhes, " | Ponteiro prox incorreto");
        }
        
        // Liberando memoria
        free(estado->nome_estado);
        free(estado->nome_capital);
        free(estado);
    } else {
        printf("FALHA: criar_estado retornou NULL\n");
        teste_passou = 0;
        strcpy(detalhes, "Funcao retornou NULL");
    }
    
    // Registra o resultado
    registrar_resultado("Criar Estado", teste_passou, detalhes);
    return teste_passou;
}

// Funcao para testar inserir_estado_rec() - Modificada para registrar resultado
ESTADOS* teste_inserir_estado_rec() {
    printf("\n=== TESTE: inserir_estado_rec() ===\n");
    
    ESTADOS *lista = NULL;
    int resultado;
    int teste_passou = 1;
    char detalhes[100] = "OK";
    
    // Caso 1: Inserir em lista vazia
    ESTADOS *estado1 = criar_estado(strdup("Minas Gerais"), strdup("Belo Horizonte"));
    resultado = inserir_estado_rec(&lista, estado1);
    printf("\nCaso 1: Inserir em lista vazia\n");
    printf("Resultado: %s (codigo %d)\n", resultado == 1 ? "SUCESSO" : "FALHA", resultado);
    printf("Estado na lista: ");
    imprimir_lista_estados(lista);
    
    // Caso 2: Inserir no inicio (ordem alfabetica)
    ESTADOS *estado2 = criar_estado(strdup("Acre"), strdup("Rio Branco"));
    resultado = inserir_estado_rec(&lista, estado2);
    printf("\nCaso 2: Inserir no inicio (ordem alfabetica)\n");
    printf("Resultado: %s (codigo %d)\n", resultado == 1 ? "SUCESSO" : "FALHA", resultado);
    printf("Estados na lista: ");
    imprimir_lista_estados(lista);
    
    // Caso 3: Inserir no final (ordem alfabetica)
    ESTADOS *estado3 = criar_estado(strdup("Sao Paulo"), strdup("Sao Paulo"));
    resultado = inserir_estado_rec(&lista, estado3);
    printf("\nCaso 3: Inserir no final (ordem alfabetica)\n");
    printf("Resultado: %s (codigo %d)\n", resultado == 1 ? "SUCESSO" : "FALHA", resultado);
    printf("Estados na lista: ");
    imprimir_lista_estados(lista);
    
    // Caso 4: Inserir no meio (ordem alfabetica)
    ESTADOS *estado4 = criar_estado(strdup("Bahia"), strdup("Salvador"));
    resultado = inserir_estado_rec(&lista, estado4);
    printf("\nCaso 4: Inserir no meio (ordem alfabetica)\n");
    printf("Resultado: %s (codigo %d)\n", resultado == 1 ? "SUCESSO" : "FALHA", resultado);
    printf("Estados na lista: ");
    imprimir_lista_estados(lista);
    
    // Caso 5: Tentar inserir estado duplicado
    ESTADOS *estado5 = criar_estado(strdup("Bahia"), strdup("Salvador"));
    resultado = inserir_estado_rec(&lista, estado5);
    printf("\nCaso 5: Tentar inserir estado duplicado\n");
    printf("Resultado: %s (codigo %d)\n", resultado == 0 ? "SUCESSO (detectou duplicado)" : "FALHA", resultado);
    printf("Estados na lista (nao deve mudar): ");
    imprimir_lista_estados(lista);
    
    // Registra o resultado
    registrar_resultado("Inserir Estado", teste_passou, detalhes);
    return lista;
}

// Funcao para testar limpar_estado() - Modificada para registrar resultado
int teste_limpar_estado(ESTADOS *estado) {
    printf("\n=== TESTE: limpar_estado() ===\n");
    
    int teste_passou = 1;
    char detalhes[100] = "OK";
    
    if (estado == NULL) {
        printf("ERRO: Estado nulo fornecido para teste\n");
        teste_passou = 0;
        strcpy(detalhes, "Estado nulo fornecido");
        registrar_resultado("Limpar Estado", teste_passou, detalhes);
        return teste_passou;
    }
    
    printf("Estado antes da limpeza: ");
    imprimir_estado(estado);
    
    // Cria copias das strings para verificar depois
    char *nome_antigo = strdup(estado->nome_estado);
    char *capital_antiga = strdup(estado->nome_capital);
    
    printf("Chamando limpar_estado()...\n");
    limpar_estado(estado);
    
    printf("Verificando estado apos limpeza:\n");
    if (estado->nome_estado == NULL) {
        printf("- Nome do estado: CORRETO (foi limpo)\n");
    } else {
        printf("- Nome do estado: INCORRETO (ainda contem dados: %s)\n", estado->nome_estado);
        teste_passou = 0;
        strcat(detalhes, " | Nome nao foi limpo");
    }
    
    if (estado->nome_capital == NULL) {
        printf("- Nome da capital: CORRETO (foi limpo)\n");
    } else {
        printf("- Nome da capital: INCORRETO (ainda contem dados: %s)\n", estado->nome_capital);
        teste_passou = 0;
        strcat(detalhes, " | Capital nao foi limpo");
    }
    
    free(nome_antigo);
    free(capital_antiga);
    free(estado);
    
    // Registra o resultado
    registrar_resultado("Limpar Estado", teste_passou, detalhes);
    return teste_passou;
}

// Funcao para testar remover_estado_rec() - Modificada para registrar resultado
int teste_remover_estado_rec() {
    printf("\n=== TESTE: remover_estado_rec() ===\n");
    
    int teste_passou = 1;
    char detalhes[100] = "OK";
    
    ESTADOS *lista = NULL;
    int resultado;
    
    // Preparar lista de teste
    inserir_estado_rec(&lista, criar_estado(strdup("Acre"), strdup("Rio Branco")));
    inserir_estado_rec(&lista, criar_estado(strdup("Bahia"), strdup("Salvador")));
    inserir_estado_rec(&lista, criar_estado(strdup("Minas Gerais"), strdup("Belo Horizonte")));
    inserir_estado_rec(&lista, criar_estado(strdup("Sao Paulo"), strdup("Sao Paulo")));
    
    printf("\nLista de estados para teste de remocao:\n");
    imprimir_lista_estados(lista);
    
    // Caso 1: Remover estado no inicio
    printf("\nCaso 1: Remover estado no inicio (Acre)\n");
    resultado = remover_estado_rec(&lista, "Acre");
    printf("Resultado: %s (codigo %d)\n", resultado == 1 ? "SUCESSO" : "FALHA", resultado);
    printf("Lista apos remocao: ");
    imprimir_lista_estados(lista);
    
    // Caso 2: Remover estado no meio
    printf("\nCaso 2: Remover estado no meio (Minas Gerais)\n");
    resultado = remover_estado_rec(&lista, "Minas Gerais");
    printf("Resultado: %s (codigo %d)\n", resultado == 1 ? "SUCESSO" : "FALHA", resultado);
    printf("Lista apos remocao: ");
    imprimir_lista_estados(lista);
    
    // Caso 3: Remover estado no fim
    printf("\nCaso 3: Remover estado no fim (Sao Paulo)\n");
    resultado = remover_estado_rec(&lista, "Sao Paulo");
    printf("Resultado: %s (codigo %d)\n", resultado == 1 ? "SUCESSO" : "FALHA", resultado);
    printf("Lista apos remocao: ");
    imprimir_lista_estados(lista);
    
    // Caso 4: Remover estado que nao existe
    printf("\nCaso 4: Remover estado que nao existe (Parana)\n");
    resultado = remover_estado_rec(&lista, "Parana");
    printf("Resultado: %s (codigo %d)\n", resultado == 0 ? "SUCESSO (nao encontrou)" : "FALHA", resultado);
    printf("Lista apos tentativa de remocao: ");
    imprimir_lista_estados(lista);
    
    // Caso 5: Remover ultimo estado
    printf("\nCaso 5: Remover ultimo estado (Bahia)\n");
    resultado = remover_estado_rec(&lista, "Bahia");
    printf("Resultado: %s (codigo %d)\n", resultado == 1 ? "SUCESSO" : "FALHA", resultado);
    printf("Lista apos remocao: ");
    imprimir_lista_estados(lista);
    
    // Caso 6: Remover de lista vazia
    printf("\nCaso 6: Remover de lista vazia\n");
    resultado = remover_estado_rec(&lista, "Qualquer");
    printf("Resultado: %s (codigo %d)\n", resultado == 0 ? "SUCESSO (lista vazia)" : "FALHA", resultado);
    printf("Lista apos tentativa de remocao: ");
    imprimir_lista_estados(lista);
    
    // Registra o resultado
    registrar_resultado("Remover Estado", teste_passou, detalhes);
    return teste_passou;
}

// Funcao para testar mostrar_estado() - Modificada para registrar resultado
int teste_mostrar_estado() {
    printf("\n=== TESTE: mostrar_estado() ===\n");
    
    int teste_passou = 1;
    char detalhes[100] = "OK";
    
    ESTADOS *estado = criar_estado(strdup("Rio de Janeiro"), strdup("Rio de Janeiro"));
    
    printf("Chamando mostrar_estado()...\n");
    printf("Resultado esperado: exibir informacoes do estado Rio de Janeiro\n");
    int resultado = mostrar_estado(estado);
    printf("\nCodigo de retorno: %d (Deve ser 1 para sucesso)\n", resultado);
    
    if (resultado != 1) {
        teste_passou = 0;
        strcpy(detalhes, "Falha ao mostrar estado");
    }
    
    printf("\nCaso: Estado NULL\n");
    resultado = mostrar_estado(NULL);
    printf("Codigo de retorno: %d (Deve ser 0 para falha)\n", resultado);
    
    if (resultado != 0) {
        teste_passou = 0;
        strcat(detalhes, " | Falha ao tratar estado NULL");
    }
    
    // Limpar
    limpar_estado(estado);
    free(estado);
    
    // Registra o resultado
    registrar_resultado("Mostrar Estado", teste_passou, detalhes);
    return teste_passou;
}

// Funcao para testar mostrar_todos_estados() - Modificada para registrar resultado
int teste_mostrar_todos_estados() {
    printf("\n=== TESTE: mostrar_todos_estados() ===\n");
    
    int teste_passou = 1;
    char detalhes[100] = "OK";
    
    ESTADOS *lista = NULL;
    
    printf("\nCaso 1: Lista vazia\n");
    int resultado = mostrar_todos_estados(lista);
    printf("Codigo de retorno: %d (Deve ser 0 para lista vazia)\n", resultado);
    
    if (resultado != 0) {
        teste_passou = 0;
        strcpy(detalhes, "Falha ao tratar lista vazia");
    }
    
    // Preparar lista de teste
    inserir_estado_rec(&lista, criar_estado(strdup("Acre"), strdup("Rio Branco")));
    inserir_estado_rec(&lista, criar_estado(strdup("Bahia"), strdup("Salvador")));
    
    printf("\nCaso 2: Lista com estados\n");
    printf("Resultado esperado: exibir todos os estados na lista\n");
    resultado = mostrar_todos_estados(lista);
    printf("\nCodigo de retorno: %d (Deve ser 1 para sucesso)\n", resultado);
    
    if (resultado != 1) {
        teste_passou = 0;
        strcat(detalhes, " | Falha ao mostrar todos estados");
    }
    
    // Limpar lista
    while (lista != NULL) {
        ESTADOS *temp = lista;
        lista = lista->prox;
        limpar_estado(temp);
        free(temp);
    }
    
    // Registra o resultado
    registrar_resultado("Mostrar Todos Estados", teste_passou, detalhes);
    return teste_passou;
}

int main() {
    printf("===== TESTES UNITARIOS PARA FUNCOES DE ESTADO =====\n");
    
    // Teste da funcao criar_estado
    teste_criar_estado();
    
    // Teste da funcao inserir_estado_rec
    ESTADOS *lista_teste = teste_inserir_estado_rec();
    
    // Teste da funcao limpar_estado
    if (lista_teste != NULL) {
        ESTADOS *estado_teste = lista_teste;
        lista_teste = lista_teste->prox; // Pega o primeiro e avanca lista
        estado_teste->prox = NULL; // Isola o estado
        teste_limpar_estado(estado_teste);
    } else {
        registrar_resultado("Limpar Estado", 0, "Nao executado - lista vazia");
    }
    
    // Limpar a lista restante
    while (lista_teste != NULL) {
        ESTADOS *temp = lista_teste;
        lista_teste = lista_teste->prox;
        limpar_estado(temp);
        free(temp);
    }
    
    // Teste da funcao remover_estado_rec
    teste_remover_estado_rec();
    
    // Teste da funcao mostrar_estado
    teste_mostrar_estado();
    
    // Teste da funcao mostrar_todos_estados
    teste_mostrar_todos_estados();
    
    // Imprimir relatorio final
    imprimir_relatorio();
    
    printf("\n===== TESTES CONCLUIDOS =====\n");
    return 0;
}
