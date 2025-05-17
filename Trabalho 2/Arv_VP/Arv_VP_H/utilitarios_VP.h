#pragma once
#include <stdbool.h>
#include "../Arv_VP_H/STRUCTS_VP.h"

// int ehBissexto(int ano);
bool eh_bissexto(int ano);
int validar_data(const char *data);
int capturar_data(char *data);
void imprimir_data(const char *data);
int validar_cpf(const char *cpf);
int capturar_cpf(char *cpf);
int validar_cep(const char *cep);
int capturar_cep(char *cep);
int autocorrigir_cep(char *cep_entrada, char *cep_corrigido);///////////
char *ler_string();
void ler_string_simples(char *buffer, size_t tamanho);
int digitar_int();
void limpar_buffer();
void pausar();