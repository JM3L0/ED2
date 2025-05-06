#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../Arv_VP_H/STRUCTS.h"
#include "../Arv_VP_H/utilitarios.h"

/*cd "c:\Users\jsous\OneDrive\Área de Trabalho\ED2\Trabalho 2"
gcc -o Arv_VP/zTestes/teste_cep Arv_VP/zTestes/testes_cep.c Arv_VP/Arv_VP_C/utilitarios.c -I Arv_VP/Arv_VP_H
Arv_VP\zTestes\teste_cep*/

// Funcao para testar validar_cep
void testar_validar_cep() {
    printf("\n===== TESTE DA FUNCAO validar_cep =====\n");
    
    // Casos de teste: CEPs validos (formato XXXXX-XXX)
    const char* ceps_validos[] = {
        "12345-678",    // Formato padrão
        "00000-000",
        "99999-999",
        "54321-876"
    };
    
    // Casos de teste: CEPs invalidos
    const char* ceps_invalidos[] = {
        "1234-5678",     // Hífen na posição errada
        "12345678",      // Sem hífen
        "123456-78",     // Hífen na posição errada
        "12345-67",      // Comprimento incorreto
        "1234-567",      // Comprimento incorreto
        "abcde-fgh",     // Letras em vez de números
        "12345-abc",     // Letras após o hífen
        "abc45-678",     // Letras antes do hífen
        "12345 678",     // Espaço em vez de hífen
        "12345.678",     // Ponto em vez de hífen
        "-",             // Apenas hífen
        "",              // Vazio
        "12345-67X"      // Caractere não numérico
    };
    
    int testes_passados = 0;
    int total_testes = sizeof(ceps_validos) / sizeof(char*) + 
                       sizeof(ceps_invalidos) / sizeof(char*);
    
    printf("Testando CEPs que DEVEM ser validos:\n");
    for (int i = 0; i < sizeof(ceps_validos) / sizeof(char*); i++) {
        int resultado = validar_cep(ceps_validos[i]);
        
        printf("  CEP %s: %s", 
               ceps_validos[i],
               resultado ? "Valido [OK]" : "Invalido [X]");
        
        if (resultado) {
            testes_passados++;
            printf(" (CORRETO)\n");
        } else {
            printf(" (ERRO - deveria ser valido)\n");
        }
    }
    
    printf("\nTestando CEPs que NAO DEVEM ser validos:\n");
    for (int i = 0; i < sizeof(ceps_invalidos) / sizeof(char*); i++) {
        int resultado = validar_cep(ceps_invalidos[i]);
        
        printf("  CEP %s: %s", 
               ceps_invalidos[i],
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

// Funcao para testar validacao adicional de CEP
void testar_validacao_adicional_cep() {
    printf("\n===== SUGESTAO: MELHORIAS NA VALIDACAO DE CEP =====\n");
    printf("A funcao validar_cep atual verifica apenas o formato XXXXX-XXX.\n");
    printf("Melhorias possiveis para a validacao de CEP:\n");
    
    printf("\n1. Verificar se o CEP existe na base dos Correios\n");
    printf("   - Isso exigiria uma API ou banco de dados dos Correios\n");
    
    printf("\n2. Validar faixas de CEP por regiao\n");
    printf("   - Exemplo: CEPs de 01000-000 a 05999-999 sao de Sao Paulo/SP\n");
    printf("   - Exemplo: CEPs de 20000-000 a 23799-999 sao do Rio de Janeiro/RJ\n");
    
    printf("\n3. Implementar autocorrecao de formatos comuns\n");
    printf("   - Exemplo: Converter \"12345678\" para \"12345-678\"\n");
    printf("   - Exemplo: Remover espaços: \"12345 678\" para \"12345-678\"\n");
    
    printf("\nEstas sugestoes podem melhorar a experiencia do usuario\n");
    printf("e reduzir erros na entrada de dados.\n");
}

// Funcao principal para executar todos os testes
int main() {
    printf("============================================================\n");
    printf("             TESTES UNITARIOS DE FUNCOES DE CEP             \n");
    printf("============================================================\n");
    
    // Executar os testes
    testar_validar_cep();
    testar_validacao_adicional_cep();
    
    printf("\n============================================================\n");
    printf("                    TESTES CONCLUIDOS                       \n");
    printf("============================================================\n");
    
    return 0;
}
