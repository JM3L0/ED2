#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <limits.h>

#define NUM_DISCO 4
#define NUM_PINO 3
#define NUM_CONFIG (1 << (2 * NUM_DISCO)) // 3^4 = 81
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

    // Verificar se ha disco para mover e se o movimento e valido
    if (disco_origem == -1) return 0; // Nenhum disco no pino de origem
    if (disco_destino == -1) return 1; // Pino de destino vazio
    return tamanho_origem < tamanho_destino; // Disco de origem deve ser menor
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
        // Tentar mover um disco de cada pino para cada outro pino
        for (int origem = 1; origem <= NUM_PINO; origem++) {
            for (int destino = 1; destino <= NUM_PINO; destino++) {
                if (origem == destino) continue;
                if (movimento_valido(config, origem, destino)) {
                    Configuracao nova_config = config;
                    // Mover o menor disco do pino origem para destino
                    for (int d = 0; d < NUM_DISCO; d++) {
                        if (nova_config.disco[d] == origem) {
                            nova_config.disco[d] = destino;
                            break;
                        }
                    }
                    int j = configuracao_para_indice(nova_config);
                    matriz[i][j] = 1;
                }
            }
        }
    }
}

// Imprime a matriz de adjacencia
void imprimir_matriz_adjacencia(int matriz[NUM_CONFIG][NUM_CONFIG]) {
    printf("Matriz de adjacencia:\n");
    for (int i = 0; i < NUM_CONFIG; i++) {
        for (int j = 0; j < NUM_CONFIG; j++) {
            printf("%d ", matriz[i][j]);
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

    for (int contador = 0; contador < NUM_CONFIG; contador++) {
        int minimo = INFINITO, u = -1;
        for (int v = 0; v < NUM_CONFIG; v++) {
            if (!visitado[v] && distancia[v] < minimo) {
                minimo = distancia[v];
                u = v;
            }
        }
        if (u == -1) break;
        visitado[u] = 1;

        for (int v = 0; v < NUM_CONFIG; v++) {
            if (!visitado[v] && matriz[u][v] == 1) {
                if (distancia[u] + 1 < distancia[v]) {
                    distancia[v] = distancia[u] + 1;
                    anterior[v] = u;
                }
            }
        }
    }
}

// Imprime o caminho
void imprimir_caminho(int anterior[], int fim, int inicio) {
    if (fim == inicio) {
        Configuracao config = indice_para_configuracao(inicio);
        printf("Configuracao: (%d, %d, %d, %d)\n", config.disco[0], config.disco[1], config.disco[2], config.disco[3]);
        return;
    }
    if (anterior[fim] == -1) {
        printf("Nenhum caminho encontrado.\n");
        return;
    }
    imprimir_caminho(anterior, anterior[fim], inicio);
    Configuracao config = indice_para_configuracao(fim);
    printf("Configuracao: (%d, %d, %d, %d)\n", config.disco[0], config.disco[1], config.disco[2], config.disco[3]);
}

int main() {
    int matriz[NUM_CONFIG][NUM_CONFIG];
    int distancia[NUM_CONFIG], anterior[NUM_CONFIG];

    // Configuracoes inicial (todos no pino 1) e final (todos no pino 3)
    Configuracao config_inicial = {{1, 1, 1, 1}};
    Configuracao config_final = {{3, 3, 3, 3}};
    int inicio = configuracao_para_indice(config_inicial);
    int fim = configuracao_para_indice(config_final);

    // Medir tempo
    clock_t inicio_tempo = clock();

    // Construir matriz de adjacencia
    construir_matriz_adjacencia(matriz);

    // Executar Dijkstra
    dijkstra(matriz, inicio, distancia, anterior);

    // Medir tempo final
    clock_t fim_tempo = clock();
    double tempo_gasto = (double)(fim_tempo - inicio_tempo) * 1000.0 / CLOCKS_PER_SEC;

    // Imprimir resultados
    printf("Menor numero de movimentos: %d\n", distancia[fim]);
    printf("Caminho:\n");
    imprimir_caminho(anterior, fim, inicio);
    printf("Tempo gasto: %f milissegundos\n", tempo_gasto);

    // Imprimir matriz de adjacencia
    imprimir_matriz_adjacencia(matriz);

    return 0;
}