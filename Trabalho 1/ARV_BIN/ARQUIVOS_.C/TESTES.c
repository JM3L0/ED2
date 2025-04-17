#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../ARQUIVOS_.H/TESTES.h"
#include "../ARQUIVOS_.H/STRUCTS.h"
#include "../ARQUIVOS_.H/artista.h"
#include "../ARQUIVOS_.H/album.h"
#include "../ARQUIVOS_.H/musica.h"

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