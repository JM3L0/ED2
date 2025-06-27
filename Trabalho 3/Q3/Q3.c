#include <stdio.h>
#include <stdlib.h>
#include "../Q3/Util.h"
#include <limits.h>

#define INFINITO -1
#define INVALIDO -1

typedef struct no
{
    int indice;
    float valor;
    int marcado;
} No;

No *alocar_no(int tam)
{
    No *vetor;

    vetor = (No *) malloc(sizeof(No) * tam);

    if(!vetor)
    {
        printf("Erro ao alocar vetor de nos Dijkstra");
        exit(EXIT_FAILURE);
    }

    for(int i = 0; i < tam; i++)
    {
        vetor[i].indice = INVALIDO;
        vetor[i].valor = INFINITO;
        vetor[i].marcado = 0;
    }

    return vetor;
}

int exibir_caminho(int pos_atual, No *vetor_nos, int pos_inicial)
{
    int retorno = 1;
    if(pos_atual != INVALIDO)
    {
        if(pos_atual != pos_inicial)
            retorno = exibir_caminho(vetor_nos[pos_atual].indice, vetor_nos, pos_inicial);

        if(retorno)
            printf("[%d] -> ", pos_atual);
    }
    else
        retorno = 0;

    return retorno;
}

int buscar_maior_no(No *vetor_nos, int n_vertices)
{
    float maior = INFINITO;
    int pos_maior = INVALIDO;

    for(int i = 0; i < n_vertices; i++)
    {
        if(!vetor_nos[i].marcado && vetor_nos[i].valor > maior)
        {
            maior = vetor_nos[i].valor;
            pos_maior = i;
        }
    }

    return pos_maior;
}

No *dijkstra(int pos_atual, int pos_final, int n_vertices, float matriz[][n_vertices])
{
    No *vetor_nos;
    vetor_nos = NULL;

    if(pos_atual != INVALIDO)
    {
        vetor_nos = alocar_no(n_vertices);
        vetor_nos[pos_atual].indice = pos_atual;
        vetor_nos[pos_atual].valor = 1;

        for(int it = 0; it < n_vertices && !vetor_nos[pos_final].marcado && pos_atual != INVALIDO; it++)
        {
            vetor_nos[pos_atual].marcado = 1;
            for(int i = 0; i < n_vertices; i++)
            {
                if(matriz[pos_atual][i])
                {
                    float valor_aresta = vetor_nos[pos_atual].valor * matriz[pos_atual][i];

                    if(!vetor_nos[i].marcado && valor_aresta > vetor_nos[i].valor)
                    {
                        vetor_nos[i].valor = valor_aresta;
                        vetor_nos[i].indice = pos_atual;
                    }
                }
            }
            pos_atual = buscar_maior_no(vetor_nos, n_vertices);
        }
    }

    return vetor_nos;
}

int inicializa_grafo(float matriz[][100]){
    int n_vertices = 0;
    
    printf("Informe o numero de vertices (max 100): ");
    scanf("%d", &n_vertices);
    
    if (n_vertices <= 0 || n_vertices > 100) {
        printf("Numero de vertices invalido!\n");
        n_vertices = 0;
    } else {
        for (int i = 0; i < n_vertices; i++) {
            for (int j = 0; j < n_vertices; j++) {
                matriz[i][j] = 0;
            }
        }
        printf("Grafo criado com %d vertices.\n", n_vertices);
    }
    return n_vertices;
}

int main() {
    int n_vertices = 0;
    float matriz[100][100]; // Fixed maximum size
    int opcao = 0;
    
    while (1) {
        printf("\n=== Menu ===\n");
        printf("1. Criar novo grafo\n");
        printf("2. Adicionar aresta\n");
        printf("3. Encontrar caminho mais confiavel\n");
        printf("4. Exibir matriz de adjacencia\n");
        printf("5. Carregar grafo exemplo\n");
        printf("0. Sair\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);
        
        switch (opcao) {
            case 1: 
                n_vertices = inicializa_grafo(matriz);
                break;
            
            case 2: 
                adicionar_aresta(n_vertices, matriz);
                break;
            
            case 3: 
                if (n_vertices == 0) {
                    printf("Crie um grafo primeiro!\n");
                } else {
                    int origem, destino;
                    
                    printf("Informe o vertice de origem (0 a %d): ", n_vertices - 1);
                    scanf("%d", &origem);
                    printf("Informe o vertice de destino (0 a %d): ", n_vertices - 1);
                    scanf("%d", &destino);
                    
                    if (origem < 0 || origem >= n_vertices || destino < 0 || destino >= n_vertices) {
                        printf("Vertices invalidos!\n");
                    } else {
                        float temp_matriz[n_vertices][n_vertices];
                        for (int i = 0; i < n_vertices; i++) {
                            for (int j = 0; j < n_vertices; j++) {
                                temp_matriz[i][j] = matriz[i][j];
                            }
                        }
                        
                        No *vetor_nos = dijkstra(origem, destino, n_vertices, temp_matriz);
                        
                        if (vetor_nos != NULL) {
                            printf("\nCaminho mais confiavel de [%d] ate [%d]:\n", origem, destino);
                            if (!exibir_caminho(destino, vetor_nos, origem))
                                printf("Nao ha caminho valido");
                            else
                                printf("\nConfiabilidade: %.4f", vetor_nos[destino].valor);
                            
                            printf("\n");
                            free(vetor_nos);
                        }
                    }
                }
                break;
            
            case 4: 
                if (n_vertices == 0) {
                    printf("Crie um grafo primeiro!\n");
                } else {
                    exibir_matriz(n_vertices, matriz);
                }
                break;
            
            case 5: 
                n_vertices = 7;
                int posicoes[][2] = {{0, 1}, {0, 2}, {1, 3}, {1, 4}, {1, 5},
                                     {2, 3}, {2, 4}, {2, 5}, {3, 6}, {4, 6}, {5, 6}};
                            
                float valores[] = {0.3, 0.5, 0.9, 0.4, 0.7,
                                  0.1, 0.3, 0.4, 0.9, 0.2, 0.5};
                
                for (int i = 0; i < n_vertices; i++) {
                    for (int j = 0; j < n_vertices; j++) {
                        matriz[i][j] = 0;
                    }
                }
                
                int n_arestas = sizeof(valores) / sizeof(float);
                
                for (int i = 0; i < n_arestas; i++) {
                    matriz[posicoes[i][0]][posicoes[i][1]] = valores[i];
                }
                
                printf("Grafo exemplo carregado com %d vertices e %d arestas.\n", n_vertices, n_arestas);
                break;
            
            case 0: 
                printf("Programa finalizado.\n");
                return 0;
            
            default:
                printf("Opcao invalida!\n");
                break;
        }
    }
    
    return 0;
}
