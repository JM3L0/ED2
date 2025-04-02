#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "artista.h"
#include "album.h"
#include "musica.h"
#include "utilitarios.h"

void menu_dos_print()
{
    printf("[1]Cadastrar Artista\n");
    printf("[2]Cadastrar Album\n");
    printf("[3]Cadastrar Musica\n");
    printf("[4]Cadastrar Playlist\n");
    printf("Digite a opcao desejada: ");
}

void menu_geral()
{
    int opcao;
    menu_dos_print();
    scanf("%d", &opcao);
    
    switch (opcao)
    {
    case 1:
        /* code */
        break;
    
    default:
        break;
    }
}