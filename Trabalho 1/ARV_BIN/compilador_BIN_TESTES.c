#include <stdio.h>
#include <stdlib.h>

int main()
{
    // Comando de compilação usando caminhos relativos
    const char *comando = "gcc -o arvore_binaria ../ARQUIVOS_BIN.C/TESTES_BIN.c ../ARQUIVOS_BIN.C/playlist_BIN.c ../ARQUIVOS_BIN.C/musica_playlist_BIN.c ../ARQUIVOS_BIN.C/interface_BIN.c ../ARQUIVOS_BIN.C/artista_BIN.c ../ARQUIVOS_BIN.C/utilitarios_BIN.c ../ARQUIVOS_BIN.C/album_BIN.c ../ARQUIVOS_BIN.C/musica_BIN.c -I ARQUIVOS_.H";
    //../ARQUIVOS_BIN.C/TESTES_BIN.c 

    printf("Compilando o projeto...\n");

    // Executa o comando de compilação
    int retorno = system(comando);

    // Verifica se a compilação foi bem-sucedida
    if (retorno == 0)
    {
        printf("Compilacao concluida com sucesso!\n");
        system("arvore_binaria.exe");
    }
    else
    {
        printf("Erro durante a compilacao. Codigo de retorno: %d\n", retorno);
        printf("Certifique-se de que esta executando este programa no diretorio principal do projeto.\n");
    }

    return 0;
}