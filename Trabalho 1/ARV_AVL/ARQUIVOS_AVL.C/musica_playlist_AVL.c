#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../ARQUIVOS_AVL.H/musica_playlist_AVL.h"
#include "../ARQUIVOS_AVL.H/STRUCTS_AVL.h"
#include "../ARQUIVOS_AVL.H/playlist_AVL.h"
#include "../ARQUIVOS_AVL.H/utilitarios_AVL.h"

MUSICA_PLAYLIST *alocar_musica_playlist(char *titulo_musica, float duracao_musica, char *nome_artista, char *titulo_album)
{
    MUSICA_PLAYLIST *nova_musica_play = malloc(sizeof(MUSICA_PLAYLIST));
    if (nova_musica_play != NULL)
    {
        nova_musica_play->titulo_musica = titulo_musica;
        nova_musica_play->duracao_musica = duracao_musica;
        nova_musica_play->artista_musica = nome_artista;
        nova_musica_play->album_musica = titulo_album;
        nova_musica_play->esq = NULL;
        nova_musica_play->dir = NULL;
    }
    return (nova_musica_play);
}
int inserir_musica_playlist(MUSICA_PLAYLIST **raiz, MUSICA_PLAYLIST *no)
{
    int inseriu = 1;

    if (*raiz == NULL)
        *raiz = no;
    else
    {
        // Compara título, álbum e artista como uma chave composta
        int cmp_titulo = strcasecmp(no->titulo_musica, (*raiz)->titulo_musica);
        int cmp_album = strcasecmp(no->album_musica, (*raiz)->album_musica);
        int cmp_artista = strcasecmp(no->artista_musica, (*raiz)->artista_musica);

        // Verifica se a musica ja existe (todos os campos iguais)
        if (cmp_titulo == 0 && cmp_album == 0 && cmp_artista == 0)
        {
            free(no);
            inseriu = 0;
        }
        else
        {
            // Decide a sub-arvore com base na chave composta
            if ((cmp_titulo < 0) || (cmp_titulo == 0 && cmp_album < 0) || (cmp_titulo == 0 && cmp_album == 0 && cmp_artista < 0))
                inseriu = inserir_musica_playlist(&(*raiz)->esq, no);
            else
                inseriu = inserir_musica_playlist(&(*raiz)->dir, no);
        }
    }

    if (inseriu)
    {
        balanceamento_musica_playlist(raiz);
        atualizar_altura_musica_playlist(*raiz);
    }

    return inseriu;
}

int verifica_se_musica_esta_na_playlist(MUSICA_PLAYLIST *raiz, char *titulo_musica, char *artista_musica, char *album_musica) // percorre todas as playlists e imprime as musicas
{
    int achou_mus = 0;
    if (raiz != NULL)
    {
        if (achou_mus == 0)
        {

            achou_mus |= verifica_se_musica_esta_na_playlist(raiz->esq, titulo_musica, artista_musica, album_musica);
            if (strcasecmp(raiz->titulo_musica, titulo_musica) == 0 && strcasecmp(raiz->artista_musica, artista_musica) == 0 && strcasecmp(raiz->album_musica, album_musica) == 0)
                achou_mus = 1;
            achou_mus |= verifica_se_musica_esta_na_playlist(raiz->dir, titulo_musica, artista_musica, album_musica);
        }
    }
    return (achou_mus);
}

