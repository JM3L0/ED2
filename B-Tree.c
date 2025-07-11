#include <stdio.h>
#include <stdlib.h>
#define M 3

struct no {
    int n;
    int chaves[M-1];
    struct no *p[M];
}*raiz=NULL;

enum StatusChave { Duplicado, BuscaFalhou, Sucesso, InserirAqui, MenosChaves };

void inserir(int chave);
void exibir(struct no *raiz, int);
void removerNo(int x);
enum StatusChave ins(struct no *r, int x, int* y, struct no** u);
int posicaoBusca(int x, int *vet_chaves, int n);
enum StatusChave remover(struct no *r, int x);
void limparBuffer(void);


int main()
{
    int chave;
    int escolha;
    while(1)
    {
        printf("1.Adicionar\n");
        printf("2.Remover\n");
        printf("3.Visualizar\n");
        printf("4.Sair\n");
        printf("Digite sua escolha: ");
        scanf("%d",&escolha); limparBuffer();
        switch(escolha)
        {
        case 1:
            printf("Digite um numero: ");
            scanf("%d",&chave); limparBuffer();
            inserir(chave);
            break;
        case 2:
            printf("Digite um numero: ");
            scanf("%d",&chave); limparBuffer();
            removerNo(chave);
            break;
        case 3:
            printf("Arvore 2-3:\n");
            exibir(raiz,0);
            break;
        case 4:
            exit(1);
        default:
            printf("Opcao invalida.\n");
            break;
        }
    }
    return 0;
}

void inserir(int chave)
{
    struct no *novoNo;
    int chaveAcima;
    enum StatusChave valor;
    valor = ins(raiz, chave, &chaveAcima, &novoNo);
    if (valor == Duplicado)
        printf("Numero ja existe\n");
    if (valor == InserirAqui)
    {
        struct no *novaRaiz = raiz;
        raiz=malloc(sizeof(struct no));
        raiz->n = 1;
        raiz->chaves[0] = chaveAcima;
        raiz->p[0] = novaRaiz;
        raiz->p[1] = novoNo;
    }
}

enum StatusChave ins(struct no *ptr, int chave, int *chaveAcima, struct no **novoNo)
{
    struct no *novoPtr, *ultimoPtr;
    int pos, i, n, posDivisao;
    int novaChave, ultimaChave;
    enum StatusChave valor;
    if (ptr == NULL)
    {
        *novoNo = NULL;
        *chaveAcima = chave;
        return InserirAqui;
    }
    n = ptr->n;
    pos = posicaoBusca(chave, ptr->chaves, n);
    if (pos < n && chave == ptr->chaves[pos])
        return Duplicado;
    valor = ins(ptr->p[pos], chave, &novaChave, &novoPtr);
    if (valor != InserirAqui)
        return valor;
    if (n < M - 1)
    {
        pos = posicaoBusca(novaChave, ptr->chaves, n);
        for (i=n; i>pos; i--)
        {
            ptr->chaves[i] = ptr->chaves[i-1];
            ptr->p[i+1] = ptr->p[i];
        }
        ptr->chaves[pos] = novaChave;
        ptr->p[pos+1] = novoPtr;
        ++ptr->n;
        return Sucesso;
    }
    if (pos == M - 1)
    {
        ultimaChave = novaChave;
        ultimoPtr = novoPtr;
    }
    else
    {
        ultimaChave = ptr->chaves[M-2];
        ultimoPtr = ptr->p[M-1];
        for (i=M-2; i>pos; i--)
        {
            ptr->chaves[i] = ptr->chaves[i-1];
            ptr->p[i+1] = ptr->p[i];
        }
        ptr->chaves[pos] = novaChave;
        ptr->p[pos+1] = novoPtr;
    }
    posDivisao = (M - 1)/2;
    (*chaveAcima) = ptr->chaves[posDivisao];

    (*novoNo)=malloc(sizeof(struct no));
    ptr->n = posDivisao;
    (*novoNo)->n = M-1-posDivisao;
    for (i=0; i < (*novoNo)->n; i++)
    {
        (*novoNo)->p[i] = ptr->p[i + posDivisao + 1];
        if(i < (*novoNo)->n - 1)
            (*novoNo)->chaves[i] = ptr->chaves[i + posDivisao + 1];
        else
            (*novoNo)->chaves[i] = ultimaChave;
    }
    (*novoNo)->p[(*novoNo)->n] = ultimoPtr;
    return InserirAqui;
}

void exibir(struct no *ptr, int espacos)
{
    if (ptr)
    {
        int i;
        for(i=1; i<=espacos; i++)
            printf(" ");
        for (i=0; i < ptr->n; i++)
            printf("%d ",ptr->chaves[i]);
        printf("\n");
        for (i=0; i <= ptr->n; i++)
            exibir(ptr->p[i], espacos+10);
    }
}

