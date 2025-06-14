#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <limits.h>

#ifdef _WIN32
#include <windows.h>
#else
#include <time.h>
#endif

#define NUM_DISCO 4
#define NUM_PINO 3
#define NUM_CONFIG 81 // 3^4 = 81
#define INFINITO INT_MAX

// Estrutura para representar uma configuracao
typedef struct {
    int disco[NUM_DISCO];
} Configuracao;

// Funcao para converter configuracao em indice (base 3)
int configuracao_para_indice(Configuracao config) {
    int indice = 0;
    for (int i = 0; i < NUM_DISCO; i++) {
        indice = indice * NUM_PINO + (config.disco[i] - 1);
    }
    return indice;
}

// Funcao para converter indice em configuracao
Configuracao indice_para_configuracao(int indice) {
    Configuracao config;
    for (int i = NUM_DISCO - 1; i >= 0; i--) {
        config.disco[i] = (indice % NUM_PINO) + 1;
        indice /= NUM_PINO;
    }
    return config;
}

// Verifica se o movimento de pino_origem para pino_destino e valido na configuracao
int movimento_valido(Configuracao config, int pino_origem, int pino_destino) {
    int resultado = 0;
    int disco_origem = -1, disco_destino = -1;
    int tamanho_origem = NUM_DISCO + 1, tamanho_destino = NUM_DISCO + 1;

    // Encontrar o menor disco no pino de origem e destino
    for (int i = 0; i < NUM_DISCO; i++) {
        if (config.disco[i] == pino_origem && (disco_origem == -1 || i < disco_origem)) {
            disco_origem = i;
            tamanho_origem = i + 1;
        }
        if (config.disco[i] == pino_destino && (disco_destino == -1 || i < disco_destino)) {
            disco_destino = i;
            tamanho_destino = i + 1;
        }
    }

    // Verificar se o movimento e valido
    if (disco_origem != -1) {
        if (disco_destino == -1 || tamanho_origem < tamanho_destino) {
            resultado = 1;
        }
    }
    return resultado;
}

// Gera a matriz de adjacencia
void construir_matriz_adjacencia(int matriz[NUM_CONFIG][NUM_CONFIG]) {
    for (int i = 0; i < NUM_CONFIG; i++) {
        for (int j = 0; j < NUM_CONFIG; j++) {
            matriz[i][j] = 0;
        }
    }

    for (int i = 0; i < NUM_CONFIG; i++) {
        Configuracao config = indice_para_configuracao(i);
        for (int origem = 1; origem <= NUM_PINO; origem++) {
            for (int destino = 1; destino <= NUM_PINO; destino++) {
                if (origem != destino && movimento_valido(config, origem, destino)) {
                    Configuracao nova_config = config;
                    for (int d = 0; d < NUM_DISCO && nova_config.disco[d] != origem; d++) {
                        nova_config.disco[d] = nova_config.disco[d];
                    }
                    for (int d = 0; d < NUM_DISCO; d++) {
                        if (nova_config.disco[d] == origem) {
                            nova_config.disco[d] = destino;
                            d = NUM_DISCO; // Para sair do loop sem break
                        }
                    }
                    int j = configuracao_para_indice(nova_config);
                    matriz[i][j] = 1;
                }
            }
        }
    }
}

// Imprime a matriz de adjacencia completa
void imprimir_matriz_adjacencia(int matriz[NUM_CONFIG][NUM_CONFIG]) {
    printf("\n=== Matriz de Adjacencia ===\n");
    // Imprimir cabecalho das colunas
    printf("   ");
    for (int j = 0; j < NUM_CONFIG; j++) {
        printf("%2d ", j);
    }
    printf("\n");
    printf("   ");
    for (int j = 0; j < NUM_CONFIG; j++) {
        printf("---");
    }
    printf("\n");

    // Imprimir linhas da matriz
    for (int i = 0; i < NUM_CONFIG; i++) {
        printf("%2d|", i);
        for (int j = 0; j < NUM_CONFIG; j++) {
            printf("%2d ", matriz[i][j]);
        }
        printf("\n");
    }
}

