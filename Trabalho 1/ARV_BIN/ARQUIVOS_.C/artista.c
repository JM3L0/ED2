#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../ARQUIVOS_.H/artista.h"
#include "../ARQUIVOS_.H/album.h"
#include "../ARQUIVOS_.H/musica.h"
#include "../ARQUIVOS_.H/utilitarios.h"
#include "../ARQUIVOS_.H/interface.h"
#include "../ARQUIVOS_.H/playlist.h"

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
    return (novo);
}

ARTISTAS *cadastrar_artista()
{
    char *nome, *tipo_artista, *estilo_musical;
    int num_album;
    ARTISTAS *artista;
    artista = NULL;
    // fazer ponteiro para albuns

    printf("Digite o nome do Artista: ");
    nome = ler_string();
    printf("Digite o tipo do Artista: ");
    tipo_artista = ler_string();
    printf("Digite o estilo musical do Artista: ");
    estilo_musical = ler_string();
    printf("Digite o numero de albuns do Artista: ");
    scanf("%d", &num_album);
    getchar(); // Limpa o buffer do teclado

    artista = aloca_no_artista(nome, tipo_artista, estilo_musical, num_album, NULL);
    return (artista);
}

int inserir_artista(ARTISTAS **raiz, ARTISTAS *no)
{
    int inseriu = 0;

    if (*raiz == NULL)
        *raiz = no;
    else if (strcasecmp(no->nome_artista, (*raiz)->nome_artista) < 0)
        inserir_artista(&(*raiz)->esq, no);
    else if (strcasecmp(no->nome_artista, (*raiz)->nome_artista) > 0)
        inserir_artista(&(*raiz)->dir, no);
    else
        inseriu = 1;
    return (inseriu);
}

void imprimir_todos_os_dados_fo_artista(ARTISTAS *raiz)
{
    if (raiz != NULL)
    {
        imprimir_todos_os_dados_fo_artista(raiz->esq);
        printf("\n\n");
        printf("Artista: %s\n", raiz->nome_artista);
        printf("Tipo: %s\n", raiz->tipo_artista);
        printf("Estilo: %s\n", raiz->estilo_musical);
        printf("Numero de albuns: %d\n", raiz->numero_albuns);
        imprimir_todos_os_dados_fo_artista(raiz->dir);
    }
}

void imprimir_todos_artistas(ARTISTAS *raiz)
{
    if (raiz != NULL)
    {
        imprimir_todos_artistas(raiz->esq);
        printf("\n\n");
        printf("Artista: %s\n", raiz->nome_artista);
        imprimir_todos_artistas(raiz->dir);
    }
}

int main()
{
    ARTISTAS *raiz, *no;
    int inseriu;
    raiz = NULL; 
    no = NULL;

    no = cadastrar_artista();
    inseriu = inserir_artista(&raiz, no);
    if (inseriu == 0)
        printf("Artista inserido com sucesso!\n");
    else
        printf("Artista ja existe na arvore!\n");
    no = cadastrar_artista();
    inseriu = inserir_artista(&raiz, no);
    if (inseriu == 0)
        printf("Artista inserido com sucesso!\n");
    else
        printf("Artista ja existe na arvore!\n");
    
    

    if (raiz != NULL)
    {
        imprimir_todos_os_dados_fo_artista(raiz);
        imprimir_todos_artistas(raiz);
        free(raiz->nome_artista);
        free(raiz->tipo_artista);
        free(raiz->estilo_musical);
        free(raiz);
    }
    return 0;
}
