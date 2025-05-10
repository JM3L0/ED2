/*
cd "c:\Users\raild\Downloads\UFPI\4° período\ED2\Trabalho 2\Arv_VP\zTestes"
gcc -o teste_cidades teste_cidades.c ../Arv_VP_C/Cidades.c ../Arv_VP_C/utilitarios.c -I "../Arv_VP_H"
.\teste_cidades
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../Arv_VP_H/Cidades.h"
#include "../Arv_VP_H/utilitarios.h"
#include "../Arv_VP_H/STRUCTS.h"

// Definicoes para nomes de cores (constantes usadas no arquivo original Cidades.c)
#define RED 1
#define BLACK 0

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
    printf("%-60s %s\n", descricao, sucesso ? "[OK]" : "[FALHA]");
}

// Funcao para contar nos na arvore (auxiliar para testes)
int contar_nos_arvore(CIDADES *raiz) {
    if (raiz == NULL) {
        return 0;
    }
    return 1 + contar_nos_arvore(raiz->esq) + contar_nos_arvore(raiz->dir);
}

// Funcoes de teste
void teste_criacao_cidade();
void teste_insercao_cidade();
void teste_busca_cidade();
void teste_remocao_cidade();
void teste_rotacoes();
void teste_troca_cores();

int main() {
    printf("========== TESTES DE CIDADES (ARVORE RUBRO-NEGRA) ==========\n\n");
    
    printf("--- Teste de Criacao ---\n");
    teste_criacao_cidade();
    
    printf("\n--- Teste de Insercao ---\n");
    teste_insercao_cidade();
    
    printf("\n--- Teste de Busca ---\n");
    teste_busca_cidade();
    
    printf("\n--- Teste de Rotacoes ---\n");
    teste_rotacoes();
    
    printf("\n--- Teste de Troca de Cores ---\n");
    teste_troca_cores();
    
    printf("\n--- Teste de Remocao ---\n");
    teste_remocao_cidade();
    
    printf("\n========== TESTES CONCLUIDOS ==========\n");
    return 0;
}

// Teste de criacao de cidade
void teste_criacao_cidade() {
    printf("CASO 1: Criacao normal de cidade\n");
    
    CIDADES *cidade = criaCidade(duplicar_string("Teresina"), 800000);
    if (cidade != NULL) {
        exibir_resultado("Cidade criada com sucesso", 1);
        exibir_resultado("Nome da cidade correto", strcmp(cidade->nome_cidade, "Teresina") == 0);
        exibir_resultado("Populacao correta", cidade->populacao_city == 800000);
        exibir_resultado("Cor inicializada como VERMELHO", cidade->cor == RED);
        exibir_resultado("Ponteiro CEP inicializado como NULL", cidade->cep == NULL);
        exibir_resultado("Ponteiro esq inicializado como NULL", cidade->esq == NULL);
        exibir_resultado("Ponteiro dir inicializado como NULL", cidade->dir == NULL);
        
        // Limpar memoria
        limpar_no_cidade(cidade);
        free(cidade);
    } else {
        exibir_resultado("Falha ao criar cidade", 0);
    }
    
    printf("\nCASO 2: Verificar valores no limite\n");
    
    // Teste com string vazia
    CIDADES *cidade_vazia = criaCidade(duplicar_string(""), 0);
    exibir_resultado("Criar cidade com nome vazio", cidade_vazia != NULL);
    if (cidade_vazia != NULL) {
        limpar_no_cidade(cidade_vazia);
        free(cidade_vazia);
    }
    
    // Teste com populacao grande
    CIDADES *cidade_grande = criaCidade(duplicar_string("Cidade Grande"), 2147483647);
    exibir_resultado("Criar cidade com populacao maxima (INT_MAX)", cidade_grande != NULL);
    if (cidade_grande != NULL) {
        limpar_no_cidade(cidade_grande);
        free(cidade_grande);
    }
    
    // Teste com nome muito grande
    char *nome_grande = (char*)malloc(1000);
    if (nome_grande) {
        memset(nome_grande, 'X', 999);
        nome_grande[999] = '\0';
        
        CIDADES *cidade_nome_grande = criaCidade(nome_grande, 123456);
        exibir_resultado("Criar cidade com nome muito grande (1000 chars)", cidade_nome_grande != NULL);
        if (cidade_nome_grande != NULL) {
            limpar_no_cidade(cidade_nome_grande);
            free(cidade_nome_grande);
        }
    } else {
        free(nome_grande);
        exibir_resultado("Criar cidade com nome muito grande", 0);
    }
}

// Teste de insercao de cidade
void teste_insercao_cidade() {
    CIDADES *raiz = NULL;
    
    printf("CASO 1: Inserir em arvore vazia\n");
    CIDADES *cidade1 = criaCidade(duplicar_string("Teresina"), 800000);
    CIDADES *inserido = inserir_Cidade(&raiz, cidade1);
    exibir_resultado("Inserir cidade em arvore vazia", inserido != NULL);
    exibir_resultado("Raiz possui cor PRETA apos insercao", raiz != NULL && raiz->cor == BLACK);
    exibir_resultado("Contagem de nos correta (1)", contar_nos_arvore(raiz) == 1);
    
    printf("\nCASO 2: Inserir cidade a esquerda (ordem alfabetica)\n");
    CIDADES *cidade2 = criaCidade(duplicar_string("Belem"), 1400000);
    inserido = inserir_Cidade(&raiz, cidade2);
    exibir_resultado("Inserir cidade a esquerda", inserido != NULL);
    exibir_resultado("Contagem de nos correta (2)", contar_nos_arvore(raiz) == 2);
    
    printf("\nCASO 3: Inserir cidade a direita (ordem alfabetica)\n");
    CIDADES *cidade3 = criaCidade(duplicar_string("Vitoria"), 350000);
    inserido = inserir_Cidade(&raiz, cidade3);
    exibir_resultado("Inserir cidade a direita", inserido != NULL);
    exibir_resultado("Contagem de nos correta (3)", contar_nos_arvore(raiz) == 3);
    
    printf("\nCASO 4: Inserir mais cidades para testar balanceamento\n");
    CIDADES *cidade4 = criaCidade(duplicar_string("Curitiba"), 1800000);
    inserido = inserir_Cidade(&raiz, cidade4);
    CIDADES *cidade5 = criaCidade(duplicar_string("Manaus"), 2100000);
    inserido = inserir_Cidade(&raiz, cidade5);
    CIDADES *cidade6 = criaCidade(duplicar_string("Fortaleza"), 2600000);
    inserido = inserir_Cidade(&raiz, cidade6);
    CIDADES *cidade7 = criaCidade(duplicar_string("Macapa"), 450000);
    inserido = inserir_Cidade(&raiz, cidade7);
    
    exibir_resultado("Inserir multiplas cidades para teste de balanceamento", 
                    contar_nos_arvore(raiz) == 7);
    
    printf("\nResultado da arvore apos insercoes:\n");
    imprimir_todas_cidades(raiz);
    
    // Limpar arvore
    desalocar_arvore_cidades(&raiz);
    exibir_resultado("Desalocar arvore completa", raiz == NULL);
}

// Teste de busca de cidade
void teste_busca_cidade() {
    CIDADES *raiz = NULL;
    
    // Criar arvore para testes
    inserir_Cidade(&raiz, criaCidade(duplicar_string("Belem"), 1400000));
    inserir_Cidade(&raiz, criaCidade(duplicar_string("Curitiba"), 1800000));
    inserir_Cidade(&raiz, criaCidade(duplicar_string("Fortaleza"), 2600000));
    inserir_Cidade(&raiz, criaCidade(duplicar_string("Macapa"), 450000));
    inserir_Cidade(&raiz, criaCidade(duplicar_string("Manaus"), 2100000));
    inserir_Cidade(&raiz, criaCidade(duplicar_string("Teresina"), 800000));
    inserir_Cidade(&raiz, criaCidade(duplicar_string("Vitoria"), 350000));
    
    printf("Arvore para testes criada com %d cidades\n", contar_nos_arvore(raiz));
    
    printf("\nCASO 1: Buscar cidade existente (raiz)\n");
    CIDADES *resultado = buscar_cidade(raiz, "Teresina");
    exibir_resultado("Buscar cidade na raiz", resultado != NULL && strcmp(resultado->nome_cidade, "Teresina") == 0);
    
    printf("\nCASO 2: Buscar cidade existente (folha)\n");
    resultado = buscar_cidade(raiz, "Vitoria");
    exibir_resultado("Buscar cidade folha", resultado != NULL && strcmp(resultado->nome_cidade, "Vitoria") == 0);
    
    printf("\nCASO 3: Buscar cidade existente (no interno)\n");
    resultado = buscar_cidade(raiz, "Fortaleza");
    exibir_resultado("Buscar cidade no interno", resultado != NULL && strcmp(resultado->nome_cidade, "Fortaleza") == 0);
    
    printf("\nCASO 4: Busca case-insensitive\n");
    resultado = buscar_cidade(raiz, "MANAUS");
    exibir_resultado("Busca case-insensitive", resultado != NULL && strcasecmp(resultado->nome_cidade, "MANAUS") == 0);
    
    printf("\nCASO 5: Buscar cidade inexistente\n");
    resultado = buscar_cidade(raiz, "Salvador");
    exibir_resultado("Buscar cidade inexistente retorna NULL", resultado == NULL);
    
    printf("\nCASO 6: Buscar em arvore vazia\n");
    CIDADES *raiz_vazia = NULL;
    resultado = buscar_cidade(raiz_vazia, "Qualquer");
    exibir_resultado("Buscar em arvore vazia retorna NULL", resultado == NULL);
    
    // Limpar arvore
    desalocar_arvore_cidades(&raiz);
}

// Teste de rotacoes
void teste_rotacoes() {
    CIDADES *raiz = NULL;
    
    printf("CASO 1: Testar rotacao a esquerda\n");
    
    // Criar uma pequena arvore para teste de rotacao
    raiz = criaCidade(duplicar_string("M"), 1000);
    raiz->cor = BLACK;
    raiz->dir = criaCidade(duplicar_string("Z"), 2000);
    raiz->dir->cor = RED;
    
    printf("Arvore antes da rotacao:\n");
    printf("  M (BLACK)\n");
    printf("   \\\n");
    printf("    Z (RED)\n");
    
    // Realizar rotacao a esquerda
    rotacao_esquerda(&raiz);
    
    exibir_resultado("Rotacao a esquerda realizada", 
                     raiz != NULL && 
                     strcmp(raiz->nome_cidade, "Z") == 0 && 
                     raiz->esq != NULL && 
                     strcmp(raiz->esq->nome_cidade, "M") == 0);
    
    printf("Arvore apos rotacao a esquerda:\n");
    printf("  Z (RED)\n");
    printf(" /\n");
    printf("M (RED)\n");
    
    // Limpar esta arvore
    desalocar_arvore_cidades(&raiz);
    
    printf("\nCASO 2: Testar rotacao a direita\n");
    
    // Criar uma pequena arvore para teste de rotacao
    raiz = criaCidade(duplicar_string("M"), 1000);
    raiz->cor = BLACK;
    raiz->esq = criaCidade(duplicar_string("A"), 500);
    raiz->esq->cor = RED;
    
    printf("Arvore antes da rotacao:\n");
    printf("    M (BLACK)\n");
    printf("   /\n");
    printf("  A (RED)\n");
    
    // Realizar rotacao a direita
    rotacao_direita(&raiz);
    
    exibir_resultado("Rotacao a direita realizada", 
                     raiz != NULL && 
                     strcmp(raiz->nome_cidade, "A") == 0 && 
                     raiz->dir != NULL && 
                     strcmp(raiz->dir->nome_cidade, "M") == 0);
    
    printf("Arvore apos rotacao a direita:\n");
    printf("  A (RED)\n");
    printf("   \\\n");
    printf("    M (RED)\n");
    
    // Limpar esta arvore
    desalocar_arvore_cidades(&raiz);
}

// Teste de troca de cores
void teste_troca_cores() {
    printf("CASO 1: Testar troca de cores em um no com filhos\n");
    
    // Criar uma pequena arvore para teste de troca de cores
    CIDADES *raiz = criaCidade(duplicar_string("M"), 1000);
    raiz->cor = BLACK;
    raiz->esq = criaCidade(duplicar_string("A"), 500);
    raiz->esq->cor = RED;
    raiz->dir = criaCidade(duplicar_string("Z"), 2000);
    raiz->dir->cor = RED;
    
    printf("Cores antes da troca:\n");
    printf("  M (BLACK)\n");
    printf(" / \\\n");
    printf("A   Z (RED)\n");
    printf("(RED)\n");
    
    // Realizar troca de cores
    trocar_cor(raiz);
    
    exibir_resultado("Troca de cores realizada", 
                     raiz->cor == RED && 
                     raiz->esq->cor == BLACK && 
                     raiz->dir->cor == BLACK);
    
    printf("Cores apos a troca:\n");
    printf("  M (RED)\n");
    printf(" / \\\n");
    printf("A   Z (BLACK)\n");
    printf("(BLACK)\n");
    
    // Limpar esta arvore
    desalocar_arvore_cidades(&raiz);
}

// Teste de remocao de cidade
void teste_remocao_cidade() {
    CIDADES *raiz = NULL;
    int resultado;
    
    // Criar arvore para testes
    inserir_Cidade(&raiz, criaCidade(duplicar_string("Belem"), 1400000));
    inserir_Cidade(&raiz, criaCidade(duplicar_string("Curitiba"), 1800000));
    inserir_Cidade(&raiz, criaCidade(duplicar_string("Fortaleza"), 2600000));
    inserir_Cidade(&raiz, criaCidade(duplicar_string("Macapa"), 450000));
    inserir_Cidade(&raiz, criaCidade(duplicar_string("Manaus"), 2100000));
    inserir_Cidade(&raiz, criaCidade(duplicar_string("Teresina"), 800000));
    inserir_Cidade(&raiz, criaCidade(duplicar_string("Vitoria"), 350000));
    
    int contagem_inicial = contar_nos_arvore(raiz);
    printf("Arvore para testes criada com %d cidades\n", contagem_inicial);
    
    printf("\nCASO 1: Remover cidade folha\n");
    resultado = remover_cidade_arvore(&raiz, "Vitoria");
    exibir_resultado("Remover cidade folha", resultado == 1);
    exibir_resultado("Contagem de nos apos remocao", contar_nos_arvore(raiz) == contagem_inicial - 1);
    
    printf("\nCASO 2: Remover cidade com um filho\n");
    // Inserir uma cidade que garantidamente terá apenas um filho
    inserir_Cidade(&raiz, criaCidade(duplicar_string("Aracaju"), 650000));
    int contagem_atual = contar_nos_arvore(raiz);
    
    // Tentamos remover uma cidade que provavelmente tem um filho
    resultado = remover_cidade_arvore(&raiz, "Belem");
    exibir_resultado("Remover cidade com um filho", resultado == 1);
    exibir_resultado("Contagem de nos apos remocao", contar_nos_arvore(raiz) == contagem_atual - 1);
    
    printf("\nCASO 3: Remover cidade com dois filhos\n");
    resultado = remover_cidade_arvore(&raiz, "Teresina");
    exibir_resultado("Remover cidade com dois filhos", resultado == 1);
    
    printf("\nCASO 4: Remover cidade inexistente\n");
    resultado = remover_cidade_arvore(&raiz, "Salvador");
    exibir_resultado("Remover cidade inexistente retorna 0", resultado == 0);
    
    printf("\nCASO 5: Remover todas as cidades ate arvore ficar vazia\n");
    while (raiz != NULL) {
        CIDADES *cidade = raiz; // Pegar a raiz atual
        resultado = remover_cidade_arvore(&raiz, cidade->nome_cidade);
        exibir_resultado("Remover cidade da arvore", resultado == 1);
    }
    
    exibir_resultado("Arvore vazia apos remover todas as cidades", raiz == NULL);
    
    printf("\nCASO 6: Remover de arvore vazia\n");
    resultado = remover_cidade_arvore(&raiz, "Qualquer");
    exibir_resultado("Remover de arvore vazia retorna 0", resultado == 0);
}