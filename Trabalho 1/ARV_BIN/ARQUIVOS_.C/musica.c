#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"../ARQUIVOS_.H/artista.h"  
#include"../ARQUIVOS_.H/album.h"
#include"../ARQUIVOS_.H/musica.h"
#include"../ARQUIVOS_.H/utilitarios.h"
// #include"../ARQUIVOS_.H/interface.h"
// #include"../ARQUIVOS_.H/playlist.h"

// MUSICAS *alocar_musica(char *titulo_musica, float duracao_musica)
// {
//     MUSICAS *nova_musica = malloc(sizeof(MUSICAS));
//     if (nova_musica != NULL)
//     {
//         nova_musica->titulo_musica = titulo_musica;
//         nova_musica->duracao_musica = duracao_musica;
//         nova_musica->esq = NULL;
//         nova_musica->dir = NULL;
//     }
//     return (nova_musica);
// }

// MUSICAS *cadastrar_musica()
// {
//     char *titulo_musica;
//     float duracao_musica;
//     MUSICAS *musica = NULL;

//     printf("Digite o titulo da Musica: ");
//     titulo_musica = ler_string();
//     printf("Digite a duracao da Musica: ");
//     scanf("%f", &duracao_musica);

//     musica = alocar_musica(titulo_musica, duracao_musica);
//     return (musica);
// }