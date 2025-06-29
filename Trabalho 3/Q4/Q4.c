#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Tamanho máximo que a tabela pode ter (usaremos o maior entre 121 e 180)
#define MAX_TAMANHO 180

// Estrutura para armazenar os dados de um funcionario
typedef struct {
    char matricula[7]; // 6 digitos + '\0'
    char nome[50];
    char funcao[30];
    float salario;
} Funcionario;

// Estrutura para a tabela hash
typedef struct {
    Funcionario tabela[MAX_TAMANHO];
    int tamanho;
    int colisoes;
} TabelaHash;

// Funcao para inicializar a tabela hash
void inicializarTabela(TabelaHash* th, int tamanho) {
    th->tamanho = tamanho;
    th->colisoes = 0;
    
    // Inicializa todas as posicoes com matricula vazia
    for (int i = 0; i < MAX_TAMANHO; i++) {
        th->tabela[i].matricula[0] = '\0';
    }
}

// Funcao hashing A: rotacao de 2 digitos para a esquerda depois extrai o 2o, 4o e 5o digitos
int hashingA(char* matricula, int tamanho) {
    // Rotacao de 2 digitos para a esquerda: "123456" -> "345612"
    char rotacionado[7];
    strcpy(rotacionado, matricula + 2); // Copia a partir do 3º digito
    rotacionado[4] = matricula[0];     // Adiciona o 1º digito
    rotacionado[5] = matricula[1];     // Adiciona o 2º digito
    rotacionado[6] = '\0';
    
    // Extrai o 2º, 4º e 5º digitos do numero rotacionado
    char extraido[4];
    extraido[0] = rotacionado[1]; // 2º digito
    extraido[1] = rotacionado[3]; // 4º digito
    extraido[2] = rotacionado[4]; // 5º digito
    extraido[3] = '\0';
    
    // Converte para inteiro e calcula o resto da divisao
    int valor = atoi(extraido);
    return valor % tamanho;
}

// Funcao para tratamento de colisao do metodo A
int resolverColisaoA(char* matricula, int posicao, int tamanho) {
    // Soma o primeiro digito da matricula
    int incremento = matricula[0] - '0';
    return (posicao + incremento) % tamanho;
}

// Funcao hashing B: fold shift com 3 digitos
int hashingB(char* matricula, int tamanho) {
    // Primeiro grupo: 1º, 2º e 6º digitos
    char grupo1[4];
    grupo1[0] = matricula[0];
    grupo1[1] = matricula[1];
    grupo1[2] = matricula[5];
    grupo1[3] = '\0';
    
    // Segundo grupo: 3º, 4º e 5º digitos
    char grupo2[4];
    grupo2[0] = matricula[2];
    grupo2[1] = matricula[3];
    grupo2[2] = matricula[4];
    grupo2[3] = '\0';
    
    // Soma os dois grupos e calcula o resto da divisao
    int valor = atoi(grupo1) + atoi(grupo2);
    return valor % tamanho;
}

// Funcao para tratamento de colisao do metodo B
int resolverColisaoB(char* matricula, int posicao, int tamanho) {
    // Soma o valor formado pelo primeiro e sexto digito
    char digitos[3];
    digitos[0] = matricula[0];
    digitos[1] = matricula[5];
    digitos[2] = '\0';
    
    int incremento = atoi(digitos);
    return (posicao + incremento) % tamanho;
}

// Funcao para inserir um funcionario na tabela hash usando o metodo A
void inserirA(TabelaHash* th, Funcionario func) {
    int posicao = hashingA(func.matricula, th->tamanho);
    int posicaoOriginal = posicao;
    int inserido = 0;
    
    // Se a posicao estiver vazia, insere diretamente
    if (th->tabela[posicao].matricula[0] == '\0') {
        th->tabela[posicao] = func;
        inserido = 1;
    } 
    
    if (inserido == 0) {
        // Ocorreu colisao
        th->colisoes++;
        
        // Tenta resolver com a estrategia de colisao
        int tentativas = 0;
        while (tentativas < th->tamanho && inserido == 0) {
            posicao = resolverColisaoA(func.matricula, posicao, th->tamanho);
            
            // Se encontrou posicao vazia, insere
            if (th->tabela[posicao].matricula[0] == '\0') {
                th->tabela[posicao] = func;
                inserido = 1;
            }
            
            // Proxima tentativa
            tentativas++;
        }
        
        // Se nao conseguiu inserir, substitui a primeira posicao encontrada
        if (inserido == 0) {
            th->tabela[posicaoOriginal] = func;
        }
    }
}

