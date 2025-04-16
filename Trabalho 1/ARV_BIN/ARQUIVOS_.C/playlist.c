#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../ARQUIVOS_.H/utilitarios.h"
#include "../ARQUIVOS_.H/playlist.h"
#include "../ARQUIVOS_.H/STRUCTS.h"
#include "../ARQUIVOS_.H/musica_playlist.h"

PLAYLIST *aloca_no_playlist(char *titulo_playlist)
{
    PLAYLIST *novo = malloc(sizeof(PLAYLIST));
    if (novo != NULL)
    {
        novo->titulo_playlist = titulo_playlist;
        novo->quantidade_musicas_playlist = 0;
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

    printf("Digite o nome da Playlist: ");
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
        inseriu = inserir_playlist(&(*raiz)->esq, no);
    else if (strcasecmp(no->titulo_playlist, (*raiz)->titulo_playlist) > 0)
        inseriu = inserir_playlist(&(*raiz)->dir, no);
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

int imprimir_playlists(PLAYLIST *raiz) // apens imprime todas as playlists (titulo da playlist) existentes
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

int imprime_dados_de_uma_playlist(PLAYLIST *raiz) // imprime os dados da playlist
{
    int imprimiu = 0;
    if (raiz != NULL)
    {
        printf("Playlist: %s\n", raiz->titulo_playlist);
        printf("Quantidade de musicas: %d\n", raiz->quantidade_musicas_playlist);
        printf("--------------------------------------------------\n");
        imprimiu = imprime_todas_as_musicas_da_playlist(raiz->arv_musicas_playlist);
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

void liberar_arv_playlist(PLAYLIST **raiz) // apagar tudo (CABUM)
{
    if (*raiz != NULL)
    {
        liberar_arv_playlist(&(*raiz)->esq);
        liberar_arv_playlist(&(*raiz)->dir);
        limpar_no_playlist(*raiz);
        free(*raiz);
        *raiz = NULL;
    }
}

int eh_folha(PLAYLIST *no)
{
    return (no->esq == NULL && no->dir == NULL);
}

PLAYLIST *so_um_filho_playlist(PLAYLIST *no){

    PLAYLIST *filho = NULL;
    if (no->esq != NULL && no->dir == NULL)
        filho = no->esq;
    else if (no->esq == NULL && no->dir != NULL)
        filho = no->dir;

    return (filho);
}

PLAYLIST *menor_no_playlist(PLAYLIST *raiz)
{
    PLAYLIST *menor = NULL;
    if (raiz != NULL)
    {
        menor = raiz;
        while (menor->esq != NULL)
            menor = menor->esq;
    }
    return (menor);
}

int remove_playlist(PLAYLIST **raiz, char *titulo_playlist)
{
    int removeu = 1;
    if (*raiz != NULL)
    {
        if (strcasecmp((*raiz)->titulo_playlist, titulo_playlist) == 0)
        {
            PLAYLIST *aux, *filho;

            if (eh_folha(*raiz))
            {
                aux = *raiz;
                *raiz = NULL;
                //free(aux);
            }
            else if (filho = so_um_filho_playlist(*raiz))
            {
                aux = *raiz;
                *raiz = filho;
                //free(aux);
            }
            else
            {
                aux = menor_no_playlist((*raiz)->dir);
                (*raiz)->titulo_playlist = aux->titulo_playlist;
                removeu = remove_playlist(&(*raiz)->dir, aux->titulo_playlist);
            }
            liberar_arv_musica_playlist(aux->arv_musicas_playlist);
            limpar_no_playlist(*raiz);
            free(aux);
        }
        else
        {
            removeu = remove_playlist(&(*raiz)->esq, titulo_playlist);
            if (removeu == 0)
                removeu = remove_playlist(&(*raiz)->dir, titulo_playlist);
        }
    }
    return (removeu);
}