#pragma once

#include "../Arv_23_H/STRUCTS_23.h"

// data
void obter_data_atual(DATA *data_atual);
int eh_bissexto(int ano);
int dias_no_mes(int mes, int ano);
int validar_data(DATA *data);
int capturar_data(DATA *data);
void imprimir_data(DATA data);

// cpf
int validar_cpf(const char *cpf);
int capturar_cpf(char *cpf);

// cep
int validar_cep(const char *cep);
int capturar_cep(char *cep);
int autocorrigir_cep(char *cep_entrada, char *cep_corrigido);

// outros
char *ler_string();
void ler_string_simples(char *buffer, size_t tamanho);
int digitar_int();
void limpar_buffer();
void pausar();
void mensagens_do_remover(StatusRemocao status);