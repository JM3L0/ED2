#ifndef ALBUM_AVL_H
#define ALBUM_AVL_H
#include "artista_AVL.h"
#include "utilitarios_AVL.h"
#include "STRUCTS_AVL.h"

ALBUNS *aloca_no_album(char *titulo_album, int ano_lancamento);
ALBUNS *existe_album(ALBUNS *raiz, char *titulo_album);
ALBUNS *cadastrar_album(ALBUNS *album);
int inserir_album(ALBUNS **raiz, ALBUNS *no);
int imprimir_todos_albuns_de_um_artista(ARTISTAS *raiz, char *nome_artista);
int imprimir_todos_albuns(ALBUNS *raiz);
int imprimir_albuns_artita_ano(ARTISTAS *raiz, char *nome_artista, int ano_lancamento);
int imprimir_albuns_ano(ALBUNS *raiz, int ano_lancamento);
int mostrar_todos_artistas_album_ano(ARTISTAS *raiz, int ano_lancamento);
void mostrar_dados_album(ALBUNS *raiz);
void limpar_no_album(ALBUNS *raiz);   // limpa os dados do album sem liberar o nó
void liberar_arv_album(ALBUNS **raiz); // limpa a arvore album
int pegar_altura_album(ALBUNS *raiz);
void atualizar_altura_album(ALBUNS *raiz);
int fator_balanceamento_album(ALBUNS *no);
void rotacao_esq_album(ALBUNS **raiz);
void rotacao_dir_album(ALBUNS **raiz);
void balanceamento_album(ALBUNS **raiz);

#endif