int posicaoBusca(int chave, int *vet_chaves, int n)
{
    int pos=0;
    while (pos < n && chave > vet_chaves[pos])
        pos++;
    return pos;
}

void removerNo(int chave)
{
    struct no *novaRaiz;
    enum StatusChave valor;
    valor = remover(raiz, chave);
    switch (valor)
    {
    case BuscaFalhou:
        printf("Numero %d nao encontrado\n", chave);
        break;
    case MenosChaves:
        novaRaiz = raiz;
        raiz = raiz->p[0];
        free(novaRaiz);
        break;
    }
}

enum StatusChave remover(struct no *ptr, int chave)
{
    int pos, i, pivot, n, min;
    int *vet_chaves;
    enum StatusChave valor;
    struct no **p, *esqPtr, *dirPtr;

    if (ptr == NULL)
        return BuscaFalhou;
    n = ptr->n;
    vet_chaves = ptr->chaves;
    p = ptr->p;
    min = (M - 1)/2;

    pos = posicaoBusca(chave, vet_chaves, n);
    if (p[0] == NULL)
    {
        if (pos == n || chave < vet_chaves[pos])
            return BuscaFalhou;
        for (i=pos+1; i < n; i++)
        {
            vet_chaves[i-1] = vet_chaves[i];
            p[i] = p[i+1];
        }
        return --ptr->n >= (ptr==raiz ? 1 : min) ? Sucesso : MenosChaves;
    }

    if (pos < n && chave == vet_chaves[pos])
    {
        struct no *qp = p[pos], *qp1;
        int nchave;
        while(1)
        {
            nchave = qp->n;
            qp1 = qp->p[nchave];
            if (qp1 == NULL)
                break;
            qp = qp1;
        }
        vet_chaves[pos] = qp->chaves[nchave-1];
        qp->chaves[nchave - 1] = chave;
    }
    valor = remover(p[pos], chave);
    if (valor != MenosChaves)
        return valor;

    if (pos > 0 && p[pos-1]->n > min)
    {
        pivot = pos - 1;
        esqPtr = p[pivot];
        dirPtr = p[pos];
        
        dirPtr->p[dirPtr->n + 1] = dirPtr->p[dirPtr->n];
        for (i=dirPtr->n; i>0; i--)
        {
            dirPtr->chaves[i] = dirPtr->chaves[i-1];
            dirPtr->p[i] = dirPtr->p[i-1];
        }
        dirPtr->n++;
        dirPtr->chaves[0] = vet_chaves[pivot];
        dirPtr->p[0] = esqPtr->p[esqPtr->n];
        vet_chaves[pivot] = esqPtr->chaves[--esqPtr->n];
        return Sucesso;
    }
    
    if (pos < n && p[pos + 1]->n > min)
    {
        pivot = pos;
        esqPtr = p[pivot];
        dirPtr = p[pivot+1];
        
        esqPtr->chaves[esqPtr->n] = vet_chaves[pivot];
        esqPtr->p[esqPtr->n + 1] = dirPtr->p[0];
        vet_chaves[pivot] = dirPtr->chaves[0];
        esqPtr->n++;
        dirPtr->n--;
        for (i=0; i < dirPtr->n; i++)
        {
            dirPtr->chaves[i] = dirPtr->chaves[i+1];
            dirPtr->p[i] = dirPtr->p[i+1];
        }
        dirPtr->p[dirPtr->n] = dirPtr->p[dirPtr->n + 1];
        return Sucesso;
    }

    if(pos == n)
        pivot = pos-1;
    else
        pivot = pos;

    esqPtr = p[pivot];
    dirPtr = p[pivot+1];
    
    esqPtr->chaves[esqPtr->n] = vet_chaves[pivot];
    esqPtr->p[esqPtr->n + 1] = dirPtr->p[0];
    for (i=0; i < dirPtr->n; i++)
    {
        esqPtr->chaves[esqPtr->n + 1 + i] = dirPtr->chaves[i];
        esqPtr->p[esqPtr->n + 2 + i] = dirPtr->p[i+1];
    }
    esqPtr->n = esqPtr->n + dirPtr->n + 1;
    free(dirPtr);
    for (i=pos+1; i < n; i++)
    {
        vet_chaves[i-1] = vet_chaves[i];
        p[i] = p[i+1];
    }
    return --ptr->n >= (ptr == raiz ? 1 : min) ? Sucesso : MenosChaves;
}

void limparBuffer(void) {
  char c;
  printf("");
  while (c=getchar()!='\n') ;
}