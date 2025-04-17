#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <time.h>
#include "../ARQUIVOS_.H/TESTES.h"
#include "../ARQUIVOS_.H/STRUCTS.h"
#include "../ARQUIVOS_.H/artista.h"
#include "../ARQUIVOS_.H/album.h"
#include "../ARQUIVOS_.H/musica.h"
#include "../ARQUIVOS_.H/musica_playlist.h"


// #define NUM_ARTISTAS 10000

// // Estrutura para armazenar nomes de artistas
// typedef struct {
//     char *nome;
// } ArtistaEntrada;

// // Função para obter tempo com alta precisão
// double get_time() {
//     struct timeval tv;
//     gettimeofday(&tv, NULL);
//     return tv.tv_sec + tv.tv_usec / 1000000.0;
// }

// // Gera 1000 nomes de artistas únicos
// ArtistaEntrada *gerar_entradas() {
//     ArtistaEntrada *entradas = malloc(NUM_ARTISTAS * sizeof(ArtistaEntrada));
//     if (!entradas) return NULL;

//     for (int i = 0; i < NUM_ARTISTAS; i++) {
//         char nome[20];
//         snprintf(nome, sizeof(nome), "Artista%03d", i); // Ex.: Artista000, Artista001
//         entradas[i].nome = strdup(nome);
//     }
//     return entradas;
// }

// // Embaralha as entradas para ordem aleatória
// void embaralhar_entradas(ArtistaEntrada *entradas, int tamanho) {
//     for (int i = tamanho - 1; i > 0; i--) {
//         int j = rand() % (i + 1);
//         char *temp = entradas[i].nome;
//         entradas[i].nome = entradas[j].nome;
//         entradas[j].nome = temp;
//     }
// }

// // Libera as entradas
// void liberar_entradas(ArtistaEntrada *entradas) {
//     for (int i = 0; i < NUM_ARTISTAS; i++) {
//         free(entradas[i].nome);
//     }
//     free(entradas);
// }

// // Funções para remoção de artistas
// int eh_folha_artista(ARTISTAS *no) {
//     return (no->esq == NULL && no->dir == NULL);
// }

// ARTISTAS *so_um_filho_artista(ARTISTAS *no) {
//     if (no->esq != NULL && no->dir == NULL) return no->esq;
//     if (no->esq == NULL && no->dir != NULL) return no->dir;
//     return NULL;
// }

// ARTISTAS *menor_no_artista(ARTISTAS *raiz) {
//     ARTISTAS *menor = raiz;
//     while (menor->esq != NULL) menor = menor->esq;
//     return menor;
// }

// int remove_artista(ARTISTAS **raiz, char *nome_artista) {
//     if (*raiz == NULL) return 0;

//     int cmp = strcasecmp(nome_artista, (*raiz)->nome_artista);
//     if (cmp == 0) {
//         ARTISTAS *aux = *raiz, *filho;
//         int removeu = 1;

//         if (eh_folha_artista(*raiz)) {
//             *raiz = NULL;
//         } else if ((filho = so_um_filho_artista(*raiz)) != NULL) {
//             *raiz = filho;
//         } else {
//             aux = menor_no_artista((*raiz)->dir);
//             liberar_arv_album(&(*raiz)->arv_albuns);
//             limpar_no_artista(*raiz);

//             (*raiz)->nome_artista = aux->nome_artista;
//             (*raiz)->tipo_artista = aux->tipo_artista;
//             (*raiz)->estilo_musical = aux->estilo_musical;
//             (*raiz)->numero_albuns = aux->numero_albuns;
//             (*raiz)->arv_albuns = aux->arv_albuns;

//             aux->nome_artista = NULL;
//             aux->tipo_artista = NULL;
//             aux->estilo_musical = NULL;
//             aux->arv_albuns = NULL;
//             removeu = remove_artista(&(*raiz)->dir, aux->nome_artista);
//         }

//         liberar_arv_album(&aux->arv_albuns);
//         limpar_no_artista(aux);
//         free(aux);
//         return removeu;
//     } else if (cmp < 0) {
//         return remove_artista(&(*raiz)->esq, nome_artista);
//     } else {
//         return remove_artista(&(*raiz)->dir, nome_artista);
//     }
// }

// // Teste de inserção em ordem crescente
// double testar_insercao_crescente(ArtistaEntrada *entradas, ARTISTAS **raiz) {
//     double inicio = get_time();
//     for (int i = 0; i < NUM_ARTISTAS; i++) {
//         ARTISTAS *artista = aloca_no_artista(strdup(entradas[i].nome), strdup("Solo"), strdup("Pop"));
//         if (artista) {
//             inserir_artista(raiz, artista);
//         }
//     }
//     double fim = get_time();
//     return fim - inicio;
// }

