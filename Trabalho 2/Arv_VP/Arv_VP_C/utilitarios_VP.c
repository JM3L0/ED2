#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <stdbool.h>
#include "../Arv_VP_H/STRUCTS_VP.h"
#include "../Arv_VP_H/utilitarios_VP.h"

////////////////////////////////////////////// DATA //////////////////////////////////////////////

/**
 * Verifica se o ano é bissexto
 * @param ano O ano a ser verificado
 * @return true se for bissexto, false caso contrário
 */

bool eh_bissexto(int ano) {
    bool resultado = false;
    
    if ((ano % 4 == 0 && ano % 100 != 0) || ano % 400 == 0) {
        resultado = true;
    }
    
    return resultado;
}

/**
 * Valida uma data no formato "DD/MM/AAAA"
 * @param data String contendo a data a ser validada
 * @return 1 se a data for válida, 0 caso contrário
 */
int validar_data(const char *data) {
    int valido = 0;
    
    // Verifica se o ponteiro é válido e se o formato básico está correto
    if (data && strlen(data) == 10 && data[2] == '/' && data[5] == '/') {
        int dia, mes, ano;
        
        // Extrai dia, mês e ano da string
        if (sscanf(data, "%2d/%2d/%4d", &dia, &mes, &ano) == 3) {
            // Verifica limites básicos
            if (ano >= 1900 && ano <= 2025 && mes >= 1 && mes <= 12) {
                // Determina o número de dias no mês
                int dias_por_mes[] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
                
                // Ajusta fevereiro para anos bissextos
                if (mes == 2 && eh_bissexto(ano)) {
                    dias_por_mes[2] = 29;
                }
                
                // Verifica se o dia é válido para o mês
                if (dia >= 1 && dia <= dias_por_mes[mes]) {
                    valido = 1;
                }
            }
        }
    }
    
    return valido;
}

/**
 * Captura e valida a data de nascimento como string
 * @param data Buffer para armazenar a data válida
 * @return 1 se a operação foi bem-sucedida, 0 caso contrário
 */
int capturar_data(char *data) {
    int sucesso = 0;
    
    if (data) {
        char entrada[15];
        
        do {
            printf("Digite a data de nascimento (DD/MM/AAAA): ");
            
            if (fgets(entrada, sizeof(entrada), stdin)) {
                // Remove o caractere de nova linha
                entrada[strcspn(entrada, "\n")] = '\0';
                
                // Verifica formato básico
                if (strlen(entrada) != 10 || entrada[2] != '/' || entrada[5] != '/') {
                    printf("Erro: formato invalido! Use DD/MM/AAAA.\n");
                } else if (validar_data(entrada)) {
                    strcpy(data, entrada);
                    sucesso = 1;
                } else {
                    printf("Erro: data invalida!\n");
                }
            } else {
                printf("Erro: falha na leitura\n");
            }
        } while (!sucesso);
    } else {
        printf("Erro: ponteiro data nulo\n");
    }
    
    return sucesso;
}

/**
 * Imprime a data no formato "DD/MM/AAAA"
 * @param data String contendo a data a ser impressa
 */
void imprimir_data(const char *data) {
    if (data) {
        printf("Data: %s\n", data);
    } else {
        printf("Erro: ponteiro data nulo\n");
    }
}
////////////////////////////////////////////// CPF //////////////////////////////////////////////

// Valida o formato do CPF (11 dígitos numéricos)
int validar_cpf(const char *cpf)
{
    int valido = 0;
    if (cpf && strlen(cpf) == 11)
    {
        int todos_digitos = 1;
        for (int i = 0; i < 11 && todos_digitos; i++)
        {
            if (!isdigit(cpf[i]))
                todos_digitos = 0;
        }
        valido = todos_digitos;
    }
    return valido;
}

// Captura o CPF do usuário e valida o formato
int capturar_cpf(char *cpf)
{
    int sucesso = 0;
    char entrada[13]; // 11 dígitos + \n + \0

    do {

        printf("Digite o CPF (11 digitos, sem pontos ou tracos): ");
        if (fgets(entrada, sizeof(entrada), stdin))
        {
            entrada[strcspn(entrada, "\n")] = '\0';
            if (validar_cpf(entrada))
            {
                strcpy(cpf, entrada);
                sucesso = 1;
            }
            else
                printf("Erro: CPF deve ter 11 digitos numericos!\n");
        }
        else
            printf("Erro: falha na leitura!\n");
    }while (sucesso == 0);

    return sucesso;
}

