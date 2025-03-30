#include <stdio.h>
#include <stdlib.h>
#include <string.h>



int main(){
    int opcao;

    do{
        menu();
        scanf("%d", &opcao);

        switch(opcao){
            case 1:
                printf("Cadastrar artista\n");
                break;
            case 2:
                printf("Cadastrar album\n");
                break;
            case 3:
                printf("Cadastrar musica\n");
                break;
            case 0:
                printf("Saindo...\n");
                break;
            default:
                printf("\nOpcao invalida! Tente novamente.\n");
        }
    }while(opcao != 0);

    return 0;
}