int cadastrar_musica_playlist(ARTISTAS *raiz_artista, PLAYLIST *raiz_playlist)
{

    int retorno;

    char nome_artista[50];
    printf("Digite o nome do artista da musica que deseja adicionar: ");
    ler_string_simples(nome_artista, sizeof(nome_artista));
    ARTISTAS *artista = existe_artista(raiz_artista, nome_artista);

    if (artista)
    {
        char titulo_album[50];
        printf("Digite o titulo do album da musica que deseja adicionar: ");
        ler_string_simples(titulo_album, sizeof(titulo_album));
        ALBUNS *album = existe_album(artista->arv_albuns, titulo_album);

        if (album)
        {
            char titulo_musica[50];
            printf("Digite o titulo da musica a ser adicionada na playlist: ");
            ler_string_simples(titulo_musica, sizeof(titulo_musica));
            MUSICAS *musica = existe_musica(album->arv_musicas, titulo_musica);

            if (musica)
            {
                MUSICA_PLAYLIST *musica_playlist = alocar_musica_playlist(musica->titulo_musica, musica->duracao_musica, artista->nome_artista, album->titulo_album);
                if (musica_playlist != NULL)
                    retorno = inserir_musica_playlist(&raiz_playlist->arv_musicas_playlist, musica_playlist);
                else
                    retorno = 5; // Erro ao alocar memoria para a musica na playlist
            }
            else
                retorno = 2; // Musica nao encontrada!
        }
        else
            retorno = 3; // Album nao encontrado!
    }
    else
        retorno = 4; // Artista nao encontrado!

    return (retorno);
}
/*---------------------------------- Funções de Imprimir ----------------------------------*/

int imprime_todas_as_musicas_da_playlist(MUSICA_PLAYLIST *raiz) // imprime todas as musicas
{
    int imprimiu = 0;
    if (raiz != NULL)
    {
        imprimiu = imprime_todas_as_musicas_da_playlist(raiz->esq);
        printf("Musica: %s\n", raiz->titulo_musica);
        imprimiu = imprime_todas_as_musicas_da_playlist(raiz->dir);
        imprimiu = 1;
    }
    return (imprimiu);
}

/*-------------------------------------- remover musica da playlist* --------------------------------------*/

int eh_folha_musica_playlist(MUSICA_PLAYLIST *raiz)
{
    return (raiz->esq == NULL && raiz->dir == NULL);
}

MUSICA_PLAYLIST *so_um_filho_musica_playlist(MUSICA_PLAYLIST *raiz)
{
    MUSICA_PLAYLIST *filho = NULL;
    if (raiz->esq != NULL && raiz->dir == NULL)
        filho = raiz->esq;
    else if (raiz->esq == NULL && raiz->dir != NULL)
        filho = raiz->dir;
    return (filho);
}

int dois_filhos(MUSICA_PLAYLIST *raiz)
{
    return (raiz->esq != NULL && raiz->dir != NULL);
}

MUSICA_PLAYLIST *menor_musica_playlist(MUSICA_PLAYLIST *raiz)
{
    MUSICA_PLAYLIST *menor = raiz;
    while (menor->esq != NULL)
        menor = menor->esq;
    return (menor);
}

int remove_musica_playlist(MUSICA_PLAYLIST **raiz, char *titulo_musica, char *titulo_album, char *nome_artista)
{
    int removeu = 0;

    if (*raiz != NULL && titulo_musica != NULL && titulo_album != NULL && nome_artista != NULL)
    {
        int cmp_titulo = strcasecmp(titulo_musica, (*raiz)->titulo_musica);
        int cmp_album = strcasecmp(titulo_album, (*raiz)->album_musica);
        int cmp_artista = strcasecmp(nome_artista, (*raiz)->artista_musica);

        if (cmp_titulo == 0 && cmp_album == 0 && cmp_artista == 0)
        {
            MUSICA_PLAYLIST *aux = *raiz;
            MUSICA_PLAYLIST *filho;

            if (eh_folha_musica_playlist(*raiz))
                *raiz = NULL;
            else if ((filho = so_um_filho_musica_playlist(*raiz)) != NULL)
                *raiz = filho;
            else
            {
                MUSICA_PLAYLIST *menor = menor_musica_playlist((*raiz)->dir);

                // Copia os dados do menor nó
                (*raiz)->titulo_musica = menor->titulo_musica;
                (*raiz)->duracao_musica = menor->duracao_musica;
                (*raiz)->album_musica = menor->album_musica;
                (*raiz)->artista_musica = menor->artista_musica;

                // Remove o menor nó da subárvore direita usando os valores originais
                removeu = remove_musica_playlist(&(*raiz)->dir, menor->titulo_musica, menor->album_musica, menor->artista_musica);
                aux = menor; // O nó menor será liberado
            }

            free(aux);
            removeu = 1;
        }
        else
        {
            if ((cmp_titulo < 0) || (cmp_titulo == 0 && cmp_album < 0) || (cmp_titulo == 0 && cmp_album == 0 && cmp_artista < 0))
                removeu = remove_musica_playlist(&(*raiz)->esq, titulo_musica, titulo_album, nome_artista);
            else
                removeu = remove_musica_playlist(&(*raiz)->dir, titulo_musica, titulo_album, nome_artista);
            balanceamento_musica_playlist(raiz);
            atualizar_altura_musica_playlist(*raiz);
        }
    }

    return removeu;
}

