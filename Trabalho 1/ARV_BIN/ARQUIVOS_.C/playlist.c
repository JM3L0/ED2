#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../ARQUIVOS_.H/artista.h"
#include "../ARQUIVOS_.H/album.h"
#include "../ARQUIVOS_.H/musica.h"
#include "../ARQUIVOS_.H/utilitarios.h"
// #include"../ARQUIVOS_.H/interface.h"
#include "../ARQUIVOS_.H/playlist.h"
#include "../ARQUIVOS_.H/STRUCTS.h"

PLAYLIST *aloca_no_playlist(char *titulo_playlist)
{
    PLAYLIST *novo = malloc(sizeof(PLAYLIST));
    if (novo != NULL)
    {
        novo->titulo_playlist = titulo_playlist;
        novo->arv_musicas_playlist = NULL;
        novo->esq = NULL;
        novo->dir = NULL;
    }
    return (novo);
}

PLAYLIST *existe_playlist(PLAYLIST *raiz, char *titulo_playlist) // deve ser testado
{
    PLAYLIST *existe_play;
    existe_play = NULL;

    if (raiz != NULL)
    {
        if (strcasecmp(raiz->titulo_playlist, titulo_playlist) == 0)
            existe_play = raiz;
        else if (strcasecmp(titulo_playlist, raiz->titulo_playlist) < 0)
            existe_play = existe_playlist(raiz->esq, titulo_playlist);
        else
            existe_play = existe_playlist(raiz->dir, titulo_playlist);
    }
    return (existe_play);
}

PLAYLIST *cadastrar_playlist()
{
    char *titulo_playlist;
    PLAYLIST *playlist;

    playlist = NULL;

    printf("Digite o nome do Artista: ");
    titulo_playlist = ler_string();
    playlist = aloca_no_playlist(titulo_playlist);
    return (playlist);
}

int inserir_playlist(PLAYLIST **raiz, PLAYLIST *no)
{
    int inseriu = 1;

    if (*raiz == NULL)
        *raiz = no;
    else if (strcasecmp(no->titulo_playlist, (*raiz)->titulo_playlist) < 0)
        inseriu = inserir_artista(&(*raiz)->esq, no);
    else if (strcasecmp(no->titulo_playlist, (*raiz)->titulo_playlist) > 0)
        inseriu = inserir_artista(&(*raiz)->dir, no);
    else
    {

        limpar_no_playlist(no);
        free(no);
        no = NULL;
        inseriu = 0;
    }

    return (inseriu);
}

/*---------------------------------- Funções de Printar ----------------------------------*/


int imprimir_playlists(PLAYLIST *raiz)//apens imprime todas as playlists (titulo da playlist) existentes
{
    int imprimiu = 0;
    if (raiz != NULL)
    {

        imprimir_playlists(raiz->esq);
        printf("%s\n", raiz->titulo_playlist);
        imprimir_playlists(raiz->dir);
        imprimiu = 1;
    }
    return (imprimiu);
}


/*---------------------------------- Funções de Limpar ----------------------------------*/
void limpar_no_playlist(PLAYLIST *no)
{
    if (no != NULL)
    {
        free(no->titulo_playlist);
        no->titulo_playlist = NULL;
    }
}