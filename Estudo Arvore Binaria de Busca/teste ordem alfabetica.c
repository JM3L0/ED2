#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef struct No {
    char *valor;
    struct No *esquerda;
    struct No *direita;
} No;

No* criarNo(const char *str) {
    No *novo = (No*)malloc(sizeof(No));
    novo->valor = (char*)malloc(strlen(str) + 1);
    strcpy(novo->valor, str);
    novo->esquerda = NULL;
    novo->direita = NULL;
    return novo;
}

// Função de comparação personalizada
int compararAlfabetico(const char *str1, const char *str2) {
    char s1[strlen(str1) + 1], s2[strlen(str2) + 1];
    strcpy(s1, str1);
    strcpy(s2, str2);

    // Converte para minúsculas apenas para comparação inicial
    for (int i = 0; s1[i]; i++) s1[i] = tolower((unsigned char)s1[i]);
    for (int i = 0; s2[i]; i++) s2[i] = tolower((unsigned char)s2[i]);

    int cmp = strcmp(s1, s2);
    if (cmp != 0) {
        return cmp; // Ordem alfabética ignorando caso
    }

    // Se iguais ignorando caso, usa a original para desempate (minúsculas antes)
    return strcmp(str1, str2);
}

No* inserir(No *raiz, const char *str) {
    if (raiz == NULL) {
        return criarNo(str);
    }

    int cmp = compararAlfabetico(str, raiz->valor);

    if (cmp < 0) {
        raiz->esquerda = inserir(raiz->esquerda, str);
    } else if (cmp > 0) {
        raiz->direita = inserir(raiz->direita, str);
    }
    // Ignora duplicatas exatas (ex.: dois "apple")

    return raiz;
}

void emOrdem(No *raiz) {
    if (raiz != NULL) {
        emOrdem(raiz->esquerda);
        printf("%s\n", raiz->valor);
        emOrdem(raiz->direita);
    }
}

void liberarArvore(No *raiz) {
    if (raiz != NULL) {
        liberarArvore(raiz->esquerda);
        liberarArvore(raiz->direita);
        free(raiz->valor);
        free(raiz);
    }
}

int main() {
    No *raiz = NULL;

    raiz = inserir(raiz, "Banana");
    raiz = inserir(raiz, "Apple");
    raiz = inserir(raiz, "Applee");
    raiz = inserir(raiz, "apple");
    raiz = inserir(raiz, "applee");
    raiz = inserir(raiz, "cat");
    raiz = inserir(raiz, "Cat");

    printf("Palavras em ordem alfabética (humana):\n");
    emOrdem(raiz);

    liberarArvore(raiz);
    return 0;
}