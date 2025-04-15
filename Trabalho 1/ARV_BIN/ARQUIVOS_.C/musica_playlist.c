#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../ARQUIVOS_.H/musica_playlist.h"
#include "../ARQUIVOS_.H/STRUCTS.h"
#include "../ARQUIVOS_.H/playlist.h"
#include "../ARQUIVOS_.H/utilitarios.h"

MUSICA_PLAYLIST *alocar_musica_playlist(char *titulo_musica, float duracao_musica, char *nome_artista, char *titulo_album)
{
    MUSICA_PLAYLIST *nova_musica_play = malloc(sizeof(MUSICA_PLAYLIST));
    if (nova_musica_play != NULL)
    {
        nova_musica_play->titulo_musica = titulo_musica;
        nova_musica_play->duracao_musica = duracao_musica;
        nova_musica_play->artista_musica = nome_artista;
        nova_musica_play->album_musica = titulo_album;
        nova_musica_play->esq = NULL;
        nova_musica_play->dir = NULL;
    }
    return (nova_musica_play);
}

int inserir_musica_playlist(MUSICA_PLAYLIST **raiz, MUSICA_PLAYLIST *no, char *titulo_album, char *nome_artista)
{
    int inseriu = 1;

    if (*raiz == NULL)
        *raiz = no;
    else if (strcasecmp(no->titulo_musica, (*raiz)->titulo_musica) < 0)
        inseriu = inserir_musica_playlist(&(*raiz)->esq, no, nome_artista, titulo_album);
    else if (strcasecmp(no->titulo_musica, (*raiz)->titulo_musica) > 0)
        inseriu = inserir_musica_playlist(&(*raiz)->dir, no, nome_artista, titulo_album);
    else
    {
        if (strcasecmp(no->album_musica, (*raiz)->album_musica) < 0)
            inseriu = inserir_musica_playlist(&(*raiz)->esq, no, nome_artista, titulo_album);
        else if (strcasecmp(no->album_musica, (*raiz)->album_musica) > 0)
            inseriu = inserir_musica_playlist(&(*raiz)->dir, no, nome_artista, titulo_album);
        else
        {
            if (strcasecmp(no->artista_musica, (*raiz)->artista_musica) < 0)
                inseriu = inserir_musica_playlist(&(*raiz)->esq, no, nome_artista, titulo_album);
            else if (strcasecmp(no->artista_musica, (*raiz)->artista_musica) > 0)
                inseriu = inserir_musica_playlist(&(*raiz)->dir, no, nome_artista, titulo_album);
            else
            {
                free(no);
                no = NULL;
                inseriu = 0;
            }
        }
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

void limpar_no_musica_playlist(MUSICA_PLAYLIST *no)
{
    if (no != NULL)
    {
        free(no->titulo_musica);
        no->titulo_musica = NULL;
    }
}

/*---------------------------------- Funções de Imprimir ----------------------------------*/

int imprime_todas_as_musicas_da_playlist(MUSICA_PLAYLIST *raiz) // imprime todas as musicas
{
    int imprimiu = 0;
    if (raiz != NULL)
    {
        imprimiu = imprime_todas_as_musicas_da_playlist(raiz->esq);
        printf("Musica: %s\n", raiz->titulo_musica);
        imprimiu = imprime_todas_as_musicas_da_playlist(raiz->dir);
        imprimiu = 1;
    }
    return (imprimiu);
}

int cadastrar_musica_playlist(ARTISTAS *raiz_artista, PLAYLIST *raiz_playlist)
{

    int opcao, retorno;

    char nome_artista[50];
    printf("Digite o nome do artista referente a musica que deseja adicionar: ");
    ler_string_simples(nome_artista, sizeof(nome_artista));
    ARTISTAS *artista = existe_artista(raiz_artista, nome_artista);

    if (artista)
    {
        char titulo_album[50];
        printf("Digite o titulo do album referente a musica que deseja adicionar: ");
        ler_string_simples(titulo_album, sizeof(titulo_album));
        ALBUNS *album = existe_album(artista->arv_albuns, titulo_album);

        if (album)
        {
            char titulo_musica[50];
            printf("Digite o titulo da musica a ser adicioanda na playlist: ");
            ler_string_simples(titulo_musica, sizeof(titulo_musica));
            MUSICAS *musica = existe_musica(album->arv_musicas, titulo_musica);

            if (musica)
            {
                MUSICA_PLAYLIST *musica_playlist = alocar_musica_playlist(musica->titulo_musica, musica->duracao_musica, artista->nome_artista, album->titulo_album);
                retorno = inserir_musica_playlist(&raiz_playlist->arv_musicas_playlist, musica_playlist, album->titulo_album, artista->nome_artista);
                // musica cadastrada na playlist, retorno = 1
                // musica ja existe na playlist, retorno = 0
            }
            else
                retorno = 2; // Musica nao encontrada!
        }
        else
            retorno = 3; // Album nao encontrado!
    }
    else
        retorno = 4; // Artista nao encontrado!

    return (retorno);
}

/*-------------------------------------- remover musica da playlist* --------------------------------------*/

int eh_folha(MUSICA_PLAYLIST *raiz)
{
    return (raiz->esq == NULL && raiz->dir == NULL);
}

// int so_um_filho(MUSICA_PLAYLIST *raiz)
// {
//     return ((raiz->esq == NULL && raiz->dir != NULL) || (raiz->esq != NULL && raiz->dir == NULL));
// }

MUSICA_PLAYLIST *so_um_filho_musica_playlist(MUSICA_PLAYLIST *raiz)
{
    MUSICA_PLAYLIST *filho = NULL;
    if (raiz->esq != NULL && raiz->dir == NULL)
        filho = raiz->esq;
    else if (raiz->esq == NULL && raiz->dir != NULL)
        filho = raiz->dir;
    return (filho);
}

int dois_filhos(MUSICA_PLAYLIST *raiz)
{
    return (raiz->esq != NULL && raiz->dir != NULL);
}

MUSICA_PLAYLIST *menor_musica_playlist(MUSICA_PLAYLIST *raiz)
{
    MUSICA_PLAYLIST *menor = raiz;
    while (menor->esq != NULL)
        menor = menor->esq;
    return (menor);
}

int remove_musica_platlist(MUSICA_PLAYLIST **raiz, char *titulo_musica, char *titulo_album, char *nome_artista)
{
    int removeu = 1;
    if (*raiz != NULL)
    {
        if (strcasecmp((*raiz)->titulo_musica, titulo_musica) == 0 && strcasecmp((*raiz)->artista_musica, nome_artista) == 0 && strcasecmp((*raiz)->album_musica, titulo_album) == 0)
        {
            MUSICA_PLAYLIST *aux , *filho;

            if (eh_folha(*raiz))
            {
                aux = *raiz;
                *raiz = NULL;
                free(aux);
            }else if (filho = so_um_filho_musica_playlist(*raiz))
            {
                aux = *raiz;
                *raiz = filho;
                free(aux);
            }else
            {
                aux = menor_musica_playlist((*raiz)->dir);
                (*raiz)->titulo_musica = aux->titulo_musica;
                (*raiz)->duracao_musica = aux->duracao_musica;
                (*raiz)->artista_musica = aux->artista_musica;
                (*raiz)->album_musica = aux->album_musica;
                remove_musica_platlist(&(*raiz)->dir, aux->titulo_musica, aux->album_musica, aux->artista_musica);
            }
        }
        else{
            if (strcasecmp(titulo_musica, (*raiz)->titulo_musica) < 0)
                removeu = remove_musica_platlist(&(*raiz)->esq, titulo_musica, titulo_album, nome_artista);
            else if (strcasecmp(titulo_musica, (*raiz)->titulo_musica) > 0)
                removeu = remove_musica_platlist(&(*raiz)->dir, titulo_musica, titulo_album, nome_artista);
            else
            {
                if (strcasecmp(titulo_album, (*raiz)->album_musica) < 0)
                    removeu = remove_musica_platlist(&(*raiz)->esq, titulo_musica, titulo_album, nome_artista);
                else if (strcasecmp(titulo_album, (*raiz)->album_musica) > 0)
                    removeu = remove_musica_platlist(&(*raiz)->dir, titulo_musica, titulo_album, nome_artista);
                else
                {
                    if (strcasecmp(nome_artista, (*raiz)->artista_musica) < 0)
                        removeu = remove_musica_platlist(&(*raiz)->esq, titulo_musica, titulo_album, nome_artista);
                    else //if (strcasecmp(nome_artista, (*raiz)->artista_musica) > 0)
                        removeu = remove_musica_platlist(&(*raiz)->dir, titulo_musica, titulo_album, nome_artista);
                    
                }
            }
        }
    }else{
        removeu = 0; // Raiz nula, nada a remover
    }

    return (removeu);
}