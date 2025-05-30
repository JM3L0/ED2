#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../ARQUIVOS_BIN.H/utilitarios_BIN.h"

char *ler_string()
{
    char buffer[256], *string;
    string = NULL;

    if (fgets(buffer, sizeof(buffer), stdin) != NULL)
    {
        buffer[strcspn(buffer, "\n")] = 0;
        int i = 0;
        while (buffer[i] && (buffer[i] == ' ' || buffer[i] == '\t'))
            i++;
        if (buffer[i] != '\0')
        {
            int j = strlen(buffer) - 1;
            while (j >= i && (buffer[j] == ' ' || buffer[j] == '\t'))
                j--;
            buffer[j + 1] = '\0';
            string = strdup(buffer + i);
        }
    }
    return string;
}

void ler_string_simples(char *buffer, size_t tamanho)
{
    if (fgets(buffer, tamanho, stdin) != NULL)
    {
        buffer[strcspn(buffer, "\n")] = 0;
        if (strlen(buffer) == 0) {
            printf("Entrada invalida! Digite novamente: ");
            ler_string_simples(buffer, tamanho);
        }
    }
    else
        buffer[0] = '\0';
}

int digitar_int() // muito bom
{
    int numero = 0;

    while (scanf("%d", &numero) != 1 || numero < 0)
    {
        printf("Numero invalido ");
        printf("Digite novamente: ");
        limpar_buffer();
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
    do
    {
        c = getchar();
    } while (c != '\n' && c != EOF); // Consome tudo até o Enter
}

float digitar_float()
{
    float numero = 0;

    while (scanf("%f", &numero) != 1 || numero < 0)
    {
        printf("Numero invalido ");
        printf("Digite novamente: ");
        limpar_buffer();
    }
    limpar_buffer();
    return numero;
}

void mensagens_de_erro_e_sucesso_add_musica_playlist(int erro)
{ // PARA IDENTIFICAR O ERRO DO ADD MUSICA PLAYLIST
    switch (erro)
    {
    case 1:
        printf("\nMusica adicionada a playlist com sucesso\n");
        break;
    case 0:
        printf("\nMusica ja existente na playlist!\n");
        break;
    case 2:
        printf("\nERRO: Musica nao encontrada!\n");
        break;
    case 3:
        printf("\nERRO: Album nao encontrado!\n");
        break;
    case 4:
        printf("\nERRO: Artista nao encontrado!\n");
        break;
    case 5:
        printf("\nERRO: Erro ao alocar memoria para a musica na playlist!\n");
        break;
    default:
        printf("\nErro desconhecido!\n");
        break;
    }
}

int digitar_ano()
{
    int ano = 0;
    while (scanf("%d", &ano) != 1 || ano < 1850 || ano > 2025)
    {
        printf("Ano invalido! Digite novamente: ");
        limpar_buffer();
    }
    limpar_buffer();
    return ano;
}

void imprime_duracao(float duracao)
{
    int minutos = (int)duracao;
    float segundos = (duracao - minutos) * 100;

    if (segundos > 59) 
        segundos = 59;
    printf("%d:%02d\n", minutos, (int)segundos);
}