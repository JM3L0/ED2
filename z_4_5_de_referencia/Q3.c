#include <stdio.h>
#include <stdlib.h>
#include <string.h>   // Para strcpy, strcmp
#include <limits.h>   // Para INT_MIN em alguns casos, se necessário
#include <inttypes.h> // Para PRIu64 e SCNu64

// --- Configurações Globais ---
// O tamanho do bloco é 1MB.
const unsigned long long TAMANHO_BLOCO_EM_BYTES = 1024ULL * 1024ULL;
unsigned long long ENDERECO_MEMORIA_TOTAL_FINAL_GLOBAL_CONFIG;
unsigned long long ENDERECO_BASE_MEMORIA_GLOBAL_CONFIG = 0;

// --- Estruturas de Dados ---
typedef struct MemSegmento
{
    char situacao; // 'L' para Livre, 'O' para Ocupado
    int bloco_inicial;
    int bloco_final;
    unsigned long long endereco_inicial_seg;
    unsigned long long endereco_final_seg;
} MemSegmento;

typedef struct Arv45_Mem
{
    MemSegmento infos[4];        // Máximo de 4 segmentos por nó
    int nInfos;                  // Número de segmentos válidos neste nó (0 a 4)
    struct Arv45_Mem *filhos[5]; // Máximo de 5 filhos (índices 0 a 4)
} Arv45_Mem;

// --- Enum para Status da Remoção (da árvore) ---
typedef enum
{
    ARV_STATUS_OK,
    ARV_STATUS_UNDERFLOW,
    ARV_STATUS_ERRO,
    ARV_STATUS_NAO_ENCONTRADO
} ArvStatusOperacao;

// --- Protótipos das Funções ---

// Funções Auxiliares para MemSegmento
int calcular_num_blocos_para_seg(MemSegmento seg);
void imprimir_info_segmento(MemSegmento seg);
MemSegmento criar_obj_segmento(char situacao, int b_ini, int b_fim, unsigned long long e_ini, unsigned long long e_fim);
int segmentos_sao_adjacentes_e_mesmo_estado(MemSegmento s1, MemSegmento s2);
MemSegmento fundir_dois_segmentos_mem(MemSegmento s1, MemSegmento s2);

// Funções da Árvore 4-5 Adaptada para MemSegmento
Arv45_Mem *cria_no_arv_mem(MemSegmento seg_inicial, Arv45_Mem *filho_esq, Arv45_Mem *filho_dir_seg_inicial);
int eh_folha_arv_mem(Arv45_Mem *no);
void adiciona_seg_em_no_com_espaco_arv_mem(Arv45_Mem *no_atual, MemSegmento seg_novo, Arv45_Mem *filho_dir_seg_novo);
Arv45_Mem *quebra_no_arv_mem(Arv45_Mem **no_original_ptr, MemSegmento seg_a_inserir, Arv45_Mem *filho_dir_seg_a_inserir, MemSegmento *segmento_promovido_saida);
int insere_seg_recursivo_arv_mem(Arv45_Mem **raiz_ptr, MemSegmento seg_para_inserir, MemSegmento *segmento_promovido_acima, Arv45_Mem **novo_irmao_a_direita);
int insere_segmento_na_arvore_mem(Arv45_Mem **raiz, MemSegmento seg_para_inserir);

Arv45_Mem *buscar_menor_elemento_no_arv_mem(Arv45_Mem *no);
// Protótipos movidos para antes de tratar_underflow_na_arv_mem
ArvStatusOperacao redistribuir_esq_arv_mem(Arv45_Mem *filho_underflow, Arv45_Mem *pai_no, Arv45_Mem *irmao_esq, int pos_filho_no_pai);
ArvStatusOperacao redistribuir_dir_arv_mem(Arv45_Mem *filho_underflow, Arv45_Mem *pai_no, Arv45_Mem *irmao_dir, int pos_filho_no_pai);
ArvStatusOperacao fundir_esq_arv_mem(Arv45_Mem **ponteiro_filho_em_underflow, Arv45_Mem *pai_no, Arv45_Mem *irmao_esq, int pos_filho_no_pai);
ArvStatusOperacao fundir_dir_arv_mem(Arv45_Mem **ponteiro_filho_em_underflow, Arv45_Mem *pai_no, Arv45_Mem *irmao_dir, int pos_filho_no_pai);
ArvStatusOperacao tratar_underflow_na_arv_mem(Arv45_Mem **ponteiro_filho_no_pai, Arv45_Mem *pai_no, int pos_filho_no_pai);
ArvStatusOperacao remover_seg_recursivo_arv_mem(Arv45_Mem **ponteiro_no_atual_ptr, int bloco_inicial_chave_remover, Arv45_Mem *pai_do_no_atual);
int remover_segmento_da_arvore_mem(Arv45_Mem **raiz, int bloco_inicial_chave_remover);

// Funções de Gerenciamento de Memória de Alto Nível
void percorrer_arvore_mem_em_ordem_e_executar(Arv45_Mem *raiz, void (*acao_sobre_segmento)(MemSegmento));
int buscar_segmento_que_contem_bloco_arv_mem(Arv45_Mem *raiz, int bloco_num, MemSegmento *segmento_encontrado_copia);
int buscar_segmento_adjacente_na_arvore_mem(Arv45_Mem *raiz, MemSegmento segmento_ref, int buscar_anterior, MemSegmento *adjacente_encontrado_copia);
void consolidar_segmentos_na_arvore_mem(Arv45_Mem **raiz, int bloco_inicial_referencia);

void cadastrar_layout_inicial_memoria_usr(Arv45_Mem **raiz);
int alocar_blocos_memoria_usr(Arv45_Mem **raiz, int num_blocos_req, MemSegmento *segmento_alocado_info_saida);
int liberar_blocos_memoria_usr(Arv45_Mem **raiz, int bloco_inicial_liberar, int bloco_final_liberar);

void libera_arvore_mem_completa(Arv45_Mem **raiz);
void menu_gerenciador_memoria(Arv45_Mem **raiz);
void imprime_arvore_mem_visual(Arv45_Mem *raiz, char *prefixo, int eh_ultimo, int eh_raiz);

// --- Implementações ---

// Funções Auxiliares para MemSegmento
int calcular_num_blocos_para_seg(MemSegmento seg)
{
    int num_b = 0;
    if (seg.bloco_final >= seg.bloco_inicial)
    {
        num_b = seg.bloco_final - seg.bloco_inicial + 1;
    }
    return num_b;
}

void imprimir_info_segmento(MemSegmento seg)
{
    printf("Seg: Bloco %d-%d (%d blocos), End: %" PRIu64 "-%" PRIu64 ", Situ: %c\n", // Uso de PRIu64
           seg.bloco_inicial, seg.bloco_final, calcular_num_blocos_para_seg(seg),
           seg.endereco_inicial_seg, seg.endereco_final_seg,
           seg.situacao);
}
MemSegmento criar_obj_segmento(char situacao, int b_ini, int b_fim, unsigned long long e_ini, unsigned long long e_fim)
{
    MemSegmento seg;
    seg.situacao = situacao;
    seg.bloco_inicial = b_ini;
    seg.bloco_final = b_fim;
    seg.endereco_inicial_seg = e_ini;
    seg.endereco_final_seg = e_fim;
    return seg;
}

int segmentos_sao_adjacentes_e_mesmo_estado(MemSegmento s1, MemSegmento s2)
{
    int adjacentes = 0;
    // Verifica se os segmentos são válidos (bloco_inicial != -1 é um indicador comum de invalidade)
    if (s1.bloco_inicial != -1 && s2.bloco_inicial != -1 && s1.situacao == s2.situacao)
    {
        if (s1.bloco_final + 1 == s2.bloco_inicial)
        {
            adjacentes = 1;
        }
        else if (s2.bloco_final + 1 == s1.bloco_inicial)
        {
            adjacentes = 1;
        }
    }
    return adjacentes;
}

