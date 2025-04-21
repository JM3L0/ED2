#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>
#include "../ARQUIVOS_BIN.H/interface_BIN.h"
#include "../ARQUIVOS_BIN.H/artista_BIN.h"
#include "../ARQUIVOS_BIN.H/album_BIN.h"
#include "../ARQUIVOS_BIN.H/musica_BIN.h"
#include "../ARQUIVOS_BIN.H/playlist_BIN.h"
#include "../ARQUIVOS_BIN.H/musica_playlist_BIN.h"
#include "../ARQUIVOS_BIN.H/TESTES_BIN.h"

char *ler_ate_delimitador(FILE *f, char delimitador)
{
    char buffer[256];
    int i = 0;
    char c;
    while ((c = fgetc(f)) != EOF && c != delimitador && c != '\n' && i < 255)
    {
        buffer[i++] = c;
    }
    buffer[i] = '\0';
    if (i == 0 && c == EOF)
    {
        return NULL; // Fim do arquivo
    }
    // Remover espaços e \r (compatibilidade com Windows)
    int len = strlen(buffer);
    while (len > 0 && (buffer[len - 1] == ' ' || buffer[len - 1] == '\r'))
    {
        buffer[--len] = '\0';
    }
    char *resultado = strdup(buffer);
    if (!resultado)
    {
        printf("Erro de alocação em ler_ate_delimitador\n");
        return NULL;
    }
    return resultado;
}

// Função para inserir artistas
int inserir_artistas(ARTISTAS **raiz, const char *arquivo)
{
    FILE *f = fopen(arquivo, "r");
    if (!f)
    {
        printf("Erro ao abrir o arquivo %s\n", arquivo);
        return 0;
    }
    char *nome, *tipo, *estilo;
    int contador = 0;
    while ((nome = ler_ate_delimitador(f, ';')) != NULL)
    {
        if (nome[0] == '\0')
        {
            free(nome);
            continue;
        }
        tipo = ler_ate_delimitador(f, ';');
        if (!tipo)
        {
            free(nome);
            continue;
        }
        estilo = ler_ate_delimitador(f, '\n');
        if (!estilo)
        {
            free(nome);
            free(tipo);
            continue;
        }
        ARTISTAS *novo = aloca_no_artista(nome, tipo, estilo);
        if (!novo)
        {
            free(nome);
            free(tipo);
            free(estilo);
            continue;
        }
        inserir_artista(raiz, novo);
        contador++;
    }
    fclose(f);
    return contador;
}

// Função para inserir álbuns
int inserir_albuns(ARTISTAS *raiz, const char *arquivo)
{
    FILE *f = fopen(arquivo, "r");
    if (!f)
    {
        printf("Erro ao abrir o arquivo %s\n", arquivo);
        return 0;
    }
    char *nome_artista, *titulo_album;
    int ano, contador = 0;
    while ((nome_artista = ler_ate_delimitador(f, ';')) != NULL)
    {
        if (nome_artista[0] == '\0')
        {
            free(nome_artista);
            continue;
        }
        titulo_album = ler_ate_delimitador(f, ';');
        if (!titulo_album)
        {
            free(nome_artista);
            continue;
        }
        if (fscanf(f, "%d\n", &ano) != 1)
        {
            free(nome_artista);
            free(titulo_album);
            continue;
        }
        ARTISTAS *artista = existe_artista(raiz, nome_artista);
        if (artista)
        {
            ALBUNS *novo = aloca_no_album(titulo_album, ano);
            if (!novo)
            {
                free(nome_artista);
                free(titulo_album);
                continue;
            }
            inserir_album(&artista->arv_albuns, novo);
            free(nome_artista);
            contador++;
        }
        else
        {
            free(nome_artista);
            free(titulo_album);
        }
    }
    fclose(f);
    return contador;
}

