#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../ARQUIVOS_AVL.H/album_AVL.h"
#include "../ARQUIVOS_AVL.H/musica_AVL.h"
#include "../ARQUIVOS_AVL.H/utilitarios_AVL.h"

ALBUNS *aloca_no_album(char *titulo_album, int ano_lancamento)
{
    ALBUNS *novo = malloc(sizeof(ALBUNS));
    if (novo != NULL)
    {
        novo->titulo_album = titulo_album;
        novo->ano_lancamento = ano_lancamento;
        novo->quantidade_musicas = 0;
        novo->arv_musicas = NULL;
        novo->esq = NULL;
        novo->dir = NULL;
    }
    return (novo);
}

ALBUNS *cadastrar_album(ALBUNS *album)
{
    int ano_lancamento, erro = 0;
    char *titulo_album;

    printf("Digite o titulo do album: ");
    titulo_album = ler_string();

    if (titulo_album == NULL)
        erro = 1;
    if (!erro)
    {
        printf("Digite o ano de lancamento do Album: ");
        ano_lancamento = digitar_ano();
    }
    if (!erro)
    {
        album = aloca_no_album(titulo_album, ano_lancamento);
        if(album == NULL)
            erro = 1;
    }
    if (erro)
    {
        free(titulo_album);
        free(album);
        album = NULL;
    }
    return (album);
}

ALBUNS *existe_album(ALBUNS *raiz, char *titulo_album)
{ 
    ALBUNS *existe_alb;
    existe_alb = NULL;

    if (raiz != NULL)
    {
        if (strcasecmp(raiz->titulo_album, titulo_album) == 0)
            existe_alb = raiz;
        else if (strcasecmp(titulo_album, raiz->titulo_album) < 0)
            existe_alb = existe_album(raiz->esq, titulo_album);
        else
            existe_alb = existe_album(raiz->dir, titulo_album);
    }
    return (existe_alb);
}

int inserir_album(ALBUNS **raiz, ALBUNS *no)
{
    int inseriu = 1;

    if (*raiz == NULL)
        *raiz = no;
    else if (strcasecmp(no->titulo_album, (*raiz)->titulo_album) < 0)
        inseriu = inserir_album(&(*raiz)->esq, no);
    else if (strcasecmp(no->titulo_album, (*raiz)->titulo_album) > 0)
        inseriu = inserir_album(&(*raiz)->dir, no);
    else
    {
        limpar_no_album(no);
        free(no);
        no = NULL;
        inseriu = 0;
    }
    if(inseriu){
        balanceamento_album(raiz); // Balanceia a árvore após a inserção
        atualizar_altura_album(*raiz); // Atualiza a altura do nó raiz
    }
    return (inseriu);
}

/*---------------------------------- Funções de Imprimir ----------------------------------*/
void mostrar_dados_album(ALBUNS *raiz) // imprime os albuns de acordo com o tipo
{
    if (raiz != NULL)
    {
        printf("\n------\n");
        printf("> Album: %s\n", raiz->titulo_album);
        printf("  > Ano: %d\n", raiz->ano_lancamento);
        printf("  > Quantidade de musicas: %d\n\n", raiz->quantidade_musicas);
    }
}

int imprimir_todos_albuns(ALBUNS *raiz) // Imprime todos os albuns (só o nome)
{
    int imprimiu = 0;

    if (raiz != NULL)
    {
        imprimiu = imprimir_todos_albuns(raiz->esq);
        mostrar_dados_album(raiz);
        imprimiu = imprimir_todos_albuns(raiz->dir);
        imprimiu = 1;
    }
    return (imprimiu);
}

int imprimir_todos_albuns_de_um_artista(ARTISTAS *raiz, char *nome_artista) // imprime os artistas de acordo com o tipo
{                                                                           // FUNÇÃO FILTRO
    ARTISTAS *artista;
    artista = existe_artista(raiz, nome_artista);

    int imprimiu = 0;
    if (artista != NULL && artista->arv_albuns != NULL)
        imprimiu = imprimir_todos_albuns(artista->arv_albuns);
    return (imprimiu);
}

int imprimir_albuns_artita_ano(ARTISTAS *raiz, char *nome_artista, int ano_lancamento) // imprime os albuns de acordo com o tipo
{                                                                                      // FUNÇÃO FILTRO
    ARTISTAS *artista;
    artista = existe_artista(raiz, nome_artista);

    int imprimiu = 0;
    if (artista != NULL && artista->arv_albuns != NULL)
        imprimiu = imprimir_albuns_ano(artista->arv_albuns, ano_lancamento);
    return (imprimiu);
}

