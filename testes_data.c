#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Trabalho 2/Arv_VP/Arv_VP_H/STRUCTS.h"
#include "Trabalho 2/Arv_VP/Arv_VP_H/utilitarios.h"

/*cd "c:\Users\jsous\OneDrive\Área de Trabalho\ED2"
gcc -o testes_data testes_data.c "Trabalho 2\Arv_VP\Arv_VP_C\utilitarios.c" -I.
.\testes_data*/

// OBSERVACAO IMPORTANTE: A funcao validar_data em utilitarios.c 
// nao possui um return ao final. Isso precisa ser corrigido para:
// return valida;

// Funcao para testar eh_bissexto
void testar_eh_bissexto() {
    printf("\n===== TESTE DA FUNCAO eh_bissexto =====\n");
    
    // Anos bissextos
    int anos_bissextos[] = {2000, 2004, 2008, 2012, 2016, 2020, 2024};
    
    // Anos nao bissextos
    int anos_nao_bissextos[] = {1900, 2001, 2002, 2003, 2005, 2100};
    
    printf("Anos que DEVEM ser bissextos:\n");
    int testes_passados = 0;
    for (int i = 0; i < sizeof(anos_bissextos) / sizeof(int); i++) {
        int resultado = eh_bissexto(anos_bissextos[i]);
        printf("  Ano %d: %s", anos_bissextos[i], resultado ? "E bissexto [OK]" : "NAO e bissexto [X]");
        
        if (resultado) {
            testes_passados++;
            printf(" (CORRETO)\n");
        } else {
            printf(" (ERRO)\n");
        }
    }
    
    printf("\nAnos que NAO DEVEM ser bissextos:\n");
    for (int i = 0; i < sizeof(anos_nao_bissextos) / sizeof(int); i++) {
        int resultado = eh_bissexto(anos_nao_bissextos[i]);
        printf("  Ano %d: %s", anos_nao_bissextos[i], resultado ? "E bissexto [X]" : "NAO e bissexto [OK]");
        
        if (!resultado) {
            testes_passados++;
            printf(" (CORRETO)\n");
        } else {
            printf(" (ERRO)\n");
        }
    }
    
    int total_testes = sizeof(anos_bissextos) / sizeof(int) + sizeof(anos_nao_bissextos) / sizeof(int);
    printf("\nResultado: %d/%d testes passados\n", testes_passados, total_testes);
}

// Funcao para testar dias_no_mes
void testar_dias_no_mes() {
    printf("\n===== TESTE DA FUNCAO dias_no_mes =====\n");
    
    // Definir resultados esperados
    int dias_esperados_2020[] = {31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31}; // Ano bissexto
    int dias_esperados_2021[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31}; // Ano nao bissexto
    
    printf("Testando ano bissexto (2020):\n");
    int testes_passados = 0;
    int total_testes = 24; // 12 meses em 2 anos
    
    for (int mes = 1; mes <= 12; mes++) {
        int resultado = dias_no_mes(mes, 2020);
        int esperado = dias_esperados_2020[mes-1];
        
        printf("  Mes %2d: %2d dias", mes, resultado);
        
        if (resultado == esperado) {
            testes_passados++;
            printf(" [OK] (CORRETO)\n");
        } else {
            printf(" [X] (ERRO - esperado: %d)\n", esperado);
        }
    }
    
    printf("\nTestando ano nao bissexto (2021):\n");
    for (int mes = 1; mes <= 12; mes++) {
        int resultado = dias_no_mes(mes, 2021);
        int esperado = dias_esperados_2021[mes-1];
        
        printf("  Mes %2d: %2d dias", mes, resultado);
        
        if (resultado == esperado) {
            testes_passados++;
            printf(" [OK] (CORRETO)\n");
        } else {
            printf(" [X] (ERRO - esperado: %d)\n", esperado);
        }
    }
    
    printf("\nResultado: %d/%d testes passados\n", testes_passados, total_testes);
}

