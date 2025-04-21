#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "../ARQUIVOS_AVL.H/TESTES_AVL.h"

// #define NUM_ARTISTAS 1000
// #define NUM_ALBUNS 100
// #define NUM_MUSICAS 10

// Estrutura temporária para embaralhamento
typedef struct {
    char *linha;
} Linha;

// Função para embaralhar um array
void embaralhar(Linha *arr, size_t n) {
    Linha tmp;
    for (size_t i = n - 1; i > 0; i--) {
        size_t j = rand() % (i + 1);
        tmp = arr[j];
        arr[j] = arr[i];
        arr[i] = tmp;
    }
}

void gerar_arquivos(const char *sufixo) {
    srand(time(NULL));

    // Arrays para armazenar linhas
    Linha *linhas_artistas = malloc(NUM_ARTISTAS * sizeof(Linha));
    Linha *linhas_albuns = malloc(NUM_ARTISTAS * NUM_ALBUNS * sizeof(Linha));
    Linha *linhas_musicas = malloc(NUM_ARTISTAS * NUM_ALBUNS * NUM_MUSICAS * sizeof(Linha));

    // Gerar dados para artistas
    for (int i = 0; i < NUM_ARTISTAS; i++) {
        char linha[100];
        sprintf(linha, "Artista%04d;Banda;Rock", i + 1);
        linhas_artistas[i].linha = strdup(linha);
    }

    // Gerar dados para álbuns
    for (int i = 0; i < NUM_ARTISTAS; i++) {
        for (int j = 0; j < NUM_ALBUNS; j++) {
            char linha[100];
            sprintf(linha, "Artista%04d;Album%04d;%d", i + 1, j + 1, 2000 + (j % 25));
            int idx = i * NUM_ALBUNS + j;
            linhas_albuns[idx].linha = strdup(linha);
        }
    }

    // Gerar dados para músicas
    for (int i = 0; i < NUM_ARTISTAS; i++) {
        for (int j = 0; j < NUM_ALBUNS; j++) {
            for (int k = 0; k < NUM_MUSICAS; k++) {
                char linha[100];
                float duracao = 3.0 + (rand() % 30) / 10.0;
                sprintf(linha, "Artista%04d;Album%04d;Musica%04d;%.1f", i + 1, j + 1, k + 1, duracao);
                int idx = i * NUM_ALBUNS * NUM_MUSICAS + j * NUM_MUSICAS + k;
                linhas_musicas[idx].linha = strdup(linha);
            }
        }
    }

    // Escrever arquivos
    char nome_arquivo[50];
    if (strcmp(sufixo, "crescente") == 0) {
        sprintf(nome_arquivo, "artistas_crescente.txt");
        FILE *f = fopen(nome_arquivo, "w");
        for (int i = 0; i < NUM_ARTISTAS; i++) {
            fprintf(f, "%s\n", linhas_artistas[i].linha);
        }
        fclose(f);

        sprintf(nome_arquivo, "albuns_crescente.txt");
        f = fopen(nome_arquivo, "w");
        for (int i = 0; i < NUM_ARTISTAS * NUM_ALBUNS; i++) {
            fprintf(f, "%s\n", linhas_albuns[i].linha);
        }
        fclose(f);

        sprintf(nome_arquivo, "musicas_crescente.txt");
        f = fopen(nome_arquivo, "w");
        for (int i = 0; i < NUM_ARTISTAS * NUM_ALBUNS * NUM_MUSICAS; i++) {
            fprintf(f, "%s\n", linhas_musicas[i].linha);
        }
        fclose(f);
    }
    else if (strcmp(sufixo, "decrescente") == 0) {
        sprintf(nome_arquivo, "artistas_decrescente.txt");
        FILE *f = fopen(nome_arquivo, "w");
        for (int i = NUM_ARTISTAS - 1; i >= 0; i--) {
            fprintf(f, "%s\n", linhas_artistas[i].linha);
        }
        fclose(f);

        sprintf(nome_arquivo, "albuns_decrescente.txt");
        f = fopen(nome_arquivo, "w");
        for (int i = NUM_ARTISTAS * NUM_ALBUNS - 1; i >= 0; i--) {
            fprintf(f, "%s\n", linhas_albuns[i].linha);
        }
        fclose(f);

        sprintf(nome_arquivo, "musicas_decrescente.txt");
        f = fopen(nome_arquivo, "w");
        for (int i = NUM_ARTISTAS * NUM_ALBUNS * NUM_MUSICAS - 1; i >= 0; i--) {
            fprintf(f, "%s\n", linhas_musicas[i].linha);
        }
        fclose(f);
    }
    else if (strcmp(sufixo, "aleatorio") == 0) {
        embaralhar(linhas_artistas, NUM_ARTISTAS);
        embaralhar(linhas_albuns, NUM_ARTISTAS * NUM_ALBUNS);
        embaralhar(linhas_musicas, NUM_ARTISTAS * NUM_ALBUNS * NUM_MUSICAS);

        sprintf(nome_arquivo, "artistas_aleatorio.txt");
        FILE *f = fopen(nome_arquivo, "w");
        for (int i = 0; i < NUM_ARTISTAS; i++) {
            fprintf(f, "%s\n", linhas_artistas[i].linha);
        }
        fclose(f);

        sprintf(nome_arquivo, "albuns_aleatorio.txt");
        f = fopen(nome_arquivo, "w");
        for (int i = 0; i < NUM_ARTISTAS * NUM_ALBUNS; i++) {
            fprintf(f, "%s\n", linhas_albuns[i].linha);
        }
        fclose(f);

        sprintf(nome_arquivo, "musicas_aleatorio.txt");
        f = fopen(nome_arquivo, "w");
        for (int i = 0; i < NUM_ARTISTAS * NUM_ALBUNS * NUM_MUSICAS; i++) {
            fprintf(f, "%s\n", linhas_musicas[i].linha);
        }
        fclose(f);
    }

    // Liberar memória
    for (int i = 0; i < NUM_ARTISTAS; i++) free(linhas_artistas[i].linha);
    for (int i = 0; i < NUM_ARTISTAS * NUM_ALBUNS; i++) free(linhas_albuns[i].linha);
    for (int i = 0; i < NUM_ARTISTAS * NUM_ALBUNS * NUM_MUSICAS; i++) free(linhas_musicas[i].linha);
    free(linhas_artistas);
    free(linhas_albuns);
    free(linhas_musicas);
}

int main() {
    gerar_arquivos("crescente");
    gerar_arquivos("decrescente");
    gerar_arquivos("aleatorio");
    printf("Arquivos de teste gerados com sucesso!\n");
    return 0;
}