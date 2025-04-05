#ifndef ALBUM_H
#define ALBUM_H

#include "artista.h"
#include "musica.h"
#include "album.h"
#include "playlist.h"
#include "utilitarios.h"
#include "interface.h"
typedef struct ALBUNS
{
    char *titulo_album;
    int ano_lancamento;
    int quantidade_musicas;
    struct ALBUNS *esq, *dir;
    struct MUSICAS *arv_musicas;
} ALBUNS;

//ALBUNS *aloca_no_album(char *titulo_album, int ano_lancamento, int quat_musics, int numero_albuns, MUSICAS *arv_musicas);
#endif