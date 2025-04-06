#ifndef ALBUM_H
#define ALBUM_H
#include "artista.h"
#include "utilitarios.h"
#include "interface.h"
#include "STRUCTS.h"



ALBUNS *aloca_no_album(char *titulo_album, int ano_lancamento);
ALBUNS *existe_album(ALBUNS *raiz, char *titulo_album);
ALBUNS *cadastrar_album();
int inserir_album(ALBUNS **raiz, ALBUNS *no);
int imprimir_todos_albuns_de_um_artista(ARTISTAS *raiz, char *nome_artista);
int imprimir_todos_albuns(ALBUNS *raiz);
int imprimir_albuns_artita_ano(ARTISTAS *raiz, char *nome_artista, int ano_lancamento);
int imprimir_albuns_ano(ALBUNS *raiz, int ano_lancamento);
int todos_artistas_album_ano(ARTISTAS *raiz, int ano_lancamento);
int adicionar_album_a_artista(ARTISTAS *raiz, ALBUNS *album, char *nome_artista);

//ALBUNS *aloca_no_album(char *titulo_album, int ano_lancamento, int quat_musics, int numero_albuns, MUSICAS *arv_musicas);
#endif