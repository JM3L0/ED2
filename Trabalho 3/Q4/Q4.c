#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Estrutura para armazenar os dados de um funcionario
typedef struct {
    char matricula[7]; // 6 digitos + '\0'
    char nome[50];
    char funcao[30];
    float salario;
} Funcionario;

// Estrutura para a tabela hash
typedef struct {
    Funcionario* tabela;
    int tamanho;
    int colisoes;
} TabelaHash;

// Funcao para inicializar a tabela hash
void inicializarTabela(TabelaHash* th, int tamanho) {
    th->tabela = (Funcionario*)malloc(tamanho * sizeof(Funcionario));
    th->tamanho = tamanho;
    th->colisoes = 0;
    
    // Inicializa todas as posicoes com matricula vazia
    for (int i = 0; i < tamanho; i++) {
        th->tabela[i].matricula[0] = '\0';
    }
}

// Funcao para liberar a memoria da tabela
void liberarTabela(TabelaHash* th) {
    free(th->tabela);
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
    
    // Se a posicao estiver vazia, insere diretamente
    if (th->tabela[posicao].matricula[0] == '\0') {
        th->tabela[posicao] = func;
        return;
    }
    
    // Ocorreu colisao
    th->colisoes++;
    
    // Tenta resolver com a estrategia de colisao
    int tentativas = 0;
    while (tentativas < th->tamanho) {
        posicao = resolverColisaoA(func.matricula, posicao, th->tamanho);
        
        // Se encontrou posicao vazia, insere
        if (th->tabela[posicao].matricula[0] == '\0') {
            th->tabela[posicao] = func;
            return;
        }
        
        // Proxima tentativa
        tentativas++;
    }
    
    // Se nao conseguiu inserir, substitui a primeira posicao encontrada
    th->tabela[posicaoOriginal] = func;
}

// Funcao para inserir um funcionario na tabela hash usando o metodo B
void inserirB(TabelaHash* th, Funcionario func) {
    int posicao = hashingB(func.matricula, th->tamanho);
    int posicaoOriginal = posicao;
    
    // Se a posicao estiver vazia, insere diretamente
    if (th->tabela[posicao].matricula[0] == '\0') {
        th->tabela[posicao] = func;
        return;
    }
    
    // Ocorreu colisao
    th->colisoes++;
    
    // Tenta resolver com a estrategia de colisao
    int tentativas = 0;
    while (tentativas < th->tamanho) {
        posicao = resolverColisaoB(func.matricula, posicao, th->tamanho);
        
        // Se encontrou posicao vazia, insere
        if (th->tabela[posicao].matricula[0] == '\0') {
            th->tabela[posicao] = func;
            return;
        }
        
        // Proxima tentativa
        tentativas++;
    }
    
    // Se nao conseguiu inserir, substitui a primeira posicao encontrada
    th->tabela[posicaoOriginal] = func;
}

// Funcao para buscar um funcionario pelo numero de matricula (metodo A)
Funcionario* buscarA(TabelaHash* th, char* matricula) {
    int posicao = hashingA(matricula, th->tamanho);
    
    // Se a posicao esta vazia, nao encontrou
    if (th->tabela[posicao].matricula[0] == '\0') {
        return NULL;
    }
    
    // Se encontrou na posicao hash, retorna
    if (strcmp(th->tabela[posicao].matricula, matricula) == 0) {
        return &th->tabela[posicao];
    }
    
    // Tenta as posicoes alternativas
    int tentativas = 0;
    while (tentativas < th->tamanho) {
        posicao = resolverColisaoA(matricula, posicao, th->tamanho);
        
        // Se a posicao esta vazia, nao encontrou
        if (th->tabela[posicao].matricula[0] == '\0') {
            return NULL;
        }
        
        // Se encontrou, retorna
        if (strcmp(th->tabela[posicao].matricula, matricula) == 0) {
            return &th->tabela[posicao];
        }
        
        tentativas++;
    }
    
    return NULL;
}

// Funcao para buscar um funcionario pelo numero de matricula (metodo B)
Funcionario* buscarB(TabelaHash* th, char* matricula) {
    int posicao = hashingB(matricula, th->tamanho);
    
    // Se a posicao esta vazia, nao encontrou
    if (th->tabela[posicao].matricula[0] == '\0') {
        return NULL;
    }
    
    // Se encontrou na posicao hash, retorna
    if (strcmp(th->tabela[posicao].matricula, matricula) == 0) {
        return &th->tabela[posicao];
    }
    
    // Tenta as posicoes alternativas
    int tentativas = 0;
    while (tentativas < th->tamanho) {
        posicao = resolverColisaoB(matricula, posicao, th->tamanho);
        
        // Se a posicao esta vazia, nao encontrou
        if (th->tabela[posicao].matricula[0] == '\0') {
            return NULL;
        }
        
        // Se encontrou, retorna
        if (strcmp(th->tabela[posicao].matricula, matricula) == 0) {
            return &th->tabela[posicao];
        }
        
        tentativas++;
    }
    
    return NULL;
}

