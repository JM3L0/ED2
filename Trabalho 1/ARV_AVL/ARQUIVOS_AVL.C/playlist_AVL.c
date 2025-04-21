#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../ARQUIVOS_AVL.H/utilitarios_AVL.h"
#include "../ARQUIVOS_AVL.H/playlist_AVL.h"
#include "../ARQUIVOS_AVL.H/STRUCTS_AVL.h"
#include "../ARQUIVOS_AVL.H/musica_playlist_AVL.h"

PLAYLIST *aloca_no_playlist(char *titulo_playlist)
{
    PLAYLIST *novo = malloc(sizeof(PLAYLIST));
    if (novo != NULL)
    {
        novo->titulo_playlist = titulo_playlist;
        novo->quantidade_musicas_playlist = 0;
        novo->arv_musicas_playlist = NULL;
        novo->esq = NULL;
        novo->dir = NULL;
    }
    return (novo);
}

PLAYLIST *existe_playlist(PLAYLIST *raiz, char *titulo_playlist) // deve ser testado
{
    PLAYLIST *existe_play;
    existe_play = NULL;

    if (raiz != NULL)
    {
        if (strcasecmp(raiz->titulo_playlist, titulo_playlist) == 0)
            existe_play = raiz;
        else if (strcasecmp(titulo_playlist, raiz->titulo_playlist) < 0)
            existe_play = existe_playlist(raiz->esq, titulo_playlist);
        else
            existe_play = existe_playlist(raiz->dir, titulo_playlist);
    }
    return (existe_play);
}

PLAYLIST *cadastrar_playlist()
{
    char *titulo_playlist;
    PLAYLIST *playlist;
    playlist = NULL;
    int erro = 0;

    printf("Digite o nome da Playlist: ");
    titulo_playlist = ler_string();

    if (titulo_playlist == NULL)
        erro = 1;
    if (!erro)
    {
        playlist = aloca_no_playlist(titulo_playlist);
        if (playlist == NULL)
            erro = 1;
    }
    if (erro)
    {
        free(titulo_playlist);
        free(playlist);
        playlist = NULL;
    }
    return (playlist);
}

int inserir_playlist(PLAYLIST **raiz, PLAYLIST *no)
{
    int inseriu = 1;

    if (*raiz == NULL)
        *raiz = no;
    else if (strcasecmp(no->titulo_playlist, (*raiz)->titulo_playlist) < 0)
        inseriu = inserir_playlist(&(*raiz)->esq, no);
    else if (strcasecmp(no->titulo_playlist, (*raiz)->titulo_playlist) > 0)
        inseriu = inserir_playlist(&(*raiz)->dir, no);
    else
    {

        limpar_no_playlist(no);
        free(no);
        no = NULL;
        inseriu = 0;
    }

    if (inseriu)
    {
        balanceamento_playlist(raiz);     // Realiza o balanceamento da árvore
        atualizar_altura_playlist(*raiz); // Atualiza a altura do nó após a inserção
    }
    return (inseriu);
}

int percorre_todas_as_playlists_buscando_uma_musica(PLAYLIST *raiz, char *titulo_musica, char *album_musica, char *artista_musica) // percorre todas as playlists e imprime as musicas{
{
    int retorno = 0;

    if (raiz != NULL)
    {
        if (retorno == 0)
        {

            retorno |= percorre_todas_as_playlists_buscando_uma_musica(raiz->esq, titulo_musica, album_musica, artista_musica);
            if (raiz->arv_musicas_playlist != NULL)
                retorno |= verifica_se_musica_esta_na_playlist(raiz->arv_musicas_playlist, titulo_musica, album_musica, artista_musica);
            retorno |= percorre_todas_as_playlists_buscando_uma_musica(raiz->dir, titulo_musica, album_musica, artista_musica);
        }
    }
    return (retorno);
}

/*---------------------------------- Funções de Printar ----------------------------------*/

int imprimir_todas_as_playlists(PLAYLIST *raiz) // apens imprime todas as playlists (titulo da playlist) existentes
{
    int imprimiu = 0;

    if (raiz != NULL)
    {
        imprimir_todas_as_playlists(raiz->esq);
        printf("Nome: %s\n", raiz->titulo_playlist);
        printf("Altura: %d\n", raiz->altura_playlist);
        imprimir_todas_as_playlists(raiz->dir);
        imprimiu = 1;
    }
    return (imprimiu);
}

int imprime_dados_de_uma_playlist(PLAYLIST *raiz) // imprime os dados da playlist
{
    int imprimiu = 0;

    if (raiz != NULL)
    {
        printf("Playlist: %s\n", raiz->titulo_playlist);
        printf("Quantidade de musicas: %d\n", raiz->quantidade_musicas_playlist);
        printf("--------------------------------------------------\n");
        imprimiu = imprime_todas_as_musicas_da_playlist(raiz->arv_musicas_playlist);
        imprimiu = 1;
    }
    return (imprimiu);
}

/*---------------------------------- Funções de Limpar ----------------------------------*/
void limpar_no_playlist(PLAYLIST *no)
{
    if (no != NULL)
    {
        free(no->titulo_playlist);
        no->titulo_playlist = NULL;
    }
}

