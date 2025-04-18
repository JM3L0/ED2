#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../ARQUIVOS_BIN.H/artista_BIN.h"
#include "../ARQUIVOS_BIN.H/album_BIN.h"
#include "../ARQUIVOS_BIN.H/musica_BIN.h"
#include "../ARQUIVOS_BIN.H/utilitarios_BIN.h"
#include "../ARQUIVOS_BIN.H/interface_BIN.h"
#include "../ARQUIVOS_BIN.H/playlist_BIN.h"
#include "../ARQUIVOS_BIN.H/musica_playlist_BIN.h"
#include "../ARQUIVOS_BIN.H/TESTES_BIN.h"

void menu_dos_print_de_opcoes()
{
    printf("\n[1] - Cadastrar Artista\n");
    printf("[2] - Cadastrar Album\n");
    printf("[3] - Cadastrar Musica\n");
    printf("[4] - Mostrar todos os artistas cadastrados\n");
    printf("[5] - Mostrar todos os artistas cadastrados de um determinado tipo\n");
    printf("[6] - Mostrar todos os artistas cadastrados de um determinado estilo musical\n");
    printf("[7] - Mostrar todos os artistas cadastrados de um determinado estilo musical e tipo de artista\n");
    printf("[8] - Mostrar todos os albuns de um determinado artista\n");
    printf("[9] - Mostrar todos os albuns de um determinado ano de um artista\n");
    printf("[10] - Mostrar todas as musicas de um determinado album de um determinado artista\n");
    printf("[11] - Mostrar todos os albuns de um determinado ano de todos os artistas cadastrados\n");
    printf("[12] - Mostrar  os  dados  de  uma  determinada  Musica\n");
    printf("[13] - Cadastrar playlist\n");
    printf("[14] - Mostrar todos os dados de uma determinada playlist\n");
    printf("[15] - Remover uma musica de uma determinada playlist\n");
    printf("[16] - Remover uma playlist\n");
    printf("[17] - Remover  uma  musica  de  um  determinado  album  de  um  determinado  artista\n");
    printf("[18] - Adicionar musica a uma playlist\n");
    printf("[19] - Mostrar todas as playlists\n");
    printf("[0] - Encerrar Programa\n");
}

