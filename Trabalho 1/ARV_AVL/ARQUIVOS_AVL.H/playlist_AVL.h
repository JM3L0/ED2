#ifndef PLAYLIST_AVL_H
#define PLAYLIST_AVL_H
#include "STRUCTS_AVL.h"
#include "musica_AVL.h"
#include "utilitarios_AVL.h"

PLAYLIST *aloca_no_playlist(char *titulo_playlist);
PLAYLIST *existe_playlist(PLAYLIST *raiz, char *titulo_playlist);
PLAYLIST *cadastrar_playlist();
int inserir_playlist(PLAYLIST **raiz, PLAYLIST *no);
int imprimir_todas_as_playlists(PLAYLIST *raiz);
void limpar_no_playlist(PLAYLIST *no);
int imprime_dados_de_uma_playlist(PLAYLIST *raiz);
void liberar_arv_playlist(PLAYLIST **raiz);
int eh_folha_playlist(PLAYLIST *no);
PLAYLIST *so_um_filho_playlist(PLAYLIST *no);
PLAYLIST *menor_no_playlist(PLAYLIST *raiz);
int remove_playlist(PLAYLIST **raiz, char *titulo_playlist);
int percorre_todas_as_playlists_buscando_uma_musica(PLAYLIST *raiz, char* titulo_musica, char* album_musica, char* artista_musica);

#endif