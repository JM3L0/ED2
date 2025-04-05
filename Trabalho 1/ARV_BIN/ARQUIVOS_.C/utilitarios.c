#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include"../ARQUIVOS_.H/artista.h"  
#include"../ARQUIVOS_.H/album.h"
#include"../ARQUIVOS_.H/musica.h"
#include"../ARQUIVOS_.H/utilitarios.h"
#include"../ARQUIVOS_.H/interface.h"
#include"../ARQUIVOS_.H/playlist.h"

char *ler_string()
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

int digitar_int()
{
    int numero = 0;
    
    while (scanf("%hd", &numero) != 1 || numero < 0)
    {
        printf("Numero invalido ");
        limpar_buffer();
        printf("Digite novamente: ");
    }
    
    return numero;
}

void limpar_buffer()
{
    while (getchar() != '\n');
    
}