// Função para inserir músicas
int inserir_musicas(ARTISTAS *raiz, const char *arquivo)
{
    FILE *f = fopen(arquivo, "r");
    if (!f)
    {
        printf("Erro ao abrir o arquivo %s\n", arquivo);
        return 0;
    }
    char *nome_artista, *titulo_album, *titulo_musica;
    float duracao;
    int contador = 0;
    while ((nome_artista = ler_ate_delimitador(f, ';')) != NULL)
    {
        if (nome_artista[0] == '\0')
        {
            free(nome_artista);
            continue;
        }
        titulo_album = ler_ate_delimitador(f, ';');
        if (!titulo_album)
        {
            free(nome_artista);
            continue;
        }
        titulo_musica = ler_ate_delimitador(f, ';');
        if (!titulo_musica)
        {
            free(nome_artista);
            free(titulo_album);
            continue;
        }
        if (fscanf(f, "%f\n", &duracao) != 1)
        {
            free(nome_artista);
            free(titulo_album);
            free(titulo_musica);
            continue;
        }
        ARTISTAS *artista = existe_artista(raiz, nome_artista);
        if (artista)
        {
            ALBUNS *album = existe_album(artista->arv_albuns, titulo_album);
            if (album)
            {
                MUSICAS *novo = alocar_musica(titulo_musica, duracao);
                if (!novo)
                {
                    free(nome_artista);
                    free(titulo_album);
                    free(titulo_musica);
                    continue;
                }
                inserir_musica(&album->arv_musicas, novo);
                contador++;
            }
            else
            {
                free(titulo_musica);
            }
        }
        free(nome_artista);
        free(titulo_album);
    }
    fclose(f);
    return contador;
}

