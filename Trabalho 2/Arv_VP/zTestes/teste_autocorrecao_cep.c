#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../Arv_VP_H/utilitarios_VP.h"
#include "../Arv_VP_H/STRUCTS_VP.h"

/*cd "c:\Users\jsous\OneDrive\Área de Trabalho\ED2\Trabalho 2"
gcc -o Arv_VP/zTestes/teste_autocorrecao_cep Arv_VP/zTestes/teste_autocorrecao_cep.c Arv_VP/Arv_VP_C/utilitarios.c -I Arv_VP/Arv_VP_H
Arv_VP\zTestes\teste_autocorrecao_cep*/

int main() {
    printf("===== TESTE DE AUTOCORRECAO DE CEP =====\n\n");
    
    char cep_entrada[50];
    char cep_corrigido[10];
    
    // Array de CEPs para testar
    const char* ceps_para_testar[] = {
        "12345678",      // Sem hifen
        "12345 678",     // Com espaco
        "12345.678",     // Com ponto
        "123-45678",     // Hifen na posicao errada
        "1234567",       // Poucos digitos
        "123456789",     // Muitos digitos
        "abcdefgh",      // Apenas letras
        "12345-678"      // Ja correto
    };
    
    for (int i = 0; i < sizeof(ceps_para_testar) / sizeof(char*); i++) {
        strcpy(cep_entrada, ceps_para_testar[i]);
        
        printf("Testando CEP: \"%s\"\n", cep_entrada);
        
        if (validar_cep(cep_entrada)) {
            printf("- O CEP ja esta no formato valido\n");
        } else if (autocorrigir_cep(cep_entrada, cep_corrigido)) {
            printf("- Corrigido para: \"%s\"\n", cep_corrigido);
            
            if (validar_cep(cep_corrigido)) {
                printf("- O CEP corrigido e valido\n");
            } else {
                printf("- ERRO: O CEP corrigido nao e valido!\n");
            }
        } else {
            printf("- Nao foi possivel corrigir o formato\n");
        }
        printf("\n");
    }
    
    // Teste interativo
    printf("===== TESTE INTERATIVO =====\n");
    printf("Digite um CEP (ou 'sair' para encerrar): ");
    
    while (1) {
        fgets(cep_entrada, sizeof(cep_entrada), stdin);
        cep_entrada[strcspn(cep_entrada, "\n")] = 0;
        
        if (strcmp(cep_entrada, "sair") == 0) {
            break;
        }
        
        if (validar_cep(cep_entrada)) {
            printf("CEP ja esta no formato valido: %s\n", cep_entrada);
        } else if (autocorrigir_cep(cep_entrada, cep_corrigido)) {
            printf("CEP corrigido: %s\n", cep_corrigido);
        } else {
            printf("CEP invalido e nao pode ser corrigido\n");
        }
        
        printf("\nDigite outro CEP (ou 'sair' para encerrar): ");
    }
    
    return 0;
}
