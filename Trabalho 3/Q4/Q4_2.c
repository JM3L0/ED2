#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_TAMANHO 180
#define METODO_A 1
#define METODO_B 2

typedef struct {
    char matricula[7];
    char nome[50];
    char funcao[30];
    float salario;
} Funcionario;

typedef struct {
    Funcionario tabela[MAX_TAMANHO];
    int tamanho;
    int colisoes;
} TabelaHash;

void inicializarTabela(TabelaHash* th, int tamanho) {
    th->tamanho = tamanho;
    th->colisoes = 0;
    
    memset(th->tabela, 0, sizeof(th->tabela));
}

int calcularHash(char* matricula, int tamanho, int metodo) {
    int resultado = 0;
    
    if (metodo == METODO_A) {
        // Método A: rotação + extração de dígitos
        char rotacionado[7];

        strcpy(rotacionado, matricula + 2);
        rotacionado[4] = matricula[0];
        rotacionado[5] = matricula[1];
        rotacionado[6] = '\0';
        
        char extraido[4];
        extraido[0] = rotacionado[1];
        extraido[1] = rotacionado[3];
        extraido[2] = rotacionado[4];
        extraido[3] = '\0';
        
        resultado = atoi(extraido);
    } else {
        // Método B: fold shift
        char grupo1[4], grupo2[4];

        grupo1[0] = matricula[0]; 
        grupo1[1] = matricula[1]; 
        grupo1[2] = matricula[5]; 
        grupo1[3] = '\0';

        grupo2[0] = matricula[2]; 
        grupo2[1] = matricula[3]; 
        grupo2[2] = matricula[4]; 
        grupo2[3] = '\0';
        
        resultado = atoi(grupo1) + atoi(grupo2);
    }
    
    return resultado % tamanho;
}

int resolverColisao(char* matricula, int posicao, int tamanho, int metodo) {
    int incremento = 0;
    
    if (metodo == METODO_A) {
        // Método A: soma o primeiro dígito
        incremento = matricula[0] - '0';
    } else {
        // Método B: soma o valor do primeiro e sexto dígito
        char digitos[3];
        digitos[0] = matricula[0]; 
        digitos[1] = matricula[5]; 
        digitos[2] = '\0';
        incremento = atoi(digitos);
    }
    
    return (posicao + incremento) % tamanho;
}

void inserir(TabelaHash* th, Funcionario func, int metodo) {
    int posicao = calcularHash(func.matricula, th->tamanho, metodo);
    int posicaoOriginal = posicao;
    int inserido = 0;
    
    if (th->tabela[posicao].matricula[0] == '\0') {
        th->tabela[posicao] = func;
        inserido = 1;

    } else {
        th->colisoes++;
        
        int tentativas = 0;
        while (tentativas < th->tamanho && inserido == 0) {
            posicao = resolverColisao(func.matricula, posicao, th->tamanho, metodo);
            
            if (th->tabela[posicao].matricula[0] == '\0') {
                th->tabela[posicao] = func;
                inserido = 1;
            }
            
            tentativas++;
        }
        
        if (inserido == 0) {
            th->tabela[posicaoOriginal] = func;
        }
    }
}

Funcionario* buscar(TabelaHash* th, char* matricula, int metodo) {
    int posicao = calcularHash(matricula, th->tamanho, metodo);
    Funcionario* resultado = NULL;
    int encontrado = 0;
    int tentativas = 0;
    
    while (tentativas < th->tamanho && encontrado == 0) {
        
        if (th->tabela[posicao].matricula[0] != '\0') {
            if (strcmp(th->tabela[posicao].matricula, matricula) == 0) {
                resultado = &th->tabela[posicao];
                encontrado = 1;
            }
        }

        if (!encontrado)
            posicao = resolverColisao(matricula, posicao, th->tamanho, metodo);
        tentativas++;
    }
    
    return resultado;
}

// int carregarFuncionarios(const char* nomeArquivo, Funcionario* funcionarios, int maxFuncionarios) {
//     FILE* arquivo = fopen(nomeArquivo, "r");
//     int contador = 0;
    
//     if (arquivo != NULL) {
//         char linha[100];
        
//         while (fgets(linha, sizeof(linha), arquivo) && contador < maxFuncionarios) {
//             linha[strcspn(linha, "\n")] = 0;
            
//             char* token = strtok(linha, ";");
//             int dadosValidos = 1;
            
//             if (token != NULL) {
//                 strcpy(funcionarios[contador].matricula, token);
//                 token = strtok(NULL, ";");
                
//                 if (token != NULL) {
//                     strcpy(funcionarios[contador].nome, token);
//                     token = strtok(NULL, ";");
                    