// // Teste de inserção em ordem decrescente
// double testar_insercao_decrescente(ArtistaEntrada *entradas, ARTISTAS **raiz) {
//     double inicio = get_time();
//     for (int i = NUM_ARTISTAS - 1; i >= 0; i--) {
//         ARTISTAS *artista = aloca_no_artista(strdup(entradas[i].nome), strdup("Solo"), strdup("Pop"));
//         if (artista) {
//             inserir_artista(raiz, artista);
//         }
//     }
//     double fim = get_time();
//     return fim - inicio;
// }

// // Teste de inserção em ordem aleatória
// double testar_insercao_aleatoria(ArtistaEntrada *entradas, ARTISTAS **raiz) {
//     ArtistaEntrada *entradas_copia = malloc(NUM_ARTISTAS * sizeof(ArtistaEntrada));
//     for (int i = 0; i < NUM_ARTISTAS; i++) {
//         entradas_copia[i].nome = strdup(entradas[i].nome);
//     }
//     embaralhar_entradas(entradas_copia, NUM_ARTISTAS);

//     double inicio = get_time();
//     for (int i = 0; i < NUM_ARTISTAS; i++) {
//         ARTISTAS *artista = aloca_no_artista(strdup(entradas_copia[i].nome), strdup("Solo"), strdup("Pop"));
//         if (artista) {
//             inserir_artista(raiz, artista);
//         }
//     }
//     double fim = get_time();

//     liberar_entradas(entradas_copia);
//     return fim - inicio;
// }

// // Teste de remoção em ordem crescente
// double testar_remocao_crescente(ArtistaEntrada *entradas, ARTISTAS **raiz) {
//     double inicio = get_time();
//     for (int i = 0; i < NUM_ARTISTAS; i++) {
//         remove_artista(raiz, entradas[i].nome);
//     }
//     double fim = get_time();
//     return fim - inicio;
// }

// // Teste de remoção em ordem decrescente
// double testar_remocao_decrescente(ArtistaEntrada *entradas, ARTISTAS **raiz) {
//     double inicio = get_time();
//     for (int i = NUM_ARTISTAS - 1; i >= 0; i--) {
//         remove_artista(raiz, entradas[i].nome);
//     }
//     double fim = get_time();
//     return fim - inicio;
// }

// // Teste de remoção em ordem aleatória
// double testar_remocao_aleatoria(ArtistaEntrada *entradas, ARTISTAS **raiz) {
//     ArtistaEntrada *entradas_copia = malloc(NUM_ARTISTAS * sizeof(ArtistaEntrada));
//     for (int i = 0; i < NUM_ARTISTAS; i++) {
//         entradas_copia[i].nome = strdup(entradas[i].nome);
//     }
//     embaralhar_entradas(entradas_copia, NUM_ARTISTAS);

//     double inicio = get_time();
//     for (int i = 0; i < NUM_ARTISTAS; i++) {
//         remove_artista(raiz, entradas_copia[i].nome);
//     }
//     double fim = get_time();

//     liberar_entradas(entradas_copia);
//     return fim - inicio;
// }

// // Teste de busca de músicas (30 execuções)
// double testar_busca_musicas(ARTISTAS *raiz_artista, char *nome_artista) {
//     double tempos[30], media = 0.0;

//     for (int i = 0; i < 30; i++) {
//         double inicio = get_time();
//         ARTISTAS *artista = existe_artista(raiz_artista, nome_artista);
//         if (artista && artista->arv_albuns) {
//             imprimir_todos_albuns_de_um_artista(raiz_artista, nome_artista);
//         }
//         double fim = get_time();
//         tempos[i] = fim - inicio;
//     }

//     for (int i = 0; i < 30; i++) {
//         media += tempos[i];
//     }
//     media /= 30;

//     return media;
// }

// // Função principal de testes
// void executar_testes() {
//     srand(time(NULL)); // Inicializa semente para embaralhamento
//     ArtistaEntrada *entradas = gerar_entradas();
//     if (!entradas) {
//         printf("Erro ao gerar entradas!\n");
//         return;
//     }

//     ARTISTAS *raiz = NULL;
//     double tempo;

//     // Teste de inserção crescente
//     tempo = testar_insercao_crescente(entradas, &raiz);
//     printf("Insercao crescente: %.6f segundos\n", tempo);
//     liberar_arv_artista(&raiz);

//     // Teste de inserção decrescente
//     tempo = testar_insercao_decrescente(entradas, &raiz);
//     printf("Insercao decrescente: %.6f segundos\n", tempo);
//     liberar_arv_artista(&raiz);

//     // Teste de inserção aleatória
//     tempo = testar_insercao_aleatoria(entradas, &raiz);
//     printf("Insercao aleatoria: %.6f segundos\n", tempo);