/*-------------------------------- Liberar Arvore musica playlist --------------------------------*/

void liberar_arv_musica_playlist(MUSICA_PLAYLIST **raiz)
{
    if (*raiz != NULL)
    {
        liberar_arv_musica_playlist(&(*raiz)->esq);
        liberar_arv_musica_playlist(&(*raiz)->dir);
        free(*raiz);
        *raiz = NULL;
    }
}

/*---------------------------------- Funções de Balanceamento ----------------------------------*/

int pegar_altura_musica_playlist(MUSICA_PLAYLIST *raiz)
{
    int altura = -1;
    if (raiz)
    {
        altura = raiz->altura_musica_playlist;
    }
    return (altura);
}

void atualizar_altura_musica_playlist(MUSICA_PLAYLIST *raiz)
{
    if (raiz != NULL)
    {
        int altura_esq = pegar_altura_musica_playlist(raiz->esq);
        int altura_dir = pegar_altura_musica_playlist(raiz->dir);
        raiz->altura_musica_playlist = (altura_esq > altura_dir ? altura_esq : altura_dir) + 1;
    }
}

int fator_balanceamento_musica_playlist(MUSICA_PLAYLIST *no)
{
    int fator = 0;
    if (no)
    {
        fator = pegar_altura_musica_playlist(no->esq) - pegar_altura_musica_playlist(no->dir);
    }
    return (fator);
}

void rotacao_esq_musica_playlist(MUSICA_PLAYLIST **raiz)
{
    MUSICA_PLAYLIST *aux;
    aux = (*raiz)->dir;
    (*raiz)->dir = aux->esq;
    aux->esq = *raiz;
    (*raiz) = aux;
    atualizar_altura_musica_playlist((*raiz)->esq);
    atualizar_altura_musica_playlist((*raiz));
}

void rotacao_dir_musica_playlist(MUSICA_PLAYLIST **raiz)
{
    MUSICA_PLAYLIST *aux;
    aux = (*raiz)->esq;
    (*raiz)->esq = aux->dir;
    aux->dir = *raiz;
    (*raiz) = aux;
    atualizar_altura_musica_playlist((*raiz)->dir);
    atualizar_altura_musica_playlist((*raiz));
}

void balanceamento_musica_playlist(MUSICA_PLAYLIST **raiz)
{
    if (*raiz)
    {
        if (fator_balanceamento_musica_playlist(*raiz) == 2)
        {
            if (fator_balanceamento_musica_playlist((*raiz)->esq) < 0)
                rotacao_esq_musica_playlist(&((*raiz)->esq));
            rotacao_dir_musica_playlist(raiz);
        }
        else if (fator_balanceamento_musica_playlist(*raiz) == -2)
        {
            if (fator_balanceamento_musica_playlist((*raiz)->dir) > 0)
                rotacao_dir_musica_playlist(&((*raiz)->dir));
            rotacao_esq_musica_playlist(raiz);
        }
    }
}