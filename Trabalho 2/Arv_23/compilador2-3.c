#include <stdio.h>
#include <stdlib.h>

int main()
{
    const char *comando;
    const char *executavel;
    
    // Detectando o sistema operacional
    #ifdef _WIN32
        // Windows
        comando = "gcc -o Arv_23 ../Arv_23_C/Interface_23.c ../Arv_23_C/Estados_23.c ../Arv_23_C/Cidades_23.c ../Arv_23_C/CEPs_23.c ../Arv_23_C/Pessoas_23.c ../Arv_23_C/utilitarios_23.c -IArv_23_H";
        executavel = "Arv_23.exe";
    #else
        // Linux, macOS ou outros sistemas Unix-like
        comando = "gcc -o Arv_23 ../Arv_23_C/Interface_23.c ../Arv_23_C/Estados_23.c ../Arv_23_C/Cidades_23.c ../Arv_23_C/CEPs_23.c ../Arv_23_C/Pessoas_23.c ../Arv_23_C/utilitarios_23.c -IArv_23_H";
        executavel = "./Arv_23";
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