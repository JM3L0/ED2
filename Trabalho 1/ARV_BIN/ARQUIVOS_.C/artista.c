#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../ARQUIVOS_.H/artista.h"
#include "../ARQUIVOS_.H/album.h"
#include "../ARQUIVOS_.H/musica.h"
#include "../ARQUIVOS_.H/utilitarios.h"
#include "../ARQUIVOS_.H/interface.h"
#include "../ARQUIVOS_.H/playlist.h"

ARTISTAS *aloca_no_artista(char *nome, char *tipo_artista, char *estilo_musical, ALBUNS *arv_albuns)
{
    ARTISTAS *novo = malloc(sizeof(ARTISTAS));
    if (novo != NULL)
    {
        novo->nome_artista = nome;
        novo->tipo_artista = tipo_artista;
        novo->estilo_musical = estilo_musical;
        novo->numero_albuns = 0;
        novo->arv_albuns = arv_albuns;
        novo->esq = NULL;
        novo->dir = NULL;
    }
    return (novo);
}

ARTISTAS *existe_artista(ARTISTAS *raiz, char *nome_artista) // deve ser testado
{
    ARTISTAS *existe_art;
    existe_art = NULL;

    if (raiz != NULL)
    {
        if (strcasecmp(raiz->nome_artista, nome_artista) == 0)
            existe_art = raiz;
        else if (strcasecmp(nome_artista, raiz->nome_artista) < 0)
            existe_art = existe_artista(raiz->esq, nome_artista);
        else
            existe_art = existe_artista(raiz->dir, nome_artista);
    }
    return (existe_art);
}

ARTISTAS *cadastrar_artista()
{
    char *nome, *tipo_artista, *estilo_musical;
    ARTISTAS *artista;
    // fazer ponteiro para albuns

    artista = NULL;

    printf("Digite o nome do Artista: ");
    nome = ler_string();
    printf("Digite o tipo do Artista: ");
    tipo_artista = ler_string();
    printf("Digite o estilo musical do Artista: ");
    estilo_musical = ler_string();
    artista = aloca_no_artista(nome, tipo_artista, estilo_musical, NULL);
    return (artista);
}

int inserir_artista(ARTISTAS **raiz, ARTISTAS *no)
{
    int inseriu = 0;

    if (*raiz == NULL)
        *raiz = no;
    else if (strcasecmp(no->nome_artista, (*raiz)->nome_artista) < 0)
        inseriu = inserir_artista(&(*raiz)->esq, no);
    else if (strcasecmp(no->nome_artista, (*raiz)->nome_artista) > 0)
        inseriu = inserir_artista(&(*raiz)->dir, no);
    else
        inseriu = 1;
    return (inseriu);
}

/*---------------------------------- Funções de Imprimir ----------------------------------*/
int imprimir_todos_os_dados_dos_artistas(ARTISTAS *raiz)
{
    int imprimiu = 0;

    if (raiz != NULL)
    {
        imprimiu = imprimir_todos_os_dados_dos_artistas(raiz->esq);
        printf("\n\n");
        printf("Artista: %s\n", raiz->nome_artista);
        printf("Tipo: %s\n", raiz->tipo_artista);
        printf("Estilo: %s\n", raiz->estilo_musical);
        printf("Numero de albuns: %d\n", raiz->numero_albuns);
        imprimiu = 1;
        imprimiu = imprimir_todos_os_dados_dos_artistas(raiz->dir);
    }
    return (imprimiu);
}

int imprimir_todos_artistas(ARTISTAS *raiz) // Imprime todos os artistas (só o nome)
{
    int imprimiu = 0;
    if (raiz != NULL)
    {
        imprimiu = imprimir_todos_artistas(raiz->esq);
        printf("\n\n");
        printf("Artista: %s\n", raiz->nome_artista);
        imprimiu = 1;
        imprimiu = imprimir_todos_artistas(raiz->dir);
    }
    return (imprimiu);
}

int imprimir_artista_tipo(ARTISTAS *raiz, char *tipo_artista) // imprime os artistas de acordo com o tipo
{
    int imprimiu = 0;
    if (raiz != NULL)
    {
        imprimiu = imprimir_artista_tipo(raiz->esq, tipo_artista);
        if (strcasecmp(raiz->tipo_artista, tipo_artista) == 0)
        {
            printf("Artista: %s\n", raiz->nome_artista);
            imprimiu = 1;
        }
        imprimiu = imprimir_artista_tipo(raiz->dir, tipo_artista);
    }
    return (imprimiu);
}

int imprimir_artista_estilo(ARTISTAS *raiz, char *estilo_musical) // imprime os artistas de acordo com o estilo musical
{
    int imprimiu = 0;
    if (raiz != NULL)
    {
        imprimiu = imprimir_artista_estilo(raiz->esq, estilo_musical);
        if (strcasecmp(raiz->estilo_musical, estilo_musical) == 0)
        {
            printf("Artista: %s\n", raiz->nome_artista);
            imprimiu = 1;
        }
        imprimiu = imprimir_artista_estilo(raiz->dir, estilo_musical);
    }
    return (imprimiu);
}

int imprimir_artista_estilo_e_tipo(ARTISTAS *raiz, char *estilo_musical, char *tipo_artista) // imprime os artistas de acordo com o estilo musical e tipo de artista
{
    int imprimiu = 0;
    if (raiz != NULL)
    {
        imprimiu = imprimir_artista_estilo_tipo(raiz->esq, estilo_musical, tipo_artista);
        if (strcasecmp(raiz->estilo_musical, estilo_musical) == 0 && strcasecmp(raiz->tipo_artista, tipo_artista) == 0)
        {
            imprimiu = printf("Artista: %s\n", raiz->nome_artista);
            imprimiu = 1;
        }
        imprimiu = imprimir_artista_estilo_tipo(raiz->dir, estilo_musical, tipo_artista);
    }
    return (imprimiu);
}

/*---------------------------------- Remover Artista ----------------------------------*/

int art_eh_folha(ARTISTAS *raiz) // verifica se o artista é folha
{
    return !(raiz->esq || raiz->dir);
}

int art_so_um_filho(ARTISTAS *raiz) // verifica se o artista tem um filho
{
    return (raiz->esq == NULL) != (raiz->dir == NULL);
}

ARTISTAS *art_menor_no(ARTISTAS *raiz) // retorna o menor nó da subárvore
{
    ARTISTAS *menor = raiz;

    while (menor->esq != NULL)
        menor = menor->esq;
    return menor;
}