void liberar_arv_playlist(PLAYLIST **raiz) // apagar tudo (CABUM)
{
    if (*raiz != NULL)
    {
        if ((*raiz)->arv_musicas_playlist != NULL)
        {
            liberar_arv_musica_playlist(&(*raiz)->arv_musicas_playlist);
            (*raiz)->arv_musicas_playlist = NULL;
        }

        liberar_arv_playlist(&(*raiz)->esq);
        liberar_arv_playlist(&(*raiz)->dir);
        limpar_no_playlist(*raiz);
        free(*raiz);
        *raiz = NULL;
    }
}

/*---------------------------------- Funções de Balancear ----------------------------------*/

int eh_folha_playlist(PLAYLIST *no)
{
    return (no->esq == NULL && no->dir == NULL);
}

PLAYLIST *so_um_filho_playlist(PLAYLIST *no)
{
    PLAYLIST *filho;
    filho = NULL;

    if (no->esq != NULL && no->dir == NULL)
        filho = no->esq;
    else if (no->esq == NULL && no->dir != NULL)
        filho = no->dir;

    return (filho);
}

PLAYLIST *menor_no_playlist(PLAYLIST *raiz)
{
    PLAYLIST *menor;
    menor = NULL;

    if (raiz != NULL)
    {
        menor = raiz;
        while (menor->esq != NULL)
            menor = menor->esq;
    }
    return (menor);
}

int remove_playlist(PLAYLIST **raiz, char *titulo_playlist)
{
    int removeu = 0;

    if (*raiz != NULL)
    {
        if (strcasecmp((*raiz)->titulo_playlist, titulo_playlist) == 0)
        {
            PLAYLIST *aux, *filho;
            aux = *raiz;
            removeu = 1;

            if (eh_folha_playlist(*raiz))
            {
                *raiz = NULL;
                liberar_arv_musica_playlist(&aux->arv_musicas_playlist);
                limpar_no_playlist(aux);
                free(aux);
            }
            else if ((filho = so_um_filho_playlist(*raiz)) != NULL)
            {
                *raiz = filho;
                liberar_arv_musica_playlist(&aux->arv_musicas_playlist);
                limpar_no_playlist(aux);
                free(aux);
            }
            else
            {

                aux = menor_no_playlist((*raiz)->dir);
                liberar_arv_musica_playlist(&(*raiz)->arv_musicas_playlist);
                limpar_no_playlist(*raiz);

                (*raiz)->titulo_playlist = strdup(aux->titulo_playlist); // Aloca nova memória para o título da playlist
                (*raiz)->quantidade_musicas_playlist = aux->quantidade_musicas_playlist;
                (*raiz)->arv_musicas_playlist = aux->arv_musicas_playlist;

                aux->arv_musicas_playlist = NULL;

                removeu = remove_playlist(&(*raiz)->dir, aux->titulo_playlist);
            }
        }
        else
        {
            if(strcasecmp(titulo_playlist, (*raiz)->titulo_playlist) < 0)
                removeu = remove_playlist(&(*raiz)->esq, titulo_playlist);
            else
                removeu = remove_playlist(&(*raiz)->dir, titulo_playlist);
        }
        if (removeu)
        {
            balanceamento_playlist(raiz);     // Realiza o balanceamento da árvore
            atualizar_altura_playlist(*raiz); // Atualiza a altura do nó após a remoção
        }
    }
    return (removeu);
}

/*---------------------------------- Funções de Balanceamento ----------------------------------*/

int pegar_altura_playlist(PLAYLIST *raiz)
{
    int altura = -1;

    if (raiz)
        altura = raiz->altura_playlist;
    return (altura);
}

void atualizar_altura_playlist(PLAYLIST *raiz)
{
    if (raiz != NULL)
    {
        int altura_esq = pegar_altura_playlist(raiz->esq);
        int altura_dir = pegar_altura_playlist(raiz->dir);
        raiz->altura_playlist = (altura_esq > altura_dir ? altura_esq : altura_dir) + 1;
    }
}

int fator_balanceamento_playlist(PLAYLIST *no)
{
    int fator = 0;

    if (no)
        fator = pegar_altura_playlist(no->esq) - pegar_altura_playlist(no->dir);
    return (fator);
}

void rotacao_esq_playlist(PLAYLIST **raiz)
{
    PLAYLIST *aux;
    aux = (*raiz)->dir;
    (*raiz)->dir = aux->esq;
    aux->esq = *raiz;
    (*raiz) = aux;
    atualizar_altura_playlist((*raiz)->esq);
    atualizar_altura_playlist((*raiz));
}

void rotacao_dir_playlist(PLAYLIST **raiz)
{
    PLAYLIST *aux;
    aux = (*raiz)->esq;
    (*raiz)->esq = aux->dir;
    aux->dir = *raiz;
    (*raiz) = aux;
    atualizar_altura_playlist((*raiz)->dir);
    atualizar_altura_playlist((*raiz));
}

void balanceamento_playlist(PLAYLIST **raiz)
{
    if (*raiz)
    {
        if (fator_balanceamento_playlist(*raiz) == 2)
        {
            if (fator_balanceamento_playlist((*raiz)->esq) < 0)
                rotacao_esq_playlist(&((*raiz)->esq));
            rotacao_dir_playlist(raiz);
        }
        else if (fator_balanceamento_playlist(*raiz) == -2)
        {
            if (fator_balanceamento_playlist((*raiz)->dir) > 0)
                rotacao_dir_playlist(&((*raiz)->dir));
            rotacao_esq_playlist(raiz);
        }
    }
}