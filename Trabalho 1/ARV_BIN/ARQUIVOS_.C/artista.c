#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include"../ARQUIVOS_.H/artista.h"  
#include"../ARQUIVOS_.H/album.h"
#include"../ARQUIVOS_.H/musica.h"
#include"../ARQUIVOS_.H/utilitarios.h"
#include"../ARQUIVOS_.H/interface.h"
#include"../ARQUIVOS_.H/playlist.h"

ARTISTAS *aloca_no_artista(char *nome, char *tipo_artista, char *estilo_musical, int numero_albuns, ALBUNS *arv_albuns)
{
    ARTISTAS *novo = malloc(sizeof(ARTISTAS));
    if (novo != NULL)
    {
        novo->nome_artista = nome;
        novo->tipo_artista = tipo_artista;
        novo->estilo_musical = estilo_musical;
        novo->numero_albuns = numero_albuns;
        novo->arv_albuns = arv_albuns;
        novo->esq = NULL;
        novo->dir = NULL;
    }
    return novo;
}

ARTISTAS* cadastrar_artista()
{
    char *nome, *tipo_artista, *estilo_musical;
    int num_album;
    ARTISTAS *artista;
    artista = NULL; // inicializa o ponteiro para artista
    // fazer ponteiro para albuns

    printf("Digite o nome do Artista: ");
    nome = ler_string();
    printf("Digite o tipo do Artista: ");
    tipo_artista = ler_string();
    printf("Digite o estilo musical do Artista: ");
    estilo_musical = ler_string();
    printf("Digite o numero de albuns do Artista: ");
    scanf("%d", &num_album);

    artista = aloca_no_artista(nome, tipo_artista, estilo_musical, num_album, NULL);

}
int main(){
    ARTISTAS *artista;
    artista = cadastrar_artista();
    
    if (artista != NULL)
    {
        printf("Artista cadastrado: %s\n", artista->nome_artista);
        printf("Tipo: %s\n", artista->tipo_artista);
        printf("Estilo: %s\n", artista->estilo_musical);
        printf("Numero de albuns: %d\n", artista->numero_albuns);
        free(artista->nome_artista);
        free(artista->tipo_artista);
        free(artista->estilo_musical);
        free(artista);
    }
    return 0;
}