// Funcao para inserir um funcionario na tabela hash usando o metodo B
void inserirB(TabelaHash* th, Funcionario func) {
    int posicao = hashingB(func.matricula, th->tamanho);
    int posicaoOriginal = posicao;
    int inserido = 0;
    
    // Se a posicao estiver vazia, insere diretamente
    if (th->tabela[posicao].matricula[0] == '\0') {
        th->tabela[posicao] = func;
        inserido = 1;
    } 
    
    if (inserido == 0) {
        // Ocorreu colisao
        th->colisoes++;
        
        // Tenta resolver com a estrategia de colisao
        int tentativas = 0;
        while (tentativas < th->tamanho && inserido == 0) {
            posicao = resolverColisaoB(func.matricula, posicao, th->tamanho);
            
            // Se encontrou posicao vazia, insere
            if (th->tabela[posicao].matricula[0] == '\0') {
                th->tabela[posicao] = func;
                inserido = 1;
            }
            
            // Proxima tentativa
            tentativas++;
        }
        
        // Se nao conseguiu inserir, substitui a primeira posicao encontrada
        if (inserido == 0) {
            th->tabela[posicaoOriginal] = func;
        }
    }
}

// Funcao para buscar um funcionario pelo numero de matricula (metodo A)
Funcionario* buscarA(TabelaHash* th, char* matricula) {
    int posicao = hashingA(matricula, th->tamanho);
    Funcionario* resultado = NULL;
    int encontrado = 0;
    
    // Se a posicao nao esta vazia, verifica se é o funcionario procurado
    if (th->tabela[posicao].matricula[0] != '\0') {
        // Se encontrou na posicao hash, retorna
        if (strcmp(th->tabela[posicao].matricula, matricula) == 0) {
            resultado = &th->tabela[posicao];
            encontrado = 1;
        }
    } else {
        encontrado = 1; // Posição vazia, não precisa procurar mais
    }
    
    if (encontrado == 0) {
        // Tenta as posicoes alternativas
        int tentativas = 0;
        
        while (tentativas < th->tamanho && encontrado == 0) {
            posicao = resolverColisaoA(matricula, posicao, th->tamanho);
            
            // Se a posicao esta vazia, nao encontrou
            if (th->tabela[posicao].matricula[0] == '\0') {
                encontrado = 1; // Termina a busca
            } else {
                // Se encontrou, guarda o resultado
                if (strcmp(th->tabela[posicao].matricula, matricula) == 0) {
                    resultado = &th->tabela[posicao];
                    encontrado = 1;
                }
            }
            
            tentativas++;
        }
    }
    
    return resultado;
}

// Funcao para buscar um funcionario pelo numero de matricula (metodo B)
Funcionario* buscarB(TabelaHash* th, char* matricula) {
    int posicao = hashingB(matricula, th->tamanho);
    Funcionario* resultado = NULL;
    int encontrado = 0;
    
    // Se a posicao nao esta vazia, verifica se é o funcionario procurado
    if (th->tabela[posicao].matricula[0] != '\0') {
        // Se encontrou na posicao hash, retorna
        if (strcmp(th->tabela[posicao].matricula, matricula) == 0) {
            resultado = &th->tabela[posicao];
            encontrado = 1;
        }
    } else {
        encontrado = 1; // Posição vazia, não precisa procurar mais
    }
    
    if (encontrado == 0) {
        // Tenta as posicoes alternativas
        int tentativas = 0;
        
        while (tentativas < th->tamanho && encontrado == 0) {
            posicao = resolverColisaoB(matricula, posicao, th->tamanho);
            
            // Se a posicao esta vazia, nao encontrou
            if (th->tabela[posicao].matricula[0] == '\0') {
                encontrado = 1; // Termina a busca
            } else {
                // Se encontrou, guarda o resultado
                if (strcmp(th->tabela[posicao].matricula, matricula) == 0) {
                    resultado = &th->tabela[posicao];
                    encontrado = 1;
                }
            }
            
            tentativas++;
        }
    }
    
    return resultado;
}