MemSegmento fundir_dois_segmentos_mem(MemSegmento s1, MemSegmento s2)
{
    MemSegmento fundido;
    fundido.situacao = s1.situacao;
    if (s1.bloco_inicial < s2.bloco_inicial)
    {
        fundido.bloco_inicial = s1.bloco_inicial;
        fundido.endereco_inicial_seg = s1.endereco_inicial_seg;
        fundido.bloco_final = s2.bloco_final;
        fundido.endereco_final_seg = s2.endereco_final_seg;
    }
    else
    {
        fundido.bloco_inicial = s2.bloco_inicial;
        fundido.endereco_inicial_seg = s2.endereco_inicial_seg;
        fundido.bloco_final = s1.bloco_final;
        fundido.endereco_final_seg = s1.endereco_final_seg;
    }
    return fundido;
}

// Funções da Árvore 4-5 Adaptada
Arv45_Mem *cria_no_arv_mem(MemSegmento seg_inicial, Arv45_Mem *filho_esq, Arv45_Mem *filho_dir_seg_inicial)
{
    Arv45_Mem *no = (Arv45_Mem *)malloc(sizeof(Arv45_Mem));
    int i;
    if (no != NULL)
    {
        no->infos[0] = seg_inicial;
        no->nInfos = 1;
        no->filhos[0] = filho_esq;
        no->filhos[1] = filho_dir_seg_inicial;
        for (i = 2; i < 5; i++)
        {
            no->filhos[i] = NULL;
        }
    }
    return no;
}

int eh_folha_arv_mem(Arv45_Mem *no)
{
    int resultado = 0;
    if (no != NULL)
    {
        resultado = 1;
        int i = 0;
        // Não usar break
        int continuar_loop_filhos = 1;
        while (i <= no->nInfos && i < 5 && continuar_loop_filhos)
        {
            if (no->filhos[i] != NULL)
            {
                resultado = 0;
                continuar_loop_filhos = 0; // Para o loop
            }
            i++;
        }
        if (resultado == 1 && no->nInfos == 0 && no->filhos[0] != NULL)
        {
            resultado = 0;
        }
    }
    return resultado;
}

void adiciona_seg_em_no_com_espaco_arv_mem(Arv45_Mem *no_atual, MemSegmento seg_nova, Arv45_Mem *filho_dir_seg_nova)
{
    if (no_atual != NULL && no_atual->nInfos < 4)
    {
        int j;
        int pos_insercao = 0;
        while (pos_insercao < no_atual->nInfos && seg_nova.bloco_inicial > no_atual->infos[pos_insercao].bloco_inicial)
        {
            pos_insercao++;
        }

        for (j = no_atual->nInfos - 1; j >= pos_insercao; j--)
        {
            no_atual->infos[j + 1] = no_atual->infos[j];
        }
        for (j = no_atual->nInfos; j >= pos_insercao + 1; j--)
        {
            no_atual->filhos[j + 1] = no_atual->filhos[j];
        }

        no_atual->infos[pos_insercao] = seg_nova;
        no_atual->filhos[pos_insercao + 1] = filho_dir_seg_nova;
        no_atual->nInfos++;
    }
    else
    {
        if (no_atual != NULL)
        {
            // fprintf(stderr, "Erro: Tentativa de adicionar segmento em no cheio (nInfos=%d) ou nulo sem quebra.\n", no_atual->nInfos);
        }
        else
        {
            fprintf(stderr, "Erro: Tentativa de adicionar segmento em no nulo.\n");
        }
    }
}

Arv45_Mem *quebra_no_arv_mem(Arv45_Mem **no_original_ptr, MemSegmento N_nova, Arv45_Mem *Nc_novo_filho_dir,
                             MemSegmento *segmento_promovido_saida)
{
    Arv45_Mem *no_orig = *no_original_ptr;
    Arv45_Mem *novo_no_dir = NULL;

    MemSegmento s1_orig = no_orig->infos[0], s2_orig = no_orig->infos[1], s3_orig = no_orig->infos[2], s4_orig = no_orig->infos[3];
    Arv45_Mem *c0_orig = no_orig->filhos[0], *c1_orig = no_orig->filhos[1], *c2_orig = no_orig->filhos[2],
              *c3_orig = no_orig->filhos[3], *c4_orig = no_orig->filhos[4];

    if (N_nova.bloco_inicial < s1_orig.bloco_inicial)
    {
        *segmento_promovido_saida = s2_orig;
        no_orig->infos[0] = N_nova;
        no_orig->infos[1] = s1_orig;
        no_orig->filhos[0] = c0_orig;
        no_orig->filhos[1] = Nc_novo_filho_dir;
        no_orig->filhos[2] = c1_orig;
        no_orig->nInfos = 2;
        novo_no_dir = cria_no_arv_mem(s3_orig, c2_orig, c3_orig);
        if (novo_no_dir)
        {
            novo_no_dir->infos[1] = s4_orig;
            novo_no_dir->filhos[2] = c4_orig;
            novo_no_dir->nInfos = 2;
        }
    }
    else if (N_nova.bloco_inicial < s2_orig.bloco_inicial)
    {
        *segmento_promovido_saida = s2_orig;
        no_orig->infos[1] = N_nova;
        no_orig->filhos[2] = Nc_novo_filho_dir;
        no_orig->nInfos = 2;
        novo_no_dir = cria_no_arv_mem(s3_orig, c2_orig, c3_orig);
        if (novo_no_dir)
        {
            novo_no_dir->infos[1] = s4_orig;
            novo_no_dir->filhos[2] = c4_orig;
            novo_no_dir->nInfos = 2;
        }
    }
    else if (N_nova.bloco_inicial < s3_orig.bloco_inicial)
    {
        *segmento_promovido_saida = N_nova;
        no_orig->filhos[2] = c2_orig;
        no_orig->nInfos = 2;
        novo_no_dir = cria_no_arv_mem(s3_orig, Nc_novo_filho_dir, c3_orig);
        if (novo_no_dir)
        {
            novo_no_dir->infos[1] = s4_orig;
            novo_no_dir->filhos[2] = c4_orig;
            novo_no_dir->nInfos = 2;
        }
    }
    else if (N_nova.bloco_inicial < s4_orig.bloco_inicial)
    {
        *segmento_promovido_saida = s3_orig;
        no_orig->nInfos = 2;
        novo_no_dir = cria_no_arv_mem(N_nova, c3_orig, Nc_novo_filho_dir);
        if (novo_no_dir)
        {
            novo_no_dir->infos[1] = s4_orig;
            novo_no_dir->filhos[2] = c4_orig;
            novo_no_dir->nInfos = 2;
        }
    }
    else
    {
        *segmento_promovido_saida = s3_orig;
        no_orig->nInfos = 2;
        novo_no_dir = cria_no_arv_mem(s4_orig, c3_orig, c4_orig);
        if (novo_no_dir)
        {
            novo_no_dir->infos[1] = N_nova;
            novo_no_dir->filhos[2] = Nc_novo_filho_dir;
            novo_no_dir->nInfos = 2;
        }
    }

    no_orig->infos[2] = (MemSegmento){0};
    no_orig->infos[3] = (MemSegmento){0};
    no_orig->filhos[3] = NULL;
    no_orig->filhos[4] = NULL;

    if (novo_no_dir != NULL)
    {
        if (novo_no_dir->nInfos < 2)
        {
            novo_no_dir->infos[1] = (MemSegmento){0};
            novo_no_dir->filhos[2] = NULL;
        }
        for (int k = novo_no_dir->nInfos; k < 4; ++k)
            novo_no_dir->infos[k] = (MemSegmento){0};
        for (int k = novo_no_dir->nInfos + 1; k < 5; ++k)
            novo_no_dir->filhos[k] = NULL;
    }
    return novo_no_dir;
}

