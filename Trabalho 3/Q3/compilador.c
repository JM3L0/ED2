#include <stdio.h>
#include <stdlib.h>

int main()
{
    const char *comando;
    const char *executavel;
    
    // Detectando o sistema operacional
    #ifdef _WIN32
        // Windows
        comando = "gcc -o Q3 Q3.c Util.c";
        executavel = "Q3.exe";
    #else
        // Linux, macOS ou outros sistemas Unix-like
        comando = "gcc Q3.c Util.c -o Q3";
        executavel = "./Q3";
    #endif

    printf("Compilando o projeto...\n");

    // Executa o comando de compilação
    int retorno = system(comando);

    // Verifica se a compilação foi bem-sucedida
    if (retorno == 0)
    {
        printf("Compilação concluída com sucesso!\n");
        system(executavel);
    }
    else
    {
        printf("Erro durante a compilação. Código de retorno: %d\n", retorno);
        printf("Certifique-se de que está executando este programa no diretório principal do projeto.\n");
        printf("Verifique se os arquivos Q3.c e Util.c estão presentes no diretório atual.\n");
        printf("Dica: Rode 'ls' (Linux/macOS) ou 'dir' (Windows) para listar os arquivos no diretório.\n");
    }

    return 0;
}