// Funcao para gerar uma matricula aleatoria
void gerarMatricula(char* matricula) {
    for (int i = 0; i < 6; i++) {
        matricula[i] = '0' + (rand() % 10);
    }
    matricula[6] = '\0';
}

// Funcao para gerar uma funcao aleatoria
void gerarFuncao(char* funcao) {
    char funcoes[][30] = {"Analista", "Programador", "Gerente", "Diretor", "Contador", "Secretario", "Assistente", "Consultor", "Estagiario", "Coordenador"};
    
    strcpy(funcao, funcoes[rand() % 10]);
}

// Funcao para gerar um salario aleatorio
float gerarSalario() {
    return 1000.0f + (rand() % 9000);
}

// Funcao para criar um arquivo de banco de dados com 10000 funcionarios
void criarBancoDeDados(const char* nomeArquivo) {
    FILE* arquivo = fopen(nomeArquivo, "w");
    int sucesso = 0;
    
    if (arquivo != NULL) {
        srand(time(NULL));
        
        for (int i = 0; i < 10000; i++) {
            char matricula[7];
            char nome[50];
            char funcao[30];
            float salario;
            
            gerarMatricula(matricula);
            // Gera nomes sequenciais: Funcionario0001, Funcionario0002, etc.
            sprintf(nome, "Funcionario%04d", i+1);
            gerarFuncao(funcao);
            salario = gerarSalario();
            
            fprintf(arquivo, "%s;%s;%s;%.2f\n", matricula, nome, funcao, salario);
        }
        
        fclose(arquivo);
        printf("Banco de dados criado com sucesso: %s\n", nomeArquivo);
        sucesso = 1;
    }
    
    if (sucesso == 0) {
        printf("Erro ao criar o arquivo de banco de dados.\n");
    }
}

// Funcao para carregar os funcionarios do arquivo
int carregarFuncionarios(const char* nomeArquivo, Funcionario* funcionarios, int maxFuncionarios) {
    FILE* arquivo = fopen(nomeArquivo, "r");
    int contador = 0;
    
    if (arquivo != NULL) {
        char linha[100];
        
        while (fgets(linha, sizeof(linha), arquivo) && contador < maxFuncionarios) {
            // Remover o caractere de nova linha
            linha[strcspn(linha, "\n")] = 0;
            
            // Separar os campos
            char* token = strtok(linha, ";");
            int dadosValidos = 1;
            
            if (token != NULL) {
                strcpy(funcionarios[contador].matricula, token);
                token = strtok(NULL, ";");
                
                if (token != NULL) {
                    strcpy(funcionarios[contador].nome, token);
                    token = strtok(NULL, ";");
                    
                    if (token != NULL) {
                        strcpy(funcionarios[contador].funcao, token);
                        token = strtok(NULL, ";");
                        
                        if (token != NULL) {
                            funcionarios[contador].salario = atof(token);
                        } else {
                            dadosValidos = 0;
                        }
                    } else {
                        dadosValidos = 0;
                    }
                } else {
                    dadosValidos = 0;
                }
            } else {
                dadosValidos = 0;
            }
            
            if (dadosValidos == 1) {
                contador++;
            }
        }
        
        fclose(arquivo);
    } else {
        printf("Erro ao abrir o arquivo de banco de dados.\n");
    }
    
    return contador;
}

// Função para calcular a taxa de ocupação da tabela
float calcularTaxaOcupacao(TabelaHash* th) {
    int posicoes_ocupadas = 0;
    
    for (int i = 0; i < th->tamanho; i++) {
        if (th->tabela[i].matricula[0] != '\0') {
            posicoes_ocupadas++;
        }
    }
    
    return (float)posicoes_ocupadas / th->tamanho * 100;
}