int insere_seg_recursivo_arv_mem(Arv45_Mem **raiz_ptr, MemSegmento seg_para_inserir,
                                 MemSegmento *segmento_promovido_acima, Arv45_Mem **novo_irmao_a_direita)
{
    int status_operacao = 0;
    Arv45_Mem *no_atual = (raiz_ptr != NULL) ? *raiz_ptr : NULL;

    *segmento_promovido_acima = (MemSegmento){.bloco_inicial = -1};
    *novo_irmao_a_direita = NULL;

    if (no_atual == NULL)
    {
        *segmento_promovido_acima = seg_para_inserir;
        status_operacao = 1;
    }
    else
    {
        int duplicado = 0;
        int i_dup = 0;
        // Não usar break no loop de duplicado
        while (i_dup < no_atual->nInfos && !duplicado)
        {
            if (seg_para_inserir.bloco_inicial == no_atual->infos[i_dup].bloco_inicial)
            {
                duplicado = 1;
            }
            i_dup++;
        }

        if (duplicado)
        {
            fprintf(stderr, "Segmento com bloco inicial %d ja existe.\n", seg_para_inserir.bloco_inicial);
        }
        else
        {
            if (eh_folha_arv_mem(no_atual))
            {
                if (no_atual->nInfos < 4)
                { // 4 é MAX_SEGMENTOS_NO_NO
                    adiciona_seg_em_no_com_espaco_arv_mem(no_atual, seg_para_inserir, NULL);
                    status_operacao = 1;
                }
                else
                {
                    *novo_irmao_a_direita = quebra_no_arv_mem(raiz_ptr, seg_para_inserir, NULL, segmento_promovido_acima);
                    status_operacao = 1;
                }
            }
            else
            {
                Arv45_Mem **proximo_filho_ptr = NULL;
                int i = 0;
                while (i < no_atual->nInfos && seg_para_inserir.bloco_inicial > no_atual->infos[i].bloco_inicial)
                {
                    i++;
                }
                proximo_filho_ptr = &(no_atual->filhos[i]);

                MemSegmento seg_vindo_de_baixo;
                seg_vindo_de_baixo.bloco_inicial = -1;
                Arv45_Mem *filho_dir_vindo_de_baixo = NULL;

                if (insere_seg_recursivo_arv_mem(proximo_filho_ptr, seg_para_inserir, &seg_vindo_de_baixo, &filho_dir_vindo_de_baixo))
                {
                    if (seg_vindo_de_baixo.bloco_inicial != -1)
                    {
                        if (no_atual->nInfos < 4)
                        { // 4 é MAX_SEGMENTOS_NO_NO
                            adiciona_seg_em_no_com_espaco_arv_mem(no_atual, seg_vindo_de_baixo, filho_dir_vindo_de_baixo);
                            status_operacao = 1;
                        }
                        else
                        {
                            *novo_irmao_a_direita = quebra_no_arv_mem(raiz_ptr, seg_vindo_de_baixo, filho_dir_vindo_de_baixo, segmento_promovido_acima);
                            status_operacao = 1;
                        }
                    }
                    else
                    {
                        status_operacao = 1;
                    }
                }
            }
        }
    }
    return status_operacao;
}

int insere_segmento_na_arvore_mem(Arv45_Mem **raiz, MemSegmento seg_para_inserir)
{
    int sucesso = 0;
    MemSegmento segmento_promovido_da_raiz;
    segmento_promovido_da_raiz.bloco_inicial = -1;
    Arv45_Mem *novo_no_irmao_da_raiz = NULL;

    if (raiz != NULL)
    {
        if (*raiz == NULL)
        {
            *raiz = cria_no_arv_mem(seg_para_inserir, NULL, NULL);
            if (*raiz != NULL)
            {
                sucesso = 1;
            }
        }
        else
        {
            int duplicado = 0;
            int i_dup = 0;
            while (i_dup < (*raiz)->nInfos && !duplicado)
            {
                if (seg_para_inserir.bloco_inicial == (*raiz)->infos[i_dup].bloco_inicial)
                {
                    duplicado = 1;
                }
                i_dup++;
            }

            if (duplicado)
            {
                // fprintf(stderr, "Segmento com bloco inicial %d ja existe na raiz (insere_segmento_na_arvore_mem).\n", seg_para_inserir.bloco_inicial);
            }
            else
            {
                if (insere_seg_recursivo_arv_mem(raiz, seg_para_inserir, &segmento_promovido_da_raiz, &novo_no_irmao_da_raiz))
                {
                    if (segmento_promovido_da_raiz.bloco_inicial != -1)
                    {
                        Arv45_Mem *antiga_raiz = *raiz;
                        *raiz = cria_no_arv_mem(segmento_promovido_da_raiz, antiga_raiz, novo_no_irmao_da_raiz);
                        if (*raiz != NULL)
                        {
                            sucesso = 1;
                        }
                        else
                        {
                            fprintf(stderr, "Erro ao criar nova raiz apos quebra!\n");
                            if (novo_no_irmao_da_raiz)
                                free(novo_no_irmao_da_raiz);
                        }
                    }
                    else
                    {
                        sucesso = 1;
                    }
                }
            }
        }
    }
    return sucesso;
}

// --- Implementações de Remoção e Balanceamento (Esboços Estruturais) ---
Arv45_Mem *buscar_menor_elemento_no_arv_mem(Arv45_Mem *no)
{
    Arv45_Mem *resultado = NULL;
    Arv45_Mem *atual = no;
    if (atual != NULL)
    {
        int continuar_descida = (atual->filhos[0] != NULL);
        while (continuar_descida)
        {
            atual = atual->filhos[0];
            if (atual->filhos[0] == NULL)
            {
                continuar_descida = 0;
            }
        }
        resultado = atual;
    }
    return resultado;
}

ArvStatusOperacao tratar_underflow_na_arv_mem(Arv45_Mem **ponteiro_filho_no_pai, Arv45_Mem *pai_no, int pos_filho_no_pai)
{
    ArvStatusOperacao status = ARV_STATUS_OK;
    Arv45_Mem *filho_underflow = *ponteiro_filho_no_pai;
    (void)pos_filho_no_pai; // Usado para determinar irmãos, mas não diretamente aqui

    // printf("Tratar Underflow para Arv45_Mem (bloco %d) - Logica de balanceamento detalhada necessaria.\n",
    //        filho_underflow && filho_underflow->nInfos > 0 ? filho_underflow->infos[0].bloco_inicial : -1);

    if (pai_no == NULL && filho_underflow->nInfos == 0)
    {
        status = ARV_STATUS_UNDERFLOW;
    }
    else if (pai_no != NULL)
    {
        Arv45_Mem *irmao_esq = (pos_filho_no_pai > 0) ? pai_no->filhos[pos_filho_no_pai - 1] : NULL;
        Arv45_Mem *irmao_dir = (pos_filho_no_pai < pai_no->nInfos) ? pai_no->filhos[pos_filho_no_pai + 1] : NULL;
        int operacao_feita = 0;

        if (irmao_dir != NULL && irmao_dir->nInfos > 1)
        { // 1 é MIN_SEGMENTOS_NO_NO_VALIDO
            status = redistribuir_dir_arv_mem(filho_underflow, pai_no, irmao_dir, pos_filho_no_pai);
            operacao_feita = 1;
        }
        if (!operacao_feita && irmao_esq != NULL && irmao_esq->nInfos > 1)
        {
            status = redistribuir_esq_arv_mem(filho_underflow, pai_no, irmao_esq, pos_filho_no_pai);
            operacao_feita = 1;
        }
        if (!operacao_feita && irmao_dir != NULL && irmao_dir->nInfos == 1)
        {
            status = fundir_dir_arv_mem(ponteiro_filho_no_pai, pai_no, irmao_dir, pos_filho_no_pai);
            operacao_feita = 1;
        }
        if (!operacao_feita && irmao_esq != NULL && irmao_esq->nInfos == 1)
        {
            status = fundir_esq_arv_mem(ponteiro_filho_no_pai, pai_no, irmao_esq, pos_filho_no_pai);
            operacao_feita = 1;
        }

        if (!operacao_feita)
        {
            status = ARV_STATUS_UNDERFLOW; // Não conseguiu resolver
        }
    }
    return status;
}

