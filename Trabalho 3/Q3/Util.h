#pragma once

int *alocar_int(int tam);
int **alocar_matriz(int linhas, int colunas);
void liberar_vetor(int **vetor);
void liberar_matriz(int ***matriz, int linhas);
int *inicializar_vetor(int tam, int num);
int *copiar_vetor(int *vetor, int tam);
void exibir_vetor(int *vetor, int tam);
void exibir_matriz(int n_vertices, float matriz[][n_vertices]);
int vetor_igual(int *vetor1, int *vetor2, int tam);
void adicionar_aresta(int n_vertices, float matriz[][n_vertices]);