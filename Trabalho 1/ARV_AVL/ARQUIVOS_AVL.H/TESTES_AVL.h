#ifndef DADOS_PREDEFINIDOS_AVL_H
#define DADOS_PREDEFINIDOS_AVL_H

#include "artista_AVL.h"
#include "playlist_AVL.h"

#define NUM_ARTISTAS 3000
#define NUM_ALBUNS 30
#define NUM_MUSICAS 30

//não muda mais
#define NUM_EXECUCOES 10
#define REPETICOES_RAPIDAS 1000

#define CAMINHO_ARQUIVOS "../ENTRADAS_AVL/" // Ajuste para "../ENTRADAS/" se necessário

int inserir_artistas(ARTISTAS **raiz, const char *arquivo);
int inserir_albuns(ARTISTAS *raiz, const char *arquivo);
int inserir_musicas(ARTISTAS *raiz, const char *arquivo);
void testar_operacao(const char *sufixo, FILE *f_resultados);

#endif