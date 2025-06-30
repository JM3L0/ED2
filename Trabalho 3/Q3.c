#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INFINITO -1
#define INVALIDO -1
#define MAX_VERTICES 100

//---------- Definições de Estruturas -----------
typedef struct no
{
    int indice;
    float valor;
    int marcado;
} No;

// exibe a matriz de adjacência do grafo
// recebe o número de vértices e a matriz de adjacência
void exibir_matriz(int n_vertices, float matriz[][n_vertices]){
    printf("\nMatriz de adjacencia do grafo:\n");
    printf("   ");
    for (int i = 0; i < n_vertices; i++)
        printf("%4d ", i);
    printf("\n");
    
    for (int i = 0; i < n_vertices; i++) {
        printf("%2d ", i);
        for (int j = 0; j < n_vertices; j++)
            printf("%.2f ", matriz[i][j]);
        printf("\n");
    }
}

// adiciona uma aresta ao grafo
// verifica se o grafo já foi criado e se os vértices informados são válidos
void adicionar_aresta(int n_vertices, float matriz[][n_vertices])
{
    if (n_vertices == 0)
        printf("Crie um grafo primeiro!\n");
    else {
        int origem, destino;
        float confiabilidade;
        
        printf("Informe o vertice de origem (0 a %d): ", n_vertices - 1);
        scanf("%d", &origem);
        printf("Informe o vertice de destino (0 a %d): ", n_vertices - 1);
        scanf("%d", &destino);
        
        if (origem < 0 || origem >= n_vertices || destino < 0 || destino >= n_vertices)
            printf("Vertices invalidos!\n");
        else {
            printf("Informe a confiabilidade da aresta (0 a 1): ");
            scanf("%f", &confiabilidade);
            
            if (confiabilidade <= 0 || confiabilidade > 1)
                printf("Confiabilidade deve estar entre 0 e 1!\n");
            else {
                matriz[origem][destino] = confiabilidade;
                matriz[destino][origem] = confiabilidade;
                printf("Aresta adicionada com sucesso!\n");
            }
        }
    }
}

// inicializa todos os nós do vetor
// com valores padrão: índice inválido, valor infinito e não marcado
void inicializar_nos(No vetor_nos[], int n_vertices)
{
    for(int i = 0; i < n_vertices; i++)
    {
        vetor_nos[i].indice = INVALIDO;
        vetor_nos[i].valor = INFINITO;
        vetor_nos[i].marcado = 0;
    }
}

// exibe o caminho encontrado pelo algoritmo de Dijkstra
// recursivamente, começando do nó atual até o nó inicial
int exibir_caminho(int pos_atual, No vetor_nos[], int pos_inicial)
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


// -------------- Implementação do Algoritmo de Dijkstra -----------
void dijkstra(int pos_atual, int pos_final, int n_vertices, float matriz[][n_vertices], No vetor_nos[])
{
    if(pos_atual != INVALIDO)
    {
        inicializar_nos(vetor_nos, n_vertices);
        vetor_nos[pos_atual].indice = pos_atual;
        vetor_nos[pos_atual].valor = 1;

        for(int i = 0; i < n_vertices && !vetor_nos[pos_final].marcado && pos_atual != INVALIDO; i++)
        {
            vetor_nos[pos_atual].marcado = 1;
            
            // Processa todos os vizinhos do vértice atual
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
            
            float maior = INFINITO;
            pos_atual = INVALIDO;
            
            for(int j = 0; j < n_vertices; j++)
            {
                if(!vetor_nos[j].marcado && vetor_nos[j].valor > maior)
                {
                    maior = vetor_nos[j].valor;
                    pos_atual = j;
                }
            }
        }
    }
}

// inicializa o grafo com um número de vértices
// verifica se o número de vértices é válido e inicializa a matriz de adjac
int inicializa_grafo(int n_vertices, float matriz[][MAX_VERTICES]){
    
    if (n_vertices <= 0 || n_vertices > MAX_VERTICES) {
        printf("Numero de vertices invalido!\n");
        n_vertices = 0;
    } else {
        memset(matriz, 0, sizeof(float) * MAX_VERTICES * MAX_VERTICES);
        printf("Grafo criado com %d vertices.\n", n_vertices);
    }
    return n_vertices;
}

int main() {
    int n_vertices = 0;
    float matriz[MAX_VERTICES][MAX_VERTICES];
    int opcao = 0;

    do {
        printf("\n=== Menu ===\n");
        printf("1. Criar novo grafo\n");
        printf("2. Adicionar aresta\n");
        printf("3. Encontrar caminho mais confiavel\n");
        printf("4. Exibir matriz de adjacencia\n");
        printf("0. Sair\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);
        
        switch (opcao) {
            case 1: 
                printf("Informe o numero de vertices (max %d): ", MAX_VERTICES);
                scanf("%d", &n_vertices);
                n_vertices = inicializa_grafo(n_vertices, matriz);
                break;
            
            case 2: 
                adicionar_aresta(n_vertices, matriz);
                break;
            
            case 3: 
                if (n_vertices == 0)
                    printf("Crie um grafo primeiro!\n");
                else {
                    int origem, destino;
                    
                    printf("Informe o vertice de origem (0 a %d): ", n_vertices - 1);
                    scanf("%d", &origem);
                    printf("Informe o vertice de destino (0 a %d): ", n_vertices - 1);
                    scanf("%d", &destino);
                    
                    if (origem < 0 || origem >= n_vertices || destino < 0 || destino >= n_vertices)
                        printf("Vertices invalidos!\n");
                    else {
                        No vetor_nos[n_vertices];
                        dijkstra(origem, destino, n_vertices, matriz, vetor_nos);
                        
                        printf("\nCaminho mais confiavel de [%d] ate [%d]:\n", origem, destino);
                        if (!exibir_caminho(destino, vetor_nos, origem))
                            printf("Nao ha caminho valido");
                        else
                            printf("\nConfiabilidade: %.4f", vetor_nos[destino].valor);
                        
                        printf("\n");
                    }
                }
                break;
            
            case 4: 
                if (n_vertices == 0)
                    printf("Crie um grafo primeiro!\n");
                else
                    exibir_matriz(n_vertices, matriz);
                break;

            case 0: 
                printf("Programa finalizado.\n");
                break;
            
            default:
                printf("Opcao invalida!\n");
                break;
        }
    }while (opcao != 0);
    return 0;
}