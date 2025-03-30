#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct String{
    char *nome;
}String;

char* ler_string(){
    char buffer[256], *string;

    if(fgets(buffer, sizeof(buffer), stdin) == NULL)
        string = NULL;
    else{
        buffer[strcspn(buffer, "\n")] = 0;
        string = strdup(buffer);
    }
    return string;
}

int verifica_cadastro(char *artista){
    if(artista != NULL)
        return 1;
    else
        return 0;
}

void cadastrar_artista(char **artista){
    printf("Digite o nome do artista: ");
    *artista = ler_string();
    if((verifica_cadastro(*artista)) == 1)
        printf("Artista cadastrado com sucesso!\n");
    else
        printf("Erro ao cadastrar artista!\n");
}





int main() {
    char *nome;
    cadastrar_artista(&nome);
    if (nome) {
        printf("Nome digitado: %s\n", nome);
        free(nome); // Libera a memória alocada
    }
    return 0;
}