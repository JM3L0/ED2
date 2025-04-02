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
void cadastrar_artista();
#endif