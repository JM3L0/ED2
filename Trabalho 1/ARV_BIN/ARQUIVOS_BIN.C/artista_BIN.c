#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../ARQUIVOS_BIN.H/artista_BIN.h"
#include "../ARQUIVOS_BIN.H/album_BIN.h"
#include "../ARQUIVOS_BIN.H/utilitarios_BIN.h"

ARTISTAS *aloca_no_artista(char *nome, char *tipo_artista, char *estilo_musical)
{
    ARTISTAS *novo = malloc(sizeof(ARTISTAS));
    if (novo != NULL)
    {
        novo->nome_artista = nome;
        novo->tipo_artista = tipo_artista;
        novo->estilo_musical = estilo_musical;
        novo->numero_albuns = 0;
        novo->arv_albuns = NULL;
        novo->esq = NULL;
        novo->dir = NULL;
    }
    return (novo);
}

ARTISTAS *existe_artista(ARTISTAS *raiz, char *nome_artista)
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
    ARTISTAS *artista ;
    nome = NULL;
    tipo_artista = NULL;
    estilo_musical = NULL;
    artista = NULL;
    int erro = 0;

    printf("Digite o nome do Artista: ");
    nome = ler_string();
    if (nome == NULL) 
        erro = 1;

    if (!erro) {
        printf("Digite o tipo do Artista: ");
        tipo_artista = ler_string();
        if (tipo_artista == NULL) 
            erro = 1;
    }

    if (!erro) {
        printf("Digite o estilo musical do Artista: ");
        estilo_musical = ler_string();
        if (estilo_musical == NULL) 
            erro = 1;
    }

    if (!erro) {
        artista = aloca_no_artista(nome, tipo_artista, estilo_musical);
        if (artista == NULL) 
            erro = 1;
    }

    if (erro) {
        free(nome);
        free(tipo_artista);
        free(estilo_musical);
        free(artista);
        artista = NULL;
    }

    return artista;
}

int inserir_artista(ARTISTAS **raiz, ARTISTAS *no)
{
    int inseriu = 1;

    if (*raiz == NULL)
        *raiz = no;
    else if (strcasecmp(no->nome_artista, (*raiz)->nome_artista) < 0)
        inseriu = inserir_artista(&(*raiz)->esq, no);
    else if (strcasecmp(no->nome_artista, (*raiz)->nome_artista) > 0)
        inseriu = inserir_artista(&(*raiz)->dir, no);
    else
    {

        limpar_no_artista(no);
        free(no);
        no = NULL;
        inseriu = 0;
    }

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
        imprimiu = imprimir_todos_os_dados_dos_artistas(raiz->dir);
        imprimiu = 1;
    }
    return (imprimiu);
}

int imprimir_artista_tipo(ARTISTAS *raiz, char *tipo_artista) // imprime os artistas de acordo com o tipo
{
    int imprimiu = 0;
    if (raiz != NULL)
    {
        imprimiu |= imprimir_artista_tipo(raiz->esq, tipo_artista);
        if (strcasecmp(raiz->tipo_artista, tipo_artista) == 0)
        {
            printf("Artista: %s\n", raiz->nome_artista);
            imprimiu = 1;
        }
        imprimiu |= imprimir_artista_tipo(raiz->dir, tipo_artista); // |= é o operador OR
    }
    return (imprimiu);
}

int imprimir_artista_estilo(ARTISTAS *raiz, char *estilo_musical) // imprime os artistas de acordo com o estilo musical
{
    int imprimiu = 0;
    if (raiz != NULL)
    {
        imprimiu |= imprimir_artista_estilo(raiz->esq, estilo_musical);
        if (strcasecmp(raiz->estilo_musical, estilo_musical) == 0)
        {
            printf("Artista: %s\n", raiz->nome_artista);
            imprimiu = 1;
        }
        imprimiu |= imprimir_artista_estilo(raiz->dir, estilo_musical); // |= é o operador OR
    }
    return (imprimiu);
}

int imprimir_artista_estilo_e_tipo(ARTISTAS *raiz, char *estilo_musical, char *tipo_artista) // imprime os artistas de acordo com o estilo musical e tipo de artista
{
    int imprimiu = 0;
    if (raiz != NULL)
    {
        imprimiu |= imprimir_artista_estilo_e_tipo(raiz->esq, estilo_musical, tipo_artista);
        if (strcasecmp(raiz->estilo_musical, estilo_musical) == 0 && strcasecmp(raiz->tipo_artista, tipo_artista) == 0)
        {
            imprimiu = printf("Artista: %s\n", raiz->nome_artista);
            imprimiu = 1;
        }
        imprimiu |= imprimir_artista_estilo_e_tipo(raiz->dir, estilo_musical, tipo_artista); // |= é o operador OR
    }
    return (imprimiu);
}

/*---------------------------------- Limpar Arv Artista ----------------------------------*/

void limpar_no_artista(ARTISTAS *no) // limpa os dados do artista sem liberar o nó
{
    if (no != NULL)
    {
        free(no->nome_artista);
        no->nome_artista = NULL;

        free(no->tipo_artista);
        no->tipo_artista = NULL;

        free(no->estilo_musical);
        no->estilo_musical = NULL;
    }
}

void liberar_arv_artista(ARTISTAS **raiz) // limpa a arvore artista
{
    if (*raiz != NULL)
    {
        if ((*raiz)->arv_albuns != NULL) // se a arvore de albuns não for nula
        {
            liberar_arv_album(&(*raiz)->arv_albuns); // limpa a arvore de albuns
            (*raiz)->arv_albuns = NULL;              // seta a arvore de albuns como nula
        }
        liberar_arv_artista(&(*raiz)->esq);
        liberar_arv_artista(&(*raiz)->dir);
        limpar_no_artista(*raiz);
        free(*raiz);
        *raiz = NULL;
    }
}