//                     if (token != NULL) {
//                         strcpy(funcionarios[contador].funcao, token);
//                         token = strtok(NULL, ";");
                        
//                         if (token != NULL) {
//                             funcionarios[contador].salario = atof(token);
//                         } else {
//                             dadosValidos = 0;
//                         }
//                     } else {
//                         dadosValidos = 0;
//                     }
//                 } else {
//                     dadosValidos = 0;
//                 }
//             } else {
//                 dadosValidos = 0;
//             }
            
//             if (dadosValidos == 1) {
//                 contador++;
//             }
//         }
        
//         fclose(arquivo);
//     } else {
//         printf("Erro ao abrir o arquivo de banco de dados.\n");
//     }
    
//     return contador;
// }

int carregarFuncionarios(const char* nomeArquivo, Funcionario* funcionarios, int maxFuncionarios) {
    FILE* arquivo = fopen(nomeArquivo, "r");
    int contador = 0;
    
    if (arquivo != NULL) {
        char linha[100];
        char matricula[7], nome[50], funcao[30];
        float salario;
        
        while (fgets(linha, sizeof(linha), arquivo) && contador < maxFuncionarios) {
            // Remove o caractere de nova linha
            linha[strcspn(linha, "\n")] = 0;
            
            // Tenta ler todos os campos de uma vez usando sscanf
            if (sscanf(linha, "%6[^;];%49[^;];%29[^;];%f", matricula, nome, funcao, &salario) == 4) {
                
                // Copia os dados para a estrutura
                strcpy(funcionarios[contador].matricula, matricula);
                strcpy(funcionarios[contador].nome, nome);
                strcpy(funcionarios[contador].funcao, funcao);
                funcionarios[contador].salario = salario;
                contador++;
            }
        }
        
        fclose(arquivo);
    } else {
        printf("Erro ao abrir o arquivo de banco de dados.\n");
    }
    
    return contador;
}

float calcularTaxaOcupacao(TabelaHash* th) {
    int posicoes_ocupadas = 0;
    
    for (int i = 0; i < th->tamanho; i++) {
        if (th->tabela[i].matricula[0] != '\0') {
            posicoes_ocupadas++;
        }
    }
    
    return (float)posicoes_ocupadas / th->tamanho * 100;
}

void testarMetodo(int tamanho, const char* nomeArquivo, int metodo) {
    TabelaHash tabela;
    inicializarTabela(&tabela, tamanho);
    
    Funcionario funcionarios[10000];
    int numFuncionarios = carregarFuncionarios(nomeArquivo, funcionarios, 10000);
    int sucesso = 0;
    
    if (numFuncionarios > 0) {
        clock_t inicio = clock();
        
        for (int i = 0; i < numFuncionarios; i++) {
            inserir(&tabela, funcionarios[i], metodo);
        }
        
        clock_t fim = clock();
        double tempo_insercao = (double)(fim - inicio) / CLOCKS_PER_SEC;
        
        inicio = clock();
        
        for (int i = 0; i < numFuncionarios; i++) {
            buscar(&tabela, funcionarios[i].matricula, metodo);
        }
        
        fim = clock();
        double tempo_busca = (double)(fim - inicio) / CLOCKS_PER_SEC;
        
        printf("Metodo %c (tamanho %d):\n", metodo == METODO_A ? 'A' : 'B', tamanho);
        printf("Numero de colisoes: %d\n", tabela.colisoes);
        printf("Tempo de insercao: %.6f segundos\n", tempo_insercao);
        printf("Tempo de busca: %.6f segundos\n", tempo_busca);
        printf("Taxa de ocupacao: %.2f%%\n\n", calcularTaxaOcupacao(&tabela));
        
        sucesso = 1;
    }
    
    if (sucesso == 0) {
        printf("Nao foi possivel carregar os funcionarios do arquivo.\n");
    }
}

int main() {
    const char* nomeArquivo = "funcionarios.txt";
    
    FILE* arquivo = fopen(nomeArquivo, "r");
    if (arquivo != NULL) {
        fclose(arquivo);
        printf("Usando banco de dados existente: %s\n", nomeArquivo);
    } 
    
    printf("\n=== COMPARACAO DE METODOS DE HASHING ===\n\n");
    
    printf("===== TAMANHO 121 =====\n\n");
    testarMetodo(121, nomeArquivo, METODO_A);
    testarMetodo(121, nomeArquivo, METODO_B);
    
    printf("===== TAMANHO 180 =====\n\n");
    testarMetodo(180, nomeArquivo, METODO_A);
    testarMetodo(180, nomeArquivo, METODO_B);
    
    return 0;
}