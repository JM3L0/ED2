#include <stdio.h>
#include <stdlib.h>

int main() {
    // Comando de compilação usando caminhos relativos
    const char *comando = "gcc -o artista ARV_BIN/ARQUIVOS_.C/artista.c ARV_BIN/ARQUIVOS_.C/utilitarios.c ARV_BIN/ARQUIVOS_.C/album.c ARV_BIN/ARQUIVOS_.C/musica.c -IARV_BIN/ARQUIVOS_.H";

    printf("Compilando o projeto...\n");

    // Executa o comando de compilação
    int retorno = system(comando);

    // Verifica se a compilação foi bem-sucedida
    if (retorno == 0) {
        printf("Compilação concluída com sucesso!\n");
    } else {
        printf("Erro durante a compilação. Código de retorno: %d\n", retorno);
        printf("Certifique-se de que está executando este programa no diretório principal do projeto.\n");
    }

    return 0;
}