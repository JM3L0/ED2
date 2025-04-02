#include <stdio.h>
#include <stdlib.h>

int main()
{
    // Comando de compilação usando caminhos relativos
    const char *comando = "gcc -o artista ../ARQUIVOS_.C/artista.c ../ARQUIVOS_.C/utilitarios.c ../ARQUIVOS_.C/album.c ../ARQUIVOS_.C/musica.c -I ARQUIVOS_.H";

    printf("Compilando o projeto...\n");

    // Executa o comando de compilação
    int retorno = system(comando);

    // Verifica se a compilação foi bem-sucedida
    if (retorno == 0)
    {
        printf("Compilacao concluida com sucesso!\n");
        system("artista.exe");
    }
    else
    {
        printf("Erro durante a compilacao. Codigo de retorno: %d\n", retorno);
        printf("Certifique-se de que esta executando este programa no diretorio principal do projeto.\n");
    }

    return 0;
}