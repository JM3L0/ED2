#pragma once
#include "../Arv_VP_H/STRUCTS_VP.h"

ESTADOS *existe_estado(ESTADOS *cabeca, char *nome_estado);
ESTADOS *aloca_estado(char *nome_estado);
int inserir_estado_rec(ESTADOS **cabeca, ESTADOS *novo_estado);
ESTADOS *cadastro_estado();
void desalocar_estado(ESTADOS **estado);
int remover_estado_rec(ESTADOS **cabeca, char *nome_estado);
int mostrar_estado(ESTADOS *estado);
int mostrar_todos_estados(ESTADOS *cabeca);
ESTADOS *verifica_estado_mais_populoso(ESTADOS* cabeca);