#include <stdio.h>
#include <stdlib.h>

int main()
{
    const char *comando;
    const char *executavel;
    
    // Detectando o sistema operacional
    #ifdef _WIN32
        // Windows
        comando = "gcc -o arvore_AVL ../ARQUIVOS_AVL.C/main_AVL.c ../ARQUIVOS_AVL.C/playlist_AVL.c ../ARQUIVOS_AVL.C/musica_playlist_AVL.c ../ARQUIVOS_AVL.H/STRUCTS_AVL.h ../ARQUIVOS_AVL.C/interface_AVL.c ../ARQUIVOS_AVL.C/artista_AVL.c ../ARQUIVOS_AVL.C/utilitarios_AVL.c ../ARQUIVOS_AVL.C/album_AVL.c ../ARQUIVOS_AVL.C/musica_AVL.c -I ARQUIVOS_AVL.H";
        executavel = "arvore_AVL.exe";
    #else
        // Linux, macOS ou outros sistemas Unix-like
        comando = "gcc -o arvore_AVL ../ARQUIVOS_AVL.C/main_AVL.c ../ARQUIVOS_AVL.C/playlist_AVL.c ../ARQUIVOS_AVL.C/musica_playlist_AVL.c ../ARQUIVOS_AVL.H/STRUCTS_AVL.h ../ARQUIVOS_AVL.C/interface_AVL.c ../ARQUIVOS_AVL.C/artista_AVL.c ../ARQUIVOS_AVL.C/utilitarios_AVL.c ../ARQUIVOS_AVL.C/album_AVL.c ../ARQUIVOS_AVL.C/musica_AVL.c -I ARQUIVOS_AVL.H";
        executavel = "./arvore_AVL";
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