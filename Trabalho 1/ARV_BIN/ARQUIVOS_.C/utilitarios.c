#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// #include"../ARQUIVOS_.H/artista.h"  
// #include"../ARQUIVOS_.H/album.h"
// #include"../ARQUIVOS_.H/musica.h"
#include"../ARQUIVOS_.H/utilitarios.h"
// #include"../ARQUIVOS_.H/interface.h"
// #include"../ARQUIVOS_.H/playlist.h"

char *ler_string()//nunca lhe esquecerei (ou não)
{
    char buffer[256], *string;

    if (fgets(buffer, sizeof(buffer), stdin) == NULL)
        string = NULL;
    else
    {
        buffer[strcspn(buffer, "\n")] = 0;
        string = strdup(buffer);
    }
    return string;
}

void ler_string_simples(char *buffer, size_t tamanho)
{
    if (fgets(buffer, tamanho, stdin) != NULL)
    {
        buffer[strcspn(buffer, "\n")] = 0; // Remove o caractere de nova linha, se existir
    }
    else
    {
        buffer[0] = '\0'; // Garante que o buffer fique vazio em caso de erro
    }
}

int digitar_int()//muito bom
{
    int numero = 0;
    
    while (scanf("%d", &numero) != 1 || numero < 0)
    {
        printf("Numero invalido ");
        printf("Digite novamente: ");
    }
    limpar_buffer();
    
    return numero;
}

void limpar_buffer()
{
    while (getchar() != '\n');
    
}

void pausar()
{
    printf("\n\nPressione Enter para continuar...");
    int c;
    do {
        c = getchar();
    } while (c != '\n' && c != EOF); // Consome tudo até o Enter
}