//     // Teste de remoção crescente
//     testar_insercao_crescente(entradas, &raiz); // Reinsere para remoção
//     tempo = testar_remocao_crescente(entradas, &raiz);
//     printf("Remocao crescente: %.6f segundos\n", tempo);
//     liberar_arv_artista(&raiz);

//     // Teste de remoção decrescente
//     testar_insercao_crescente(entradas, &raiz); // Reinsere para remoção
//     tempo = testar_remocao_decrescente(entradas, &raiz);
//     printf("Remocao decrescente: %.6f segundos\n", tempo);
//     liberar_arv_artista(&raiz);

//     // Teste de remoção aleatória
//     testar_insercao_crescente(entradas, &raiz); // Reinsere para remoção
//     tempo = testar_remocao_aleatoria(entradas, &raiz);
//     printf("Remocao aleatoria: %.6f segundos\n", tempo);
//     liberar_arv_artista(&raiz);

//     // Teste de busca
//     inicializar_dados_predefinidos(&raiz, NULL); // Usa dados predefinidos
//     tempo = testar_busca_musicas(raiz, "rail");
//     printf("Busca de musicas (media de 30 execucoes): %.6f segundos\n", tempo);
//     liberar_arv_artista(&raiz);

//     liberar_entradas(entradas);
// }
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static int cadastrar_artistas_predefinidos(ARTISTAS **raiz) {
    int retorno = 1;
    ARTISTAS *artista1 = aloca_no_artista(strdup("rail"), strdup("Banda"), strdup("Rock"));
    ARTISTAS *artista2 = aloca_no_artista(strdup("joao"), strdup("Solo"), strdup("Samba"));

    if (!artista1 || !artista2) {
        limpar_no_artista(artista1);
        limpar_no_artista(artista2);
        free(artista1);
        free(artista2);
        return 0;
    }

    retorno &= inserir_artista(raiz, artista1);
    printf(retorno ? "Artista 'rail' cadastrado com sucesso!\n" : "Erro ao cadastrar 'rail'!\n");

    retorno &= inserir_artista(raiz, artista2);
    printf(retorno ? "Artista 'joao' cadastrado com sucesso!\n" : "Erro ao cadastrar 'joao'!\n");

    return retorno;
}

static int cadastrar_albuns_predefinidos(ARTISTAS *raiz) {
    int retorno = 1;
    ALBUNS *album1 = aloca_no_album(strdup("SIM"), 2000);
    ALBUNS *album2 = aloca_no_album(strdup("NAO"), 3000);
    ALBUNS *album3 = aloca_no_album(strdup("MUSICS"), 2001);

    if (!album1 || !album2 || !album3) {
        limpar_no_album(album1);
        limpar_no_album(album2);
        limpar_no_album(album3);
        free(album1);
        free(album2);
        free(album3);
        return 0;
    }

    ARTISTAS *rail = existe_artista(raiz, "rail");
    if (rail) {
        retorno &= inserir_album(&rail->arv_albuns, album1);
        if (retorno) {
            printf("Album 'SIM' cadastrado para rail!\n");
            rail->numero_albuns++;
        } else {
            printf("Erro ao cadastrar 'SIM'!\n");
        }

        retorno &= inserir_album(&rail->arv_albuns, album3);
        if (retorno) {
            printf("Album 'MUSICS' cadastrado para rail!\n");
            rail->numero_albuns++;
        } else {
            printf("Erro ao cadastrar 'MUSICS'!\n");
        }
    }

    ARTISTAS *joao = existe_artista(raiz, "joao");
    if (joao) {
        retorno &= inserir_album(&joao->arv_albuns, album2);
        if (retorno) {
            printf("Album 'NAO' cadastrado para joao!\n");
            joao->numero_albuns++;
        } else {
            printf("Erro ao cadastrar 'NAO'!\n");
        }
    }

    return retorno;
}

