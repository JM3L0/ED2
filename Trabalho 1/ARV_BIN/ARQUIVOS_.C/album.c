#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../ARQUIVOS_.H/artista.h"
#include "../ARQUIVOS_.H/album.h"
#include "../ARQUIVOS_.H/musica.h"
#include "../ARQUIVOS_.H/utilitarios.h"
#include "../ARQUIVOS_.H/interface.h"
#include "../ARQUIVOS_.H/playlist.h"

ALBUNS *aloca_no_album(char *titulo_album, int ano_lancamento)
{
    ALBUNS *novo = malloc(sizeof(ALBUNS));
    if (novo != NULL)
    {
        novo->titulo_album = titulo_album;
        novo->ano_lancamento = ano_lancamento;
        novo->quantidade_musicas = 0;
        novo->arv_musicas = NULL;
        novo->esq = NULL;
        novo->dir = NULL;
    }
    return (novo);
}

ALBUNS *existe_album(ALBUNS *raiz, char *titulo_album)
{
    ALBUNS *existe_alb;
    existe_alb = NULL;

    if (raiz != NULL)
    {
        if (strcasecmp(raiz->titulo_album, titulo_album) == 0)
            existe_alb = raiz;
        else if (strcasecmp(titulo_album, raiz->titulo_album) < 0)
            existe_alb = existe_album(raiz->esq, titulo_album);
        else
            existe_alb = existe_album(raiz->dir, titulo_album);
    }
    return (existe_alb);
}

ALBUNS *cadastrar_album()
{
    char *titulo_album;
    int ano_lancamento, quat_musics;
    ALBUNS *album;
    album = NULL;

    printf("Digite o titulo do Album: ");
    titulo_album = ler_string();
    printf("Digite o ano de lancamento do Album: ");
    scanf("%d", &ano_lancamento);
    getchar(); // Limpa o buffer do teclado
    printf("Digite a quantidade de musicas do Album: ");
    scanf("%d", &quat_musics);
    getchar(); // Limpa o buffer do teclado

    album = aloca_no_album(titulo_album, ano_lancamento);
    return (album);
}