// Funcao para gerar uma matricula aleatoria
void gerarMatricula(char* matricula) {
    for (int i = 0; i < 6; i++) {
        matricula[i] = '0' + (rand() % 10);
    }
    matricula[6] = '\0';
}

// Funcao para gerar um nome aleatorio simples
void gerarNome(char* nome) {
    char nomes[][10] = {"Joao", "Maria", "Pedro", "Ana", "Carlos", "Lucia", "Jose", "Paulo", "Marta", "Sandra"};
    char sobrenomes[][15] = {"Silva", "Santos", "Oliveira", "Souza", "Lima", "Pereira", "Costa", "Ferreira", "Ribeiro", "Almeida"};
    
    sprintf(nome, "%s %s", nomes[rand() % 10], sobrenomes[rand() % 10]);
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
    if (arquivo == NULL) {
        printf("Erro ao criar o arquivo de banco de dados.\n");
        return;
    }
    
    srand(time(NULL));
    
    for (int i = 0; i < 10000; i++) {
        char matricula[7];
        char nome[50];
        char funcao[30];
        float salario;
        
        gerarMatricula(matricula);
        gerarNome(nome);
        gerarFuncao(funcao);
        salario = gerarSalario();
        
        fprintf(arquivo, "%s;%s;%s;%.2f\n", matricula, nome, funcao, salario);
    }
    
    fclose(arquivo);
    printf("Banco de dados criado com sucesso: %s\n", nomeArquivo);
}

// Funcao para carregar os funcionarios do arquivo
int carregarFuncionarios(const char* nomeArquivo, Funcionario* funcionarios, int maxFuncionarios) {
    FILE* arquivo = fopen(nomeArquivo, "r");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo de banco de dados.\n");
        return 0;
    }
    
    int contador = 0;
    char linha[100];
    
    while (fgets(linha, sizeof(linha), arquivo) && contador < maxFuncionarios) {
        // Remover o caractere de nova linha
        linha[strcspn(linha, "\n")] = 0;
        
        // Separar os campos
        char* token = strtok(linha, ";");
        if (token == NULL) continue;
        strcpy(funcionarios[contador].matricula, token);
        
        token = strtok(NULL, ";");
        if (token == NULL) continue;
        strcpy(funcionarios[contador].nome, token);
        
        token = strtok(NULL, ";");
        if (token == NULL) continue;
        strcpy(funcionarios[contador].funcao, token);
        
        token = strtok(NULL, ";");
        if (token == NULL) continue;
        funcionarios[contador].salario = atof(token);
        
        contador++;
    }
    
    fclose(arquivo);
    return contador;
}

// Funcao para testar o desempenho do metodo A
void testarMetodoA(int tamanho, const char* nomeArquivo) {
    TabelaHash tabela;
    inicializarTabela(&tabela, tamanho);
    
    Funcionario funcionarios[10000];
    int numFuncionarios = carregarFuncionarios(nomeArquivo, funcionarios, 10000);
    
    if (numFuncionarios == 0) {
        printf("Nao foi possivel carregar os funcionarios do arquivo.\n");
        return;
    }
    
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
    printf("Taxa de ocupacao: %.2f%%\n\n", (float)numFuncionarios / tamanho * 100);
    
    liberarTabela(&tabela);
}

// Funcao para testar o desempenho do metodo B
void testarMetodoB(int tamanho, const char* nomeArquivo) {
    TabelaHash tabela;
    inicializarTabela(&tabela, tamanho);
    
    Funcionario funcionarios[10000];
    int numFuncionarios = carregarFuncionarios(nomeArquivo, funcionarios, 10000);
    
    if (numFuncionarios == 0) {
        printf("Nao foi possivel carregar os funcionarios do arquivo.\n");
        return;
    }
    
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
    printf("Taxa de ocupacao: %.2f%%\n\n", (float)numFuncionarios / tamanho * 100);
    
    liberarTabela(&tabela);
}

int main() {
    const char* nomeArquivo = "funcionarios.txt";
    
    // Verificar se o arquivo ja existe
    FILE* arquivo = fopen(nomeArquivo, "r");
    if (arquivo == NULL) {
        // Se nao existir, cria o arquivo de banco de dados
        criarBancoDeDados(nomeArquivo);
    } else {
        fclose(arquivo);
        printf("Usando banco de dados existente: %s\n", nomeArquivo);
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