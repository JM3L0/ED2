#ifndef MUSICA_H_INCLUDED
#define MUSICA_H_INCLUDED

#include "artista.h"
#include "musica.h"
#include "album.h"
#include "playlist.h"
#include "utilitarios.h"
#include "interface.h"
typedef struct MUSICAS
{
    char *titulo_musica;
    float quantidade_minutos;
    struct MUSICAS *esq, *dir;
} MUSICAS;
#endif