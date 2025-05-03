#pragma once

#include "../Arv_VP_H/STRUCTS.h"

int eh_bissexto(int ano);
int dias_no_mes(int mes, int ano);
int validar_data(DATA *data);
int capturar_data(DATA *data);
int validar_cpf(const char *cpf);
int capturar_cpf(char *cpf);
int validar_cep(const char *cep);
int capturar_cep(char *cep);
