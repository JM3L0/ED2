#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "album.h"

char *ler_string()
{
    char buffer[256], *string;

    if (fgets(buffer, sizeof(buffer), stdin) == NULL)
        string = NULL;
    else
    {
        buffer[strcspn(buffer, "\n")] = 0;
        string = strdup(buffer);
    }
    return string;
}