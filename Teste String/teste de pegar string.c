#include<stdio.h>
#include<stdlib.h>
#include<string.h>

typedef struct String{
    char *nome;
}String;

int main(){

    char *nome;

    scanf("%[^\n]", nome);

    printf("%s", nome);

}