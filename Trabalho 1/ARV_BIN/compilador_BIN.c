#include <stdio.h>
#include <stdlib.h>

int main()
{
    const char *comando;
    const char *executavel;
    
    // Detectando o sistema operacional
    #ifdef _WIN32
        // Windows
        comando = "gcc -o arvore_binaria ../ARQUIVOS_BIN.C/playlist_BIN.c ../ARQUIVOS_BIN.C/musica_playlist_BIN.c ../ARQUIVOS_BIN.C/main_BIN.c ../ARQUIVOS_BIN.C/interface_BIN.c ../ARQUIVOS_BIN.C/artista_BIN.c ../ARQUIVOS_BIN.C/utilitarios_BIN.c ../ARQUIVOS_BIN.C/album_BIN.c ../ARQUIVOS_BIN.C/musica_BIN.c -I ARQUIVOS_.H";
        executavel = "arvore_binaria.exe";
    #else
        // Linux, macOS ou outros sistemas Unix-like
        comando = "gcc -o arvore_binaria ../ARQUIVOS_BIN.C/playlist_BIN.c ../ARQUIVOS_BIN.C/musica_playlist_BIN.c ../ARQUIVOS_BIN.C/main_BIN.c ../ARQUIVOS_BIN.C/interface_BIN.c ../ARQUIVOS_BIN.C/artista_BIN.c ../ARQUIVOS_BIN.C/utilitarios_BIN.c ../ARQUIVOS_BIN.C/album_BIN.c ../ARQUIVOS_BIN.C/musica_BIN.c -I ARQUIVOS_.H";
        executavel = "./arvore_binaria";
    #endif
    
    printf("Compilando o projeto...\n");

    // Executa o comando de compilação
    int retorno = system(comando);

    // Verifica se a compilação foi bem-sucedida
    if (retorno == 0)
    {
        printf("Compilacao concluida com sucesso!\n");
        system(executavel);
    }
    else
    {
        printf("Erro durante a compilacao. Codigo de retorno: %d\n", retorno);
        printf("Certifique-se de que esta executando este programa no diretorio principal do projeto.\n");
    }

    return 0;
}