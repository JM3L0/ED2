#ifndef MUSICA_AVL_H_INCLUDED
#define MUSICA_AVL_H_INCLUDED
#include "STRUCTS_AVL.h"
#include "album_AVL.h"
#include "utilitarios_AVL.h"

MUSICAS *alocar_musica(char *titulo_musica, float duracao_musica);
MUSICAS *cadastrar_musica(MUSICAS *musica);
int inserir_musica(MUSICAS **raiz, MUSICAS *no);
MUSICAS *existe_musica(MUSICAS *raiz, char *titulo_musica);
int imprimir_todas_as_musicas(MUSICAS *raiz);
int imprime_dados_da_musica_album_artista(ARTISTAS *raiz_art, char *nome_musica);
int imprime_dados_da_musica_album(ALBUNS *raiz_alb, char *nome_musica);
int imprime_dados_da_musica_buscada(MUSICAS *raiz_mus, char *nome_musica);
void limpar_no_musica(MUSICAS *no);
void liberar_arv_musica(MUSICAS **raiz);
int eh_folha_musica (MUSICAS *raiz);
MUSICAS *so_um_filho_musica(MUSICAS *raiz);
int dois_filhos_musica(MUSICAS *raiz);
MUSICAS *menor_no_musica(MUSICAS *raiz);
int remove_musica(MUSICAS **raiz, char *titulo_musica);
int pegar_altura_musica(MUSICAS *raiz);
void atualizar_altura_musica(MUSICAS *raiz);
int fator_balanceamento_musica(MUSICAS *raiz);
void rotacao_dir_musica(MUSICAS **raiz);
void rotacao_esq_musica(MUSICAS **raiz);
void balanceamento_musica(MUSICAS **raiz);

#endif