// Funcao para testar validar_data (requer que seja corrigida primeiro)
void testar_validar_data() {
    printf("\n===== TESTE DA FUNCAO validar_data =====\n");
    printf("AVISO: Para executar este teste, a funcao validar_data precisa retornar o valor 'valida'.\n");
    printf("Adicione 'return valida;' ao final da funcao validar_data em utilitarios.c.\n\n");
    
    // Casos de teste: datas validas
    DATA datas_validas[] = {
        {1, 1, 2000},     // 01/01/2000
        {29, 2, 2020},    // 29/02/2020 (bissexto)
        {31, 12, 1999},   // 31/12/1999
        {28, 2, 2021},    // 28/02/2021 (nao bissexto)
        {15, 7, 1950}     // 15/07/1950
    };
    
    // Casos de teste: datas invalidas
    DATA datas_invalidas[] = {
        {29, 2, 2021},    // 29/02/2021 (nao e bissexto)
        {31, 4, 2020},    // 31/04/2020 (abril tem 30 dias)
        {0, 5, 2020},     // 00/05/2020 (dia invalido)
        {10, 0, 2020},    // 10/00/2020 (mes invalido)
        {10, 13, 2020},   // 10/13/2020 (mes invalido)
        {31, 6, 2022}     // 31/06/2022 (junho tem 30 dias)
    };
    
    // Data futura para teste
    DATA data_futura;
    obter_data_atual(&data_futura);
    data_futura.ano += 1; // Um ano no futuro
    
    int testes_passados = 0;
    int total_testes = sizeof(datas_validas)/sizeof(DATA) + 
                       sizeof(datas_invalidas)/sizeof(DATA) + 1; // +1 para data futura
    
    printf("Testando datas que DEVEM ser validas:\n");
    for (int i = 0; i < sizeof(datas_validas)/sizeof(DATA); i++) {
        DATA data = datas_validas[i];
        int resultado = validar_data(&data);
        
        printf("  Data %02d/%02d/%04d: %s", 
               data.dia, data.mes, data.ano,
               resultado ? "Valida [OK]" : "Invalida [X]");
        
        if (resultado) {
            testes_passados++;
            printf(" (CORRETO)\n");
        } else {
            printf(" (ERRO - deveria ser valida)\n");
        }
    }
    
    printf("\nTestando datas que NAO DEVEM ser validas:\n");
    for (int i = 0; i < sizeof(datas_invalidas)/sizeof(DATA); i++) {
        DATA data = datas_invalidas[i];
        int resultado = validar_data(&data);
        
        printf("  Data %02d/%02d/%04d: %s", 
               data.dia, data.mes, data.ano,
               resultado ? "Valida [X]" : "Invalida [OK]");
        
        if (!resultado) {
            testes_passados++;
            printf(" (CORRETO)\n");
        } else {
            printf(" (ERRO - deveria ser invalida)\n");
        }
    }
    
    printf("\nTestando data futura (deve ser invalida):\n");
    int resultado = validar_data(&data_futura);
    printf("  Data %02d/%02d/%04d: %s", 
           data_futura.dia, data_futura.mes, data_futura.ano,
           resultado ? "Valida [X]" : "Invalida [OK]");
    
    if (!resultado) {
        testes_passados++;
        printf(" (CORRETO)\n");
    } else {
        printf(" (ERRO - uma data futura deveria ser invalida)\n");
    }
    
    printf("\nResultado: %d/%d testes passados\n", testes_passados, total_testes);
}

// Funcao para testar a obtencao e impressao da data atual
void testar_data_atual() {
    printf("\n===== TESTE DAS FUNCOES obter_data_atual e imprimir_data =====\n");
    
    DATA data;
    obter_data_atual(&data);
    
    printf("Data atual obtida: ");
    imprimir_data(data);
    
    printf("Verifique se a data acima corresponde a data de hoje.\n");
}

// Funcao principal para executar todos os testes
int main() {
    printf("============================================================\n");
    printf("             TESTES UNITARIOS DE FUNCOES DE DATA            \n");
    printf("============================================================\n");
    
    // Executar os testes
    testar_eh_bissexto();
    testar_dias_no_mes();
    testar_validar_data();
    testar_data_atual();
    
    printf("\n============================================================\n");
    printf("                    TESTES CONCLUIDOS                       \n");
    printf("============================================================\n");
    
    printf("\nOBSERVACAO IMPORTANTE: Para que os testes funcionem corretamente,\n");
    printf("a funcao validar_data em utilitarios.c deve retornar o valor 'valida'.\n");
    printf("Adicione 'return valida;' ao final da funcao.\n");
    
    return 0;
}
