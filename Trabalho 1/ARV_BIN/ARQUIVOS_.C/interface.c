#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../ARQUIVOS_.H/artista.h"
#include "../ARQUIVOS_.H/album.h"
#include "../ARQUIVOS_.H/musica.h"
#include "../ARQUIVOS_.H/utilitarios.h"
#include "../ARQUIVOS_.H/interface.h"
#include "../ARQUIVOS_.H/playlist.h"

void menu_dos_print()
{
    printf("\n[1] - Cadastrar Artista\n");
    printf("[2] - Cadastrar Album\n");
    printf("[3] - Cadastrar Musica\n");
    printf("[4] - Mostrar todos os artistas cadastrados\n");
    printf("[5] - Mostrar todos os artistas cadastrados de um determinado tipo\n");
    printf("[6] - Mostrar todos os artistas cadastrados de um determinado estilo musical\n");
    printf("[7] - Mostrar todos os artistas cadastrados de um determinado estilo musical e tipo de artista\n");
    printf("[8] - Mostrar todos os álbuns de um determinado artista\n");
    printf("[9] - Mostrar todos os álbuns de um determinado ano de um artista\n");
    printf("[10] - Mostrar todas as músicas de um determinado álbum de um determinado artista\n");
    printf("[11] - Mostrar todos os álbuns de um determinado ano de todos os artistas cadastrados\n");
    printf("[12] - Mostrar  os  dados  de  uma  determinada  Música\n");
    printf("[13] - Cadastrar playlist\n");
    printf("[14] - Mostrar todos os dados de uma determinada playlist\n");
    printf("[15] - Remover uma música de uma determinada playlist\n");
    printf("[16] - Remover uma playlist\n");
    printf("[17] - Remover  uma  música  de  um  determinado  álbum  de  um  determinado  artista\n");
}

void menu_geral()
{
    ARTISTAS *raiz, *no;
    raiz = NULL;
    no = NULL;

    int opcao, retorno = 0;

    while (opcao != 0)
    {
        menu_dos_print();
        opcao = digitar_int();
        limpar_buffer(); // Limpa o buffer do teclado
        switch (opcao)
        {
        case 1:

            no = cadastrar_artista();
            retorno = inserir_artista(&raiz, no);
            break;
        case 2:
            retorno = imprimir_artista_estilo(raiz, "rock");
            if (retorno == 0)
            {
                printf("\nArvore de Artistas Vazia <!>\n");
            }

            break;
        case 3:
            limpar_arv_artista(&raiz);
            break;
        case 4:
            // cadastrar_playlist();
            break;
        default:
            printf("Opcao invalida!\n");
        }
    }
}