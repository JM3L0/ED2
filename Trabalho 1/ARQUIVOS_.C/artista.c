#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "artista.h"
#include "album.h"
#include "musica.h"
#include "utilitarios.h"

ARTISTAS *aloca_no_artista(char *nome, char *tipo_artista, char *estilo_musical, int numero_albuns, ALBUNS *arv_albuns)
{
    ARTISTAS *novo = malloc(sizeof(ARTISTAS));
    if (novo != NULL)
    {
        novo->nome_artista = nome;
        novo->tipo_artista = tipo_artista;
        novo->estilo_musical = estilo_musical;
        novo->numero_albuns = numero_albuns;
        novo->arv_albuns = arv_albuns;
        novo->esq = NULL;
        novo->dir = NULL;
    }
    return novo;
}