static int cadastrar_musicas_predefinidas(ARTISTAS *raiz) {
    int retorno = 1;
    MUSICAS *musica1 = alocar_musica(strdup("winx"), 2.66);
    MUSICAS *musica2 = alocar_musica(strdup("kwid"), 1.99);
    MUSICAS *musica3 = alocar_musica(strdup("corola manual"), 2.99);

    if (!musica1 || !musica2 || !musica3) {
        limpar_no_musica(musica1);
        limpar_no_musica(musica2);
        limpar_no_musica(musica3);
        free(musica1);
        free(musica2);
        free(musica3);
        return 0;
    }

    ARTISTAS *rail = existe_artista(raiz, "rail");
    if (rail) {
        ALBUNS *sim = existe_album(rail->arv_albuns, "SIM");
        if (sim) {
            retorno &= inserir_musica(&sim->arv_musicas, musica1);
            if (retorno) {
                printf("Musica 'winx' cadastrada em SIM!\n");
                sim->quantidade_musicas++;
            } else {
                printf("Erro ao cadastrar 'winx'!\n");
            }
        }

        ALBUNS *musics = existe_album(rail->arv_albuns, "MUSICS");
        if (musics) {
            retorno &= inserir_musica(&musics->arv_musicas, musica3);
            if (retorno) {
                printf("Musica 'corola manual' cadastrada em MUSICS!\n");
                musics->quantidade_musicas++;
            } else {
                printf("Erro ao cadastrar 'corola manual'!\n");
            }
        }
    }

    ARTISTAS *joao = existe_artista(raiz, "joao");
    if (joao) {
        ALBUNS *nao = existe_album(joao->arv_albuns, "NAO");
        if (nao) {
            retorno &= inserir_musica(&nao->arv_musicas, musica2);
            if (retorno) {
                printf("Musica 'kwid' cadastrada em NAO!\n");
                nao->quantidade_musicas++;
            } else {
                printf("Erro ao cadastrar 'kwid'!\n");
            }
        }
    }

    return retorno;
}

static int cadastrar_playlists_predefinidas(PLAYLIST **raiz) {
    int retorno = 1;
    PLAYLIST *playlist1 = aloca_no_playlist(strdup("testando"));
    PLAYLIST *playlist2 = aloca_no_playlist(strdup("tome"));

    if (!playlist1 || !playlist2) {
        limpar_no_playlist(playlist1);
        limpar_no_playlist(playlist2);
        free(playlist1);
        free(playlist2);
        return 0;
    }

    retorno &= inserir_playlist(raiz, playlist1);
    printf(retorno ? "Playlist 'testando' cadastrada com sucesso!\n" : "Erro ao cadastrar 'testando'!\n");

    retorno &= inserir_playlist(raiz, playlist2);
    printf(retorno ? "Playlist 'tome' cadastrada com sucesso!\n" : "Erro ao cadastrar 'tome'!\n");

    return retorno;
}

static int adicionar_musicas_playlists_predefinidas(ARTISTAS *raiz_artista, PLAYLIST *raiz_playlist) {
    int retorno = 1;

    PLAYLIST *testando = existe_playlist(raiz_playlist, "testando");
    if (testando) {
        MUSICA_PLAYLIST *mp1 = alocar_musica_playlist(strdup("winx"), 2.66, strdup("rail"), strdup("SIM"));
        if (mp1) {
            retorno &= inserir_musica_playlist(&testando->arv_musicas_playlist, mp1);
            if (retorno) {
                printf("Musica 'winx' adicionada a playlist 'testando'!\n");
                testando->quantidade_musicas_playlist++;
            } else {
                printf("Erro ao adicionar 'winx' a playlist 'testando'!\n");
            }
        } else {
            retorno = 0;
        }
    }

    PLAYLIST *tome = existe_playlist(raiz_playlist, "tome");
    if (tome) {
        MUSICA_PLAYLIST *mp2 = alocar_musica_playlist(strdup("kwid"), 1.99, strdup("joao"), strdup("NAO"));
        if (mp2) {
            retorno &= inserir_musica_playlist(&tome->arv_musicas_playlist, mp2);
            if (retorno) {
                printf("Musica 'kwid' adicionada a playlist 'tome'!\n");
                tome->quantidade_musicas_playlist++;
            } else {
                printf("Erro ao adicionar 'kwid' a playlist 'tome'!\n");
            }
        } else {
            retorno = 0;
        }
    }

    return retorno;
}

void inicializar_dados_predefinidos(ARTISTAS **raiz_artista, PLAYLIST **raiz_playlist) {
    if (!cadastrar_artistas_predefinidos(raiz_artista)) {
        printf("Erro ao cadastrar artistas predefinidos!\n");
        return;
    }

    if (!cadastrar_albuns_predefinidos(*raiz_artista)) {
        printf("Erro ao cadastrar albuns predefinidos!\n");
        return;
    }

    if (!cadastrar_musicas_predefinidas(*raiz_artista)) {
        printf("Erro ao cadastrar musicas predefinidas!\n");
        return;
    }

    if (!cadastrar_playlists_predefinidas(raiz_playlist)) {
        printf("Erro ao cadastrar playlists predefinidas!\n");
        return;
    }

    if (!adicionar_musicas_playlists_predefinidas(*raiz_artista, *raiz_playlist)) {
        printf("Erro ao adicionar musicas as playlists predefinidas!\n");
        return;
    }

    printf("Dados predefinidos inicializados com sucesso!\n");
}