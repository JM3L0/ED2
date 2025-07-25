#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define NUM_DISCOS 4  // Número de discos
#define NUM_PINOS 3
#define MAX 81 // NUM_PINOS ^ NUM_DISCOS
#define INF 999999999
typedef struct {
    int conf[NUM_DISCOS]; // Configuração dos discos nos pinos
} NoGrafo; 

int adj[MAX][MAX]; // Matriz de adjacência

//-----------------------------------------------
// Gera todas as configurações possíveis dos discos
void gerarConfiguracoes(NoGrafo *grafo) {
    for (int i = 0; i < MAX; ++i) {
        int numero_base = i;
        for (int j = 0; j < NUM_DISCOS; ++j) {
            grafo[i].conf[j] = numero_base % NUM_PINOS + 1;
            numero_base /= NUM_PINOS;
        }
    }
}
//-----------------------------------------------
// Verifica se há uma aresta válida entre duas configurações
int ehAresta(NoGrafo config1, NoGrafo config2) {
    int diferenca_discos = 0;
    int disco_alterado = -1;
    int valida = 1; // Assume que a aresta é válida por padrão

    for (int i = 0; i < NUM_DISCOS; ++i) {
        if (config1.conf[i] != config2.conf[i]) {
            diferenca_discos++;
            disco_alterado = i;
        }
    }

    valida = (diferenca_discos == 1);

    if (valida) {
        for (int i = 0; i < NUM_DISCOS; ++i) {
            if (i != disco_alterado && i < disco_alterado) { 
                if (config1.conf[i] == config1.conf[disco_alterado] || config2.conf[i] == config2.conf[disco_alterado])
                    valida = 0; 
            }
        }
    }
    
    return valida; 
}
//-----------------------------------------------
// Preenche a matriz de adjacência do grafo
void gerarAdjacencias(NoGrafo *grafo) {
    for (int i = 0; i < MAX; ++i) {
        for (int j = 0; j < MAX; ++j)
            adj[i][j] = ehAresta(grafo[i], grafo[j]);
    }
}
//-----------------------------------------------

//Imprime o caminho mínimo encontrado
void imprimirCaminho(int *pesos, int origem, int destino) {
    printf("\nCaminho minimo entre configuracoes %d e %d:\n", origem, destino);

    int caminho[destino + 1], temp = pesos[destino], pos = 0;

    while(temp != -1)
        temp = pesos[caminho[pos++] = temp];

    for (int i = pos - 1; ~i; i--)
        printf("%d ", caminho[i]);

    printf("%d\n", destino);

    printf("Quantidade de movimentos: %d\n", pos);
}

//-----------------------------------------------
// Implementação do algoritmo de Ford-Moore-Bellman
int* fordMooreBellman(int origem) {
    //dis_p = vetor de distâncias dos vértices
    //pesos = vetor de predecessores
    //adj = matriz de adjacência
    static int dis_p[MAX], pesos[MAX];

    memset(pesos, -1, sizeof(pesos));

    for (int i = 0; i < MAX; i++) dis_p[i] = INF;
    dis_p[origem] = 0;
    for (int i = 0; i < MAX - 1; ++i) {
        for (int j = 0; j < MAX; ++j) {
            if (dis_p[j] != INF) {
                for (int k = 0; k < MAX; k++) {
                    if (adj[j][k] == 1 && dis_p[j] + adj[j][k] < dis_p[k]) {
                        dis_p[k] = dis_p[j] + adj[j][k];
                        pesos[k] = j;
                    }
                }
            }
        }
    }
    return pesos; // Retorna o vetor de predecessores
}

//-----------------------------------------------
// Implementação do algoritmo de Dijkstra
int* dijkstra(int origem) {
    static int dis_p[MAX], pesos[MAX];
    int visitado[MAX] = {0};

    memset(pesos, -1, sizeof(pesos));
    for (int i = 0; i < MAX; i++) dis_p[i] = INF;
    dis_p[origem] = 0;

    for (int i = 0; i < MAX; ++i) {
        int u = -1, min_dist = INF;
        for (int j = 0; j < MAX; ++j) {
            if (!visitado[j] && dis_p[j] < min_dist) {
                min_dist = dis_p[j];
                u = j;
            }
        }

        visitado[u] = 1;

        for (int v = 0; v < MAX; ++v) {
            if (adj[u][v] == 1 && dis_p[u] + adj[u][v] < dis_p[v]) {
                dis_p[v] = dis_p[u] + adj[u][v];
                pesos[v] = u;
            }
        }
    }
    return pesos; // Retorna o vetor de predecessores
}

