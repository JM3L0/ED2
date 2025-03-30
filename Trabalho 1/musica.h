#ifndef MUSICA_H_INCLUDED
#define MUSICA_H_INCLUDED
typedef struct MUSICAS{
    char *titulo_musica;
    float quantidade_minutos;
    struct MUSICAS *esq;
    struct MUSICAS *dir;
}MUSICAS;
#endif