ArvStatusOperacao redistribuir_esq_arv_mem(Arv45_Mem *filho_underflow, Arv45_Mem *pai_no, Arv45_Mem *irmao_esq, int pos_filho_no_pai)
{
    (void)filho_underflow;
    (void)pai_no;
    (void)irmao_esq;
    (void)pos_filho_no_pai; // Silencia warnings para placeholder
    // printf("Redistribuicao com irmao esquerdo (placeholder)\n");
    // Implementação detalhada:
    // 1. Pega a última chave (MemSegmento) e o último filho do irmao_esq.
    // 2. Pega a chave do pai (MemSegmento) entre irmao_esq e filho_underflow.
    // 3. Move chave do pai para o início do filho_underflow.
    // 4. Move chave do irmao_esq para o lugar da chave do pai.
    // 5. Move filho do irmao_esq para ser o primeiro filho do filho_underflow.
    // 6. Ajusta nInfos.
    return ARV_STATUS_OK; // Deve retornar OK se bem sucedido
}
ArvStatusOperacao redistribuir_dir_arv_mem(Arv45_Mem *filho_underflow, Arv45_Mem *pai_no, Arv45_Mem *irmao_dir, int pos_filho_no_pai)
{
    (void)filho_underflow;
    (void)pai_no;
    (void)irmao_dir;
    (void)pos_filho_no_pai; // Silencia warnings
    // printf("Redistribuicao com irmao direito (placeholder)\n");
    // Implementação detalhada:
    // 1. Pega a primeira chave (MemSegmento) e o primeiro filho do irmao_dir.
    // 2. Pega a chave do pai (MemSegmento) entre filho_underflow e irmao_dir.
    // 3. Move chave do pai para o fim do filho_underflow.
    // 4. Move chave do irmao_dir para o lugar da chave do pai.
    // 5. Move filho do irmao_dir para ser o último filho do filho_underflow.
    // 6. Ajusta nInfos e desloca chaves/filhos no irmao_dir.
    return ARV_STATUS_OK;
}
ArvStatusOperacao fundir_esq_arv_mem(Arv45_Mem **ponteiro_filho_em_underflow_ptr, Arv45_Mem *pai_no, Arv45_Mem *irmao_esq, int pos_filho_no_pai)
{
    Arv45_Mem *filho_underflow = *ponteiro_filho_em_underflow_ptr;
    (void)filho_underflow;
    (void)pai_no;
    (void)irmao_esq;
    (void)pos_filho_no_pai; // Silencia warnings
    // printf("Fusao com irmao esquerdo (placeholder)\n");
    // Implementação detalhada:
    // 1. Pega a chave do pai (MemSegmento) entre irmao_esq e filho_underflow.
    // 2. Adiciona essa chave do pai e todas as chaves/filhos do filho_underflow ao irmao_esq.
    // 3. Libera o nó filho_underflow.
    // 4. Remove a chave e o ponteiro para o filho_underflow do pai_no (deslocando outros).
    // 5. Ajusta nInfos.
    // 6. Retorna ARV_STATUS_OK se pai_no não entrou em underflow, senão ARV_STATUS_UNDERFLOW.
    return ARV_STATUS_OK;
}
ArvStatusOperacao fundir_dir_arv_mem(Arv45_Mem **ponteiro_filho_em_underflow_ptr, Arv45_Mem *pai_no, Arv45_Mem *irmao_dir, int pos_filho_no_pai)
{
    Arv45_Mem *filho_underflow = *ponteiro_filho_em_underflow_ptr;
    (void)filho_underflow;
    (void)pai_no;
    (void)irmao_dir;
    (void)pos_filho_no_pai; // Silencia warnings
    // printf("Fusao com irmao direito (placeholder)\n");
    // Similar a fundir_esq, mas filho_underflow se junta com irmao_dir.
    return ARV_STATUS_OK;
}

ArvStatusOperacao remover_seg_recursivo_arv_mem(Arv45_Mem **ponteiro_no_atual_ptr, int bloco_inicial_chave_remover, Arv45_Mem *pai_do_no_atual)
{
    ArvStatusOperacao status_final = ARV_STATUS_OK;
    Arv45_Mem *no_atual = (ponteiro_no_atual_ptr != NULL) ? *ponteiro_no_atual_ptr : NULL;
    int prosseguir = 1;

    if (no_atual == NULL)
    {
        status_final = ARV_STATUS_NAO_ENCONTRADO;
        prosseguir = 0;
    }

    if (prosseguir)
    {
        int indice_segmento_removido = -1;
        int i = 0;
        int descer_por_filho_idx = -1;

        while (i < no_atual->nInfos && bloco_inicial_chave_remover > no_atual->infos[i].bloco_inicial)
        {
            i++;
        }
        if (i < no_atual->nInfos && bloco_inicial_chave_remover == no_atual->infos[i].bloco_inicial)
        {
            indice_segmento_removido = i;
        }
        else
        {
            descer_por_filho_idx = i; // Desce pelo filho à esquerda da posição onde estaria, ou último filho
        }

        if (indice_segmento_removido != -1)
        {
            if (eh_folha_arv_mem(no_atual))
            {
                for (int k = indice_segmento_removido; k < no_atual->nInfos - 1; k++)
                {
                    no_atual->infos[k] = no_atual->infos[k + 1];
                }
                no_atual->infos[no_atual->nInfos - 1] = (MemSegmento){0};
                no_atual->nInfos--;
                if (no_atual->nInfos < 1 && pai_do_no_atual != NULL)
                {
                    status_final = ARV_STATUS_UNDERFLOW;
                }
                else if (no_atual->nInfos == 0 && pai_do_no_atual == NULL)
                {
                    status_final = ARV_STATUS_UNDERFLOW;
                }
            }
            else
            {
                Arv45_Mem *no_contem_sucessor = buscar_menor_elemento_no_arv_mem(no_atual->filhos[indice_segmento_removido + 1]);
                if (no_contem_sucessor != NULL && no_contem_sucessor->nInfos > 0)
                {
                    MemSegmento seg_sucessor = no_contem_sucessor->infos[0];
                    no_atual->infos[indice_segmento_removido] = seg_sucessor;
                    ArvStatusOperacao st_rec = remover_seg_recursivo_arv_mem(&(no_atual->filhos[indice_segmento_removido + 1]), seg_sucessor.bloco_inicial, no_atual);
                    if (st_rec == ARV_STATUS_UNDERFLOW)
                    {
                        status_final = tratar_underflow_na_arv_mem(&(no_atual->filhos[indice_segmento_removido + 1]), no_atual, indice_segmento_removido + 1);
                    }
                    else
                    {
                        status_final = st_rec;
                    }
                }
                else
                {
                    fprintf(stderr, "Erro: Sucessor nao encontrado para remocao em no interno (bloco %d).\n", bloco_inicial_chave_remover);
                    status_final = ARV_STATUS_ERRO;
                }
            }
        }
        else
        { // Descer para o filho
            if (descer_por_filho_idx != -1 && no_atual->filhos[descer_por_filho_idx] != NULL)
            {
                ArvStatusOperacao st_rec = remover_seg_recursivo_arv_mem(&(no_atual->filhos[descer_por_filho_idx]), bloco_inicial_chave_remover, no_atual);
                if (st_rec == ARV_STATUS_UNDERFLOW)
                {
                    status_final = tratar_underflow_na_arv_mem(&(no_atual->filhos[descer_por_filho_idx]), no_atual, descer_por_filho_idx);
                }
                else
                {
                    status_final = st_rec;
                }
            }
            else
            {
                status_final = ARV_STATUS_NAO_ENCONTRADO;
            }
        }
    }
    return status_final;
}

