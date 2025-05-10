/*
cd "c:\Users\raild\Downloads\UFPI\4° período\ED2\Trabalho 2\Arv_VP\Arv_VP_C"
gcc -o interface interface.c Estados.c Cidades.c CEPs.c Pessoas.c utilitarios.c -I "../Arv_VP_H"
.\interface
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../Arv_VP_H/CEPs.h"
#include "../Arv_VP_H/Cidades.h"
#include "../Arv_VP_H/Estados.h"
#include "../Arv_VP_H/Pessoas.h"
#include "../Arv_VP_H/STRUCTS.h"
#include "../Arv_VP_H/utilitarios.h"
#include "../Arv_VP_H/interface.h"

void menu_print()
{
    printf("\n[1] - Cadastrar estado\n");
    printf("[2] - Cadastrar cidade\n");
    printf("[3] - Cadastrar CEP\n");
    printf("[4] - Cadastrar pessoa\n");
    printf("[5] - Remover CEP\n");
    printf("[6] - Remover pessoa\n");
    printf("[7] - Imprimir estado mais populoso\n");
    printf("[8] - Imprimir populacao da Capital de um determinado estado\n");
    printf("[9] - Imprimir cidade mais populosa de um estado sem ser a Capital\n");
    printf("[10] - Imprimir quantas pessoas nao moram na cidade natal\n");
    printf("[11] - Imprimir qual cidade natal de uma pessoa dado o CEP da cidade\n");
    printf("[12] - Imprimir quantas pessoas nascidas em uma determinada cidade nao mora na cidade natal\n");
    printf("[13] - Imprimir quantas pessoas que moram em uma determinada cidade nao nasceram na cidade\n");
    printf("[0] - Sair\n");
}

void menu_geral()
{
    int opcao1, opcao2;

    do
    {
        menu_print();
        printf("Escolha uma opcao: ");
        opcao1 = digitar_int();
        printf("\n[1] - SIM\n[0] - NAO\nDeseja prosseguir com a acao?: ");
        opcao2 = digitar_int();
        if (opcao2 == 1)
        {
            switch (opcao1)
            {
            case 1:
                break;
            case 2:
                break;
            case 3:
                break;
            case 4:
                break;
            case 5:
                break;
            case 6:
                break;
            case 7:
                break;
            case 8:
                break;
            case 9:
                break;
            case 10:
                break;
            case 11:
                break;
            case 12:
                break;
            case 13:
                break;
            case 0:
                printf("Saindo do programa...\n");
                break;
            default:
                printf("Opção inválida! Tente novamente.\n");
                break;
            }
        }
    } while (opcao1 != 0);
}

int main()
{
    menu_geral();
    return 0;
}