void menu_geral()
{
    ARTISTAS *raiz_artista, *no_artista;
    raiz_artista = NULL;
    no_artista = NULL;

    PLAYLIST *raiz_playlist, *no_playlist;
    raiz_playlist = NULL;
    no_playlist = NULL;

    int opcao, retorno = 0, opcao2;
    inicializar_dados_predefinidos(&raiz_artista, &raiz_playlist);
    do
    {
        menu_dos_print_de_opcoes();
        printf("\nDigite a opcao desejada: ");
        opcao = digitar_int();
        printf("\n[1] - SIM\n[0] - NAO\nDeseja prosseguir com a acao?: ");
        opcao2 = digitar_int();

        if(opcao2 == 1){
            switch (opcao)
            {
            case 1:
            {
                /* Cadastrar  Artista:  cadastrar  os  dados  dos  artistas  organizados
                em  uma  árvore  binária  pelo  nome  do artista  a  qualquer  momento,
                o  usuário  pode  cadastrar  um  artista  a  qualquer  momento,  não  permita cadastro repetido.*/

                no_artista = cadastrar_artista();
                if(no_artista == NULL)
                
                    printf("\nErro de alocacao!\n");
                else{

                    retorno = inserir_artista(&raiz_artista, no_artista);
                    if (retorno == 1)
                        printf("\nArtista cadastrado com sucesso!\n");
                    else
                        printf("\nArtista nao cadastrado! Ja Existente!\n");
                }

                pausar();
                break;
            }
            case 2:
            {
                /* Cadastrar Álbuns: cadastrar os dados de Álbuns de um determinado artista organizados em uma
                árvore binária pelo título do álbum a qualquer momento, lembre-se um álbum só pode ser cadastrado
                para um artista já castrado e o álbum não pode se repetir para um mesmo artista.*/
                char nome_artista[50];

                printf("Digite o nome do artista para cadastrar o album: ");
                ler_string_simples(nome_artista, sizeof(nome_artista));

                ARTISTAS *artista = existe_artista(raiz_artista, nome_artista);

                if (artista)
                {
                    ALBUNS *album;
                    album = NULL;

                    album = cadastrar_album(album);

                    if (album == NULL)
                        printf("\nErro de alocacao!\n");
                    else{

                        retorno = inserir_album(&artista->arv_albuns, album);
        
                        if (retorno == 1)
                        {
                            printf("\nAlbum cadastrado com sucesso!\n");
                            artista->numero_albuns++;
                        }
                        else
                        {
                            printf("\nAlbum nao cadastrado!\n");
                            printf("Ja existente para esse Artista!\n");
                        }
                    }
                }
                else
                    printf("\nArtista nao encontrado!\n");

                pausar();
                break;
            }
            case 3:
            {
                /* Cadastrar Músicas:   cadastrar as músicas de um álbum de um artista em uma árvore binária organizada pelo título,
                lembre-se uma música só pode ser cadastrada para um álbum que já existe e a música não pode se repetir para um mesmo álbum.*/
                //  printf("\nCADASTRAR UMA MUSICA\n");

                char nome_artista[50];
                printf("Digite o nome do artista para cadastrar a musica: ");
                ler_string_simples(nome_artista, sizeof(nome_artista));
                ARTISTAS *artista = existe_artista(raiz_artista, nome_artista);

                if (artista)
                {
                    char titulo_album[50];
                    printf("Digite o titulo do album ao qual a musica pertence: ");
                    ler_string_simples(titulo_album, sizeof(titulo_album));
                    ALBUNS *album = existe_album(artista->arv_albuns, titulo_album);

                    if (album)
                    {
                        MUSICAS *musica;
                        musica = NULL;

                        musica = cadastrar_musica(musica);
                        
                        if (musica == NULL)
                            printf("\nErro de alocacao!\n");
                        else
                        {
                            retorno = inserir_musica(&album->arv_musicas, musica);
        
                            if (retorno == 1)
                            {
                                printf("\nMusica cadastrada com sucesso!\n");
                                album->quantidade_musicas++;
                            }
                            else
                                printf("\nMusica nao cadastrada! Ja existente!\n");
                        }
                    }
                    else
                        printf("\nAlbum nao encontrado!\n");
                }
                else
                    printf("\nArtista nao encontrado!\n");
                pausar();
                break;
            }
            case 4:
            {
                // Mostrar todos os artistas cadastrados
                printf("\n\n");
                printf("Artistas cadastrados:\n\n");
                printf("--------------------------------------------------\n");

                retorno = imprimir_todos_os_dados_dos_artistas(raiz_artista);
                if (retorno == 0)
                    printf("\nNenhum artista cadastrado!\n");
                pausar();
                break;
            }
            case 5:
            {
                // Mostrar todos os artistas cadastrados de um determinado tipo
                char tipo_artista[50];
                printf("Digite o tipo de artista: ");
                ler_string_simples(tipo_artista, sizeof(tipo_artista));

                printf("\n\n");
                printf("Artistas cadastrados do tipo %s:\n\n", tipo_artista);
                printf("--------------------------------------------------\n");

                retorno = imprimir_artista_tipo(raiz_artista, tipo_artista);
                if (retorno == 0)
                    printf("\nNenhum artista desse tipo!\n");
                pausar();
                break;
            }
            case 6:
            {
                // Mostrar todos os artistas cadastrados de um determinado estilo musical.
                char estilo_musical[50];
                printf("Digite o estilo musical: ");
                ler_string_simples(estilo_musical, sizeof(estilo_musical));

                printf("\n\n");
                printf("Artistas cadastrados do estilo %s:\n\n", estilo_musical);
                printf("--------------------------------------------------\n");

                retorno = imprimir_artista_estilo(raiz_artista, estilo_musical); 
                if (retorno == 0)
                    printf("\nNenhum artista desse estilo!\n");
                pausar();
                break;
            }
            case 7:
            {
                // Mostrar todos os artistas cadastrados de um determinado estilo musical e tipo de artista.
                char estilo_musical[50], tipo_artista[50];
                printf("Digite o tipo de artista: ");
                ler_string_simples(tipo_artista, sizeof(tipo_artista));
                printf("Digite o estilo musical: ");
                ler_string_simples(estilo_musical, sizeof(estilo_musical));

                printf("\n\n");
                printf("Artistas cadastrados do estilo %s e tipo %s:\n\n", estilo_musical, tipo_artista);
                printf("--------------------------------------------------\n");

                retorno = imprimir_artista_estilo_e_tipo(raiz_artista, estilo_musical, tipo_artista);
                if (retorno == 0)
                    printf("\nNenhum artista desse estilo e tipo!\n");
                pausar();
                break;
            }
            case 8:
            {
                // Mostrar todos os álbuns de um determinado artista.
                char nome_artista[50];
                printf("Digite o nome do artista: ");
                ler_string_simples(nome_artista, sizeof(nome_artista));
                ARTISTAS *artista = existe_artista(raiz_artista, nome_artista);

                if (artista)
                {

                    printf("\n\n");
                    printf("Albuns cadastrados do artista %s:\n\n", nome_artista);
                    printf("--------------------------------------------------\n");

                    retorno = imprimir_todos_albuns_de_um_artista(raiz_artista, nome_artista);
                    if (retorno == 0)
                        printf("\nNenhum album cadastrado desse artista!\n");
                }
                else
                    printf("\nArtista nao encontrado!\n");
                pausar();
                break;
            }
            case 9:
            {
                // Mostrar todos os álbuns de um determinado ano de um artista
                char nome_artista[50];
                printf("Digite o nome do artista: ");
                ler_string_simples(nome_artista, sizeof(nome_artista));
                ARTISTAS *artista = existe_artista(raiz_artista, nome_artista);

                if (artista)
                {
                    int ano_lancamento;

                    printf("Digite o ano de lancamento: ");
                    ano_lancamento = digitar_int();

                    printf("\n\n");
                    printf("Albuns cadastrados do artista %s no ano %d:\n\n", nome_artista, ano_lancamento);
                    printf("--------------------------------------------------\n");

                    retorno = imprimir_albuns_artita_ano(raiz_artista, nome_artista, ano_lancamento);
                    if (retorno == 0)
                        printf("\nNenhum album desse desse ano para esse artista!\n");
                }
                else
                    printf("\nArtista nao encontrado!\n");
                pausar();
                break;
            }
            case 10:
            {
                // Mostrar todas as músicas de um determinado álbum de um determinado artista
                char nome_artista[50];
                printf("Digite o nome do artista: ");
                ler_string_simples(nome_artista, sizeof(nome_artista));

                ARTISTAS *artista = existe_artista(raiz_artista, nome_artista);

                if (artista)
                {
                    retorno = imprimir_todos_albuns(artista->arv_albuns);
                    if (retorno)
                    {

                        char titulo_album[50];
                        printf("Digite o titulo do album: ");
                        ler_string_simples(titulo_album, sizeof(titulo_album));
                        ALBUNS *album = existe_album(artista->arv_albuns, titulo_album);
                        if (album)
                        {

                            printf("\n\n");
                            printf("Musicas cadastradas do album %s do artista %s:\n\n", titulo_album, nome_artista);
                            printf("--------------------------------------------------\n");

                            retorno = imprimir_todas_as_musicas(album->arv_musicas);
                            if (retorno == 0)
                                printf("\nNenhuma musica cadastrada desse album!\n");
                        }
                        else
                            printf("\nAlbum nao encontrado!\n");
                    }
                    else
                        printf("\nNenhum album cadastrado desse artista!\n");
                }
                else
                    printf("\nArtista nao encontrado!\n");
                pausar();
                break;
            }
            case 11:
            {
                // Mostrar todos os álbuns de um determinado ano de todos os artistas cadastrados
                int ano_lancamento;
                printf("Digite o ano de lancamento: ");
                ano_lancamento = digitar_int();

                printf("\n\n");
                printf("Albuns cadastrados no ano %d:\n\n", ano_lancamento);
                printf("--------------------------------------------------\n");

                retorno = mostrar_todos_artistas_album_ano(raiz_artista, ano_lancamento);
                if (retorno == 0)
                    printf("\nNenhum artista tem album cadastrado no ano %d!\n", ano_lancamento);
                pausar();
                break;
            }
            case 12:
            {
                /* Mostrar  os  dados  de  uma  determinada  Música  (usuário  entrar  com  o  título  da  música):
                nome  artista, título do álbum, ano de lançamento*/
                char nome_musica[50];
                printf("Digite o nome da musica: ");
                ler_string_simples(nome_musica, sizeof(nome_musica));

                printf("\n\n");
                printf("Dados da musica %s:\n\n", nome_musica);
                printf("--------------------------------------------------\n");

                retorno = imprime_dados_da_musica_album_artista(raiz_artista, nome_musica);
                if (retorno == 0)
                    printf("\nEssa musica nao foi encontrada!\n");
                pausar();
                break;
            }
            case 13:
            {
                /*Criar uma árvore binária de playlists, na qual cada playlist contém um nome (organização da árvore),
                e uma árvore binária de músicas. Não permita playlist com títulos iguais. As músicas das playlists devem
                estar cadastradas na árvore de músicas de um álbum de um artista. A árvore de música da playlist deve conter:
                nome do artista, título do álbum e o título da Música.*/
                no_playlist = cadastrar_playlist();
                if (no_playlist == NULL)
                    printf("\nErro de alocacao!\n");
                else
                {
                    retorno = inserir_playlist(&raiz_playlist, no_playlist);

                    if (retorno == 1)
                        printf("\nPlaylist cadastrada com sucesso!\n");
                    else
                        printf("\nErro ao cadastrar! Playlist ja existente\n");
                }
                pausar();
                break;
            }
            case 14:
            {
                // Mostrar todos os dados de uma determinada playlist
                char nome_playlist[50];
                printf("Digite o nome da playlist: ");
                ler_string_simples(nome_playlist, sizeof(nome_playlist));
                PLAYLIST *playlist = existe_playlist(raiz_playlist, nome_playlist);

                if (playlist)
                {
                    printf("\n\n");
                    printf("Dados da playlist %s:\n\n", nome_playlist);
                    printf("--------------------------------------------------\n");

                    retorno = imprime_dados_de_uma_playlist(playlist);
                }
                else
                    printf("\nPlaylist nao encontrada!\n");
                pausar();
                break;
            }
            case 15:
            { // Permita remover uma música de uma determinada playlist.

                char nome_playlist[50];
                printf("Digite o nome da playlist: ");
                ler_string_simples(nome_playlist, sizeof(nome_playlist));
                PLAYLIST *playlist = existe_playlist(raiz_playlist, nome_playlist);

                if (playlist)
                {
                    char titulo_musica[50], artista_musica[50], album_musica[50];

                    printf("Digite o nome da musica: ");
                    ler_string_simples(titulo_musica, sizeof(titulo_musica));
                    printf("Digite o nome do artista: ");
                    ler_string_simples(artista_musica, sizeof(artista_musica));
                    printf("Digite o nome do album: ");
                    ler_string_simples(album_musica, sizeof(album_musica));

                    retorno = remove_musica_playlist(&playlist->arv_musicas_playlist, titulo_musica, album_musica, artista_musica);
                    if (retorno == 1)
                    {
                        printf("\nMusica removida com sucesso!\n");
                        playlist->quantidade_musicas_playlist--;
                    }
                    else
                        printf("\nMusica nao encontrada!\n");
                }
                else
                    printf("\nPlaylist nao encontrada!\n");
                pausar();
                break;
            }
            case 16:
            {
                // Permita remover uma playlist, lembre-se de remover a árvore binária da playlist também.
                char nome_playlist[50];
                printf("Digite o nome da playlist: ");
                ler_string_simples(nome_playlist, sizeof(nome_playlist));

                PLAYLIST *playlist = existe_playlist(raiz_playlist, nome_playlist);

                if (playlist)
                {
                    retorno = remove_playlist(&raiz_playlist, nome_playlist);
                    if (retorno == 1)
                        printf("\nPlaylist removida com sucesso!\n");
                    else
                        printf("\nErro ao remover playlist!\n");
                }
                else
                    printf("\nPlaylist nao encontrada!\n");
                pausar();
                break;
            }
            case 17:
            {
                /* Permita remover uma música de um determinado álbum de um determinado artista. Lembre-se só
                poderá ser removida se não fizer parte de nenhuma playlist.*/

                char nome_artista[50];
                printf("Digite o nome do artista: ");
                ler_string_simples(nome_artista, sizeof(nome_artista));
                ARTISTAS *artista = existe_artista(raiz_artista, nome_artista);

                if (artista)
                {
                    char titulo_album[50];
                    printf("Digite o titulo do album: ");
                    ler_string_simples(titulo_album, sizeof(titulo_album));
                    ALBUNS *album = existe_album(artista->arv_albuns, titulo_album);

                    if (album)
                    {
                        char titulo_musica[50];
                        printf("Digite o titulo da musica: ");
                        ler_string_simples(titulo_musica, sizeof(titulo_musica));

                        retorno = percorre_todas_as_playlists_buscando_uma_musica(raiz_playlist, titulo_musica, nome_artista, titulo_album);
                        if (retorno == 1)
                            printf("\nEssa musica faz parte de uma playlist, nao pode ser removida!\n");
                        else
                        {
                            retorno = remove_musica(&album->arv_musicas, titulo_musica);
                            if (retorno == 1)
                            {
                                printf("\nMusica removida com sucesso!\n");
                                album->quantidade_musicas--;
                            }
                            else
                                printf("\nMusica nao encontrada!\n");
                        }
                    }
                    else
                        printf("\nAlbum nao encontrado!\n");
                }
                else
                    printf("\nArtista nao encontrado!\n");
                pausar();
                break;
            }
            case 18:
            { /*Adicionar musica a uma playlist*/
                char nome_playlist[50];
                printf("Digite o nome da playlist: ");
                ler_string_simples(nome_playlist, sizeof(nome_playlist));
                PLAYLIST *playlist = existe_playlist(raiz_playlist, nome_playlist);

                if (playlist)
                {
                    retorno = cadastrar_musica_playlist(raiz_artista, playlist);
                    if (retorno == 1)
                        playlist->quantidade_musicas_playlist++;
                    mensagens_de_erro_e_sucesso_add_musica_playlist(retorno);
                }
                else
                    printf("\nPlaylist nao encontrada!\n");
                pausar();
                break;
            }
            case 19:
            {
                // Mostrar todas as playlists
                printf("\n\n");
                printf("Playlists cadastradas:\n\n");
                printf("--------------------------------------------------\n");

                retorno = imprimir_todas_as_playlists(raiz_playlist);
                if (retorno == 0)
                    printf("\nNenhuma playlist cadastrada!\n");
                pausar();
                break;
            }
            case 0:
                printf("\n\nSaindo do programa...\n");

                liberar_arv_artista(&raiz_artista);
                liberar_arv_playlist(&raiz_playlist);

                printf("Memoria liberada com sucesso!\n");
                printf("Programa encerrado.\n\n");
                break;
            default:
                printf("Opcao invalida!\n");
                pausar();
            }
        }
    } while (opcao != 0 || opcao2 == 0);
}
