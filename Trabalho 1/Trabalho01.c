#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct PLAYLIST{
    char *titulo_playlist;
    char *nome_artista;
    char *titulo_album;
    char *titulo_musica;
    struct PLAYLIST *esq;
    struct PLAYLIST *dir;
}PLAYLIST;

typedef struct MUSICAS{
    char *titulo_musica;
    float quantidade_minutos;
    struct MUSICAS *esq;
    struct MUSICAS *dir;
}MUSICAS;

typedef struct ALBUNS{
    char *titulo_album;
    int ano_lancamento;
    int quantidade_musicas;
    struct ALBUNS *esq;
    struct ALBUNS *dir;
    struct MUSICAS *arv_musicas;
}ALBUNS;

typedef struct ARTISTAS{
    char *nome_artista;
    char *tipo_artista;
    char *estilo_musical;
    int numero_albuns;
    struct ARTISTAS *esq;
    struct ARTISTAS *dir;
    struct ALBUNS *arv_albuns;
}ARTISTAS;      

void menu(){
    printf("\n[1] - Cadastrar artista\n");
    printf("[2] - Cadastrar album\n");
    printf("[3] - Cadastrar musica\n");
    printf("[0] - Sair\n");
    printf("Escolha uma opcao: ");
}
aaaaaaaaaaaaaaaaaaaa
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