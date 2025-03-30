#include <stdio.h>
#include <stdlib.h>

typedef struct ArvBin{
    char *info;
    struct ArvBin *esq;
    struct ArvBin *dir;
}ArvBin;

int ehFolha(struct ArvBin *raiz){
    int Folha = 0;
    if(raiz != NULL){
        if(raiz->esq == NULL && raiz->dir == NULL){
            Folha = 1;
        }
    }
    return Folha;
}

ArvBin *alocaNo(char *valor){
    ArvBin *novo = malloc(sizeof(ArvBin));
    if(novo != NULL){
        novo->info = valor;
        novo->esq = NULL;
        novo->dir = NULL;
    }
    return novo;
}

int InsereArvBin(struct ArvBin **raiz, ArvBin *no){
    int inseriu = 0;

    if(*raiz == NULL)
        *raiz = no;
    else if(strcmp(no->info, (*raiz)->info))
        inseriu = InsereArvBin(&(*raiz)->esq, no);
    else if(strcmp(no->info, (*raiz)->info))
        inseriu = InsereArvBin(&(*raiz)->dir, no);
    else
        inseriu = 1;
    return inseriu;
}

int ImprimirInOrdem(struct ArvBin *raiz){
    int imprimiu = 0;
    if(raiz != NULL){
        ImprimirInOrdem(raiz->esq);
        printf("%s ", raiz->info);
        ImprimirInOrdem(raiz->dir);
        imprimiu = 1;
    }
    return imprimiu;
}

int main(){
    ArvBin *raiz, *no;
    int opcao;
    char valor[256];

    raiz = NULL, no = NULL;
    do{
        printf("\n[1] - inserir\n");
        printf("[2] - imprimir\n");
        printf("[0] - sair\n");
        printf("[3] - verificar se eh folha\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);

        switch(opcao){
            case 1:
                printf("\nDigite o valor a ser inserido: ");
                getchar();
                scanf("%[^\n]", valor);
                no = alocaNo(valor);
                if(InsereArvBin(&raiz, no) == 0)
                    printf("\nvalor inserido com sucesso\n");
                else
                    printf("\nvalor ja existe na arvore\n");
                break;
            case 2:
                if(ImprimirInOrdem(raiz) == 0)
                    printf("\narvore vazia\n");
                break;
            case 3:
                if(ehFolha(raiz))
                    printf("\nraiz eh folha\n");
                else
                    printf("\nraiz nao eh folha\n");
                break;
            case 0:
                printf("\nSaindo...\n");
                break;
        }
    }while(opcao != 0);

    return 0;
}