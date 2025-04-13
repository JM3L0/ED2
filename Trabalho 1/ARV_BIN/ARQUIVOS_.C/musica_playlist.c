#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../ARQUIVOS_.H/musica_playlist.h"
#include "../ARQUIVOS_.H/STRUCTS.h"
#include "../ARQUIVOS_.H/playlist.h"

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

////////////////////////////////////////////////////////////////////////

// MusicaPlaylist* inserirMusicaPlaylist(MusicaPlaylist *raiz, char *nome_artista, char *titulo_album, char *titulo_musica) {
//     // Se a raiz é NULL, cria nova música
//     if (raiz == NULL) {
//         MusicaPlaylist *novo = (MusicaPlaylist*)malloc(sizeof(MusicaPlaylist));
//         strcpy(novo->nome_artista, nome_artista);
//         strcpy(novo->titulo_album, titulo_album);
//         strcpy(novo->titulo_musica, titulo_musica);
//         novo->esq = novo->dir = NULL;
//         return novo;
//     }
    
//     // Comparação composta
//     int cmp_artista = strcmp(nome_artista, raiz->nome_artista);
//     if (cmp_artista < 0) {
//         raiz->esq = inserirMusicaPlaylist(raiz->esq, nome_artista, titulo_album, titulo_musica);
//     } else if (cmp_artista > 0) {
//         raiz->dir = inserirMusicaPlaylist(raiz->dir, nome_artista, titulo_album, titulo_musica);
//     } else {
//         // Mesmo artista, compara álbum
//         int cmp_album = strcmp(titulo_album, raiz->titulo_album);
//         if (cmp_album < 0) {
//             raiz->esq = inserirMusicaPlaylist(raiz->esq, nome_artista, titulo_album, titulo_musica);
//         } else if (cmp_album > 0) {
//             raiz->dir = inserirMusicaPlaylist(raiz->dir, nome_artista, titulo_album, titulo_musica);
//         } else {
//             // Mesmo artista e álbum, compara música
//             int cmp_musica = strcmp(titulo_musica, raiz->titulo_musica);
//             if (cmp_musica < 0) {
//                 raiz->esq = inserirMusicaPlaylist(raiz->esq, nome_artista, titulo_album, titulo_musica);
//             } else if (cmp_musica > 0) {
//                 raiz->dir = inserirMusicaPlaylist(raiz->dir, nome_artista, titulo_album, titulo_musica);
//             } else {
//                 // Todos os campos iguais, não insere (duplicata real)
//                 printf("Erro: A música '%s' de '%s' no álbum '%s' já está na playlist!\n", 
//                        titulo_musica, nome_artista, titulo_album);
//             }
//         }
//     }
    
//     return raiz;
// }

//////////////////////////////////////////////////////////////////////

int inserir_musica_playlist(MUSICA_PLAYLIST **raiz, MUSICA_PLAYLIST *no){
    int inseriu = 1;

    if (*raiz == NULL)
        *raiz = no;
    else if (strcasecmp(no->titulo_musica, (*raiz)->titulo_musica) < 0)
        inseriu = inserir_musica_playlist(&(*raiz)->esq, no);
    else if (strcasecmp(no->titulo_musica, (*raiz)->titulo_musica) > 0)
        inseriu = inserir_musica_playlist(&(*raiz)->dir, no);
    else
    {
        
    }
}

// int inserir_musica_playlist(MUSICA_PLAYLIST **raiz, MUSICA_PLAYLIST *no)
// {
//     int inseriu = 1;

//     if (*raiz == NULL)
//         *raiz = no;
//     else if (strcasecmp(no->titulo_musica, (*raiz)->titulo_musica) < 0)
//         inseriu = inserir_musica_playlist(&(*raiz)->esq, no);
//     else if (strcasecmp(no->titulo_musica, (*raiz)->titulo_musica) > 0)
//         inseriu = inserir_musica_playlist(&(*raiz)->dir, no);
//     else
//     {
//         limpar_no_musica_playlist(no);
//         free(no);
//         no = NULL;
//         inseriu = 0;
//     }
//     return (inseriu);
// }

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