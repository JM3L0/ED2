#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#define NUM_DISCOS 4
#define NUM_PINOS 3

// Estrutura para representar um estado da Torre de Hanói
typedef struct {
    int pinos[NUM_DISCOS]; // pinos[i] = pino onde o disco i está (0, 1 ou 2)
} EstadoHanoi;

// Função para imprimir um estado
void imprimirEstado(EstadoHanoi estado) {
    printf("(");
    for (int i = 0; i < NUM_DISCOS; i++) {
        printf("%d", estado.pinos[i]);
        if (i < NUM_DISCOS - 1) {
            printf(",");
        }
    }
    printf(")\n");
}

// Função para verificar se um movimento é válido
int movimentoValido(EstadoHanoi estado, int origem, int destino) {
    int discoOrigem = -1;
    int discoDestino = -1;

    // Encontrar o menor disco no pino de origem
    for (int i = 0; i < NUM_DISCOS; i++) {
        if (estado.pinos[i] == origem) {
            if (discoOrigem == -1 || i < discoOrigem) {
                discoOrigem = i;
            }
        }
    }

    // Encontrar o menor disco no pino de destino
    for (int i = 0; i < NUM_DISCOS; i++) {
        if (estado.pinos[i] == destino) {
            if (discoDestino == -1 || i < discoDestino) {
                discoDestino = i;
            }
        }
    }

    // Se não há disco na origem, o movimento é inválido
    if (discoOrigem == -1) {
        return 0;
    }

    // Se o pino de destino está vazio ou o disco de origem é menor que o disco de destino
    if (discoDestino == -1 || discoOrigem < discoDestino) {
        return 1;
    }

    return 0;
}

// Função para aplicar um movimento e retornar o novo estado
EstadoHanoi aplicarMovimento(EstadoHanoi estado, int origem, int destino) {
    int discoOrigem = -1;

    for (int i = 0; i < NUM_DISCOS; i++) {
        if (estado.pinos[i] == origem) {
            if (discoOrigem == -1 || i < discoOrigem) {
                discoOrigem = i;
            }
        }
    }
    estado.pinos[discoOrigem] = destino;
    return estado;
}

// Função para comparar dois estados
int compararEstados(EstadoHanoi estado1, EstadoHanoi estado2) {
    for (int i = 0; i < NUM_DISCOS; i++) {
        if (estado1.pinos[i] != estado2.pinos[i]) {
            return 0;
        }
    }
    return 1;
}

// Função para encontrar o índice de um estado na lista de estados
int encontrarIndiceEstado(EstadoHanoi *estados, int numEstados, EstadoHanoi estado) {
    for (int i = 0; i < numEstados; i++) {
        if (compararEstados(estados[i], estado)) {
            return i;
        }
    }
    return -1;
}

// Função para gerar todos os estados possíveis
void gerarEstados(EstadoHanoi *estados, int *numEstados, EstadoHanoi estadoAtual, int disco) {
    if (disco == NUM_DISCOS) {
        if (encontrarIndiceEstado(estados, *numEstados, estadoAtual) == -1) {
            estados[*numEstados] = estadoAtual;
            (*numEstados)++;
        }
        return;
    }

    for (int pino = 0; pino < NUM_PINOS; pino++) {
        estadoAtual.pinos[disco] = pino;
        gerarEstados(estados, numEstados, estadoAtual, disco + 1);
    }
}

