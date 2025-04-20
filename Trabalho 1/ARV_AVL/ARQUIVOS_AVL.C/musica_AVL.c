#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../ARQUIVOS_AVL.H/musica_AVL.h"
#include "../ARQUIVOS_AVL.H/utilitarios_AVL.h"

MUSICAS *alocar_musica(char *titulo_musica, float duracao_musica)
{
    MUSICAS *nova_musica = malloc(sizeof(MUSICAS));

    if (nova_musica != NULL)
    {
        nova_musica->titulo_musica = titulo_musica;
        nova_musica->duracao_musica = duracao_musica;
        nova_musica->esq = NULL;
        nova_musica->dir = NULL;
    }
    return (nova_musica);
}

MUSICAS *cadastrar_musica(MUSICAS *musica)
{
    float duracao_musica;
    char *titulo_musica;
    int erro = 0;

    printf("Digite o titulo da musica: ");
    titulo_musica = ler_string();

    if (titulo_musica == NULL)
        erro = 1;
    if(!erro)
    {
        printf("Digite a duracao da Musica: ");
        duracao_musica = digitar_float();
    }

    if (!erro){
        musica = alocar_musica(titulo_musica, duracao_musica);
        if (musica == NULL)
            erro = 1;
    }

    if (erro)
    {
        free(titulo_musica);
        free(musica);
        musica = NULL;
    }
    return (musica);
}

int inserir_musica(MUSICAS **raiz, MUSICAS *no)
{
    int inseriu = 1;

    if (*raiz == NULL)
        *raiz = no;
    else if (strcasecmp(no->titulo_musica, (*raiz)->titulo_musica) < 0)
        inseriu = inserir_musica(&(*raiz)->esq, no);
    else if (strcasecmp(no->titulo_musica, (*raiz)->titulo_musica) > 0)
        inseriu = inserir_musica(&(*raiz)->dir, no);
    else
    {
        limpar_no_musica(no);
        free(no);
        no = NULL;
        inseriu = 0;
    }
    
    if (inseriu ){
        balanceamento_musica(raiz); // Chama a função de balanceamento após a inserção
        atualizar_altura_musica(*raiz); // Atualiza a altura do nó após a inserção
    }
    return (inseriu);
}

MUSICAS *existe_musica(MUSICAS *raiz, char *titulo_musica)
{ // sera utilizado para as musicas
    MUSICAS *existe_mus;
    existe_mus = NULL;

    if (raiz != NULL)
    {
        if (strcasecmp(raiz->titulo_musica, titulo_musica) == 0)
            existe_mus = raiz;
        else if (strcasecmp(titulo_musica, raiz->titulo_musica) < 0)
            existe_mus = existe_musica(raiz->esq, titulo_musica);
        else
            existe_mus = existe_musica(raiz->dir, titulo_musica);
    }
    return (existe_mus);
}

/*---------------------------------- Funções de Imprimir ----------------------------------*/

int imprimir_todas_as_musicas(MUSICAS *raiz) // imprime todas as musicas
{
    int imprimiu = 0;

    if (raiz != NULL)
    {
        imprimiu = imprimir_todas_as_musicas(raiz->esq);
        printf("\n\n");
        printf("Musica: %s\n", raiz->titulo_musica);
        //printf("Duracao: %.2f minutos\n", raiz->duracao_musica);
        printf("Duracao: ");
        imprime_duracao(raiz->duracao_musica);
        printf("Altura: %d\n", raiz->altura_musica);
        imprimiu = imprimir_todas_as_musicas(raiz->dir);
        imprimiu = 1;
    }
    return (imprimiu);
}

int imprime_dados_da_musica_album_artista(ARTISTAS *raiz_art, char *nome_musica)
{
    int imprimiu = 0;

    if (raiz_art != NULL)
    {
        imprimiu |= imprime_dados_da_musica_album_artista(raiz_art->esq, nome_musica);
        if (raiz_art->arv_albuns != NULL)
        {
            int encontrou_musica = imprime_dados_da_musica_album(raiz_art->arv_albuns, nome_musica);
            if (encontrou_musica)
            {
                printf("\n> ARTISTA: %s\n", raiz_art->nome_artista);
                printf("================================\n");
            }

            imprimiu |= encontrou_musica; // Atualiza imprimiu com o resultado da busca
        }
        imprimiu |= imprime_dados_da_musica_album_artista(raiz_art->dir, nome_musica);
    }
    return (imprimiu);
}

int imprime_dados_da_musica_album(ALBUNS *raiz_alb, char *nome_musica)
{
    int imprimiu = 0;

    if (raiz_alb != NULL)
    {
        imprimiu |= imprime_dados_da_musica_album(raiz_alb->esq, nome_musica);
        if (raiz_alb->arv_musicas != NULL)
        {
            int encontrou_musica = imprime_dados_da_musica_buscada(raiz_alb->arv_musicas, nome_musica);
            if (encontrou_musica)
            {
                printf("     Album: %s\n", raiz_alb->titulo_album);
                printf("     Ano de lancamento: %4d\n", raiz_alb->ano_lancamento);
                printf("---\n");
            }

            imprimiu |= encontrou_musica; // Atualiza imprimiu com o resultado da busca
        }
        imprimiu |= imprime_dados_da_musica_album(raiz_alb->dir, nome_musica);
    }
    return (imprimiu);
}

