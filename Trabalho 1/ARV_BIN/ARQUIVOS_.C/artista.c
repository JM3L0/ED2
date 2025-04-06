#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../ARQUIVOS_.H/artista.h"
#include "../ARQUIVOS_.H/album.h"
#include "../ARQUIVOS_.H/musica.h"
#include "../ARQUIVOS_.H/utilitarios.h"
#include "../ARQUIVOS_.H/interface.h"
#include "../ARQUIVOS_.H/playlist.h"

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
    artista = aloca_no_artista(nome, tipo_artista, estilo_musical);
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
        imprimiu = imprimir_todos_os_dados_dos_artistas(raiz->dir);
        imprimiu = 1;
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
        imprimiu = imprimir_todos_artistas(raiz->dir);
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
        imprimiu |= imprimir_artista_tipo(raiz->dir, tipo_artista);// |= é o operador OR
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
        imprimiu |= imprimir_artista_estilo(raiz->dir, estilo_musical);// |= é o operador OR
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
        imprimiu |= imprimir_artista_estilo_e_tipo(raiz->dir, estilo_musical, tipo_artista);// |= é o operador OR
    }
    return (imprimiu);
}

/*---------------------------------- Limpar Arv Artista ----------------------------------*/

void limpar_no_artista(ARTISTAS *raiz) // limpa os dados do artista sem liberar o nó
{
    if (raiz != NULL)
    {
        free(raiz->nome_artista);
        raiz->nome_artista = NULL;

        free(raiz->tipo_artista);
        raiz->tipo_artista = NULL;

        free(raiz->estilo_musical);
        raiz->estilo_musical = NULL;
    }
}

void limpar_arv_artista(ARTISTAS **raiz) // limpa a arvore artista
{
    if (*raiz != NULL)
    {
        // if ((*raiz)->arv_albuns != NULL) // se a arvore de albuns não for nula
        // {
        //     limpar_arv_album(&(*raiz)->arv_albuns); // limpa a arvore de albuns
        //     (*raiz)->arv_albuns = NULL; // seta a arvore de albuns como nula
        // }
        limpar_arv_artista(&(*raiz)->esq);
        limpar_arv_artista(&(*raiz)->dir);
        limpar_no_artista(*raiz);
        free(*raiz);
        *raiz = NULL;
    }
}
// int art_eh_folha(ARTISTAS *raiz) // verifica se o artista é folha
// {
//     return !(raiz->esq || raiz->dir);
// }

// int art_so_um_filho(ARTISTAS *raiz) // verifica se o artista tem um filho
// {
//     // return (raiz->esq == NULL) != (raiz->dir == NULL);
// }

// ARTISTAS *art_menor_no(ARTISTAS *raiz) // retorna o menor nó da subárvore
// {
//     ARTISTAS *menor = raiz;

//     while (menor->esq != NULL)
//         menor = menor->esq;
//     return menor;
// }

// void copiar_no_artista(ARTISTAS **raiz, ARTISTAS **menor) // copia os dados do menor nó para o nó a ser removido
// {
    
// }


