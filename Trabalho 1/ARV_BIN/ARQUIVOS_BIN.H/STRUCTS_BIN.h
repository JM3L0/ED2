#ifndef STRUCTS_BIN_H
#define STRUCTS_BIN_H
typedef struct MUSICAS
{
    char *titulo_musica;
    float duracao_musica;
    struct MUSICAS *esq, *dir;
} MUSICAS;

typedef struct ALBUNS
{
    char *titulo_album;
    int ano_lancamento;
    int quantidade_musicas;
    struct ALBUNS *esq, *dir;
    struct MUSICAS *arv_musicas;
} ALBUNS;

typedef struct ARTISTAS
{
    char *nome_artista;
    char *tipo_artista;
    char *estilo_musical;
    int numero_albuns;
    struct ARTISTAS *esq, *dir;
    struct ALBUNS *arv_albuns;
} ARTISTAS;

typedef struct PLAYLIST
{
    char *titulo_playlist;
    int quantidade_musicas_playlist;
    struct PLAYLIST *esq, *dir;
    struct MUSICA_PLAYLIST *arv_musicas_playlist;
} PLAYLIST;

typedef struct MUSICA_PLAYLIST
{
    char *titulo_musica; 
    float duracao_musica;
    char *artista_musica;
    char *album_musica;
    struct MUSICA_PLAYLIST *esq, *dir;
} MUSICA_PLAYLIST;

#endif