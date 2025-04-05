#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"../ARQUIVOS_.H/artista.h"  
#include"../ARQUIVOS_.H/album.h"
#include"../ARQUIVOS_.H/musica.h"
#include"../ARQUIVOS_.H/utilitarios.h"
#include"../ARQUIVOS_.H/interface.h"
#include"../ARQUIVOS_.H/playlist.h"

//TESTE SOCIAL
int main()
{
    ARTISTAS *raiz, *no;
    int inseriu;
    raiz = NULL;
    no = NULL;

    no = cadastrar_artista();
    inseriu = inserir_artista(&raiz, no);
    no = cadastrar_artista();
    inseriu = inserir_artista(&raiz, no);

    if (raiz != NULL)
    {
        // imprimir_todos_os_dados_do_artista(raiz);
        // imprimir_todos_artistas(raiz);
        // imprimir_artista_tipo(raiz, "solo");
        // imprimir_artista_estilo(raiz, "rock");
        imprimir_artista_estilo_tipo(raiz, "rock", "solo");
        free(raiz->nome_artista);
        free(raiz->tipo_artista);
        free(raiz->estilo_musical);
        free(raiz);
    }
    return 0;
}