int remover_segmento_da_arvore_mem(Arv45_Mem **raiz_ptr, int bloco_inicial_chave_remover)
{
    int sucesso = 0;
    if (raiz_ptr != NULL && *raiz_ptr != NULL)
    {
        ArvStatusOperacao status_geral = remover_seg_recursivo_arv_mem(raiz_ptr, bloco_inicial_chave_remover, NULL);
        Arv45_Mem *raiz_atual = *raiz_ptr;

        if (raiz_atual != NULL && raiz_atual->nInfos == 0)
        {
            Arv45_Mem *raiz_antiga = raiz_atual;
            *raiz_ptr = raiz_antiga->filhos[0];
            free(raiz_antiga);
            status_geral = ARV_STATUS_OK;
        }

        if (status_geral == ARV_STATUS_OK)
        {
            sucesso = 1;
        }
    }
    return sucesso;
}

// --- Funções de Gerenciamento de Memória de Alto Nível ---

void percorrer_arvore_mem_em_ordem_e_executar(Arv45_Mem *raiz, void (*acao_sobre_segmento)(MemSegmento))
{
    if (raiz != NULL)
    {
        int i;
        // Loop para percorrer filhos e infos. Um nó com N infos tem N+1 filhos.
        // O filho[i] está à esquerda de infos[i].
        // O filho[nInfos] está à direita da última info (infos[nInfos-1]).
        for (i = 0; i < raiz->nInfos; i++)
        {
            if (raiz->filhos[i] != NULL)
            {
                percorrer_arvore_mem_em_ordem_e_executar(raiz->filhos[i], acao_sobre_segmento);
            }
            if (acao_sobre_segmento != NULL)
            {
                acao_sobre_segmento(raiz->infos[i]);
            }
        }
        // Processa o último filho (filho à direita da última info)
        if (raiz->filhos[raiz->nInfos] != NULL)
        {
            percorrer_arvore_mem_em_ordem_e_executar(raiz->filhos[raiz->nInfos], acao_sobre_segmento);
        }
    }
}

int buscar_segmento_que_contem_bloco_arv_mem(Arv45_Mem *raiz, int bloco_num, MemSegmento *segmento_encontrado_copia)
{
    int achou = 0;
    Arv45_Mem *no_atual = raiz;

    if (segmento_encontrado_copia != NULL)
    {
        segmento_encontrado_copia->bloco_inicial = -1;
    }

    int loop_ativo_busca = (no_atual != NULL);
    while (loop_ativo_busca)
    {
        int i = 0;
        int descer_para_filho_idx = -1; // Índice do filho para descer

        // Tenta encontrar o segmento no nó atual ou determinar qual filho seguir
        int k = 0;
        int encontrado_no_no_atual = 0;
        while (k < no_atual->nInfos && !encontrado_no_no_atual)
        {
            if (bloco_num >= no_atual->infos[k].bloco_inicial && bloco_num <= no_atual->infos[k].bloco_final)
            {
                if (segmento_encontrado_copia != NULL)
                {
                    *segmento_encontrado_copia = no_atual->infos[k];
                }
                achou = 1;
                encontrado_no_no_atual = 1; // Para sair do loop while(k...)
            }
            else if (bloco_num < no_atual->infos[k].bloco_inicial)
            {
                descer_para_filho_idx = k;  // Desce pelo filho à esquerda de infos[k]
                encontrado_no_no_atual = 1; // Para sair do loop while(k...) e tentar descer
            }
            k++;
        }

        if (achou)
        {                         // Se achou o segmento exato
            loop_ativo_busca = 0; // Termina a busca principal
        }
        else
        {
            if (descer_para_filho_idx == -1 && !achou)
            { // Se não achou e não decidiu descer, deve descer pelo último filho
                descer_para_filho_idx = no_atual->nInfos;
            }

            if (no_atual->filhos[descer_para_filho_idx] != NULL)
            {
                no_atual = no_atual->filhos[descer_para_filho_idx]; // Desce
            }
            else
            {
                loop_ativo_busca = 0; // Não há mais para onde descer
            }
        }
    }
    return achou;
}

// Esta função é um placeholder complexo, pois encontrar predecessor/sucessor em ordem numa B-Tree
// e depois verificar adjacência e estado é não trivial.
int buscar_segmento_adjacente_na_arvore_mem(Arv45_Mem *raiz, MemSegmento segmento_ref, int buscar_anterior, MemSegmento *adjacente_encontrado_copia)
{
    (void)raiz;
    (void)segmento_ref;
    (void)buscar_anterior; // Silencia warnings
    // printf("Logica de buscar_segmento_adjacente nao totalmente implementada.\n");
    if (adjacente_encontrado_copia != NULL)
    {
        adjacente_encontrado_copia->bloco_inicial = -1;
    }
    return 0;
}

void consolidar_segmentos_na_arvore_mem(Arv45_Mem **raiz, int bloco_inicial_referencia)
{
    if (raiz == NULL || *raiz == NULL || bloco_inicial_referencia < 0)
        return;

    int consolidacao_ocorreu_geral;
    do
    {
        consolidacao_ocorreu_geral = 0;
        MemSegmento seg_ref_copia;

        if (buscar_segmento_que_contem_bloco_arv_mem(*raiz, bloco_inicial_referencia, &seg_ref_copia))
        {
            if (seg_ref_copia.bloco_inicial == -1)
            {
                break;
            }

            MemSegmento vizinho_copia;

            // Tenta consolidar com vizinho esquerdo
            if (buscar_segmento_adjacente_na_arvore_mem(*raiz, seg_ref_copia, 1, &vizinho_copia))
            {
                if (segmentos_sao_adjacentes_e_mesmo_estado(vizinho_copia, seg_ref_copia))
                {
                    remover_segmento_da_arvore_mem(raiz, vizinho_copia.bloco_inicial);
                    remover_segmento_da_arvore_mem(raiz, seg_ref_copia.bloco_inicial);
                    MemSegmento fundido = fundir_dois_segmentos_mem(vizinho_copia, seg_ref_copia);
                    insere_segmento_na_arvore_mem(raiz, fundido);
                    bloco_inicial_referencia = fundido.bloco_inicial;
                    consolidacao_ocorreu_geral = 1;
                    // Recomeça a consolidação a partir do novo segmento fundido
                    // O 'continue' foi removido, o loop 'do-while' fará a reavaliação.
                }
            }
            // Se houve consolidação à esquerda, seg_ref_copia precisa ser reavaliado
            if (consolidacao_ocorreu_geral)
            {
                if (!buscar_segmento_que_contem_bloco_arv_mem(*raiz, bloco_inicial_referencia, &seg_ref_copia) || seg_ref_copia.bloco_inicial == -1)
                {
                    break; // Sai se o segmento de referência sumiu
                }
            }

            // Tenta consolidar com vizinho direito
            if (buscar_segmento_adjacente_na_arvore_mem(*raiz, seg_ref_copia, 0, &vizinho_copia))
            {
                if (segmentos_sao_adjacentes_e_mesmo_estado(seg_ref_copia, vizinho_copia))
                {
                    remover_segmento_da_arvore_mem(raiz, seg_ref_copia.bloco_inicial);
                    remover_segmento_da_arvore_mem(raiz, vizinho_copia.bloco_inicial);
                    MemSegmento fundido = fundir_dois_segmentos_mem(seg_ref_copia, vizinho_copia);
                    insere_segmento_na_arvore_mem(raiz, fundido);
                    bloco_inicial_referencia = fundido.bloco_inicial;
                    consolidacao_ocorreu_geral = 1;
                }
            }
        }
        else
        { // Segmento de referência não encontrado
            break;
        }
    } while (consolidacao_ocorreu_geral);
}

