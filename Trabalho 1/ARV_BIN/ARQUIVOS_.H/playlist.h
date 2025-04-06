#ifndef PLAYLIST_H
#define PLAYLIST_H

#include "artista.h"
#include "album.h"
#include "musica.h"
#include "playlist.h"
#include "utilitarios.h"
#include "interface.h"
typedef struct PLAYLIST
{
    char *titulo_playlist;
    char *nome_artista;
    char *titulo_album;
    char *titulo_musica;
    struct PLAYLIST *esq, *dir;
} PLAYLIST;
#endif