#ifndef STRUCTS_AVL_H
#define STRUCTS_AVL_H

typedef struct MUSICAS
{
    char *titulo_musica;
    float duracao_musica;
    int altura_musica;
    
    struct MUSICAS *esq, *dir;
} MUSICAS;

typedef struct ALBUNS
{
    char *titulo_album;
    int ano_lancamento;
    int quantidade_musicas;
    int altura_album;
    struct MUSICAS *arv_musicas;

    struct ALBUNS *esq, *dir;
} ALBUNS;

typedef struct ARTISTAS
{
    char *nome_artista;
    char *tipo_artista;
    char *estilo_musical;
    int numero_albuns;
    int altura_artista;
    struct ALBUNS *arv_albuns;

    struct ARTISTAS *esq, *dir;
} ARTISTAS;

typedef struct PLAYLIST
{
    char *titulo_playlist;
    int quantidade_musicas_playlist;
    int altura_playlist;
    struct MUSICA_PLAYLIST *arv_musicas_playlist;

    struct PLAYLIST *esq, *dir;
} PLAYLIST;

typedef struct MUSICA_PLAYLIST
{
    char *titulo_musica; 
    float duracao_musica;
    char *artista_musica;
    char *album_musica;
    int altura_musica_playlist;

    struct MUSICA_PLAYLIST *esq, *dir;
} MUSICA_PLAYLIST;

#endif