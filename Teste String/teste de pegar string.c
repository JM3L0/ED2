#include<stdio.h>
#include<stdlib.h>
#include<string.h>

typedef struct String{
    char *nome;
}String;

int main(){

    String ab;

    scanf("%c", &ab.nome);

    printf("%c", ab.nome);

}