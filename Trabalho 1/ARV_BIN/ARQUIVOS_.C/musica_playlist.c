#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../ARQUIVOS_.H/musica_playlist.h"
#include "../ARQUIVOS_.H/STRUCTS.h"
#include "../ARQUIVOS_.H/playlist.h"

MUSICA_PLAYLIST *alocar_musica_playlist(char *titulo_musica, float duracao_musica, char *artista_musica, char *album_musica)
{
    MUSICA_PLAYLIST *nova_musica_play = malloc(sizeof(MUSICA_PLAYLIST));
    if (nova_musica_play != NULL)
    {
        nova_musica_play->titulo_musica = titulo_musica;
        nova_musica_play->duracao_musica = duracao_musica;
        nova_musica_play->artista_musica = artista_musica;
        nova_musica_play->album_musica = album_musica;
        nova_musica_play->esq = NULL;
        nova_musica_play->dir = NULL;
    }
    return (nova_musica_play);
}

// MUSICA_PLAYLIST *cadastrar_musica_playlist(char *titulo_musica)
// {
//     float duracao_musica;
//     MUSICA_PLAYLIST *musica_play;
//     musica_play = NULL;

//     printf("Digite a duracao da Musica: ");
//     duracao_musica = digitar_float();

//     musica_play = alocar_musica_playlist(titulo_musica, duracao_musica, NULL, NULL);// tem que ajeitar aqui, colocar o artista e o album
//     return (musica_play);
// }

int inserir_musica_playlist(MUSICA_PLAYLIST **raiz, MUSICA_PLAYLIST *no)
{
    int inseriu = 1;

    if (*raiz == NULL)
        *raiz = no;
    else if (strcasecmp(no->titulo_musica, (*raiz)->titulo_musica) < 0)
        inseriu = inserir_musica_playlist(&(*raiz)->esq, no);
    else if (strcasecmp(no->titulo_musica, (*raiz)->titulo_musica) > 0)
        inseriu = inserir_musica_playlist(&(*raiz)->dir, no);
    else
    {
        limpar_no_musica(no);
        free(no);
        no = NULL;
        inseriu = 0;
    }
    return (inseriu);
}

MUSICA_PLAYLIST *existe_musica_playlist(MUSICA_PLAYLIST *raiz, char *titulo_musica, char *artista_musica, char *album_musica)
{ // sera utilizado para as musicas
    MUSICA_PLAYLIST *existe_music_play;
    existe_music_play = NULL;

    if (raiz != NULL)
    {
        if (strcasecmp(raiz->titulo_musica, titulo_musica) == 0 && strcasecmp(raiz->artista_musica, artista_musica) == 0 && strcasecmp(raiz->album_musica, album_musica) == 0)
            existe_music_play = raiz;
        else if (strcasecmp(titulo_musica, raiz->titulo_musica) < 0)
            existe_music_play = existe_musica_playlist(raiz->esq, titulo_musica, artista_musica, album_musica);
        else
            existe_music_play = existe_musica_playlist(raiz->dir, titulo_musica, artista_musica, album_musica);
    }
    return (existe_music_play);
}