////////////////////////////////////////////// CEP //////////////////////////////////////////////

// Valida o formato do CEP (XXXXX-XXX)
int validar_cep(const char *cep)
{
    int valido = 0;
    if (cep && strlen(cep) == 9 && cep[5] == '-')
    {
        int formato_ok = 1;
        for (int i = 0; i < 9 && formato_ok; i++)
        {
            if (i != 5 && !isdigit(cep[i]))
                formato_ok = 0;
        }
        valido = formato_ok;
    }
    return valido;
}

int capturar_cep(char *cep)
{
    int sucesso;
    char entrada[20];
    char cep_corrigido[20]; // Buffer separado para a correção

    do
    {
        printf("Digite o CEP (XXXXX-XXX): ");
        if (fgets(entrada, sizeof(entrada), stdin))
        {
            entrada[strcspn(entrada, "\n")] = '\0';

            if (validar_cep(entrada))
            {
                strcpy(cep, entrada);
                sucesso = 1; // Indica que a captura foi bem-sucedida
            }
            else if (autocorrigir_cep(entrada, cep_corrigido) && validar_cep(cep_corrigido))
            {
                strcpy(cep, cep_corrigido);
                printf("Aviso: CEP autocorrigido para %s\n", cep);
                sucesso = 1; // Indica que a captura foi bem-sucedida
            }
            else{
                printf("Erro: CEP invalido! Digite novamente\n");
                sucesso = 0; // Indica que a captura falhou
            }
        }
        else
            printf("Erro: falha na leitura!\n");
    } while (sucesso == 0);
    return sucesso;
}

// Tenta autocorrigir o formato do CEP para XXXXX-XXX
int autocorrigir_cep(char *cep_entrada, char *cep_corrigido)
{
    int sucesso = 0; // Valor de retorno padrão (falha)

    if (cep_entrada && cep_corrigido)
    {
        // Inicializa o CEP corrigido como vazio
        cep_corrigido[0] = '\0';

        // Buffer para armazenar apenas os dígitos
        char digitos[9];
        int pos_digito = 0;

        // Extrai apenas os dígitos
        for (int i = 0; cep_entrada[i] != '\0' && pos_digito < 8; i++)
        {
            if (isdigit(cep_entrada[i]))
                digitos[pos_digito++] = cep_entrada[i];
        }

        // Verifica se temos exatamente 8 dígitos
        if (pos_digito == 8)
        {
            // Formata como XXXXX-XXX
            digitos[8] = '\0'; // Garante que a string termina corretamente
            sprintf(cep_corrigido, "%.5s-%.3s", digitos, digitos + 5);
            sucesso = 1; // Correção bem-sucedida
        }
    }

    return sucesso;
}

////////////////////////////////////////////// OUTRAS FUNÇÕES //////////////////////////////////////////////

char *ler_string()
{
    char buffer[256], *string;
    string = NULL;

    if (fgets(buffer, sizeof(buffer), stdin) != NULL)
    {
        buffer[strcspn(buffer, "\n")] = 0;
        int i = 0;
        while (buffer[i] && (buffer[i] == ' ' || buffer[i] == '\t'))
            i++;
        if (buffer[i] != '\0')
        {
            int j = strlen(buffer) - 1;
            while (j >= i && (buffer[j] == ' ' || buffer[j] == '\t'))
                j--;
            buffer[j + 1] = '\0';
            string = strdup(buffer + i);
        }
    }
    return string;
}

void ler_string_simples(char *buffer, size_t tamanho)
{
    if (fgets(buffer, tamanho, stdin) != NULL)
    {
        buffer[strcspn(buffer, "\n")] = 0;
        if (strlen(buffer) == 0)
        {
            printf("Entrada invalida! Digite novamente: ");
            ler_string_simples(buffer, tamanho);
        }
    }
    else
        buffer[0] = '\0';
}

int digitar_int() // muito bom
{
    int numero = 0;

    while (scanf("%d", &numero) != 1 || numero < 0)
    {
        printf("Numero invalido ");
        printf("Digite novamente: ");
        limpar_buffer();
    }
    limpar_buffer();

    return numero;
}

void limpar_buffer()
{
    while (getchar() != '\n');
}

void pausar()
{
    printf("\n\nPressione Enter para continuar...");
    int c;
    do
        c = getchar();
    while (c != '\n' && c != EOF); // Consome tudo até o Enter
}