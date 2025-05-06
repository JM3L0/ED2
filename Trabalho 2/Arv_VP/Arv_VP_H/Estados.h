#pragma once
#include "../Arv_VP_H/STRUCTS.h"

ESTADOS *criar_estado(char *nome_estado, char *nome_capital);
int inserir_estado_rec(ESTADOS **cabeca, ESTADOS *novo_estado);
int cadastro_estado();
void limpar_estado(ESTADOS *estado);
int remover_estado_rec(ESTADOS **cabeca, char *nome_estado);
int mostrar_estado(ESTADOS *estado);
int mostrar_todos_estados(ESTADOS *cabeca);