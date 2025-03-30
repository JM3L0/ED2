#include <stdio.h>
#include <string.h>
#include <stdlib.h>

<<<<<<< HEAD
typedef struct String{
    char *nome;
}String;

int main(){

    String ab;

    scanf("%c", &ab.nome);

    printf("%c", ab.nome);
=======
char* ler_string() {
    char buffer[256]; // Buffer temporário de 256 caracteres
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) { // Lê a entrada
        printf("Erro ao ler a entrada!\n");
        return NULL; // Retorna NULL em caso de erro
    }
    
    buffer[strcspn(buffer, "\n")] = 0; // Remove o '\n' do final
    
    char* string = strdup(buffer); // Aloca dinamicamente e copia o conteúdo
    //se a alocação falhar ele retorna null
    if (!string) {
        printf("Erro de alocação de memória!\n");
        exit(1);
    }
    
    return string; // Retorna o ponteiro para a string alocada
}
>>>>>>> a90d80368cdd220c13130b43e2c70bb7b1eb013c

int main() {
    printf("Digite um nome: ");
    char* nome = ler_string();
    if (nome) {
        printf("Nome digitado: %s\n", nome);
        free(nome); // Libera a memória alocada
    }
    return 0;
}