void cadastrar_layout_inicial_memoria_usr(Arv45_Mem **raiz)
{
    char situacao_no_atual_char;
    unsigned long long endereco_final_no_atual_abs_usuario;
    int primeiro_segmento = 1;
    unsigned long long proximo_endereco_esperado = ENDERECO_BASE_MEMORIA_GLOBAL_CONFIG;
    char situacao_anterior = ' ';

    printf("--- Cadastro de Layout da Memoria ---\n");
    printf("Tamanho do Bloco: %llu bytes.\n", TAMANHO_BLOCO_EM_BYTES);
    printf("Endereco base da memoria: %llu\n", ENDERECO_BASE_MEMORIA_GLOBAL_CONFIG);
    printf("Endereco final total da memoria: %llu\n", ENDERECO_MEMORIA_TOTAL_FINAL_GLOBAL_CONFIG);

    int continuar_cadastro = 1;
    while (continuar_cadastro && proximo_endereco_esperado <= ENDERECO_MEMORIA_TOTAL_FINAL_GLOBAL_CONFIG)
    {
        MemSegmento novo_segmento;
        int erro_nesta_entrada = 0;

        novo_segmento.bloco_inicial = (proximo_endereco_esperado - ENDERECO_BASE_MEMORIA_GLOBAL_CONFIG) / TAMANHO_BLOCO_EM_BYTES;
        novo_segmento.endereco_inicial_seg = proximo_endereco_esperado;

        if (primeiro_segmento)
        {
            printf("Primeiro segmento (Endereco inicial fixo em %llu):\n", novo_segmento.endereco_inicial_seg);
            printf("  Situacao (L para Livre, O para Ocupado): ");
            if (scanf(" %c", &situacao_no_atual_char) != 1)
            {
                erro_nesta_entrada = 1;
            }
            else
            {
                novo_segmento.situacao = situacao_no_atual_char;
            } // Só atribui se scanf deu certo
            while (getchar() != '\n')
                ; // Limpa sempre, mesmo se erro no scanf
            if (!erro_nesta_entrada)
            {
                primeiro_segmento = 0;
            }
        }
        else
        {
            novo_segmento.situacao = (situacao_anterior == 'L') ? 'O' : 'L';
            printf("Proximo segmento (bloco inicial %d, endereco %llu), Situacao determinada: %c\n",
                   novo_segmento.bloco_inicial, novo_segmento.endereco_inicial_seg, novo_segmento.situacao);
        }

        if (!erro_nesta_entrada)
        {
            printf("  Informe o ENDERECO FINAL absoluto do segmento atual: ");
            if (scanf("%llu", &endereco_final_no_atual_abs_usuario) != 1)
            {
                erro_nesta_entrada = 1;
            }
            while (getchar() != '\n')
                ;
        }

        if (!erro_nesta_entrada)
        {
            if (endereco_final_no_atual_abs_usuario < novo_segmento.endereco_inicial_seg)
            {
                printf("Erro: Endereco final (%llu) menor que o inicial (%llu).\n",
                       endereco_final_no_atual_abs_usuario, novo_segmento.endereco_inicial_seg);
                erro_nesta_entrada = 1;
                if (novo_segmento.bloco_inicial == 0)
                    primeiro_segmento = 1;
            }
            else
            {
                if (endereco_final_no_atual_abs_usuario > ENDERECO_MEMORIA_TOTAL_FINAL_GLOBAL_CONFIG)
                {
                    printf("Aviso: Endereco final excede o limite. Ajustando para %llu.\n", ENDERECO_MEMORIA_TOTAL_FINAL_GLOBAL_CONFIG);
                    endereco_final_no_atual_abs_usuario = ENDERECO_MEMORIA_TOTAL_FINAL_GLOBAL_CONFIG;
                }
                novo_segmento.endereco_final_seg = endereco_final_no_atual_abs_usuario;
                novo_segmento.bloco_final = (novo_segmento.endereco_final_seg - ENDERECO_BASE_MEMORIA_GLOBAL_CONFIG) / TAMANHO_BLOCO_EM_BYTES;

                if (novo_segmento.bloco_final < novo_segmento.bloco_inicial)
                {
                    printf("Erro: Bloco final resultou menor que bloco inicial. Segmento ignorado.\n");
                    erro_nesta_entrada = 1;
                    if (novo_segmento.bloco_inicial == 0)
                        primeiro_segmento = 1;
                }
                else
                {
                    if (!insere_segmento_na_arvore_mem(raiz, novo_segmento))
                    {
                        printf("Falha ao inserir segmento na arvore.\n");
                        erro_nesta_entrada = 1;
                    }
                    else
                    {
                        proximo_endereco_esperado = novo_segmento.endereco_final_seg + 1;
                        situacao_anterior = novo_segmento.situacao;
                        if (novo_segmento.endereco_final_seg >= ENDERECO_MEMORIA_TOTAL_FINAL_GLOBAL_CONFIG)
                        {
                            printf("Ultimo endereco da memoria alcancado ou excedido.\n");
                            continuar_cadastro = 0;
                        }
                    }
                }
            }
        }

        if (erro_nesta_entrada)
        {
            if (!primeiro_segmento)
            { // Se erro e não é o primeiro (que pode ser retentado)
                printf("Erro na entrada, cadastro deste segmento abortado. Tente o proximo ou saia.\n");
            }
            else
            {
                printf("Erro na entrada do primeiro segmento. Tente novamente.\n");
            }
        }

        if (proximo_endereco_esperado > ENDERECO_MEMORIA_TOTAL_FINAL_GLOBAL_CONFIG && continuar_cadastro)
        {
            printf("Nao ha mais espaco para cadastrar alem do limite da memoria.\n");
            continuar_cadastro = 0;
        }
    }
    printf("--- Fim do Cadastro de Layout ---\n");
}

