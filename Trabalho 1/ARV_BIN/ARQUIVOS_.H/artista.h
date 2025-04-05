#ifndef ARTISTA_H
#define ARTISTA_H

#include "artista.h"
#include "musica.h"
#include "album.h"
#include "playlist.h"
#include "utilitarios.h"
#include "interface.h"
typedef struct ARTISTAS
{
    char *nome_artista;
    char *tipo_artista;
    char *estilo_musical;
    int numero_albuns;
    struct ARTISTAS *esq, *dir;
    struct ALBUNS *arv_albuns;
} ARTISTAS;

ARTISTAS *aloca_no_artista(char *nome, char *tipo_artista, char *estilo_musical, int numero_albuns, ALBUNS *arv_albuns);
ARTISTAS *cadastrar_artista();
int inserir_artista(ARTISTAS **raiz, ARTISTAS *no);
void imprimir_todos_os_dados_do_artista(ARTISTAS *raiz);
void imprimir_todos_artistas(ARTISTAS *raiz);
void imprimir_artista_tipo(ARTISTAS *raiz, char *tipo_artista);
void imprimir_artista_estilo(ARTISTAS *raiz, char *estilo_musical);
void imprimir_artista_estilo_tipo(ARTISTAS *raiz, char *estilo_musical, char *tipo_artista);
#endif