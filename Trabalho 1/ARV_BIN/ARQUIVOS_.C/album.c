#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"../ARQUIVOS_.H/artista.h"  
#include"../ARQUIVOS_.H/album.h"
#include"../ARQUIVOS_.H/musica.h"
#include"../ARQUIVOS_.H/utilitarios.h"
#include"../ARQUIVOS_.H/interface.h"
#include"../ARQUIVOS_.H/playlist.h"

ALBUNS *aloca_no_album(char *titulo_album, int ano_lancamento, int quat_musics, int numero_albuns, MUSICAS *arv_musicas)
{
    ALBUNS *novo = malloc(sizeof(ALBUNS));
    if (novo != NULL)
    {
        novo->titulo_album = titulo_album;
        novo->ano_lancamento = ano_lancamento;
        novo->quantidade_musicas = quat_musics;
        novo->arv_musicas = arv_musicas;
        novo->esq = NULL;
        novo->dir = NULL;
    }
    return (novo);
}
