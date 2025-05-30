#pragma once

#include "../Arv_23_H/STRUCTS_23.h"

//PESQUISA E INSERÇÃO
ESTADOS *existe_estado(ESTADOS *cabeca, char *nome_estado);
ESTADOS *aloca_estado(char *nome_estado);
int inserir_estado_rec(ESTADOS **cabeca, ESTADOS *novo_estado);
ESTADOS *cadastro_estado();

//LIBERAÇÃO
void liberar_todos_estados(ESTADOS **cabeca);
void liberar_no_estado(ESTADOS **estado);

//IMPRESSÃO
int mostrar_estado(ESTADOS *estado, int i);
int mostrar_todos_estados(ESTADOS *cabeca);

//ESPECIFICAS DO TRABALHO
ESTADOS *verifica_estado_mais_populoso(ESTADOS* cabeca);