#include <stdio.h>
#include <stdlib.h>
#include "../Q3/Util.h"

int *alocar_int(int tam)
{
    int *vetor;
    vetor = (int *) calloc(sizeof(int), tam);

    if(!vetor)
    {
        printf("Erro ao alocar vetor de int");
        exit(EXIT_FAILURE);
    }

    return vetor;
}

int **alocar_matriz(int linhas, int colunas)
{
    int **matriz;
    matriz = (int **) malloc(sizeof(int *) * linhas);

    if(!matriz)
    {
        printf("Erro ao alocar matriz de int");
        exit(EXIT_FAILURE);
    }

    for(int i = 0; i < colunas; i++)
        matriz[i] = alocar_int(colunas);

    return matriz;
}

void liberar_vetor(int **vetor)
{
    free(*vetor);
    *vetor = NULL;
}

void liberar_matriz(int ***matriz, int linhas)
{
    for(int i = 0; i < linhas; i++)
        liberar_vetor(&((*matriz)[i]));
    
    *matriz = NULL;
}

int *inicializar_vetor(int tam, int num)
{
    int *vetor;
    vetor = alocar_int(tam);

    for(int i = 0; i < tam; i++)
        vetor[i] = num;

    return vetor;
}

int *copiar_vetor(int *vetor, int tam)
{
    int *copia;
    copia = alocar_int(tam);

    for(int i = 0; i < tam; i++)
        copia[i] = vetor[i];

    return copia;
}

void exibir_vetor(int *vetor, int tam)
{
    for(int i = 0; i < tam; i++)
        printf("%d, ", vetor[i]);
}

void exibir_caminhos(int *vetor, int tam)
{
    for(int i = 0; i < tam; i++)
    {
        if(vetor[i])
            printf("%d -> ", i);
    }
}

void exibir_matriz(int n_vertices, float matriz[][n_vertices]){
    printf("\nMatriz de adjacencia do grafo:\n");
    printf("   ");
    for (int i = 0; i < n_vertices; i++) {
        printf("%4d ", i);
    }
    printf("\n");
    
    for (int i = 0; i < n_vertices; i++) {
        printf("%2d ", i);
        for (int j = 0; j < n_vertices; j++) {
            printf("%.2f ", matriz[i][j]);
        }
        printf("\n");
    }
}

int vetor_igual(int *vetor1, int *vetor2, int tam)
{
    int igual = 1;

    for(int j = 0; j < tam && igual; j++)
        igual = vetor1[j] == vetor2[j];

    return igual;
}

void adicionar_aresta(int n_vertices, float matriz[][n_vertices])
{
    if (n_vertices == 0) {
        printf("Crie um grafo primeiro!\n");
    } else {
        int origem, destino;
        float confiabilidade;
        
        printf("Informe o vertice de origem (0 a %d): ", n_vertices - 1);
        scanf("%d", &origem);
        printf("Informe o vertice de destino (0 a %d): ", n_vertices - 1);
        scanf("%d", &destino);
        
        if (origem < 0 || origem >= n_vertices || destino < 0 || destino >= n_vertices) {
            printf("Vertices invalidos!\n");
        } else {
            printf("Informe a confiabilidade da aresta (0 a 1): ");
            scanf("%f", &confiabilidade);
            
            if (confiabilidade <= 0 || confiabilidade > 1) {
                printf("Confiabilidade deve estar entre 0 e 1!\n");
            } else {
                matriz[origem][destino] = confiabilidade;
                matriz[destino][origem] = confiabilidade;
                printf("Aresta adicionada com sucesso!\n");
            }
        }
    }
}