#include <stdio.h>
#include <stdlib.h>

int main()
{
    // Comando de compilação usando caminhos relativos
    const char *comando = "gcc -o arvore_VP ../ARQUIVOS_VP.C/main_VP.c ../ARQUIVOS_VP.C/CEPs.c ../ARQUIVOS_VP.C/Cidades.c ../ARQUIVOS_VP.H/STRUCTS_VP.h ../ARQUIVOS_VP.C/interface_VP.c ../ARQUIVOS_VP.C/Estados.c ../ARQUIVOS_VP.C/utilitarios_VP.c ../ARQUIVOS_VP.C/Pessoas.c -I ARQUIVOS_VP.H";
    //../ARQUIVOS_VP.C/main_VP.c
    //../ARQUIVOS_VP.C/TESTES_VP.c

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