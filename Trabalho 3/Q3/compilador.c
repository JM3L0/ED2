#include <stdio.h>
#include <stdlib.h>

int main()
{
    const char *comando;
    const char *executavel;
    
    #ifdef _WIN32
        comando = "gcc -o Q3 ../Q3.c ../Util.c";
        executavel = "Q3";
    #else
        comando = "gcc -o Q3 ../Q3.c ../Util.c";
        executavel = "./Q3";
    #endif

    printf("Compilando o projeto...\n");

    int retorno = system(comando);

    if (retorno == 0)
    {
        printf("Compilacao concluida com sucesso!\n");
        system(executavel);
    }
    else
    {
        printf("Erro durante a compilacao. Codigo de retorno: %d\n", retorno);
    }

    return 0;
}