#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <stdbool.h>
#include "../Arv_VP_H/STRUCTS_VP.h"
#include "../Arv_VP_H/utilitarios_VP.h"

////////////////////////////////////////////// DATA //////////////////////////////////////////////

void obter_data_atual(DATA *data_atual)
{
    time_t agora = time(NULL);
    struct tm *tm = localtime(&agora);
    data_atual->dia = tm->tm_mday;
    data_atual->mes = tm->tm_mon + 1;     // tm_mon é 0-11, ajustamos para 1-12
    data_atual->ano = tm->tm_year + 1900; // tm_year é anos desde 1900
}
// Verifica se um ano é bissexto
int eh_bissexto(int ano)
{
    int resultado = (ano % 4 == 0 && ano % 100 != 0) || (ano % 400 == 0);
    return resultado;
}

// Retorna o número de dias em um mês, considerando ano bissexto
int dias_no_mes(int mes, int ano)
{
    int dias;
    if (mes == 2)
        dias = eh_bissexto(ano) ? 29 : 28;
    else if (mes == 4 || mes == 6 || mes == 9 || mes == 11)
        dias = 30;
    else
        dias = 31;
    return dias;
}

int validar_data(DATA *data)
{
    DATA data_atual;
    obter_data_atual(&data_atual);

    int valida = 1;
    if (data->ano < 1900 || data->ano > data_atual.ano)
        valida = 0;
    else if (data->mes < 1 || data->mes > 12)
        valida = 0;
    else if (data->dia < 1 || data->dia > dias_no_mes(data->mes, data->ano))
        valida = 0;
    else if (data->ano == data_atual.ano)
    {
        if (data->mes > data_atual.mes)
            valida = 0;
        else if (data->mes == data_atual.mes && data->dia > data_atual.dia)
            valida = 0;
    }
    return valida;
}

// Captura e valida a data de nascimento do usuário
int capturar_data(DATA *data)
{
    int sucesso = 0;
    char entrada[11];

    do {
        printf("Digite a data de nascimento (DD/MM/AAAA): ");
        if (fgets(entrada, sizeof(entrada), stdin))
        {
            entrada[strcspn(entrada, "\n")] = '\0';
            if (sscanf(entrada, "%2d/%2d/%4d", &data->dia, &data->mes, &data->ano) == 3)
            {
                if (validar_data(data))
                    sucesso = 1;
                else{

                    limpar_buffer();
                    printf("Erro: data invalida!\n");
                }
            }
            else
                printf("Erro: formato invalida!\n");
        }
    }while (sucesso == 0);
    return sucesso;
}

void imprimir_data(DATA data)
{
    printf("%02d/%02d/%04d\n", data.dia, data.mes, data.ano);
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