// Algoritmo de Dijkstra
void dijkstra(int matriz[NUM_CONFIG][NUM_CONFIG], int inicio, int distancia[], int anterior[]) {
    int visitado[NUM_CONFIG] = {0};
    for (int i = 0; i < NUM_CONFIG; i++) {
        distancia[i] = INFINITO;
        anterior[i] = -1;
    }
    distancia[inicio] = 0;

    int contador = 0;
    int encontrou_minimo = 1;
    while (contador < NUM_CONFIG && encontrou_minimo) {
        int minimo = INFINITO;
        int u = -1;
        for (int v = 0; v < NUM_CONFIG; v++) {
            if (!visitado[v] && distancia[v] < minimo) {
                minimo = distancia[v];
                u = v;
            }
        }
        encontrou_minimo = (u != -1);
        if (encontrou_minimo) {
            visitado[u] = 1;
            for (int v = 0; v < NUM_CONFIG; v++) {
                if (!visitado[v] && matriz[u][v] == 1 && distancia[u] + 1 < distancia[v]) {
                    distancia[v] = distancia[u] + 1;
                    anterior[v] = u;
                }
            }
        }
        contador++;
    }
}

// Imprime o caminho (versao iterativa)
void imprimir_caminho(int anterior[], int fim, int inicio) {
    int caminho[NUM_CONFIG];
    int tamanho = 0;
    int atual = fim;

    while (atual != -1 && tamanho < NUM_CONFIG) {
        caminho[tamanho] = atual;
        tamanho++;
        atual = anterior[atual];
    }

    if (atual == -1 && caminho[tamanho - 1] != inicio) {
        printf("Nenhum caminho encontrado.\n");
    } else {
        for (int i = tamanho - 1; i >= 0; i--) {
            Configuracao config = indice_para_configuracao(caminho[i]);
            printf("Configuracao: (%d, %d, %d, %d)\n", config.disco[0], config.disco[1], config.disco[2], config.disco[3]);
        }
    }
}

// Funcao para obter o tempo atual em nanosegundos
double obter_tempo_nanosegundos() {
    double tempo = 0.0;
#ifdef _WIN32
    LARGE_INTEGER freq, counter;
    QueryPerformanceFrequency(&freq);
    QueryPerformanceCounter(&counter);
    tempo = (double)(counter.QuadPart * 1e9 / freq.QuadPart);
#else
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    tempo = (double)(ts.tv_sec * 1e9 + ts.tv_nsec);
#endif
    return tempo;
}

int main() {
    int matriz[NUM_CONFIG][NUM_CONFIG];
    int distancia[NUM_CONFIG], anterior[NUM_CONFIG];

    // Configuracoes inicial (todos no pino 1) e final (todos no pino 3)
    Configuracao config_inicial = {{1, 1, 1, 1}};
    Configuracao config_final = {{3, 3, 3, 3}};
    int inicio = configuracao_para_indice(config_inicial);
    int fim = configuracao_para_indice(config_final);

    // Medir tempo inicial
    double inicio_tempo = obter_tempo_nanosegundos();

    // Construir matriz de adjacencia
    construir_matriz_adjacencia(matriz);

    // Executar Dijkstra
    dijkstra(matriz, inicio, distancia, anterior);

    // Medir tempo final
    double fim_tempo = obter_tempo_nanosegundos();
    double tempo_gasto = fim_tempo - inicio_tempo;

    // Imprimir resultados
    printf("Menor numero de movimentos: %d\n", distancia[fim]);
    printf("Caminho:\n");
    imprimir_caminho(anterior, fim, inicio);
    printf("Tempo de execucao: %.0f nanosegundos\n", tempo_gasto);

    // Imprimir matriz de adjacencia
    imprimir_matriz_adjacencia(matriz);

    return 0;
}