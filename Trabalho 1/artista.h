#ifndef ARTISTA_H
#define ARTISTA_H
typedef struct ARTISTAS{
    char *nome_artista;
    char *tipo_artista;
    char *estilo_musical;
    int numero_albuns;
    struct ARTISTAS *esq;
    struct ARTISTAS *dir;
    struct ALBUNS *arv_albuns;
}ARTISTAS;
#endif