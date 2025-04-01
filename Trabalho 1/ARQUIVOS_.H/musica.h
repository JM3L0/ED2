#ifndef MUSICA_H_INCLUDED
#define MUSICA_H_INCLUDED
typedef struct MUSICAS
{
    char *titulo_musica;
    float quantidade_minutos;
    struct MUSICAS *esq, *dir;
} MUSICAS;
#endif