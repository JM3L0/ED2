#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <time.h>
#include "../ARQUIVOS_AVL.H/TESTES_AVL.h"
#include "../ARQUIVOS_AVL.H/STRUCTS_AVL.h"
#include "../ARQUIVOS_AVL.H/artista_AVL.h"
#include "../ARQUIVOS_AVL.H/album_AVL.h"
#include "../ARQUIVOS_AVL.H/musica_AVL.h"
#include "../ARQUIVOS_AVL.H/musica_playlist_AVL.h"


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
    ALBUNS *album4 = aloca_no_album(strdup("teste"), 2001); // Album duplicado para teste

    if (!album1 || !album2 || !album3 || !album4) {
        limpar_no_album(album1);
        limpar_no_album(album2);
        limpar_no_album(album3);
        limpar_no_album(album4);
        free(album1);
        free(album2);
        free(album3);
        free(album4);
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
        retorno &= inserir_album(&rail->arv_albuns, album4);
        if (retorno) {
            printf("Album 'teste' cadastrado para rail!\n");
            rail->numero_albuns++;
        } else {
            printf("Erro ao cadastrar 'teste'!\n");
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
    MUSICAS *musica4 = alocar_musica(strdup("b"), 2.99);
    MUSICAS *musica5 = alocar_musica(strdup("a"), 2.99);
    MUSICAS *musica6 = alocar_musica(strdup("c"), 2.99);
    MUSICAS *musica7 = alocar_musica(strdup("d"), 2.99);
    MUSICAS *musica8 = alocar_musica(strdup("e"), 2.99);

    if (!musica1 || !musica2 || !musica3 || !musica4 || !musica5 || !musica6 || !musica7 || !musica8) {
        limpar_no_musica(musica1);
        limpar_no_musica(musica2);
        limpar_no_musica(musica3);
        limpar_no_musica(musica4);
        limpar_no_musica(musica5);
        limpar_no_musica(musica6);
        limpar_no_musica(musica7);
        limpar_no_musica(musica8);
        free(musica1);
        free(musica2);
        free(musica3);
        free(musica4);
        free(musica5);
        free(musica6);
        free(musica7);
        free(musica8);
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

        ALBUNS *teste = existe_album(rail->arv_albuns, "teste");
        if (teste) {
            retorno &= inserir_musica(&teste->arv_musicas, musica4);
            if (retorno) {
                printf("Musica 'b' cadastrada em teste!\n");
                teste->quantidade_musicas++;
            } else {
                printf("Erro ao cadastrar 'b'!\n");
            }
        }

        if (teste) {
            retorno &= inserir_musica(&teste->arv_musicas, musica5);
            if (retorno) {
                printf("Musica 'a' cadastrada em teste!\n");
                teste->quantidade_musicas++;
            } else {
                printf("Erro ao cadastrar 'a'!\n");
            }
        }

        if (teste) {
            retorno &= inserir_musica(&teste->arv_musicas, musica6);
            if (retorno) {
                printf("Musica 'c' cadastrada em teste!\n");
                teste->quantidade_musicas++;
            } else {
                printf("Erro ao cadastrar 'c'!\n");
            }
        }

        if (teste) {
            retorno &= inserir_musica(&teste->arv_musicas, musica7);
            if (retorno) {
                printf("Musica 'd' cadastrada em teste!\n");
                teste->quantidade_musicas++;
            } else {
                printf("Erro ao cadastrar 'd'!\n");
            }
        }

        if (teste) {
            retorno &= inserir_musica(&teste->arv_musicas, musica8);
            if (retorno) {
                printf("Musica 'e' cadastrada em teste!\n");
                teste->quantidade_musicas++;
            } else {
                printf("Erro ao cadastrar 'e'!\n");
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
    PLAYLIST *playlist3 = aloca_no_playlist(strdup("teste"));

    if (!playlist1 || !playlist2 || !playlist3) {
        limpar_no_playlist(playlist1);
        limpar_no_playlist(playlist2);
        limpar_no_playlist(playlist3);
        free(playlist1);
        free(playlist2);
        free(playlist3);
        return 0;
    }

    retorno &= inserir_playlist(raiz, playlist1);
    printf(retorno ? "Playlist 'testando' cadastrada com sucesso!\n" : "Erro ao cadastrar 'testando'!\n");

    retorno &= inserir_playlist(raiz, playlist2);
    printf(retorno ? "Playlist 'tome' cadastrada com sucesso!\n" : "Erro ao cadastrar 'tome'!\n");
    
    retorno &= inserir_playlist(raiz, playlist3);
    printf(retorno ? "Playlist 'teste' cadastrada com sucesso!\n" : "Erro ao cadastrar 'teste'!\n");

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

    PLAYLIST *teste = existe_playlist(raiz_playlist, "teste");
    if (teste) {
        MUSICA_PLAYLIST *mp3 = alocar_musica_playlist(strdup("b"), 2.66, strdup("rail"), strdup("teste"));
        if (mp3) {
            retorno &= inserir_musica_playlist(&teste->arv_musicas_playlist, mp3);
            if (retorno) {
                printf("Musica 'b' adicionada a playlist 'teste'!\n");
                teste->quantidade_musicas_playlist++;
            } else {
                printf("Erro ao adicionar 'b' a playlist 'teste'!\n");
            }
        } else {
            retorno = 0;
        }
    }

    teste = existe_playlist(raiz_playlist, "teste");
    if (teste) {
        MUSICA_PLAYLIST *mp4 = alocar_musica_playlist(strdup("a"), 2.66, strdup("rail"), strdup("teste"));
        if (mp4) {
            retorno &= inserir_musica_playlist(&teste->arv_musicas_playlist, mp4);
            if (retorno) {
                printf("Musica 'a' adicionada a playlist 'teste'!\n");
                teste->quantidade_musicas_playlist++;
            } else {
                printf("Erro ao adicionar 'a' a playlist 'teste'!\n");
            }
        } else {
            retorno = 0;
        }
    }

    teste = existe_playlist(raiz_playlist, "teste");
    if (teste) {
        MUSICA_PLAYLIST *mp5 = alocar_musica_playlist(strdup("c"), 2.66, strdup("rail"), strdup("teste"));
        if (mp5) {
            retorno &= inserir_musica_playlist(&teste->arv_musicas_playlist, mp5);
            if (retorno) {
                printf("Musica 'c' adicionada a playlist 'teste'!\n");
                teste->quantidade_musicas_playlist++;
            } else {
                printf("Erro ao adicionar 'c' a playlist 'teste'!\n");
            }
        } else {
            retorno = 0;
        }
    }

    teste = existe_playlist(raiz_playlist, "teste");
    if (teste) {
        MUSICA_PLAYLIST *mp6 = alocar_musica_playlist(strdup("d"), 2.66, strdup("rail"), strdup("teste"));
        if (mp6) {
            retorno &= inserir_musica_playlist(&teste->arv_musicas_playlist, mp6);
            if (retorno) {
                printf("Musica 'd' adicionada a playlist 'teste'!\n");
                teste->quantidade_musicas_playlist++;
            } else {
                printf("Erro ao adicionar 'd' a playlist 'teste'!\n");
            }
        } else {
            retorno = 0;
        }
    }

    teste = existe_playlist(raiz_playlist, "teste");
    if (teste) {
        MUSICA_PLAYLIST *mp7 = alocar_musica_playlist(strdup("e"), 2.66, strdup("rail"), strdup("teste"));
        if (mp7) {
            retorno &= inserir_musica_playlist(&teste->arv_musicas_playlist, mp7);
            if (retorno) {
                printf("Musica 'e' adicionada a playlist 'teste'!\n");
                teste->quantidade_musicas_playlist++;
            } else {
                printf("Erro ao adicionar 'e' a playlist 'teste'!\n");
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