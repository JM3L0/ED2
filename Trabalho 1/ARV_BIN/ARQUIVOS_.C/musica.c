// #include<stdio.h>
// #include<stdlib.h>
// #include<string.h>
// #include"../ARQUIVOS_.H/artista.h"  
// #include"../ARQUIVOS_.H/album.h"
// #include"../ARQUIVOS_.H/musica.h"
// #include"../ARQUIVOS_.H/utilitarios.h"
// // #include"../ARQUIVOS_.H/interface.h"
// // #include"../ARQUIVOS_.H/playlist.h"

// MUSICAS *alocar_musica(char **titulo_musica, float duracao_musica)
// {
//     MUSICAS *nova_musica = malloc(sizeof(MUSICAS));
//     if (nova_musica != NULL)
//     {
//         nova_musica->titulo_musica = *titulo_musica;
//         nova_musica->duracao_musica = duracao_musica;
//         nova_musica->esq = NULL;
//         nova_musica->dir = NULL;
//     }
//     return (nova_musica);
// }

// MUSICAS *cadastrar_musica(char **titulo_musica)
// {
//     float duracao_musica;
//     MUSICAS *musica;
//     musica = NULL;

//     printf("Digite a duracao da Musica: ");
//     scanf("%f", &duracao_musica);

//     musica = alocar_musica(titulo_musica, duracao_musica);
//     return (musica);
// }

// int inserir_musica(MUSICAS **raiz, MUSICAS *no)
// {
//     int inseriu = 1;

//     if (*raiz == NULL)
//         *raiz = no;
//     else if (strcasecmp(no->titulo_musica, (*raiz)->titulo_musica) < 0)
//         inseriu = inserir_musica(&(*raiz)->esq, no);
//     else if (strcasecmp(no->titulo_musica, (*raiz)->titulo_musica) > 0)
//         inseriu = inserir_musica(&(*raiz)->dir, no);
//     else
//         inseriu = 0;
//     return (inseriu);
// }

// MUSICAS *existe_musica(MUSICAS *raiz, char *titulo_musica)
// { // sera utilizado para as musicas
//     MUSICAS *existe_mus;
//     existe_mus = NULL;
    
//     if (raiz != NULL)
//     {
//         if (strcasecmp(raiz->titulo_musica, titulo_musica) == 0)
//             existe_mus = raiz;
//         else if (strcasecmp(titulo_musica, raiz->titulo_musica) < 0)
//             existe_mus = existe_musica(raiz->esq, titulo_musica);
//         else
//             existe_mus = existe_musica(raiz->dir, titulo_musica);
//     }
//     return (existe_mus);
// }