//-----------------------------------------------
// Imprime todas as configurações dos vértices do grafo
void imprimirGrafo(NoGrafo *grafo) {
    for (int i = 0; i < MAX; ++i) {
        printf("Configuracao %d: ", i);
        for (int j = 0; j < NUM_DISCOS; ++j) {
            printf("%d ", grafo[i].conf[j]);
        }
        printf("\n");
    }
}
//-----------------------------------------------

// Imprime a matriz de adjacência
void imprimirMatrizAdjacencia() {
    printf("\nMatriz de Adjacencia:\n");
    printf("   ");
    for (int i = 0; i < MAX; i++)
        printf("%2d ", i);

    printf("\n");
    for (int i = 0; i < MAX; i++) {
        printf("%2d ", i);
        for (int j = 0; j < MAX; j++)
            printf("%2d ", adj[i][j]);
        printf("\n");
    }
}
//-----------------------------------------------

void testes_tempo(int origem);

int main() {
    NoGrafo grafo[MAX];

    gerarConfiguracoes(grafo);
    gerarAdjacencias(grafo);

    int opcao;
    int origem = 0; // Configuração de origem
    int destino = MAX - 1;

    do {
        printf("\n\n==== MENU ====\n");
        printf("1. Imprimir configuracoes do grafo\n");
        printf("2. Imprimir matriz de adjacencia\n");
        printf("3. Encontrar caminho minimo com Dijkstra\n");
        printf("4. Encontrar caminho minimo com Ford-Moore-Bellman\n");
        printf("5. Testes de tempo\n");
        printf("0. Sair\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1:
                imprimirGrafo(grafo);
                break;
            case 2:
                imprimirMatrizAdjacencia();
                break;
            case 3:{
                int *pesos;

                printf("Encontrando caminho minimo com Dijkstra...\n");

                clock_t inicio = clock();  // Marca o tempo de início
                pesos = dijkstra(origem);

                clock_t fim = clock();     // Marca o tempo de fim

                imprimirCaminho(pesos, origem, destino);
                double tempo_execucao = ((double)(fim - inicio)) / CLOCKS_PER_SEC * 1000; // Tempo em milissegundos
                printf("Tempo de execucao (Dijkstra): %.6f ms\n", tempo_execucao);
                break;
            }
            case 4:{
                int *pesos;
                
                printf("Encontrando caminho minimo com Ford-Moore-Bellman...\n");
                
                clock_t inicio = clock();  // Marca o tempo de início
                pesos = fordMooreBellman(origem);
                clock_t fim = clock();     // Marca o tempo de fim
                
                imprimirCaminho(pesos, origem, destino);
                
                double tempo_execucao = ((double)(fim - inicio)) / CLOCKS_PER_SEC * 1000; // Tempo em milissegundos
                printf("Tempo de execucao (Ford-Moore-Bellman): %.6f ms\n", tempo_execucao);
                break;
            }
            case 5:{
                testes_tempo(origem);
                break;
            }
            case 0:
                printf("Saindo...\n");
                break;
            default:
                printf("Opcao invalida!\n");
        }
    } while (opcao != 0);

    return 0;
}

void testes_tempo(int origem) {
    printf("\n==== TESTES DE TEMPO ====\n");
    int num_testes = 1000;
    double tempo_dijkstra = 0.0, tempo_ford_moore = 0.0;
    printf("Realizando %d testes...\n", num_testes);

    int i;
    for (i = 0; i < num_testes; i++) {
        clock_t inicio = clock();
        dijkstra(origem);
        clock_t fim = clock();
        tempo_dijkstra += ((double)(fim - inicio)) / CLOCKS_PER_SEC * 1000;


        inicio = clock();
        fordMooreBellman(origem);
        fim = clock();
        tempo_ford_moore += ((double)(fim - inicio)) / CLOCKS_PER_SEC * 1000;

    }

    printf("\nTempo total Dijkstra: %.6f ms\n", tempo_dijkstra);
    printf("Tempo medio Dijkstra: %.6f ms\n", tempo_dijkstra / num_testes);
    printf("\nTempo total Ford-Moore-Bellman: %.6f ms\n", tempo_ford_moore);
    printf("Tempo medio Ford-Moore-Bellman: %.6f ms\n", tempo_ford_moore / num_testes);
    printf("\n==== FIM DOS TESTES ====\n");
}