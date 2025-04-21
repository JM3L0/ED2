#ifndef DADOS_PREDEFINIDOS_AVL_H
#define DADOS_PREDEFINIDOS_AVL_H

#include "artista_AVL.h"
#include "playlist_AVL.h"

#define NUM_ARTISTAS 100
#define NUM_ALBUNS 10
#define NUM_MUSICAS 5
#define NUM_EXECUCOES 30
#define REPETICOES_RAPIDAS 1000000
#define CAMINHO_ARQUIVOS "../ENTRADAS_AVL/" // Ajuste para "../ENTRADAS/" se necessário

int inserir_artistas(ARTISTAS **raiz, const char *arquivo);
int inserir_albuns(ARTISTAS *raiz, const char *arquivo);
int inserir_musicas(ARTISTAS *raiz, const char *arquivo);
void testar_operacao(const char *sufixo, FILE *f_resultados);

#endif