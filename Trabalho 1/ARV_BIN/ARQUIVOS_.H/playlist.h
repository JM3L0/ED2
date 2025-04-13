#ifndef PLAYLIST_H
#define PLAYLIST_H

#include "STRUCTS.h"
#include "musica.h"
#include "utilitarios.h"

PLAYLIST *aloca_no_playlist(char *titulo_playlist);
PLAYLIST *existe_playlist(PLAYLIST *raiz, char *titulo_playlist);
PLAYLIST *cadastrar_playlist();
int inserir_playlist(PLAYLIST **raiz, PLAYLIST *no);
int imprimir_playlists(PLAYLIST *raiz);
void limpar_no_playlist(PLAYLIST *no);
int imprime_dados_de_uma_playlist(PLAYLIST *raiz);
#endif