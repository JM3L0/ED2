#include <stdio.h>
#include <stdlib.h>

#define RED 1
#define BLACK 0

typedef struct arv_VP {
    int valor;
    int cor;
    struct arv_VP *esq;
    struct arv_VP *dir;
} arv_VP;

/* Protótipos das funções */
// Funções de inserção
int inserir_no(arv_VP **raiz, arv_VP *novo);
int inserir(arv_VP **raiz, arv_VP *novo);

// Funções de criação
arv_VP *alocaNo(int valor);

// Funções de rotação
void rotacao_esquerda(arv_VP **raiz);
void rotacao_direita(arv_VP **raiz);

// Funções de propriedades
int Cor(arv_VP *no);
void trocar_cor(arv_VP *raiz);
void balancear_RB(arv_VP **raiz);

// Funções de busca e impressão
arv_VP *existe_valor(arv_VP *raiz, int valor);
void imprimir_no(arv_VP *no);
void imprimir_em_ordem(arv_VP *raiz);
void imprimir_todos(arv_VP *raiz);

// Funções de desalocação
void libera_no(arv_VP **raiz);
void desalocar_arvore(arv_VP **raiz);

// Funções de consulta
int consulta(arv_VP *raiz, int valor);

// Funções de remoção
arv_VP *encontrar_menor(arv_VP *raiz);
void trocar_informacoes(arv_VP *no1, arv_VP *no2);
void mover2_esquerda(arv_VP **raiz);
void mover2_direita(arv_VP **raiz);
void remover_menor_arv(arv_VP **raiz);
int remover_no(arv_VP **raiz, int valor);
int remover_arvore(arv_VP **raiz, int valor);

/*---------------------------- Funções Arv Red Black ----------------------------*/

// =================================
// INSERCAO
// =================================

int inserir_no(arv_VP **raiz, arv_VP *novo)
{
    int inseriu = 0;

    if (*raiz == NULL)
    {
        *raiz = novo;
        inseriu = 1;
    }
    else if (novo->valor < (*raiz)->valor)
        inseriu = inserir_no(&((*raiz)->esq), novo);
    else if (novo->valor > (*raiz)->valor)
        inseriu = inserir_no(&((*raiz)->dir), novo);

    if (inseriu)
        balancear_RB(raiz);

    return inseriu;
}

int inserir(arv_VP **raiz, arv_VP *novo)
{
    int inseriu;
    inseriu = inserir_no(raiz, novo);

    if (*raiz != NULL)
        (*raiz)->cor = BLACK;

    return inseriu;
}

// =================================
// CRIACAO
// =================================

arv_VP *alocaNo(int valor)
{
    arv_VP *no;
    no = (arv_VP *)malloc(sizeof(arv_VP));

    no->valor = valor;
    no->cor = RED;
    no->esq = NULL;
    no->dir = NULL;

    return no;
}

// =================================
// ROTAÇÃO
// =================================

void rotacao_esquerda(arv_VP **raiz)
{
    arv_VP *aux;

    aux = (*raiz)->dir;
    (*raiz)->dir = aux->esq;
    aux->esq = (*raiz);
    (*raiz) = aux;

    (*raiz)->cor = (*raiz)->esq->cor;
    (*raiz)->esq->cor = RED;
}

void rotacao_direita(arv_VP **raiz)
{
    arv_VP *aux;

    aux = (*raiz)->esq;
    (*raiz)->esq = aux->dir;
    aux->dir = (*raiz);
    (*raiz) = aux;

    (*raiz)->cor = (*raiz)->dir->cor;
    (*raiz)->dir->cor = RED;
}

// =================================
// PROPRIEDADES
// =================================

int Cor(arv_VP *no)
{
    return no == NULL ? BLACK : no->cor;
}

void trocar_cor(arv_VP *raiz)
{
    raiz->cor = !(raiz->cor);

    if (raiz->esq != NULL)
        raiz->esq->cor = !(raiz->esq->cor);

    if (raiz->dir != NULL)
        raiz->dir->cor = !(raiz->dir->cor);
}

void balancear_RB(arv_VP **raiz)
{
    if (*raiz != NULL)
    {
        if (Cor((*raiz)->esq) == BLACK && Cor((*raiz)->dir) == RED)
            rotacao_esquerda(raiz);

        if (Cor((*raiz)->esq) == RED && Cor((*raiz)->esq->esq) == RED)
            rotacao_direita(raiz);

        if (Cor((*raiz)->esq) == RED && Cor((*raiz)->dir) == RED)
            trocar_cor(*raiz);
    }
}

// =================================
// BUSCA E IMPRESSÃO
// =================================

arv_VP *existe_valor(arv_VP *raiz, int valor)
{
    arv_VP *resultado = NULL;

    if (raiz != NULL)
    {
        if (valor == raiz->valor)
            resultado = raiz;
        else if (valor < raiz->valor)
            resultado = existe_valor(raiz->esq, valor);
        else
            resultado = existe_valor(raiz->dir, valor);
    }

    return resultado;
}

void imprimir_no(arv_VP *no)
{
    if (no != NULL)
    {
        printf("Valor: %d\n", no->valor);
        printf("Cor: %s\n", no->cor == RED ? "Vermelho" : "Preto");
        printf("------------------------\n");
    }
}

void imprimir_em_ordem(arv_VP *raiz)
{
    if (raiz != NULL)
    {
        imprimir_em_ordem(raiz->esq);
        imprimir_no(raiz);
        imprimir_em_ordem(raiz->dir);
    }
}

void imprimir_todos(arv_VP *raiz)
{
    if (raiz == NULL)
        printf("Arvore vazia.\n");
    else
    {
        printf("=== Conteudo da Arvore ===\n");
        imprimir_em_ordem(raiz);
    }
}

