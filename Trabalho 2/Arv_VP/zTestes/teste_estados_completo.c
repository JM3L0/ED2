/*
gcc -o teste_estados_completo Arv_VP_C/teste_estados_completo.c Arv_VP_C/Estados.c 
Arv_VP_C/utilitarios.c -I Arv_VP_H
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../Arv_VP_H/Estados.h"
#include "../Arv_VP_H/utilitarios.h"
#include "../Arv_VP_H/STRUCTS.h"

// Funcao para criar copia de string para testes
char* duplicar_string(const char* str) {
    char* result = (char*)malloc(strlen(str) + 1);
    if (result) {
        strcpy(result, str);
    }
    return result;
}

// Funcao para exibir resultado do teste
void exibir_resultado(const char* descricao, int sucesso) {
    printf("%-50s %s\n", descricao, sucesso ? "[OK]" : "[FALHA]");
}

// Funcao para contar estados na lista
int contar_estados(ESTADOS *cabeca) {
    int contador = 0;
    ESTADOS *atual = cabeca;
    
    while (atual != NULL) {
        contador++;
        atual = atual->prox;
    }
    
    return contador;
}

// Funcoes de teste
void teste_criar_estado();
void teste_inserir_estado();
void teste_remover_estado();
void teste_listar_estados();

int main() {
    printf("========== TESTES DE ESTADOS ==========\n\n");
    
    printf("--- Teste de Criacao de Estado ---\n");
    teste_criar_estado();
    
    printf("\n--- Teste de Insercao de Estado ---\n");
    teste_inserir_estado();
    
    printf("\n--- Teste de Remocao de Estado ---\n");
    teste_remover_estado();
    
    printf("\n--- Teste de Listagem de Estados ---\n");
    teste_listar_estados();
    
    printf("\n========== TESTES CONCLUIDOS ==========\n");
    return 0;
}

// Teste de criacao de estado
void teste_criar_estado() {
    printf("CASO 1: Criacao normal de estado\n");
    
    ESTADOS *estado = criar_estado(duplicar_string("Piaui"), duplicar_string("Teresina"));
    if (estado != NULL) {
        exibir_resultado("Estado criado com sucesso", 1);
        exibir_resultado("Nome do estado correto", strcmp(estado->nome_estado, "Piaui") == 0);
        exibir_resultado("Capital correta", strcmp(estado->nome_capital, "Teresina") == 0);
        exibir_resultado("Contador de cidades zerado", estado->quant_city == 0);
        exibir_resultado("Ponteiro cidade NULL", estado->cidade == NULL);
        exibir_resultado("Ponteiro proximo NULL", estado->prox == NULL);
        exibir_resultado("Ponteiro anterior NULL", estado->ant == NULL);
        
        // Limpar
        limpar_estado(estado);
        free(estado);
    } else {
        exibir_resultado("Falha ao criar estado", 0);
    }
    
    printf("\nCASO 2: Verificar valores no limite\n");
    
    // Teste com string vazia
    estado = criar_estado(duplicar_string(""), duplicar_string("Capital"));
    exibir_resultado("Criar estado com nome vazio", estado != NULL);
    if (estado != NULL) {
        limpar_estado(estado);
        free(estado);
    }
    
    // Teste com string muito grande
    char *nome_grande = (char*)malloc(1000);
    if (nome_grande) {
        memset(nome_grande, 'A', 999);
        nome_grande[999] = '\0';
        
        estado = criar_estado(nome_grande, duplicar_string("Capital"));
        exibir_resultado("Criar estado com nome muito grande", estado != NULL);
        if (estado != NULL) {
            limpar_estado(estado);
            free(estado);
        }
    } else {
        free(nome_grande); // Liberar se alocacao falhou
    }
}

// Teste de insercao de estado
void teste_inserir_estado() {
    ESTADOS *cabeca = NULL;
    int resultado;
    
    printf("CASO 1: Inserir em lista vazia\n");
    ESTADOS *estado1 = criar_estado(duplicar_string("Piaui"), duplicar_string("Teresina"));
    resultado = inserir_estado_rec(&cabeca, estado1);
    exibir_resultado("Inserir estado em lista vazia", 
                    resultado == 1 && cabeca == estado1);
    
    printf("\nCASO 2: Inserir no inicio da lista\n");
    ESTADOS *estado2 = criar_estado(duplicar_string("Acre"), duplicar_string("Rio Branco"));
    resultado = inserir_estado_rec(&cabeca, estado2);
    exibir_resultado("Inserir estado no inicio", 
                    resultado == 1 && cabeca == estado2 && cabeca->prox == estado1);
    
    printf("\nCASO 3: Inserir no meio da lista\n");
    ESTADOS *estado3 = criar_estado(duplicar_string("Ceara"), duplicar_string("Fortaleza"));
    resultado = inserir_estado_rec(&cabeca, estado3);
    exibir_resultado("Inserir estado no meio", resultado == 1);
    
    printf("\nCASO 4: Inserir no fim da lista\n");
    ESTADOS *estado4 = criar_estado(duplicar_string("Sergipe"), duplicar_string("Aracaju"));
    resultado = inserir_estado_rec(&cabeca, estado4);
    exibir_resultado("Inserir estado no fim", resultado == 1);
    
    printf("\nCASO 5: Tentar inserir estado duplicado\n");
    ESTADOS *duplicado = criar_estado(duplicar_string("Piaui"), duplicar_string("Teresina"));
    resultado = inserir_estado_rec(&cabeca, duplicado);
    exibir_resultado("Inserir estado duplicado deve falhar", resultado == 0);
    
    printf("\nLista final de estados:\n");
    mostrar_todos_estados(cabeca);
    
    int contador = contar_estados(cabeca);
    printf("Numero total de estados: %d (esperado: 4)\n", contador);
    exibir_resultado("Contagem de estados correta", contador == 4);
    
    // Limpar toda a lista
    while (cabeca != NULL) {
        ESTADOS *temp = cabeca;
        cabeca = cabeca->prox;
        limpar_estado(temp);
        free(temp);
    }
}

// Teste de remocao de estado
void teste_remover_estado() {
    ESTADOS *cabeca = NULL;
    int resultado;
    
    // Criar lista para testes
    inserir_estado_rec(&cabeca, criar_estado(duplicar_string("Acre"), duplicar_string("Rio Branco")));
    inserir_estado_rec(&cabeca, criar_estado(duplicar_string("Ceara"), duplicar_string("Fortaleza")));
    inserir_estado_rec(&cabeca, criar_estado(duplicar_string("Piaui"), duplicar_string("Teresina")));
    inserir_estado_rec(&cabeca, criar_estado(duplicar_string("Sergipe"), duplicar_string("Aracaju")));
    
    printf("Lista inicial para testes:\n");
    mostrar_todos_estados(cabeca);
    
    printf("\nCASO 1: Remover estado do inicio\n");
    resultado = remover_estado_rec(&cabeca, "Acre");
    exibir_resultado("Remover primeiro estado (Acre)", resultado == 1);
    exibir_resultado("Lista tem um estado a menos", contar_estados(cabeca) == 3);
    
    printf("\nCASO 2: Remover estado do meio\n");
    resultado = remover_estado_rec(&cabeca, "Piaui");
    exibir_resultado("Remover estado do meio (Piaui)", resultado == 1);
    exibir_resultado("Lista agora tem dois estados", contar_estados(cabeca) == 2);
    
    printf("\nCASO 3: Remover estado inexistente\n");
    resultado = remover_estado_rec(&cabeca, "Bahia");
    exibir_resultado("Remover estado inexistente deve falhar", resultado == 0);
    exibir_resultado("Lista continua com dois estados", contar_estados(cabeca) == 2);
    
    printf("\nCASO 4: Remover estado do fim\n");
    resultado = remover_estado_rec(&cabeca, "Sergipe");
    exibir_resultado("Remover estado final (Sergipe)", resultado == 1);
    exibir_resultado("Lista agora tem um estado", contar_estados(cabeca) == 1);
    
    printf("\nCASO 5: Remover ultimo estado\n");
    resultado = remover_estado_rec(&cabeca, "Ceara");
    exibir_resultado("Remover ultimo estado (Ceara)", resultado == 1);
    exibir_resultado("Lista agora esta vazia", cabeca == NULL);
    
    printf("\nCASO 6: Remover de lista vazia\n");
    resultado = remover_estado_rec(&cabeca, "Qualquer");
    exibir_resultado("Remover de lista vazia deve falhar", resultado == 0);
}

// Teste de listagem de estados
void teste_listar_estados() {
    ESTADOS *cabeca = NULL;
    int resultado;
    
    printf("CASO 1: Listar lista vazia\n");
    resultado = mostrar_todos_estados(cabeca);
    exibir_resultado("Listar lista vazia deve retornar 0", resultado == 0);
    
    printf("\nCASO 2: Listar lista com um estado\n");
    inserir_estado_rec(&cabeca, criar_estado(duplicar_string("Piaui"), duplicar_string("Teresina")));
    printf("Lista com um estado:\n");
    resultado = mostrar_todos_estados(cabeca);
    exibir_resultado("Listar lista com um estado deve retornar 1", resultado == 1);
    
    printf("\nCASO 3: Listar lista com varios estados\n");
    inserir_estado_rec(&cabeca, criar_estado(duplicar_string("Acre"), duplicar_string("Rio Branco")));
    inserir_estado_rec(&cabeca, criar_estado(duplicar_string("Sergipe"), duplicar_string("Aracaju")));
    printf("Lista com varios estados:\n");
    resultado = mostrar_todos_estados(cabeca);
    exibir_resultado("Listar lista com varios estados deve retornar 1", resultado == 1);
    
    // Limpar toda a lista
    while (cabeca != NULL) {
        ESTADOS *temp = cabeca;
        cabeca = cabeca->prox;
        limpar_estado(temp);
        free(temp);
    }
}