#include <stdio.h>
#include <stdlib.h>

int main()
{
    const char *comando;
    const char *executavel;
    
    // Detectando o sistema operacional
    #ifdef _WIN32
        // Windows
        comando = "gcc -o arvore_VP ../Arv_VP_C/CEPs_VP.c ../Arv_VP_C/Cidades_VP.c ../Arv_VP_C/interface_VP.c ../Arv_VP_C/Estados_VP.c ../Arv_VP_C/utilitarios_VP.c ../Arv_VP_C/Pessoas_VP.c -I Arv_VP_H";
        executavel = "arvore_VP.exe";
    #else
        // Linux, macOS ou outros sistemas Unix-like
        comando = "gcc -o arvore_VP ../Arv_VP_C/CEPs_VP.c ../Arv_VP_C/Cidades_VP.c ../Arv_VP_C/interface_VP.c ../Arv_VP_C/Estados_VP.c ../Arv_VP_C/utilitarios_VP.c ../Arv_VP_C/Pessoas_VP.c -I Arv_VP_H";
        executavel = "./arvore_VP";
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