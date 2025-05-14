#include <stdio.h>
#include <stdlib.h>

int main()
{
    // Comando de compilação usando caminhos relativos
    const char *comando = "gcc -o arvore_VP ../Arv_VP_C/main_VP.c ../Arv_VP_C/CEPs_VP.c ../Arv_VP_C/Cidades_VP.c ../Arv_VP_H/STRUCTS_VP.h ../Arv_VP_C/interface_VP.c ../Arv_VP_C/Estados_VP.c ../Arv_VP_C/utilitarios_VP.c ../Arv_VP_C/Pessoas_VP.c -I Arv_VP_H";
    //../Arv_VP_C/main_VP.c
    //../Arv_VP_C/TESTES_VP.c

    printf("Compilando o projeto...\n");

    // Executa o comando de compilação
    int retorno = system(comando);

    // Verifica se a compilação foi bem-sucedida
    if (retorno == 0)
    {
        printf("Compilacao concluida com sucesso!\n");
        system("arvore_VP.exe");
    }
    else
    {
        printf("Erro durante a compilacao. Codigo de retorno: %d\n", retorno);
        printf("Certifique-se de que esta executando este programa no diretorio principal do projeto.\n");
    }

    return 0;
}