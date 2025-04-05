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

ARTISTAS *aloca_no_artista(char *nome, char *tipo_artista, char *estilo_musical, ALBUNS *arv_albuns);
ARTISTAS *existe_artista(ARTISTAS *raiz, char *nome_artista);
ARTISTAS *cadastrar_artista();
int inserir_artista(ARTISTAS **raiz, ARTISTAS *no);
int imprimir_todos_os_dados_dos_artistas(ARTISTAS *raiz);
int imprimir_todos_artistas(ARTISTAS *raiz);
int imprimir_artista_tipo(ARTISTAS *raiz, char *tipo_artista);
int imprimir_artista_estilo(ARTISTAS *raiz, char *estilo_musical);
int imprimir_artista_estilo_tipo(ARTISTAS *raiz, char *estilo_musical, char *tipo_artista);
int art_eh_folha(ARTISTAS *raiz);
int art_so_um_filho(ARTISTAS *raiz);
ARTISTAS *art_menor_no(ARTISTAS *raiz);
#endif