int alocar_blocos_memoria_usr(Arv45_Mem **raiz, int num_blocos_req, MemSegmento *segmento_alocado_info_saida)
{
    int sucesso_alocacao = 0;
    if (raiz != NULL && num_blocos_req > 0)
    {
        MemSegmento segmento_livre_candidato;
        segmento_livre_candidato.bloco_inicial = -1;
        Arv45_Mem *no_pilha[100];
        int topo_pilha = -1;
        Arv45_Mem *no_atual_percurso = *raiz;
        int encontrado_candidato = 0;
        int loop_busca_ativo = (no_atual_percurso != NULL);

        while (loop_busca_ativo && !encontrado_candidato)
        {
            int desceu_ou_processou_no_interno = 0;
            if (no_atual_percurso != NULL)
            {
                if (topo_pilha < 99)
                {
                    topo_pilha++;
                    no_pilha[topo_pilha] = no_atual_percurso;
                    no_atual_percurso = no_atual_percurso->filhos[0];
                    desceu_ou_processou_no_interno = 1;
                }
                else
                {
                    fprintf(stderr, "Erro: Pilha de percurso excedida.\n");
                    loop_busca_ativo = 0;
                }
            }

            if (!desceu_ou_processou_no_interno || no_atual_percurso == NULL)
            { // Se não desceu mais à esquerda ou voltou do filho esquerdo
                if (topo_pilha >= 0)
                {
                    no_atual_percurso = no_pilha[topo_pilha];
                    topo_pilha--;

                    int i = 0;
                    int continuar_no_no_atual = 1;
                    while (i < no_atual_percurso->nInfos && !encontrado_candidato && continuar_no_no_atual)
                    {
                        if (no_atual_percurso->infos[i].situacao == 'L' &&
                            calcular_num_blocos_para_seg(no_atual_percurso->infos[i]) >= num_blocos_req)
                        {
                            segmento_livre_candidato = no_atual_percurso->infos[i];
                            encontrado_candidato = 1;
                            // Não sai do loop com break
                        }
                        if (!encontrado_candidato)
                        {
                            if (no_atual_percurso->filhos[i + 1] != NULL)
                            {
                                no_atual_percurso = no_atual_percurso->filhos[i + 1];
                                continuar_no_no_atual = 0; // Sai do loop interno para descer por este filho
                            }
                            else
                            {
                                i++; // Próxima info neste nó, se houver
                            }
                        }
                        else
                        {
                            i++; // Para sair do loop se encontrado
                        }
                    }
                    if (!continuar_no_no_atual && !encontrado_candidato)
                    {
                        // loop_busca_ativo continua para processar o novo no_atual_percurso (que é um filho)
                    }
                    else if (!encontrado_candidato)
                    {
                        no_atual_percurso = NULL; // Força pop ou fim se não há mais o que processar neste ramo
                    }
                }
                else
                { // Pilha vazia
                    loop_busca_ativo = 0;
                }
            }
        } // Fim while loop_busca_ativo

        if (encontrado_candidato && segmento_livre_candidato.bloco_inicial != -1)
        {
            // printf("Segmento livre encontrado para alocacao: "); imprimir_info_segmento(segmento_livre_candidato);
            if (!remover_segmento_da_arvore_mem(raiz, segmento_livre_candidato.bloco_inicial))
            {
                fprintf(stderr, "Erro ao remover segmento livre original da arvore.\n");
            }
            else
            {
                segmento_alocado_info_saida->situacao = 'O';
                segmento_alocado_info_saida->bloco_inicial = segmento_livre_candidato.bloco_inicial;
                segmento_alocado_info_saida->endereco_inicial_seg = segmento_livre_candidato.endereco_inicial_seg;
                segmento_alocado_info_saida->bloco_final = segmento_livre_candidato.bloco_inicial + num_blocos_req - 1;
                segmento_alocado_info_saida->endereco_final_seg = segmento_livre_candidato.endereco_inicial_seg +
                                                                  (unsigned long long)num_blocos_req * TAMANHO_BLOCO_EM_BYTES - 1;

                if (!insere_segmento_na_arvore_mem(raiz, *segmento_alocado_info_saida))
                {
                    fprintf(stderr, "Erro ao inserir segmento ocupado na arvore.\n");
                }
                else
                {
                    consolidar_segmentos_na_arvore_mem(raiz, segmento_alocado_info_saida->bloco_inicial);

                    if (segmento_livre_candidato.bloco_final > segmento_alocado_info_saida->bloco_final)
                    {
                        MemSegmento seg_livre_restante = criar_obj_segmento('L',
                                                                            segmento_alocado_info_saida->bloco_final + 1, segmento_livre_candidato.bloco_final,
                                                                            segmento_alocado_info_saida->endereco_final_seg + 1, segmento_livre_candidato.endereco_final_seg);

                        if (calcular_num_blocos_para_seg(seg_livre_restante) > 0)
                        {
                            if (!insere_segmento_na_arvore_mem(raiz, seg_livre_restante))
                            {
                                fprintf(stderr, "Erro ao inserir segmento livre restante.\n");
                            }
                            else
                            {
                                consolidar_segmentos_na_arvore_mem(raiz, seg_livre_restante.bloco_inicial);
                            }
                        }
                    }
                    sucesso_alocacao = 1;
                }
            }
        }
        else
        {
            printf("Nao foi encontrado segmento livre com %d blocos.\n", num_blocos_req);
        }
    }
    else
    {
        printf("Alocacao falhou: arvore invalida ou requisicao de blocos invalida.\n");
    }
    return sucesso_alocacao;
}

int liberar_blocos_memoria_usr(Arv45_Mem **raiz, int bloco_inicial_liberar, int bloco_final_liberar)
{
    int sucesso_liberacao = 0;
    int processar_liberacao = 1;

    if (raiz == NULL || bloco_inicial_liberar > bloco_final_liberar || bloco_inicial_liberar < 0)
    {
        printf("Liberacao falhou: parametros invalidos ou arvore nula.\n");
        processar_liberacao = 0;
    }

    if (processar_liberacao)
    {
        MemSegmento faixa_liberada = criar_obj_segmento('L',
                                                        bloco_inicial_liberar, bloco_final_liberar,
                                                        ENDERECO_BASE_MEMORIA_GLOBAL_CONFIG + (unsigned long long)bloco_inicial_liberar * TAMANHO_BLOCO_EM_BYTES,
                                                        ENDERECO_BASE_MEMORIA_GLOBAL_CONFIG + (unsigned long long)(bloco_final_liberar + 1) * TAMANHO_BLOCO_EM_BYTES - 1);

        // Lista temporária para armazenar segmentos que sobram após a liberação
        MemSegmento segmentos_ocupados_restantes[100]; // Supõe um número máximo de fragmentos
        int num_ocupados_restantes = 0;
        int erro_na_logica_complexa = 0;

        // Simulação da lógica complexa de encontrar e tratar sobreposições:
        // Esta parte precisaria de um loop que busca segmentos ocupados na faixa,
        // os remove, e adiciona os pedaços restantes à lista temporária.
        // Por ora, vamos apenas inserir a faixa liberada e tentar consolidar.
        // AVISO: SEM a remoção/ajuste dos segmentos OCUPADOS existentes que se sobrepõem
        // à faixa liberada, a árvore ficará inconsistente (segmentos L e O sobrepostos).
        // printf("AVISO: A logica de encontrar e ajustar segmentos ocupados sobrepostos pela liberacao nao esta completa.\n");
        // printf("       Apenas inserindo a faixa como livre e tentando consolidar.\n");

        if (calcular_num_blocos_para_seg(faixa_liberada) > 0)
        {
            if (!insere_segmento_na_arvore_mem(raiz, faixa_liberada))
            {
                fprintf(stderr, "Erro ao inserir a faixa liberada como segmento livre na arvore.\n");
            }
            else
            {
                consolidar_segmentos_na_arvore_mem(raiz, faixa_liberada.bloco_inicial);
                sucesso_liberacao = 1;
            }
        }
        else
        {
            consolidar_segmentos_na_arvore_mem(raiz, bloco_inicial_liberar);
            sucesso_liberacao = 1;
        }
        if (erro_na_logica_complexa)
        { // Se a lógica de tratar sobreposições falhasse
            sucesso_liberacao = 0;
        }
    }
    return sucesso_liberacao;
}

