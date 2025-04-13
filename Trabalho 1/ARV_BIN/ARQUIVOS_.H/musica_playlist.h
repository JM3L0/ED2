#ifndef MUSICA_PLAYLIST_H
#define MUSICA_PLAYLIST_H

#include "STRUCTS.h"
#include "utilitarios.h"
#include "playlist.h"
#include "musica.h"

MUSICA_PLAYLIST *alocar_musica_playlist(char *titulo_musica, float duracao_musica, char *nome_artista, char *titulo_album);
int inserir_musica_playlist(MUSICA_PLAYLIST **raiz, MUSICA_PLAYLIST *no);
MUSICA_PLAYLIST *existe_musica_playlist(MUSICA_PLAYLIST *raiz, char *titulo_musica, char *artista_musica, char *album_musica);

#endif