int imprimir_albuns_ano(ALBUNS *raiz, int ano_lancamento) // Imprime todos os albuns (só o nome)
{                                                         // FUNÇÃO FILTRO
    int imprimiu = 0;
    if (raiz != NULL)
    {
        imprimiu |= imprimir_albuns_ano(raiz->esq, ano_lancamento);
        if (raiz->ano_lancamento == ano_lancamento)
        {
            mostrar_dados_album(raiz);
            imprimiu = 1;
        }
        imprimiu |= imprimir_albuns_ano(raiz->dir, ano_lancamento);
    }
    return (imprimiu);
}

int mostrar_todos_artistas_album_ano(ARTISTAS *raiz, int ano_lancamento) // imprime os albuns de acordo com o ano
{
    int imprimiu = 0;

    if (raiz != NULL)
    {
        imprimiu |= mostrar_todos_artistas_album_ano(raiz->esq, ano_lancamento);
        if (raiz->arv_albuns != NULL)
        {
            printf("\nArtista: %s\n", raiz->nome_artista);
            int achou_ano = imprimir_albuns_ano(raiz->arv_albuns, ano_lancamento);
            if (achou_ano == 0)
                printf("O artista %s nao posssui album lancado no ano %d!\n", raiz->nome_artista, ano_lancamento);
            imprimiu |= achou_ano; // Atualiza imprimiu com o resultado da busca
        }
        imprimiu |= mostrar_todos_artistas_album_ano(raiz->dir, ano_lancamento);
    }
    return (imprimiu);
}

/*---------------------------------- Limpar Albuns ----------------------------------*/

void limpar_no_album(ALBUNS *no) // limpa os dados do album sem liberar o nó
{
    if (no != NULL)
    {
        free(no->titulo_album);
        no->titulo_album = NULL;
    }
}

void liberar_arv_album(ALBUNS **raiz) // limpa a arvore album
{
    if (*raiz != NULL)
    {
        if ((*raiz)->arv_musicas != NULL) // se a arvore de musicas não for nula
        {
            liberar_arv_musica(&(*raiz)->arv_musicas); // limpa a arvore de musicas
            (*raiz)->arv_musicas = NULL;               // limpa o ponteiro da arvore de musicas
        }
        liberar_arv_album(&(*raiz)->esq);
        liberar_arv_album(&(*raiz)->dir);
        limpar_no_album(*raiz);
        free(*raiz);
        *raiz = NULL;
    }
}

/*---------------------------------- Funções de Balanceamento ----------------------------------*/

int pegar_altura_album(ALBUNS *raiz)
{
    int altura = -1;

    if (raiz)
        altura = raiz->altura_album;
    return (altura);
}

void atualizar_altura_album(ALBUNS *raiz)
{
    if (raiz != NULL)
    {
        int altura_esq = pegar_altura_album(raiz->esq);
        int altura_dir = pegar_altura_album(raiz->dir);
        raiz->altura_album = (altura_esq > altura_dir ? altura_esq : altura_dir) + 1;
    }
}

int fator_balanceamento_album(ALBUNS *no)
{
    int fator = 0;

    if (no)
        fator = pegar_altura_album(no->esq) - pegar_altura_album(no->dir);
    return (fator);
}

void rotacao_esq_album(ALBUNS **raiz)
{
    ALBUNS *aux;

    aux = (*raiz)->dir;
    (*raiz)->dir = aux->esq;
    aux->esq = *raiz;
    (*raiz) = aux;
    atualizar_altura_album((*raiz)->esq);
    atualizar_altura_album((*raiz));
}

void rotacao_dir_album(ALBUNS **raiz)
{
    ALBUNS *aux;
    
    aux = (*raiz)->esq;
    (*raiz)->esq = aux->dir;
    aux->dir = *raiz;
    (*raiz) = aux;
    atualizar_altura_album((*raiz)->dir);
    atualizar_altura_album((*raiz));
}

void balanceamento_album(ALBUNS **raiz)
{
    if (*raiz)
    {
        if (fator_balanceamento_album(*raiz) == 2)
        {
            if (fator_balanceamento_album((*raiz)->esq) < 0)
                rotacao_esq_album(&((*raiz)->esq));
            rotacao_dir_album(raiz);
        }
        else if (fator_balanceamento_album(*raiz) == -2)
        {
            if (fator_balanceamento_album((*raiz)->dir) > 0)
                rotacao_dir_album(&((*raiz)->dir));
            rotacao_esq_album(raiz);
        }
    }
}