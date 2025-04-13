#ifndef MUSICA_H_INCLUDED
#define MUSICA_H_INCLUDED

#include "STRUCTS.h"
#include "album.h"
#include "utilitarios.h"

MUSICAS *alocar_musica(char *titulo_musica, float duracao_musica);
MUSICAS *cadastrar_musica(MUSICAS *musica);
int inserir_musica(MUSICAS **raiz, MUSICAS *no);
MUSICAS *existe_musica(MUSICAS *raiz, char *titulo_musica);
int imprimir_todas_as_musicas(MUSICAS *raiz);
int imprime_dados_da_musica_album_artista(ARTISTAS *raiz_art, char *nome_musica);
int imprime_dados_da_musica_album(ALBUNS *raiz_alb, char *nome_musica);
int imprime_dados_da_musica_buscada(MUSICAS *raiz_mus, char *nome_musica);
void limpar_no_musica(MUSICAS *no);
void limpar_arv_musica(MUSICAS **raiz);

#endif