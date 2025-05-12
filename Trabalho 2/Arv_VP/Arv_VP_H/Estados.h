#pragma once
#include "../Arv_VP_H/STRUCTS.h"

ESTADOS *existe_estado(ESTADOS *cabeca, char *nome_estado);
ESTADOS *criar_estado(char *nome_estado, char *nome_capital);
int inserir_estado_rec(ESTADOS **cabeca, ESTADOS *novo_estado);
ESTADOS *cadastro_estado();
void desalocar_estado(ESTADOS **estado);
int remover_estado_rec(ESTADOS **cabeca, char *nome_estado);
int mostrar_estado(ESTADOS *estado);
int mostrar_todos_estados(ESTADOS *cabeca);