// Funcao para testar o desempenho do metodo A
void testarMetodoA(int tamanho, const char* nomeArquivo) {
    TabelaHash tabela;
    inicializarTabela(&tabela, tamanho);
    
    Funcionario funcionarios[10000];
    int numFuncionarios = carregarFuncionarios(nomeArquivo, funcionarios, 10000);
    int sucesso = 0;
    
    if (numFuncionarios > 0) {
        // Mede o tempo de insercao
        clock_t inicio = clock();
        
        for (int i = 0; i < numFuncionarios; i++) {
            inserirA(&tabela, funcionarios[i]);
        }
        
        clock_t fim = clock();
        double tempo_insercao = (double)(fim - inicio) / CLOCKS_PER_SEC;
        
        // Mede o tempo de busca (busca todos os funcionarios)
        inicio = clock();
        
        for (int i = 0; i < numFuncionarios; i++) {
            buscarA(&tabela, funcionarios[i].matricula);
        }
        
        fim = clock();
        double tempo_busca = (double)(fim - inicio) / CLOCKS_PER_SEC;
        
        printf("Metodo A (tamanho %d):\n", tamanho);
        printf("Numero de colisoes: %d\n", tabela.colisoes);
        printf("Tempo de insercao: %.6f segundos\n", tempo_insercao);
        printf("Tempo de busca: %.6f segundos\n", tempo_busca);
        float taxa_ocupacao = calcularTaxaOcupacao(&tabela);
        printf("Taxa de ocupacao: %.2f%%\n\n", taxa_ocupacao);
        
        sucesso = 1;
    }
    
    if (sucesso == 0) {
        printf("Nao foi possivel carregar os funcionarios do arquivo.\n");
    }
}

// Funcao para testar o desempenho do metodo B
void testarMetodoB(int tamanho, const char* nomeArquivo) {
    TabelaHash tabela;
    inicializarTabela(&tabela, tamanho);
    
    Funcionario funcionarios[10000];
    int numFuncionarios = carregarFuncionarios(nomeArquivo, funcionarios, 10000);
    int sucesso = 0;
    
    if (numFuncionarios > 0) {
        // Mede o tempo de insercao
        clock_t inicio = clock();
        
        for (int i = 0; i < numFuncionarios; i++) {
            inserirB(&tabela, funcionarios[i]);
        }
        
        clock_t fim = clock();
        double tempo_insercao = (double)(fim - inicio) / CLOCKS_PER_SEC;
        
        // Mede o tempo de busca (busca todos os funcionarios)
        inicio = clock();
        
        for (int i = 0; i < numFuncionarios; i++) {
            buscarB(&tabela, funcionarios[i].matricula);
        }
        
        fim = clock();
        double tempo_busca = (double)(fim - inicio) / CLOCKS_PER_SEC;
        
        printf("Metodo B (tamanho %d):\n", tamanho);
        printf("Numero de colisoes: %d\n", tabela.colisoes);
        printf("Tempo de insercao: %.6f segundos\n", tempo_insercao);
        printf("Tempo de busca: %.6f segundos\n", tempo_busca);
        float taxa_ocupacao = calcularTaxaOcupacao(&tabela);
        printf("Taxa de ocupacao: %.2f%%\n\n", taxa_ocupacao);
        
        sucesso = 1;
    }
    
    if (sucesso == 0) {
        printf("Nao foi possivel carregar os funcionarios do arquivo.\n");
    }
}

int main() {
    const char* nomeArquivo = "funcionarios.txt";
    int arquivoExistente = 0;
    
    // Verificar se o arquivo ja existe
    FILE* arquivo = fopen(nomeArquivo, "r");
    if (arquivo != NULL) {
        fclose(arquivo);
        printf("Usando banco de dados existente: %s\n", nomeArquivo);
        // arquivoExistente = 1;
    } 
    
    if (arquivoExistente == 0) {
        // Se nao existir, cria o arquivo de banco de dados
        criarBancoDeDados(nomeArquivo);
    }
    
    printf("\n=== COMPARACAO DE METODOS DE HASHING ===\n\n");
    
    // Testa com tamanho 121
    printf("===== TAMANHO 121 =====\n\n");
    testarMetodoA(121, nomeArquivo);
    testarMetodoB(121, nomeArquivo);
    
    // Testa com tamanho 180
    printf("===== TAMANHO 180 =====\n\n");
    testarMetodoA(180, nomeArquivo);
    testarMetodoB(180, nomeArquivo);
    
    return 0;
}