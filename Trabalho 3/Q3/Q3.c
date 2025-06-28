#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <time.h>
#include "../Q3/Util.h"

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

        for(int i = 0; i < n_vertices && !vetor_nos[pos_final].marcado && pos_atual != INVALIDO; i++)
        {
            vetor_nos[pos_atual].marcado = 1;
            for(int j = 0; j < n_vertices; j++)
            {
                if(matriz[pos_atual][j])
                {
                    float valor_aresta = vetor_nos[pos_atual].valor * matriz[pos_atual][j];

                    if(!vetor_nos[j].marcado && valor_aresta > vetor_nos[j].valor)
                    {
                        vetor_nos[j].valor = valor_aresta;
                        vetor_nos[j].indice = pos_atual;
                    }
                }
            }
            pos_atual = buscar_maior_no(vetor_nos, n_vertices);
        }
    }

    return vetor_nos;
}

int inicializa_grafo(int n_vertices, float matriz[][100]){
    
    if (n_vertices <= 0 || n_vertices > 100) {
        printf("Numero de vertices invalido!\n");
        n_vertices = 0;
    } else {
        for (int i = 0; i < n_vertices; i++) {
            for (int j = 0; j < n_vertices; j++)
                matriz[i][j] = 0;
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
                printf("Informe o numero de vertices (max 100): ");
                scanf("%d", &n_vertices);
                n_vertices = inicializa_grafo(n_vertices ,matriz);
                break;
            
            case 2: {
                // int origem, destino;
                // float confiabilidade;

                // confiabilidade = pegar_ODC(&origem, &destino, n_vertices);
                // limpar_buffer();

                // printf("Origem: %d, Destino: %d, Confiabilidade: %.2f\n", origem, destino, confiabilidade);
                
                // if (confiabilidade != -1){// add aresta
                //     printf("Origem: %d, Destino: %d, Confiabilidade: %.2f\n", origem, destino, confiabilidade);
                //     matriz[origem][destino] = confiabilidade;
                //     printf("%f\n", matriz[origem][destino]);
                //     printf("Origem: %d, Destino: %d, Confiabilidade: %.2f\n", origem, destino, confiabilidade);
                //     matriz[destino][origem] = confiabilidade; 
                //     printf("%f\n", matriz[destino][origem]);
                //     printf("Origem: %d, Destino: %d, Confiabilidade: %.2f\n", origem, destino, confiabilidade);
                // }

                adicionar_aresta(n_vertices, matriz);
                
                break;
            }
            case 3: 
                if (n_vertices == 0)
                    printf("Crie um grafo primeiro!\n");
                else {
                    int origem, destino;
                    
                    printf("Informe o vertice de origem (0 a %d): ", n_vertices - 1);
                    scanf("%d", &origem);
                    printf("Informe o vertice de destino (0 a %d): ", n_vertices - 1);
                    scanf("%d", &destino);
                    
                    if (origem < 0 || origem >= n_vertices || destino < 0 || destino >= n_vertices) {
                        printf("Vertices invalidos!\n");
                    } else {
                        No *vetor_nos;
                        vetor_nos = dijkstra(origem, destino, n_vertices, matriz);
                        
                        if (vetor_nos != NULL) {
                            printf("\nCaminho mais confiavel de [%d] ate [%d]:\n", origem, destino);
                            if (!exibir_caminho(destino, vetor_nos, origem))
                                printf("Nao ha caminho valido");
                            else
                                printf("\nConfiabilidade: %.4f", vetor_nos[destino].valor);
                            
                            printf("\n");
                            free(vetor_nos);
                            vetor_nos = NULL;
                        }
                    }
                }
                break;
            
            case 4: 
                if (n_vertices == 0)
                    printf("Crie um grafo primeiro!\n");
                else
                    exibir_matriz(n_vertices, matriz);
                break;
            
            case 5:{

                n_vertices = inicializa_grafo(3, matriz);

                // Inicializa o gerador de números aleatórios
                srand(time(NULL));  // Nota: Adicione #include <time.h> no topo do seu arquivo

                printf("Preenchendo grafo exemplo com arestas aleatórias...\n");

                matriz [0][1] = (float)(rand() % 100 + 1) / 100; // Aresta de 0 a 1
                matriz [1][0] = (float)(rand() % 100 + 1) / 100; // Aresta de 0 a 2
                matriz [1][2] = (float)(rand() % 100 + 1) / 100; // Aresta de 1 a 2
                matriz [2][1] = (float)(rand() % 100 + 1) / 100; // Aresta de 1 a 3
                matriz [2][3] = (float)(rand() % 100 + 1) / 100; // Aresta de 0 a 3
                matriz [3][2] = (float)(rand() % 100 + 1) / 100; // Aresta de 2 a 3
                matriz [0][2] = (float)(rand() % 100 + 1) / 100; // Aresta de 0 a 2
                matriz [2][0] = (float)(rand() % 100 + 1) / 100; // Aresta de 2 a 0
             

                printf("Grafo exemplo criado com sucesso!\n");

                break;
            }
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