int main() {
    EstadoHanoi *estados = (EstadoHanoi *)malloc(sizeof(EstadoHanoi) * 1000); // Alocação inicial
    int numEstados = 0;

    EstadoHanoi estadoInicial;
    for (int i = 0; i < NUM_DISCOS; i++) {
        estadoInicial.pinos[i] = 0; // Todos os discos no pino 0 inicialmente
    }

    gerarEstados(estados, &numEstados, estadoInicial, 0);

    printf("Número total de estados: %d\n", numEstados);

    // Construir a matriz de adjacência
    int **matrizAdjacencia = (int **)malloc(sizeof(int *) * numEstados);
    for (int i = 0; i < numEstados; i++) {
        matrizAdjacencia[i] = (int *)malloc(sizeof(int) * numEstados);
        for (int j = 0; j < numEstados; j++) {
            matrizAdjacencia[i][j] = 0; // Inicializa com 0 (sem conexão)
        }
    }

    for (int i = 0; i < numEstados; i++) {
        for (int origem = 0; origem < NUM_PINOS; origem++) {
            for (int destino = 0; destino < NUM_PINOS; destino++) {
                if (origem != destino) {
                    if (movimentoValido(estados[i], origem, destino)) {
                        EstadoHanoi proximoEstado = aplicarMovimento(estados[i], origem, destino);
                        int indiceProximoEstado = encontrarIndiceEstado(estados, numEstados, proximoEstado);
                        if (indiceProximoEstado != -1) {
                            matrizAdjacencia[i][indiceProximoEstado] = 1; // Conexão com peso 1
                        }
                    }
                }
            }
        }
    }

    printf("Matriz de Adjacência:\n");
    for (int i = 0; i < numEstados; i++) {
        for (int j = 0; j < numEstados; j++) {
            printf("%d ", matrizAdjacencia[i][j]);
        }
        printf("\n");
    }

    // Algoritmo de Dijkstra
    int *distancia = (int *)malloc(sizeof(int) * numEstados);
    int *visitado = (int *)malloc(sizeof(int) * numEstados);
    int *anterior = (int *)malloc(sizeof(int) * numEstados);

    // Definir estado inicial e final (exemplo: todos os discos no pino 0 para todos no pino 2)
    EstadoHanoi estadoFinal;
    for (int i = 0; i < NUM_DISCOS; i++) {
        estadoFinal.pinos[i] = 2;
    }

    int inicio = encontrarIndiceEstado(estados, numEstados, estadoInicial);
    int fim = encontrarIndiceEstado(estados, numEstados, estadoFinal);

    if (inicio == -1 || fim == -1) {
        printf("Estado inicial ou final não encontrado.\n");
        return 1;
    }

    for (int i = 0; i < numEstados; i++) {
        distancia[i] = INT_MAX;
        visitado[i] = 0;
        anterior[i] = -1;
    }

    distancia[inicio] = 0;

    for (int count = 0; count < numEstados - 1; count++) {
        int u = -1;
        int minDist = INT_MAX;

        for (int v = 0; v < numEstados; v++) {
            if (!visitado[v] && distancia[v] < minDist) {
                minDist = distancia[v];
                u = v;
            }
        }

        if (u == -1) break; // Todos os nós alcançáveis foram visitados

        visitado[u] = 1;

        for (int v = 0; v < numEstados; v++) {
            if (!visitado[v] && matrizAdjacencia[u][v] && distancia[u] != INT_MAX && distancia[u] + matrizAdjacencia[u][v] < distancia[v]) {
                distancia[v] = distancia[u] + matrizAdjacencia[u][v];
                anterior[v] = u;
            }
        }
    }

    printf("\nMenor caminho do estado inicial para o estado final:\n");
    if (distancia[fim] == INT_MAX) {
        printf("Não há caminho do estado inicial para o estado final.\n");
    } else {
        printf("Distância mínima: %d\n", distancia[fim]);
        printf("Caminho: ");
        int caminho[numEstados];
        int k = 0;
        int atual = fim;
        while (atual != -1) {
            caminho[k++] = atual;
            atual = anterior[atual];
        }
        for (int i = k - 1; i >= 0; i--) {
            imprimirEstado(estados[caminho[i]]);
            if (i > 0) {
                printf(" -> ");
            }
        }
        printf("\n");
    }

    // Liberar memória
    for (int i = 0; i < numEstados; i++) {
        free(matrizAdjacencia[i]);
    }
    free(matrizAdjacencia);
    free(estados);
    free(distancia);
    free(visitado);
    free(anterior);

    return 0;
}


