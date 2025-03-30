#ifndef PLAYLIST_H
#define PLAYLIST_H
typedef struct PLAYLIST{
    char *titulo_playlist;
    char *nome_artista;
    char *titulo_album;
    char *titulo_musica;
    struct PLAYLIST *esq;
    struct PLAYLIST *dir;
}PLAYLIST;
#endif