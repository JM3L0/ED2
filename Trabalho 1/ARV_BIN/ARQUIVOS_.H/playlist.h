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
void liberar_arv_playlist(PLAYLIST **raiz);
int eh_folha(PLAYLIST *no);
PLAYLIST *so_um_filho_playlist(PLAYLIST *no);
PLAYLIST *menor_no_playlist(PLAYLIST *raiz);
int remove_playlist(PLAYLIST **raiz, char *titulo_playlist);

#endif