//================ LIBERAR ARVORE DE MEMORIA ==================
void libera_arvore_mem_completa(Arv45_Mem **raiz_ptr)
{
    if (raiz_ptr != NULL && *raiz_ptr != NULL)
    {
        Arv45_Mem *no_atual = *raiz_ptr;
        // Usa um loop em vez de chamadas recursivas separadas para cada filho nomeado
        for (int i = 0; i <= no_atual->nInfos && i < 5; ++i)
        { // nInfos chaves têm nInfos+1 filhos
            if (no_atual->filhos[i] != NULL)
            {
                libera_arvore_mem_completa(&(no_atual->filhos[i]));
            }
        }
        free(no_atual);
        *raiz_ptr = NULL;
    }
}

//================ MENU PRINCIPAL ==================
void menu_gerenciador_memoria(Arv45_Mem **raiz)
{
    int opcao = 0;
    int num_blocos_val;
    int bloco_ini_val, bloco_fim_val;
    MemSegmento seg_alocado;
    int menu_ativo = 1;
    char prefixo_impressao_arvore[1024] = ""; // Para imprime_arvore_mem_visual

    printf("Defina o ENDERECO FINAL TOTAL da memoria (ex: 134217727 para ~128MB, que sao %llu blocos de 1MB): ",
           128ULL * 1024ULL * 1024ULL / TAMANHO_BLOCO_EM_BYTES);

    int entrada_endereco_ok = 0;
    if (scanf("%llu", &ENDERECO_MEMORIA_TOTAL_FINAL_GLOBAL_CONFIG) == 1)
    {
        entrada_endereco_ok = 1;
    }
    while (getchar() != '\n')
        ;

    if (!entrada_endereco_ok || ENDERECO_MEMORIA_TOTAL_FINAL_GLOBAL_CONFIG < TAMANHO_BLOCO_EM_BYTES - 1)
    {
        printf("Entrada invalida para endereco final. Usando ~128MB (134217727) como padrao.\n");
        ENDERECO_MEMORIA_TOTAL_FINAL_GLOBAL_CONFIG = 128ULL * TAMANHO_BLOCO_EM_BYTES - 1;
    }

    cadastrar_layout_inicial_memoria_usr(raiz);

    while (menu_ativo)
    {
        printf("\n==================== MENU GERENCIADOR DE MEMORIA ====================\n");
        printf("Memoria (Blocos de %llu bytes):\n", TAMANHO_BLOCO_EM_BYTES);
        // percorrer_arvore_mem_em_ordem_e_executar(*raiz, imprimir_info_segmento); // Para lista
        imprime_arvore_mem_visual(*raiz, prefixo_impressao_arvore, 1, 1); // Para árvore
        printf("---------------------------------------------------------------------\n");
        printf("1. Alocar Blocos\n");
        printf("2. Liberar Blocos\n");
        printf("3. Recadastrar Layout Inicial (LIMPA ARVORE ATUAL)\n");
        printf("4. Sair\n");
        printf("Escolha uma opcao: ");

        int entrada_opcao_ok = 0;
        if (scanf("%d", &opcao) == 1)
        {
            entrada_opcao_ok = 1;
        }
        while (getchar() != '\n')
            ;

        if (entrada_opcao_ok)
        {
            if (opcao == 1)
            {
                printf("Quantidade de blocos para alocar: ");
                if (scanf("%d", &num_blocos_val) == 1)
                {
                    while (getchar() != '\n')
                        ;
                    if (alocar_blocos_memoria_usr(raiz, num_blocos_val, &seg_alocado))
                    {
                        printf("Alocacao bem-sucedida para:\n");
                        imprimir_info_segmento(seg_alocado);
                    }
                }
                else
                {
                    printf("Entrada invalida para quantidade de blocos.\n");
                    while (getchar() != '\n')
                        ;
                }
            }
            else if (opcao == 2)
            {
                printf("Bloco inicial a liberar: ");
                if (scanf("%d", &bloco_ini_val) == 1)
                {
                    while (getchar() != '\n')
                        ;
                    printf("Bloco final a liberar: ");
                    if (scanf("%d", &bloco_fim_val) == 1)
                    {
                        while (getchar() != '\n')
                            ;
                        if (liberar_blocos_memoria_usr(raiz, bloco_ini_val, bloco_fim_val))
                        {
                            printf("Operacao de liberacao processada.\n");
                        }
                        else
                        {
                            printf("Falha ao processar liberacao.\n");
                        }
                    }
                    else
                    {
                        printf("Entrada invalida para bloco final.\n");
                        while (getchar() != '\n')
                            ;
                    }
                }
                else
                {
                    printf("Entrada invalida para bloco inicial.\n");
                    while (getchar() != '\n')
                        ;
                }
            }
            else if (opcao == 3)
            {
                libera_arvore_mem_completa(raiz);
                *raiz = NULL;
                printf("Defina o ENDERECO FINAL TOTAL da memoria novamente: ");
                if (scanf("%llu", &ENDERECO_MEMORIA_TOTAL_FINAL_GLOBAL_CONFIG) != 1)
                { // Corrigido para %llu
                    printf("Entrada invalida. Usando ~128MB.\n");
                    ENDERECO_MEMORIA_TOTAL_FINAL_GLOBAL_CONFIG = 128ULL * TAMANHO_BLOCO_EM_BYTES - 1;
                }
                while (getchar() != '\n')
                    ;
                cadastrar_layout_inicial_memoria_usr(raiz);
            }
            else if (opcao == 4)
            {
                printf("Saindo...\n");
                menu_ativo = 0;
            }
            else
            {
                printf("Opcao invalida. Tente novamente.\n");
            }
        }
        else
        {
            printf("Entrada de opcao invalida.\n");
            opcao = 0;
        }
    }
}

//================ IMPRIME ARVORE VISUAL (Adaptada) ==================
void imprime_arvore_mem_visual(Arv45_Mem *raiz, char *prefixo, int eh_ultimo, int eh_raiz_param)
{
    if (raiz != NULL)
    {
        printf("%s", prefixo);
        if (!eh_raiz_param)
        {
            printf("%s", eh_ultimo ? "`------ " : "+------ ");
        }
        else
        {
            printf(" Raiz--> ");
        }

        printf("[");
        for (int k = 0; k < raiz->nInfos; k++)
        {
            printf("%d(%c)", raiz->infos[k].bloco_inicial, raiz->infos[k].situacao);
            if (k < raiz->nInfos - 1)
                printf("|");
        }
        printf("] (nSeg=%d)\n", raiz->nInfos);

        char novo_prefixo[1024];
        sprintf(novo_prefixo, "%s%s", prefixo, eh_raiz_param ? "         " : (eh_ultimo ? "         " : "|        "));

        int num_filhos_reais = 0;
        for (int k = 0; k <= raiz->nInfos && k < 5; ++k)
        {
            if (raiz->filhos[k])
                num_filhos_reais++;
        }
        if (raiz->nInfos == 0 && raiz->filhos[0] != NULL)
        {
            num_filhos_reais = 1;
        }

        int filhos_impressos = 0;
        for (int k = 0; k <= raiz->nInfos && k < 5; ++k)
        {
            if (raiz->filhos[k] != NULL)
            {
                filhos_impressos++;
                imprime_arvore_mem_visual(raiz->filhos[k], novo_prefixo, filhos_impressos == num_filhos_reais, 0);
            }
        }
    }
}

//================ MAIN ==================
int main()
{
    Arv45_Mem *raiz_principal_memoria = NULL;
    menu_gerenciador_memoria(&raiz_principal_memoria);
    libera_arvore_mem_completa(&raiz_principal_memoria);
    printf("\nPrograma encerrado. Memoria liberada.\n");
    return 0;
}