// Função para testar uma ordenação (crescente, decrescente, aleatória)
void testar_operacao(const char *sufixo, FILE *f_resultados)
{
    char arquivo_artistas[100], arquivo_albuns[100], arquivo_musicas[100];
    snprintf(arquivo_artistas, sizeof(arquivo_artistas), "%sartistas_%s.txt", CAMINHO_ARQUIVOS, sufixo);
    snprintf(arquivo_albuns, sizeof(arquivo_albuns), "%salbuns_%s.txt", CAMINHO_ARQUIVOS, sufixo);
    snprintf(arquivo_musicas, sizeof(arquivo_musicas), "%smusicas_%s.txt", CAMINHO_ARQUIVOS, sufixo);

    // Criar árvore para os testes
    ARTISTAS *raiz = NULL;
    struct timeval inicio_tv, fim_tv;
    double soma_tv = 0.0;
    int art_ins = 0;

    // Inserção de artistas
    for (int i = 0; i < NUM_EXECUCOES; i++)
    {
        gettimeofday(&inicio_tv, NULL);
        int artistas_inseridos = inserir_artistas(&raiz, arquivo_artistas);
        gettimeofday(&fim_tv, NULL);
        double tempo = (fim_tv.tv_sec - inicio_tv.tv_sec) + (fim_tv.tv_usec - inicio_tv.tv_usec) / 1000000.0;
        soma_tv += tempo;
        // fprintf(f_resultados, "Artistas inseridos: %d\n", artistas_inseridos);
        printf("Artistas inseridos: %d; Execucao %d de %d (tempo de exec) %.6f s\n", artistas_inseridos, i + 1, NUM_EXECUCOES, tempo);
        fprintf(f_resultados, "Artistas inseridos: %d; Execucao %d de %d (tempo de exec) %.6f s\n", artistas_inseridos, i + 1, NUM_EXECUCOES, tempo);
        art_ins += artistas_inseridos;
        liberar_arv_artista(&raiz);
        raiz = NULL;
    }
    fprintf(f_resultados, "-------------------------------------------\n");
    fprintf(f_resultados, "Artistas inseridos: %d (%d por execucao)\n", art_ins, NUM_ARTISTAS);
    fprintf(f_resultados, "Insercao artistas (%s, media %d exec): %.6f s\n", sufixo, NUM_EXECUCOES, soma_tv / NUM_EXECUCOES);
    fprintf(f_resultados, "-------------------------------------------\n");

    // Inserir artistas para testes subsequentes
    int artistas_inseridos = inserir_artistas(&raiz, arquivo_artistas);
    if (artistas_inseridos == 0)
    {
        fprintf(f_resultados, "Erro: Nenhum artista inserido\n");
        return;
    }

    // Inserção de álbuns
    soma_tv = 0.0;
    int alb_ins = 0;
    for (int i = 0; i < NUM_EXECUCOES; i++)
    {
        gettimeofday(&inicio_tv, NULL);
        int albuns_inseridos = inserir_albuns(raiz, arquivo_albuns);
        gettimeofday(&fim_tv, NULL);
        double tempo = (fim_tv.tv_sec - inicio_tv.tv_sec) + (fim_tv.tv_usec - inicio_tv.tv_usec) / 1000000.0;
        soma_tv += tempo;
        // fprintf(f_resultados, "Albuns inseridos: %d\n", albuns_inseridos);
        printf("Albuns inseridos: %d; Execucao %d de %d (tempo de exec): %.6f s\n", albuns_inseridos, i + 1, NUM_EXECUCOES, tempo);
        fprintf(f_resultados, "Albuns inseridos: %d; Execucao %d de %d (tempo de exec): %.6f s\n", albuns_inseridos, i + 1, NUM_EXECUCOES, tempo);
        alb_ins += albuns_inseridos;
        ARTISTAS *atual = raiz;
        while (atual)
        {
            liberar_arv_album(&atual->arv_albuns);
            atual->arv_albuns = NULL;
            atual = atual->dir;
        }
    }
    fprintf(f_resultados, "-------------------------------------------\n");
    fprintf(f_resultados, "Albuns inseridos: %d (%d por execucao)\n", alb_ins, NUM_ALBUNS);
    fprintf(f_resultados, "Insercao albuns (%s, media %d exec): %.6f s\n", sufixo, NUM_EXECUCOES, soma_tv / NUM_EXECUCOES);
    fprintf(f_resultados, "-------------------------------------------\n");

    // Inserir álbuns para testes subsequentes
    int albuns_inseridos = inserir_albuns(raiz, arquivo_albuns);
    if (albuns_inseridos == 0)
    {
        fprintf(f_resultados, "Erro: Nenhum album inserido\n");
        liberar_arv_artista(&raiz);
        return;
    }

    // Inserção de músicas
    soma_tv = 0.0;
    int mus_ins = 0;
    for (int i = 0; i < NUM_EXECUCOES; i++)
    {
        gettimeofday(&inicio_tv, NULL);
        int musicas_inseridas = inserir_musicas(raiz, arquivo_musicas);
        gettimeofday(&fim_tv, NULL);
        double tempo = (fim_tv.tv_sec - inicio_tv.tv_sec) + (fim_tv.tv_usec - inicio_tv.tv_usec) / 1000000.0;
        soma_tv += tempo;
        printf("Musicas inseridas: %d; Execucao %d de %d (tempo de exec): %.6f s\n", musicas_inseridas, i + 1, NUM_EXECUCOES, tempo);
        fprintf(f_resultados, "Musicas inseridas: %d; Execucao %d de %d (tempo de exec): %.6f s\n", musicas_inseridas, i + 1, NUM_EXECUCOES, tempo);
        mus_ins += musicas_inseridas;
        ARTISTAS *artista = raiz;
        while (artista)
        {
            ALBUNS *album = artista->arv_albuns;
            while (album)
            {
                liberar_arv_musica(&album->arv_musicas);
                album->arv_musicas = NULL;
                album = album->dir;
            }
            artista = artista->dir;
        }
    }
    fprintf(f_resultados, "-------------------------------------------\n");
    fprintf(f_resultados, "Musicas inseridas: %d (%d por execucao)\n", mus_ins, NUM_MUSICAS);
    fprintf(f_resultados, "Insercao musicas (%s, media %d exec): %.6f s\n", sufixo, NUM_EXECUCOES, soma_tv / NUM_EXECUCOES);
    fprintf(f_resultados, "-------------------------------------------\n");

    // Inserir músicas para testes subsequentes
    int musicas_inseridas = inserir_musicas(raiz, arquivo_musicas);
    if (musicas_inseridas == 0)
    {
        fprintf(f_resultados, "Erro: Nenhuma musica inserida\n");
        liberar_arv_artista(&raiz);
        return;
    }

    soma_tv = 0.0;
    for (int i = 0; i < NUM_EXECUCOES; i++)
    {
        gettimeofday(&inicio_tv, NULL);
        for (int j = 0; j < REPETICOES_RAPIDAS; j++)
        {
            // Gerar nomes dinamicamente
            char nome_artista[20], nome_album[20], nome_musica[20];
            int idx_artista = (j % NUM_ARTISTAS) + 1;                               // Artista0001 a NUM_ARTISTAS
            int idx_album = ((j / NUM_ARTISTAS) % NUM_ALBUNS) + 1;                  // Album0001 a NUM_ALBUNS
            // int idx_musica = ((j / (NUM_ARTISTAS * NUM_ALBUNS)) % NUM_MUSICAS) + 1; // Musica0001 a NUM_MUSICAS

            snprintf(nome_artista, sizeof(nome_artista), "Artista%04d", idx_artista);
            snprintf(nome_album, sizeof(nome_album), "Album%04d", idx_album);
            // snprintf(nome_musica, sizeof(nome_musica), "Musica%04d", idx_musica);

            ARTISTAS *artista = existe_artista(raiz, nome_artista);
            if (!artista)
            {
                fprintf(f_resultados, "Erro: %s nao encontrado\n", nome_artista);
                liberar_arv_artista(&raiz);
                return;
            }
            ALBUNS *album = existe_album(artista->arv_albuns, nome_album);
            if (!album)
            {
                fprintf(f_resultados, "Erro: %s nao encontrado para %s\n", nome_album, nome_artista);
                liberar_arv_artista(&raiz);
                return;
            }
            // MUSICAS *musica = existe_musica(album->arv_musicas, nome_musica);
            // if (!musica)
            // {
            //     fprintf(f_resultados, "Erro: %s nao encontrada para %s, %s\n", nome_musica, nome_artista, nome_album);
            //     liberar_arv_artista(&raiz);
            //     return;
            // }

            for (int k = 0; k < NUM_MUSICAS; k++)
            {
                int idx_musica = ((j / (NUM_ARTISTAS * NUM_ALBUNS)) % NUM_MUSICAS) + 1; // Musica0001 a NUM_MUSICAS
                snprintf(nome_musica, sizeof(nome_musica), "Musica%04d", idx_musica);
                MUSICAS *musica = existe_musica(album->arv_musicas, nome_musica);
                if (!musica)
                {
                    fprintf(f_resultados, "Erro: %s nao encontrada para %s, %s\n", nome_musica, nome_artista, nome_album);
                    liberar_arv_artista(&raiz);
                    return;
                }
            }
        }
        gettimeofday(&fim_tv, NULL);
        double tempo = (fim_tv.tv_sec - inicio_tv.tv_sec) + (fim_tv.tv_usec - inicio_tv.tv_usec) / 1000000.0;
        // soma_tv += tempo / REPETICOES_RAPIDAS;
        soma_tv += tempo / REPETICOES_RAPIDAS;
        //fprintf(f_resultados, "Busca musica (%s, execucao %d de %d, media %d rep): %.12f s\n", sufixo, i + 1, NUM_EXECUCOES, REPETICOES_RAPIDAS, tempo / REPETICOES_RAPIDAS);
        fprintf(f_resultados, "Busca musica (%s, execucao %d de %d, media %d rep): %.12f s\n", sufixo, i + 1, NUM_EXECUCOES, REPETICOES_RAPIDAS, (tempo / REPETICOES_RAPIDAS));
        printf("Busca musica (%s, execucao %d de %d, media %d rep): %.12f s\n", sufixo, i + 1, NUM_EXECUCOES, REPETICOES_RAPIDAS, (tempo / REPETICOES_RAPIDAS));
    }
    fprintf(f_resultados, "\nBusca musica (%s, media %d exec com %d rep cada): %.12f s\n", sufixo, NUM_EXECUCOES, REPETICOES_RAPIDAS, soma_tv / NUM_EXECUCOES);
    fprintf(f_resultados, "-------------------------------------------\n");

    // Remoção de músicas variadas (diferentes artistas, álbuns e músicas)
    soma_tv = 0.0;
    for (int i = 0; i < NUM_EXECUCOES; i++)
    {
        gettimeofday(&inicio_tv, NULL);
        for (int j = 0; j < REPETICOES_RAPIDAS; j++)
        {
            // Gerar nomes dinamicamente
            char nome_artista[20], nome_album[20], nome_musica[20];
            int idx_artista = (j % NUM_ARTISTAS) + 1;                               // Artista0001 a Artista0100
            int idx_album = ((j / NUM_ARTISTAS) % NUM_ALBUNS) + 1;                  // Album0001 a Album0050
            int idx_musica = ((j / (NUM_ARTISTAS * NUM_ALBUNS)) % NUM_MUSICAS) + 1; // Musica0001 a Musica0050

            snprintf(nome_artista, sizeof(nome_artista), "Artista%04d", idx_artista);
            snprintf(nome_album, sizeof(nome_album), "Album%04d", idx_album);
            snprintf(nome_musica, sizeof(nome_musica), "Musica%04d", idx_musica);

            // Verificar se artista e álbum existem
            ARTISTAS *artista = existe_artista(raiz, nome_artista);
            if (!artista)
            {
                fprintf(f_resultados, "Erro: %s nao encontrado para remocao\n", nome_artista);
                liberar_arv_artista(&raiz);
                return;
            }
            ALBUNS *album = existe_album(artista->arv_albuns, nome_album);
            if (!album)
            {
                fprintf(f_resultados, "Erro: %s nao encontrado para %s para remocao\n", nome_album, nome_artista);
                liberar_arv_artista(&raiz);
                return;
            }

            // Verificar se a música existe, se não, inseri-la
            MUSICAS *musica = existe_musica(album->arv_musicas, nome_musica);
            if (!musica)
            {
                MUSICAS *novo = alocar_musica(strdup(nome_musica), 3.5); // Duração arbitrária
                if (!novo)
                {
                    fprintf(f_resultados, "Erro: Falha ao alocar %s para remocao\n", nome_musica);
                    liberar_arv_artista(&raiz);
                    return;
                }
                inserir_musica(&album->arv_musicas, novo);
            }

            // Remover e reinserir a música
            remove_musica(&album->arv_musicas, nome_musica);
            MUSICAS *novo = alocar_musica(strdup(nome_musica), 3.5);
            if (!novo)
            {
                fprintf(f_resultados, "Erro: Falha ao alocar %s apos remocao\n", nome_musica);
                liberar_arv_artista(&raiz);
                return;
            }
            inserir_musica(&album->arv_musicas, novo);
        }
        gettimeofday(&fim_tv, NULL);
        double tempo = (fim_tv.tv_sec - inicio_tv.tv_sec) + (fim_tv.tv_usec - inicio_tv.tv_usec) / 1000000.0;
        //soma_tv += tempo / REPETICOES_RAPIDAS;
        soma_tv += tempo / REPETICOES_RAPIDAS;
        fprintf(f_resultados, "Remocao musica variada (%s, execucao %d de %d, media %d rep): %.12f s\n", sufixo, i + 1, NUM_EXECUCOES, REPETICOES_RAPIDAS, (tempo / REPETICOES_RAPIDAS));
        printf("Remocao musica variada (%s, execucao %d de %d, media %d rep): %.12f s\n", sufixo, i + 1, NUM_EXECUCOES, REPETICOES_RAPIDAS, (tempo / REPETICOES_RAPIDAS));
    }
    fprintf(f_resultados, "\nRemocao musica variada (%s, media %d exec com %d rep cada): %.12f s\n", sufixo, NUM_EXECUCOES, REPETICOES_RAPIDAS, soma_tv / NUM_EXECUCOES);
    fprintf(f_resultados, "-------------------------------------------\n");
    
    liberar_arv_artista(&raiz);
}

