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
    printf("[8] - Mostrar todos os albuns de um determinado artista\n");
    printf("[9] - Mostrar todos os albuns de um determinado ano de um artista\n");
    printf("[10] - Mostrar todas as musicas de um determinado album de um determinado artista\n");
    printf("[11] - Mostrar todos os albuns de um determinado ano de todos os artistas cadastrados\n");
    printf("[12] - Mostrar  os  dados  de  uma  determinada  Musica\n");
    printf("[13] - Cadastrar playlist\n");
    printf("[14] - Mostrar todos os dados de uma determinada playlist\n");
    printf("[15] - Remover uma musica de uma determinada playlist\n");
    printf("[16] - Remover uma playlist\n");
    printf("[17] - Remover  uma  musica  de  um  determinado  album  de  um  determinado  artista\n");
}

void menu_geral()
{
    ARTISTAS *raiz, *no;
    raiz = NULL;
    no = NULL;

    int opcao, retorno = 0;

    do
    {
        menu_dos_print();
        printf("\nDigite a opcao desejada: ");
        opcao = digitar_int();
        limpar_buffer();

        switch (opcao)
        {
        case 1:

            no = cadastrar_artista();
            retorno = inserir_artista(&raiz, no);

            if (retorno == 1)
                printf("\nArtista cadastrado com sucesso!\n");
            else
                printf("\nErro ao cadastrar artista!\n");
            break;

        case 2:
        {
            char nome_artista[50];
            ALBUNS *album;
            album = NULL;

            printf("Digite o nome do artista: ");
            ler_string_simples(nome_artista, sizeof(nome_artista));
            album = cadastrar_album();
            retorno = adicionar_album_a_artista(raiz, album, nome_artista);

            if (retorno == 1)
                printf("\nAlbum cadastrado com sucesso!\n");
            else
                printf("\nErro ao cadastrar album!\n");
        }
            break;
        case 3:

            break;
        case 4:

            printf("\n\n");
            printf("Artistas cadastrados:\n\n");
            printf("--------------------------------------------------\n");

            retorno = imprimir_todos_os_dados_dos_artistas(raiz);
            if (retorno == 0)
                printf("\nNenhum artista cadastrado!\n");
            pausar();
            break;
            
        case 5:
        {
            char tipo_artista[50];
            printf("Digite o tipo de artista: ");
            ler_string_simples(tipo_artista, sizeof(tipo_artista));

            printf("\n\n");
            printf("Artistas cadastrados do tipo %s:\n\n", tipo_artista);
            printf("--------------------------------------------------\n");

            retorno = imprimir_artista_tipo(raiz, tipo_artista); // tem que adicionar o tipo de artista
            if (retorno == 0)
                printf("\nNenhum artista desse tipo!\n");
            pausar();
            break;
        }
        case 6:
        {
            char estilo_musical[50];
            printf("Digite o estilo musical: ");
            ler_string_simples(estilo_musical, sizeof(estilo_musical));

            printf("\n\n");
            printf("Artistas cadastrados do estilo %s:\n\n", estilo_musical);
            printf("--------------------------------------------------\n");

            retorno = imprimir_artista_estilo(raiz, estilo_musical); // tem que adicionar o estilo musical
            if (retorno == 0)
                printf("\nNenhum artista desse estilo!\n");
            pausar();
            break;
        }
        case 7:
        {
            char estilo_musical[50], tipo_artista[50];
            printf("Digite o tipo de artista: ");
            ler_string_simples(tipo_artista, sizeof(tipo_artista));
            printf("Digite o estilo musical: ");
            ler_string_simples(estilo_musical, sizeof(estilo_musical));

            printf("\n\n");
            printf("Artistas cadastrados do estilo %s e tipo %s:\n\n", estilo_musical, tipo_artista);
            printf("--------------------------------------------------\n");

            retorno = imprimir_artista_estilo_e_tipo(raiz, estilo_musical, tipo_artista); // tem que adicionar o estilo musical e o tipo de artista
            if (retorno == 0)
                printf("\nNenhum artista desse estilo e tipo!\n");
            pausar();
            break;
        }
        case 8:
            // imprimir_albuns_artista(raiz);
            break;
        case 9:
            // imprimir_albuns_ano_artista(raiz);
            break;
        case 10:
            // imprimir_musicas_album_artista(raiz);
            break;
        case 11:
            // imprimir_albuns_ano_todos(raiz);
            break;
        case 12:
            // imprimir_dados_musica(raiz);
            break;
        case 13:
            // cadastrar_playlist();
            break;
        case 14:
            // imprimir_dados_playlist();
            break;
        case 15:
            // remover_musica_playlist();
            break;
        case 16:
            // remover_playlist();
            break;
        case 17:
            // remover_musica_album_artista(raiz);
            break;
        default:
            printf("Opcao invalida!\n");
        }
    } while (opcao != 0);
}