// =================================
// DESALOCAÇÃO
// =================================

void libera_no(arv_VP **raiz)
{
    if (*raiz != NULL)
    {
        free(*raiz);
        *raiz = NULL;
    }
}

void desalocar_arvore(arv_VP **raiz)
{
    if (*raiz != NULL)
    {
        desalocar_arvore(&((*raiz)->esq));
        desalocar_arvore(&((*raiz)->dir));
        libera_no(raiz);
    }
}

// =================================
// CONSULTA ÁRVORE
// =================================

int consulta(arv_VP *raiz, int valor)
{
    int resultado = 0;

    if (raiz != NULL)
    {
        if (valor == raiz->valor)
            resultado = 1;
        else if (valor < raiz->valor)
            resultado = consulta(raiz->esq, valor);
        else
            resultado = consulta(raiz->dir, valor);
    }

    return resultado;
}

// =================================
// REMOÇÃO
// =================================

arv_VP *encontrar_menor(arv_VP *raiz)
{
    arv_VP *menor = raiz;

    if (raiz != NULL)
    {
        while (menor->esq != NULL)
            menor = menor->esq;
    }

    return menor;
}

void trocar_informacoes(arv_VP *no1, arv_VP *no2)
{
    if (no1 != NULL && no2 != NULL)
    {
        int temp = no1->valor;
        no1->valor = no2->valor;
        no2->valor = temp;
    }
}

void mover2_esquerda(arv_VP **raiz)
{
    trocar_cor(*raiz);

    if ((*raiz)->dir != NULL && Cor((*raiz)->dir->esq) == RED)
    {
        rotacao_direita(&((*raiz)->dir));
        rotacao_esquerda(raiz);
        trocar_cor(*raiz);
    }
}

void mover2_direita(arv_VP **raiz)
{
    trocar_cor(*raiz);

    if ((*raiz)->esq != NULL && Cor((*raiz)->esq->esq) == RED)
    {
        rotacao_direita(raiz);
        trocar_cor(*raiz);
    }
}

void remover_menor_arv(arv_VP **raiz)
{
    if ((*raiz)->esq == NULL)
        libera_no(raiz);
    else
    {
        if (Cor((*raiz)->esq) == BLACK && Cor((*raiz)->esq->esq) == BLACK)
            mover2_esquerda(raiz);

        remover_menor_arv(&((*raiz)->esq));
        balancear_RB(raiz);
    }
}

int remover_no(arv_VP **raiz, int valor)
{
    int removeu = 1;

    if ((*raiz) != NULL)
    {
        if (valor < (*raiz)->valor)
        {
            if ((*raiz)->esq != NULL)
            {
                if (Cor((*raiz)->esq) == BLACK && Cor((*raiz)->esq->esq) == BLACK)
                    mover2_esquerda(raiz);
            }

            removeu = remover_no(&((*raiz)->esq), valor);
        }
        else
        {
            if (Cor((*raiz)->esq) == RED)
                rotacao_direita(raiz);

            if (valor == (*raiz)->valor && (*raiz)->dir == NULL)
                libera_no(raiz);
            else
            {
                if ((*raiz)->dir != NULL)
                {
                    if (Cor((*raiz)->dir) == BLACK && Cor((*raiz)->dir->esq) == BLACK)
                        mover2_direita(raiz);
                }

                if (valor == (*raiz)->valor)
                {
                    arv_VP *menor;
                    menor = encontrar_menor((*raiz)->dir);

                    trocar_informacoes(*raiz, menor);
                    remover_menor_arv(&((*raiz)->dir));
                }
                else
                    removeu = remover_no(&((*raiz)->dir), valor);
            }
        }

        if (*raiz != NULL)
            balancear_RB(raiz);
    }
    else
        removeu = 0;

    return removeu;
}

int remover_arvore(arv_VP **raiz, int valor)
{
    int removeu = consulta(*raiz, valor);

    if (removeu)
        removeu = remover_no(raiz, valor);

    if (*raiz != NULL)
        (*raiz)->cor = BLACK;

    return removeu;
}

// =================================
// FUNÇÃO PRINCIPAL
// =================================

int main()
{
    arv_VP *raiz = NULL;
    int opcao, valor, resultado;
    
    do {
        printf("\n==== MENU ARVORE VERMELHO-PRETA ====\n");
        printf("1. Inserir valor\n");
        printf("2. Remover valor\n");
        printf("3. Buscar valor\n");
        printf("4. Imprimir arvore\n");
        printf("0. Sair\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);
        
        switch(opcao) {
            case 1:
                printf("Digite o valor a inserir: ");
                scanf("%d", &valor);
                if (inserir(&raiz, alocaNo(valor)))
                    printf("Valor %d inserido com sucesso!\n", valor);
                else
                    printf("Valor ja existe na arvore.\n");
                break;
                
            case 2:
                printf("Digite o valor a remover: ");
                scanf("%d", &valor);
                if (remover_arvore(&raiz, valor))
                    printf("Valor %d removido com sucesso!\n", valor);
                else
                    printf("Valor nao encontrado na arvore.\n");
                break;
                
            case 3:
                printf("Digite o valor a buscar: ");
                scanf("%d", &valor);
                resultado = consulta(raiz, valor);
                if (resultado)
                    printf("Valor %d encontrado na arvore.\n", valor);
                else
                    printf("Valor nao encontrado na arvore.\n");
                break;
                
            case 4:
                imprimir_todos(raiz);
                break;
                
            case 0:
                printf("Saindo...\n");
                break;
                
            default:
                printf("Opcao invalida! Tente novamente.\n");
        }
    } while (opcao != 0);
    
    desalocar_arvore(&raiz);
    return 0;
}
