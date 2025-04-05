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
    printf("\n[1]Cadastrar Artista\n");
    printf("[2]Cadastrar Album\n");
    printf("[3]Cadastrar Musica\n");
    printf("[4]Cadastrar Playlist\n");
    printf("Digite a opcao desejada: ");
}

void menu_geral()
{
    ARTISTAS *raiz, *no;
    raiz = NULL;
    no = NULL;

    int opcao, retorno = 0;
    
    while(opcao != 0)
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
                imprimir_todos_os_dados_do_artista(raiz);
                break;
            case 3:
                // cadastrar_musica();
                break;
            case 4:
                // cadastrar_playlist();
                break;
            default:
                printf("Opcao invalida!\n");
        }
    }
}