#include <stdio.h>
#include <stdlib.h>

typedef struct ArvBin
{
    int info;
    struct ArvBin *esq, *dir;

} ArvBin;

void menu();
void executarMenu(ArvBin **Raiz);
ArvBin *CriaNo(int valor);
int inserirArvBin(ArvBin **Raiz, ArvBin *No);
int printarArvBinPreOrdem(ArvBin *Raiz);
int printarArvBinEmOrdem(ArvBin *Raiz);
int printarArvBinPosOrdem(ArvBin *Raiz);

int main()
{

    ArvBin *Raiz = NULL;

    executarMenu(&Raiz);

    return 0;
}

void menu()
{
    printf("Menu:\n");
    printf("1. Inserir elemento na arvore\n");
    printf("2. Imprimir arvore em pre-ordem\n");
    printf("3. Imprimir arvore em ordem\n");
    printf("4. Imprimir arvore em pos-ordem\n");
    printf("0. Sair\n");
}

void executarMenu(ArvBin **Raiz)
{
    int opcao, valor;
    ArvBin *novo;

    do
    {
        menu();
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);

        switch (opcao)
        {
        case 1:
            printf("Digite o valor a ser inserido: ");
            scanf("%d", &valor);
            novo = CriaNo(valor);
            if (inserirArvBin(Raiz, novo))
            {
                printf("Elemento inserido com sucesso!\n");
            }
            else
            {
                printf("Elemento ja existe na arvore!\n");
            }
            break;
        case 2:
            printf("Arvore em pre-ordem: ");
            if(!printarArvBinPreOrdem(*Raiz))
                printf("Arvore Vazia\n");
            printf("\n");
            break;
        case 3:
            printf("Arvore em ordem: ");
            if(!printarArvBinEmOrdem(*Raiz))
                printf("Arvore Vazia\n");
            printf("\n");
            break;
        case 4:
            printf("Arvore em pos-ordem: ");
            if(!printarArvBinPosOrdem(*Raiz))
                printf("Arvore Vazia\n");
            printf("\n");
            break;
        case 0:
            printf("Saindo...\n");
            break;
        default:
            printf("Opcao invalida! Tente novamente.\n");
        }
    } while (opcao != 0);
}

ArvBin *CriaNo(int valor)
{

    ArvBin *novo = malloc(sizeof(ArvBin));

    if (novo != NULL)
    {

        novo->info = valor;
        novo->esq = NULL;
        novo->dir = NULL;
    }

    return novo;
}

int inserirArvBin(ArvBin **Raiz, ArvBin *No)
{

    int inseriu = 1;

    if (*Raiz == NULL)
    {
        *Raiz = No;
    }
    else if (No->info < (*Raiz)->info)
    {
        inseriu = inserirArvBin(&(*Raiz)->esq, No);
    }
    else if (No->info > (*Raiz)->info)
    {
        inseriu = inserirArvBin(&(*Raiz)->dir, No);
    }
    else
    {
        inseriu = 0;
    }

    return (inseriu);
}

int printarArvBinPreOrdem(ArvBin *Raiz)
{
    int printou = 0;

    if (Raiz != NULL)
    {
        printf("%d ", Raiz->info);
        printou = printarArvBinEmOrdem(Raiz->esq);
        printou = printarArvBinEmOrdem(Raiz->dir);
        printou = 1;
    }

    return printou;
}

int printarArvBinEmOrdem(ArvBin *Raiz)
{
    int printou = 0;

    if (Raiz != NULL)
    {
        printou = printarArvBinEmOrdem(Raiz->esq);
        printf("%d ", Raiz->info);
        printou = printarArvBinEmOrdem(Raiz->dir);
        printou = 1;
    }

    return printou;
}

int printarArvBinPosOrdem(ArvBin *Raiz)
{
    int printou = 0;

    if (Raiz != NULL)
    {
        printou = printarArvBinEmOrdem(Raiz->esq);
        printou = printarArvBinEmOrdem(Raiz->dir);
        printf("%d ", Raiz->info);
        printou = 1;
    }

    return printou;
}


