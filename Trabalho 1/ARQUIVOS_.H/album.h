#ifndef ALBUM_H
#define ALBUM_H
typedef struct ALBUNS
{
    char *titulo_album;
    int ano_lancamento;
    int quantidade_musicas;
    struct ALBUNS *esq;
    struct ALBUNS *dir;
    struct MUSICAS *arv_musicas;
} ALBUNS;
#endif