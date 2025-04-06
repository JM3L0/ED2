#ifndef STRUCTS_H
#define STRUCTS_H
typedef struct MUSICAS
{
    char *titulo_musica;
    float quantidade_minutos;
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
    char *nome_artista;
    char *titulo_album;
    char *titulo_musica;
    struct PLAYLIST *esq, *dir;
} PLAYLIST;
#endif