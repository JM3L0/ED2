#ifndef MUSICA_PLAYLIST_H
#define MUSICA_PLAYLIST_H
#include "STRUCTS.h"
#include "utilitarios.h"
#include "playlist.h"
#include "musica.h"

MUSICA_PLAYLIST *alocar_musica_playlist(char *titulo_musica, float duracao_musica, char *nome_artista, char *titulo_album);
int inserir_musica_playlist(MUSICA_PLAYLIST **raiz, MUSICA_PLAYLIST *no);
MUSICA_PLAYLIST *existe_musica_playlist(MUSICA_PLAYLIST *raiz, char *titulo_musica, char *artista_musica, char *album_musica);
void limpar_no_musica_playlist(MUSICA_PLAYLIST *no);
int imprime_todas_as_musicas_da_playlist(MUSICA_PLAYLIST *raiz);
int cadastrar_musica_playlist(ARTISTAS *raiz_artista, PLAYLIST *raiz_playlist);
int eh_folha_musica_playlist(MUSICA_PLAYLIST *raiz);
MUSICA_PLAYLIST *so_um_filho_musica_playlist(MUSICA_PLAYLIST *raiz);
int dois_filhos(MUSICA_PLAYLIST *raiz);
MUSICA_PLAYLIST *menor_musica_playlist(MUSICA_PLAYLIST *raiz);
int remove_musica_playlist(MUSICA_PLAYLIST **raiz, char *titulo_musica, char *titulo_album, char *nome_artista);
void liberar_arv_musica_playlist(MUSICA_PLAYLIST **raiz);
int verifica_se_musica_esta_na_playlist(MUSICA_PLAYLIST *raiz, char* titulo_musica, char* artista_musica, char* album_musica);

#endif