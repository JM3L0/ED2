typedef struct MUSICAS{
    char *titulo_musica;
    float quantidade_minutos;
    struct MUSICAS *esq;
    struct MUSICAS *dir;
}MUSICAS;