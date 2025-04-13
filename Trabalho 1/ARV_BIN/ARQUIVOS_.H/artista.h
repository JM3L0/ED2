#ifndef ARTISTA_H
#define ARTISTA_H

#include "utilitarios.h"
#include "STRUCTS.h"

ARTISTAS *aloca_no_artista(char *nome, char *tipo_artista, char *estilo_musical);
ARTISTAS *existe_artista(ARTISTAS *raiz, char *nome_artista);
ARTISTAS *cadastrar_artista();
int inserir_artista(ARTISTAS **raiz, ARTISTAS *no);
int imprimir_todos_os_dados_dos_artistas(ARTISTAS *raiz);
int imprimir_todos_artistas(ARTISTAS *raiz);
int imprimir_artista_tipo(ARTISTAS *raiz, char *tipo_artista);
int imprimir_artista_estilo(ARTISTAS *raiz, char *estilo_musical);
int imprimir_artista_estilo_e_tipo(ARTISTAS *raiz, char *estilo_musical, char *tipo_artista);
void limpar_no_artista(ARTISTAS *raiz);
void limpar_arv_artista(ARTISTAS **raiz);

#endif