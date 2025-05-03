#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "../Arv_VP_H/STRUCTS.h"


////////////////////////////////////////////// DATA ////////////////////////////////////////////// 
// Verifica se um ano é bissexto
int eh_bissexto(int ano) {
    int resultado = (ano % 4 == 0 && ano % 100 != 0) || (ano % 400 == 0);
    return resultado;
}

// Retorna o número de dias em um mês, considerando ano bissexto
int dias_no_mes(int mes, int ano) {
    int dias;
    if (mes == 2) {
        dias = eh_bissexto(ano) ? 29 : 28;
    } else if (mes == 4 || mes == 6 || mes == 9 || mes == 11) {
        dias = 30;
    } else {
        dias = 31;
    }
    return dias;
}

// Valida uma data (dia, mês, ano)
int validar_data(DATA *data) {
    int valida = 1;
    if (data->ano < 1900 || data->ano > 2025) {
        valida = 0;
    } else if (data->mes < 1 || data->mes > 12) {
        valida = 0;
    } else if (data->dia < 1 || data->dia > dias_no_mes(data->mes, data->ano)) {
        valida = 0;
    }
    return valida;
}

// Captura e valida a data de nascimento do usuário
int capturar_data(DATA *data) {
    int sucesso = 0;
    char entrada[11];
    printf("Digite a data de nascimento (DD/MM/AAAA): ");
    if (fgets(entrada, sizeof(entrada), stdin)) {
        entrada[strcspn(entrada, "\n")] = '\0';
        if (sscanf(entrada, "%2d/%2d/%4d", &data->dia, &data->mes, &data->ano) == 3) {
            if (validar_data(data)) {
                sucesso = 1;
            } else {
                printf("Erro: data inválida!\n");
            }
        } else {
            printf("Erro: formato inválido!\n");
        }
    }
    return sucesso;
}

////////////////////////////////////////////// CPF //////////////////////////////////////////////

// Valida o formato do CPF (11 dígitos numéricos)
int validar_cpf(const char *cpf) {
    int valido = 0;
    if (cpf && strlen(cpf) == 11) {
        int todos_digitos = 1;
        for (int i = 0; i < 11 && todos_digitos; i++) {
            if (!isdigit(cpf[i])) {
                todos_digitos = 0;
            }
        }
        valido = todos_digitos;
    }
    return valido;
}

// Captura o CPF do usuário e valida o formato
int capturar_cpf(char *cpf) {
    int sucesso = 0;
    char entrada[13]; // 11 dígitos + \n + \0
    printf("Digite o CPF (11 dígitos, sem pontos ou traços): ");
    if (fgets(entrada, sizeof(entrada), stdin)) {
        entrada[strcspn(entrada, "\n")] = '\0';
        if (validar_cpf(entrada)) {
            strcpy(cpf, entrada);
            sucesso = 1;
        } else {
            printf("Erro: CPF deve ter 11 dígitos numéricos!\n");
        }
    } else {
        printf("Erro: falha na leitura!\n");
    }
    return sucesso;
}

////////////////////////////////////////////// CEP //////////////////////////////////////////////

// Valida o formato do CEP (XXXXX-XXX)
int validar_cep(const char *cep) {
    int valido = 0;
    if (cep && strlen(cep) == 9 && cep[5] == '-') {
        int formato_ok = 1;
        for (int i = 0; i < 9 && formato_ok; i++) {
            if (i != 5 && !isdigit(cep[i])) {
                formato_ok = 0;
            }
        }
        valido = formato_ok;
    }
    return valido;
}

// Captura o CEP do usuário e valida o formato
int capturar_cep(char *cep) {
    int sucesso = 0;
    char entrada[11]; // 9 caracteres (XXXXX-XXX) + \n + \0
    printf("Digite o CEP (XXXXX-XXX): ");
    if (fgets(entrada, sizeof(entrada), stdin)) {
        entrada[strcspn(entrada, "\n")] = '\0';
        if (validar_cep(entrada)) {
            strcpy(cep, entrada);
            sucesso = 1;
        } else {
            printf("Erro: CEP deve ter formato XXXXX-XXX!\n");
        }
    } else {
        printf("Erro: falha na leitura!\n");
    }
    return sucesso;
}