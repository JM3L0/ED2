#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../Arv_VP_H/STRUCTS.h"
#include "../Arv_VP_H/utilitarios.h"

/*cd "c:\Users\jsous\OneDrive\Área de Trabalho\ED2\Trabalho 2"
gcc -o Arv_VP/zTestes/teste_cpf Arv_VP/zTestes/testes_cpf.c Arv_VP/Arv_VP_C/utilitarios.c -I Arv_VP/Arv_VP_H
Arv_VP\zTestes\teste_cpf*/

// Funcao para testar validar_cpf
void testar_validar_cpf() {
    printf("\n===== TESTE DA FUNCAO validar_cpf =====\n");
    
    // Casos de teste: CPFs validos (apenas formato, nao validacao real)
    const char* cpfs_validos[] = {
        "12345678901",    // 11 digitos
        "98765432109",
        "00000000000",
        "99999999999"
    };
    
    // Casos de teste: CPFs invalidos
    const char* cpfs_invalidos[] = {
        "1234567890",     // Apenas 10 digitos
        "123456789012",   // 12 digitos
        "1234567890a",    // Contém letra
        "123456-7890",    // Contém caractere não numérico
        "abcdefghijk",    // Só letras
        "123 456 789",    // Contém espaços
        "",               // Vazio
        "0"               // Um digito
    };
    
    int testes_passados = 0;
    int total_testes = sizeof(cpfs_validos) / sizeof(char*) + 
                       sizeof(cpfs_invalidos) / sizeof(char*);
    
    printf("Testando CPFs que DEVEM ser validos (formato):\n");
    for (int i = 0; i < sizeof(cpfs_validos) / sizeof(char*); i++) {
        int resultado = validar_cpf(cpfs_validos[i]);
        
        printf("  CPF %s: %s", 
               cpfs_validos[i],
               resultado ? "Valido [OK]" : "Invalido [X]");
        
        if (resultado) {
            testes_passados++;
            printf(" (CORRETO)\n");
        } else {
            printf(" (ERRO - deveria ser valido)\n");
        }
    }
    
    printf("\nTestando CPFs que NAO DEVEM ser validos:\n");
    for (int i = 0; i < sizeof(cpfs_invalidos) / sizeof(char*); i++) {
        int resultado = validar_cpf(cpfs_invalidos[i]);
        
        printf("  CPF %s: %s", 
               cpfs_invalidos[i],
               resultado ? "Valido [X]" : "Invalido [OK]");
        
        if (!resultado) {
            testes_passados++;
            printf(" (CORRETO)\n");
        } else {
            printf(" (ERRO - deveria ser invalido)\n");
        }
    }
    
    printf("\nResultado: %d/%d testes passados\n", testes_passados, total_testes);
}

// Funcao para testar a validacao avancada de CPF (digitos verificadores)
void testar_validacao_avancada_cpf() {
    printf("\n===== SUGESTAO: IMPLEMENTAR VALIDACAO AVANCADA DE CPF =====\n");
    printf("A funcao validar_cpf atual verifica apenas se o CPF tem 11 digitos numericos.\n");
    printf("Uma validacao completa tambem verificaria os digitos verificadores.\n");
    
    // CPFs validos (com digitos verificadores corretos)
    const char* cpfs_realmente_validos[] = {
        "52998224725",
        "87813726272",
        "21142450240"
    };
    
    // CPFs com formato valido mas digitos verificadores incorretos
    const char* cpfs_com_dv_invalidos[] = {
        "12345678900",  // Formato OK, mas DV errado
        "98765432100",  // Formato OK, mas DV errado
        "11111111111"   // CPF invalido (todos os digitos iguais)
    };
    
    printf("\nExemplos de CPFs que passariam em uma validacao completa:\n");
    for (int i = 0; i < sizeof(cpfs_realmente_validos) / sizeof(char*); i++) {
        printf("  - %s\n", cpfs_realmente_validos[i]);
    }
    
    printf("\nExemplos de CPFs que seriam rejeitados em uma validacao completa:\n");
    for (int i = 0; i < sizeof(cpfs_com_dv_invalidos) / sizeof(char*); i++) {
        printf("  - %s\n", cpfs_com_dv_invalidos[i]);
    }
    
    printf("\nAlgoritmo para validar CPF completamente:\n");
    printf("1. Verificar se tem 11 digitos numericos\n");
    printf("2. Verificar se nao sao todos os digitos iguais\n");
    printf("3. Calcular e verificar o primeiro digito verificador\n");
    printf("4. Calcular e verificar o segundo digito verificador\n");
    
    printf("\nImplementacao sugerida para o futuro!\n");
}

// Funcao principal para executar todos os testes
int main() {
    printf("============================================================\n");
    printf("             TESTES UNITARIOS DE FUNCOES DE CPF             \n");
    printf("============================================================\n");
    
    // Executar os testes
    testar_validar_cpf();
    testar_validacao_avancada_cpf();
    
    printf("\n============================================================\n");
    printf("                    TESTES CONCLUIDOS                       \n");
    printf("============================================================\n");
    
    return 0;
}