int imprime_dados_da_musica_buscada(MUSICAS *raiz_mus, char *nome_musica)
{
    int imprimiu = 0;

    if (raiz_mus != NULL)
    {
        imprimiu |= imprime_dados_da_musica_buscada(raiz_mus->esq, nome_musica);
        if (strcasecmp(raiz_mus->titulo_musica, nome_musica) == 0)
        {
            printf("\n\n");
            printf(" > Musica: %s\n", raiz_mus->titulo_musica);                // 1 espaço
            printf("     Duracao: "); // 5 espaços
            imprime_duracao(raiz_mus->duracao_musica);
            imprimiu = 1;
        }
        imprimiu |= imprime_dados_da_musica_buscada(raiz_mus->dir, nome_musica);
    }
    return (imprimiu);
}

/*---------------------------------- Funções de limpar ----------------------------------*/

void limpar_no_musica(MUSICAS *no) // limpa os dados da musica sem liberar o nó
{
    if (no != NULL)
    {
        free(no->titulo_musica);
        no->titulo_musica = NULL;
    }
}

void liberar_arv_musica(MUSICAS **raiz) // limpa a arvore musica
{
    if (*raiz != NULL)
    {
        liberar_arv_musica(&(*raiz)->esq);
        liberar_arv_musica(&(*raiz)->dir);
        limpar_no_musica(*raiz);
        free(*raiz);
        *raiz = NULL;
    }
}

/*---------------------------------- Funções de Balancear ----------------------------------*/

int eh_folha_musica(MUSICAS *raiz)
{
    int eh_folha = 0;

    if (raiz != NULL)
    {
        if (raiz->esq == NULL && raiz->dir == NULL)
            eh_folha = 1;
        else
            eh_folha = 0;
    }
    return (eh_folha);
}

MUSICAS *so_um_filho_musica(MUSICAS *raiz)
{
    MUSICAS *no;
    no = NULL;

    if (raiz != NULL)
    {
        if (raiz->esq == NULL && raiz->dir != NULL)
            no = raiz->dir;
        else if (raiz->esq != NULL && raiz->dir == NULL)
            no = raiz->esq;
    }
    return (no);
}

int dois_filhos_musica(MUSICAS *raiz)
{
    return (raiz->esq != NULL && raiz->dir != NULL);
}

MUSICAS *menor_no_musica(MUSICAS *raiz)
{
    MUSICAS *menor;
    menor = raiz;
    while (menor->esq != NULL)
        menor = menor->esq;
    return (menor);
}

int remove_musica(MUSICAS **raiz, char *titulo_musica)
{
    int removeu = 1;
    
    if (*raiz != NULL)
    {
        if (strcasecmp((*raiz)->titulo_musica, titulo_musica) == 0)
        {
            MUSICAS *aux, *filho;
            aux = *raiz;

            if (eh_folha_musica(*raiz))
                *raiz = NULL;
            else if ((filho = so_um_filho_musica(*raiz)) != NULL)
                *raiz = filho;
            else
            {
                aux = menor_no_musica((*raiz)->dir);
                limpar_no_musica(*raiz);

                (*raiz)->titulo_musica = aux->titulo_musica;
                (*raiz)->duracao_musica = aux->duracao_musica;

                remove_musica(&(*raiz)->dir, aux->titulo_musica);
            }

            limpar_no_musica(aux);
            free(aux);
        }
        else if (strcasecmp(titulo_musica, (*raiz)->titulo_musica) < 0)
            removeu = remove_musica(&(*raiz)->esq, titulo_musica);
        else
            removeu = remove_musica(&(*raiz)->dir, titulo_musica);
        balanceamento_musica(raiz); // Chama a função de balanceamento após a remoção
        atualizar_altura_musica(*raiz); // Atualiza a altura do nó após a remoção
    }
    else
        removeu = 0;
    return (removeu);
}

/*---------------------------------- Funções de Balanceamento ----------------------------------*/

int pegar_altura_musica(MUSICAS *raiz)
{
    int altura = -1;

    if (raiz)
        altura = raiz->altura_musica;
    return (altura);
}

void atualizar_altura_musica(MUSICAS *raiz)
{
    if (raiz != NULL)
    {
        int altura_esq = pegar_altura_musica(raiz->esq);
        int altura_dir = pegar_altura_musica(raiz->dir);
        raiz->altura_musica = (altura_esq > altura_dir ? altura_esq : altura_dir) + 1;
    }
}

int fator_balanceamento_musica(MUSICAS *no)
{
    int fator = 0;

    if (no)
        fator = pegar_altura_musica(no->esq) - pegar_altura_musica(no->dir);
    return (fator);
}

void rotacao_esq_musica(MUSICAS **raiz)
{
    MUSICAS *aux;
    aux = (*raiz)->dir;
    (*raiz)->dir = aux->esq;
    aux->esq = *raiz;
    (*raiz) = aux;
    atualizar_altura_musica((*raiz)->esq);
    atualizar_altura_musica((*raiz));
}

void rotacao_dir_musica(MUSICAS **raiz)
{
    MUSICAS *aux;
    aux = (*raiz)->esq;
    (*raiz)->esq = aux->dir;
    aux->dir = *raiz;
    (*raiz) = aux;
    atualizar_altura_musica((*raiz)->dir);
    atualizar_altura_musica((*raiz));
}

void balanceamento_musica(MUSICAS **raiz)
{
    if (*raiz)
    {
        if (fator_balanceamento_musica(*raiz) == 2)
        {
            if (fator_balanceamento_musica((*raiz)->esq) < 0)
                rotacao_esq_musica(&((*raiz)->esq));
            rotacao_dir_musica(raiz);
        }
        else if (fator_balanceamento_musica(*raiz) == -2)
        {
            if (fator_balanceamento_musica((*raiz)->dir) > 0)
                rotacao_dir_musica(&((*raiz)->dir));
            rotacao_esq_musica(raiz);
        }
    }
}