int main()
{
    struct timeval inicioT, fimT;
    gettimeofday(&inicioT, NULL);
    FILE *f_resultados = fopen("resultados_tempos.txt", "w");
    if (!f_resultados)
    {
        printf("Erro ao criar resultados_tempos.txt\n");
        return 1;
    }

    fprintf(f_resultados, "Arquivo de Resultados de Testes ARVORE-BINARIA\n");
    fprintf(f_resultados, "Numero de Execucoes %d\n", NUM_EXECUCOES);
    fprintf(f_resultados, "=== Testes com Ordem Crescente ===\n");
    testar_operacao("crescente", f_resultados);
    fprintf(f_resultados, "\n=== Testes com Ordem Decrescente ===\n");
    testar_operacao("decrescente", f_resultados);
    fprintf(f_resultados, "\n=== Testes com Ordem Aleatoria ===\n");
    testar_operacao("aleatorio", f_resultados);

    gettimeofday(&fimT, NULL);
    double tempo_total = (fimT.tv_sec - inicioT.tv_sec) + (fimT.tv_usec - inicioT.tv_usec) / 1000000.0;
    int horas = (int)(tempo_total / 3600);
    int minutos = (int)((tempo_total - horas * 3600) / 60);
    int segundos = (int)(tempo_total - horas * 3600 - minutos * 60);
    int milisegundos = (int)((tempo_total - (int)tempo_total) * 1000);

    fprintf(f_resultados, "\nTempo total de execucao: %02d:%02d:%02d:%03d (horas:minutos:segundos:milisegundos)\n", horas, minutos, segundos, milisegundos);

    fclose(f_resultados);
    printf("Testes concluidos. Resultados salvos em resultados_tempos.txt\n");
    return 0;
}